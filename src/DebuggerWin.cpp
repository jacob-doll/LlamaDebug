#include <LlamaDebug/Debugger.h>
#include <LlamaDebug/Binary/Defs/PE.h>

#include <Windows.h>
#include <DbgEng.h>
#include <memory>
#include <cstdio>

namespace LlamaDebug 
{

typedef HRESULT (__stdcall *DebugCreate_t)(
    _In_ REFIID InterfaceId,
    _Out_ PVOID* Interface
);

static DebugCreate_t DLLDebugCreate = NULL;

struct WinDbgContext
{
    uintptr_t BaseAddress;
    uint32_t ProgramSize;
    IDebugClient* Client;
    IDebugControl* Control;
    IDebugDataSpaces* DataSpaces;
    IDebugRegisters* Registers;
    IDebugSymbols* Symbols;
};

static WinDbgContext g_Context;

class EventCallbacks : public DebugBaseEventCallbacks 
{
public:
    STDMETHOD_(ULONG, AddRef)(
        THIS
        )
    {
        return 1;
    }
    
    STDMETHOD_(ULONG, Release)(
        THIS
        )
    {
        return 0;
    }
    
    STDMETHOD(GetInterestMask)(
        THIS_
        _Out_ PULONG Mask
        )
    {
        *Mask = DEBUG_EVENT_BREAKPOINT | DEBUG_EVENT_EXCEPTION | 
                DEBUG_EVENT_CREATE_PROCESS | DEBUG_EVENT_EXIT_PROCESS |
                DEBUG_EVENT_LOAD_MODULE | DEBUG_EVENT_SYSTEM_ERROR;
        return S_OK;
    }

    STDMETHOD(Breakpoint)(
        THIS_
        _In_ PDEBUG_BREAKPOINT Bp
        )
    {
        return DEBUG_STATUS_BREAK;
    }

    STDMETHOD(Exception)(
        THIS_
        _In_ PEXCEPTION_RECORD64 Exception,
        _In_ ULONG FirstChance
        )
    {
        return DEBUG_STATUS_BREAK;
    }
    
    STDMETHOD(CreateProcess)(
        THIS_
        _In_ ULONG64 ImageFileHandle,
        _In_ ULONG64 Handle,
        _In_ ULONG64 BaseOffset,
        _In_ ULONG ModuleSize,
        _In_ PCSTR ModuleName,
        _In_ PCSTR ImageName,
        _In_ ULONG CheckSum,
        _In_ ULONG TimeDateStamp,
        _In_ ULONG64 InitialThreadHandle,
        _In_ ULONG64 ThreadDataOffset,
        _In_ ULONG64 StartOffset
        )
    {
        g_Context.BaseAddress = (uintptr_t) BaseOffset;
        g_Context.ProgramSize = ModuleSize;
        return DEBUG_STATUS_BREAK;
    }

    STDMETHOD(ExitProcess)(
        THIS_
        _In_ ULONG ExitCode
        )
    {
        return DEBUG_STATUS_BREAK;
    }

    STDMETHOD(LoadModule)(
        THIS_
        _In_ ULONG64 ImageFileHandle,
        _In_ ULONG64 BaseOffset,
        _In_ ULONG ModuleSize,
        _In_ PCSTR ModuleName,
        _In_ PCSTR ImageName,
        _In_ ULONG CheckSum,
        _In_ ULONG TimeDateStamp
        )
    {
        return DEBUG_STATUS_BREAK;
    }

    STDMETHOD(SystemError)(
        THIS_
        _In_ ULONG Error,
        _In_ ULONG Level
        )
    {
        return DEBUG_STATUS_BREAK;
    }
};

class OutputCallbacks : public IDebugOutputCallbacks
{
public:
    STDMETHOD(QueryInterface)(
        THIS_
        _In_ REFIID InterfaceId,
        _Out_ PVOID* Interface
        )
    {
        *Interface = NULL;

        if (IsEqualIID(InterfaceId, __uuidof(IUnknown)) ||
            IsEqualIID(InterfaceId, __uuidof(IDebugOutputCallbacks)))
        {
            *Interface = (IDebugOutputCallbacks *)this;
            AddRef();
            return S_OK;
        }
        else
        {
            return E_NOINTERFACE;
        }
    }

    STDMETHOD_(ULONG, AddRef)(
        THIS
        )
    {
        return 1;
    }

    STDMETHOD_(ULONG, Release)(
        THIS
        )
    {
        return 0;
    }

    STDMETHOD(Output)(
        THIS_
        IN ULONG Mask,
        IN PCSTR Text
        )
    {
        printf("%s", Text);
        return S_OK;
    }
};

static EventCallbacks g_EventCallbacks;
static OutputCallbacks g_OutputCallbacks;

static bool DLLInit()
{
    if (DLLDebugCreate) return true;
    HMODULE h = LoadLibrary(TEXT("dbgeng.dll"));
    if (!h) return false;

    DLLDebugCreate = (DebugCreate_t)GetProcAddress(h, "DebugCreate");
    if (!DLLDebugCreate) return false;
    return true;
}

void Terminate()
{
    g_Context.Client->EndSession(DEBUG_END_PASSIVE);
    if (g_Context.Symbols)
    {
        g_Context.Symbols->Release();
    }

    if (g_Context.Registers) 
    {
        g_Context.Registers->Release();
    }

    if (g_Context.DataSpaces) 
    {
        g_Context.DataSpaces->Release();
    }

    if (g_Context.Control) 
    {
        g_Context.Control->Release();
    }

    if (g_Context.Client) 
    {
        g_Context.Client->Release();
    }
}

static bool CreateInterfaces()
{
    if (DLLDebugCreate(__uuidof(IDebugClient), 
                            (void**)&g_Context.Client) != S_OK) 
    {
        goto fail;
    }

    if (g_Context.Client->QueryInterface(__uuidof(IDebugControl), 
                                        (void**)&g_Context.Control) != S_OK) 
    {
        goto fail;
    }

    if (g_Context.Client->QueryInterface(__uuidof(IDebugDataSpaces), 
                                        (void**)&g_Context.DataSpaces) != S_OK) 
    {
        goto fail;
    }

    if (g_Context.Client->QueryInterface(__uuidof(IDebugRegisters), 
                                        (void**)&g_Context.Registers) != S_OK) 
    {
        goto fail;
    }

    if (g_Context.Client->QueryInterface(__uuidof(IDebugSymbols),
                                            (void**)&g_Context.Symbols) != S_OK)
    {
        goto fail;
    }

    if (g_Context.Client->SetEventCallbacks(&g_EventCallbacks) != S_OK) 
    {
        goto fail;
    }

    if (g_Context.Client->SetOutputCallbacks(&g_OutputCallbacks) != S_OK)
    {
        goto fail;
    }

    return true;
fail:
    Terminate();
    return false;
}

bool InitSymbols() 
{
    if (g_Context.Symbols->SetSymbolPath("cache*;srv*https://msdl.microsoft.com/download/symbols") != S_OK)
    {
        return false;
    }
    return true;
}

bool CreateProcess(char* target) 
{
    if (g_Context.Client->CreateProcess(0, target, DEBUG_ONLY_THIS_PROCESS | CREATE_NEW_CONSOLE) != S_OK) 
    {
        return false;
    }
    return true;
}

int WaitForEvent() 
{
    HRESULT status;
    while ((status = g_Context.Control->WaitForEvent(DEBUG_WAIT_DEFAULT, 
                                                INFINITE)) == S_FALSE);
    if (FAILED(status))
    {
        return LD_STATUS_DEAD;
    }

    ULONG type, process_id, thread_id;
    g_Context.Control->GetLastEventInformation(&type, &process_id, &thread_id, NULL, 0, NULL, NULL, 0, NULL);

    int ret;

    switch (type)
    {
    case DEBUG_EVENT_BREAKPOINT: ret = LD_STATUS_BREAKPOINT; break;
    case DEBUG_EVENT_EXCEPTION: ret = LD_STATUS_EXCEPTION; break;
    case DEBUG_EVENT_CREATE_PROCESS: ret = LD_STATUS_CREATE_PROCESS; break;
    case DEBUG_EVENT_EXIT_PROCESS: ret = LD_STATUS_EXIT_PROCESS; break;
    case DEBUG_EVENT_LOAD_MODULE: ret = LD_STATUS_LOAD_MODULE; break;
    default: ret = LD_STATUS_ERROR;
    }

    return ret;
}

ULONG GetNumLoadedModules() 
{
    ULONG loaded, unloaded;
    if (g_Context.Symbols->GetNumberModules(&loaded, &unloaded) != S_OK)
    {
        return 0;
    }
    return loaded;
}

std::vector<Module> GetLoadedModules(ULONG numModules) 
{
    std::vector<Module> ret;
    PDEBUG_MODULE_PARAMETERS params = new DEBUG_MODULE_PARAMETERS[numModules];

    if (g_Context.Symbols->GetModuleParameters(numModules, NULL, 0, params) != S_OK)
    {
        return ret;
    }

    ULONG i;
    for (i = 0; i < numModules; i++)
    {
        char* modName = new char[params[i].ModuleNameSize];
        char* imageName = new char[params[i].ImageNameSize];
        if (g_Context.Symbols->GetModuleNames(
            DEBUG_ANY_ID, params[i].Base, 
            imageName, params[i].ImageNameSize, NULL,
            modName, params[i].ModuleNameSize, NULL,
            NULL, 0, NULL) != S_OK)
        {
            delete[] modName;
            delete[] imageName;
            break;
        }
        Module mod = {modName, imageName, params[i].Base, params[i].Size};
        ret.emplace_back(mod);
        delete[] modName;
        delete[] imageName; 
    }
    delete[] params;
    return ret;
}

bool Debugger::Open(char *target) 
{
    if (!DLLInit()) return false;
    if (!CreateInterfaces()) return false;
    if (!CreateProcess(target)) return false;
    if (!InitSymbols()) return false;
    return true;
}

void Debugger::Close() 
{
    Terminate();
}

int Debugger::Wait() 
{
    return WaitForEvent();
}

uint32_t Debugger::ReadMemory(uintptr_t Offset, uint8_t* Buffer, size_t Size)
{
    ULONG BytesRead = 0;
    if (g_Context.DataSpaces->ReadVirtual(Offset, Buffer, Size, &BytesRead) != S_OK)
    {
        return 0;
    }
    return BytesRead;
}

std::vector<Module> Debugger::GetModules()
{
    return GetLoadedModules(GetNumLoadedModules());
}

uintptr_t Debugger::GetProcessBase()
{
    return g_Context.BaseAddress;
}

uint32_t Debugger::GetProgramSize()
{
    return g_Context.ProgramSize;
}

} // namespace LlamaDebug
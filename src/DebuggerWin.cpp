#include "LlamaDebug/Debugger.h"

#include <Windows.h>
#include <DbgEng.h>
#include <memory>
#include <cstdio>

namespace LlamaDebug 
{
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
    OutputCallbacks(Debugger* debugger) : m_debugger(debugger) {}

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
        if (m_debugger->getOutputCallback())
        {
            m_debugger->getOutputCallback()(Text);
        }
        return S_OK;
    }

    Debugger *m_debugger;
};

class Debugger::Impl
{
public:
    Impl(Debugger* debugger) : 
        m_debugger(debugger),
        m_outputCallbacks(debugger) {}
    ~Impl() {}

    bool createInterfaces()
    {
        if (DebugCreate(__uuidof(IDebugClient), 
                                (void**)&m_client) != S_OK) 
        {
            goto fail;
        }

        if (m_client->QueryInterface(__uuidof(IDebugControl), 
                                            (void**)&m_control) != S_OK) 
        {
            goto fail;
        }

        if (m_client->QueryInterface(__uuidof(IDebugDataSpaces), 
                                            (void**)&m_dataSpaces) != S_OK) 
        {
            goto fail;
        }

        if (m_client->QueryInterface(__uuidof(IDebugRegisters), 
                                            (void**)&m_registers) != S_OK) 
        {
            goto fail;
        }

        if (m_client->QueryInterface(__uuidof(IDebugSymbols),
                                              (void**)&m_symbols) != S_OK)
        {
            goto fail;
        }

        if (m_client->SetEventCallbacks(&m_eventCallbacks) != S_OK) 
        {
            goto fail;
        }

        if (m_client->SetOutputCallbacks(&m_outputCallbacks) != S_OK)
        {
            goto fail;
        }

        return true;
    fail:
        terminate();
        return false;
    }

    void terminate()
    {
        m_client->EndSession(DEBUG_END_PASSIVE);
        if (m_symbols)
        {
            m_symbols->Release();
        }

        if (m_registers) 
        {
            m_registers->Release();
        }

        if (m_dataSpaces) 
        {
            m_dataSpaces->Release();
        }

        if (m_control) 
        {
            m_control->Release();
        }

        if (m_client) 
        {
            m_client->Release();
        }
    }

    bool createProcess(char* target) 
    {
        if (m_client->CreateProcess(0, target, DEBUG_ONLY_THIS_PROCESS | CREATE_NEW_CONSOLE) != S_OK) 
        {
            return false;
        }
        return true;
    }

    int waitForEvent() 
    {
        HRESULT status;
        while ((status = m_control->WaitForEvent(DEBUG_WAIT_DEFAULT, 
                                                 INFINITE)) == S_FALSE);
        if (FAILED(status))
        {
            return LD_STATUS_DEAD;
        }

        ULONG type, process_id, thread_id;
        m_control->GetLastEventInformation(&type, &process_id, &thread_id, NULL, 0, NULL, NULL, 0, NULL);

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

    ULONG getNumLoadedModules() 
    {
        ULONG loaded, unloaded;
        if (m_symbols->GetNumberModules(&loaded, &unloaded) != S_OK)
        {
            return 0;
        }
        return loaded;
    }

    std::vector<Module> getModules(ULONG numModules) 
    {
        std::vector<Module> ret;
        PDEBUG_MODULE_PARAMETERS params = new DEBUG_MODULE_PARAMETERS[numModules];

        if (m_symbols->GetModuleParameters(numModules, NULL, 0, params) != S_OK)
        {
            return ret;
        }

        ULONG i;
        for (i = 0; i < numModules; i++)
        {
            char* modName = new char[params[i].ModuleNameSize];
            char* imageName = new char[params[i].ImageNameSize];
            if (m_symbols->GetModuleNames(
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

    // Reference to parent class
    Debugger* m_debugger;
    // Debugger COM Interfaces
    IDebugClient* m_client;
    IDebugControl* m_control;
    IDebugDataSpaces* m_dataSpaces;
    IDebugRegisters* m_registers;
    IDebugSymbols* m_symbols;
    // Callbacks
    EventCallbacks m_eventCallbacks;
    OutputCallbacks m_outputCallbacks;
};

Debugger::Debugger() : p_impl{new Impl(this)} { }

bool Debugger::open(char *target) 
{
    if (!p_impl->createInterfaces()) return false;
    if (!p_impl->createProcess(target)) return false;
    return true;
}

void Debugger::close() 
{
    p_impl->terminate();
}

int Debugger::wait() 
{
    return p_impl->waitForEvent();
}

std::vector<Module> Debugger::getModules()
{
    return p_impl->getModules(p_impl->getNumLoadedModules());
}

} // namespace LlamaDebug
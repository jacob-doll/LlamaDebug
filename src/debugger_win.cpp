#include <llama_debug/debugger.h>

#include <Windows.h>
#include <DbgEng.h>
#include <memory>
#include <map>
#include <cstdio>

namespace llama_debug {

typedef HRESULT(__stdcall *DebugCreate_t)(
  _In_ REFIID InterfaceId,
  _Out_ PVOID *Interface);

static DebugCreate_t DLLDebugCreate = NULL;

struct windbg_context
{
  uintptr_t BaseAddress;
  uint32_t ModuleSize;
  IDebugClient *Client;
  IDebugControl *Control;
  IDebugDataSpaces *DataSpaces;
  IDebugRegisters *Registers;
  IDebugSymbols *Symbols;
  std::map<uintptr_t, ld_breakpoint> breakpoints;
};

static windbg_context g_context;

class EventCallbacks : public DebugBaseEventCallbacks
{
public:
  STDMETHOD_(ULONG, AddRef)
  (
    THIS)
  {
    return 1;
  }

  STDMETHOD_(ULONG, Release)
  (
    THIS)
  {
    return 0;
  }

  STDMETHOD(GetInterestMask)
  (
    THIS_
      _Out_ PULONG Mask)
  {
    *Mask = DEBUG_EVENT_BREAKPOINT | DEBUG_EVENT_EXCEPTION | DEBUG_EVENT_CREATE_PROCESS | DEBUG_EVENT_EXIT_PROCESS | DEBUG_EVENT_LOAD_MODULE | DEBUG_EVENT_SYSTEM_ERROR;
    return S_OK;
  }

  STDMETHOD(Breakpoint)
  (
    THIS_
      _In_ PDEBUG_BREAKPOINT Bp)
  {
    return DEBUG_STATUS_BREAK;
  }

  STDMETHOD(Exception)
  (
    THIS_
      _In_ PEXCEPTION_RECORD64 Exception,
    _In_ ULONG FirstChance)
  {
    return DEBUG_STATUS_BREAK;
  }

  STDMETHOD(CreateProcess)
  (
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
    _In_ ULONG64 StartOffset)
  {
    g_context.BaseAddress = (uintptr_t)BaseOffset;
    g_context.ModuleSize = ModuleSize;
    return DEBUG_STATUS_BREAK;
  }

  STDMETHOD(ExitProcess)
  (
    THIS_
      _In_ ULONG ExitCode)
  {
    return DEBUG_STATUS_BREAK;
  }

  STDMETHOD(LoadModule)
  (
    THIS_
      _In_ ULONG64 ImageFileHandle,
    _In_ ULONG64 BaseOffset,
    _In_ ULONG ModuleSize,
    _In_ PCSTR ModuleName,
    _In_ PCSTR ImageName,
    _In_ ULONG CheckSum,
    _In_ ULONG TimeDateStamp)
  {
    return DEBUG_STATUS_BREAK;
  }

  STDMETHOD(SystemError)
  (
    THIS_
      _In_ ULONG Error,
    _In_ ULONG Level)
  {
    return DEBUG_STATUS_BREAK;
  }
};

class OutputCallbacks : public IDebugOutputCallbacks
{
public:
  STDMETHOD(QueryInterface)
  (
    THIS_
      _In_ REFIID InterfaceId,
    _Out_ PVOID *Interface)
  {
    *Interface = NULL;

    if (IsEqualIID(InterfaceId, __uuidof(IUnknown)) || IsEqualIID(InterfaceId, __uuidof(IDebugOutputCallbacks))) {
      *Interface = (IDebugOutputCallbacks *)this;
      AddRef();
      return S_OK;
    } else {
      return E_NOINTERFACE;
    }
  }

  STDMETHOD_(ULONG, AddRef)
  (
    THIS)
  {
    return 1;
  }

  STDMETHOD_(ULONG, Release)
  (
    THIS)
  {
    return 0;
  }

  STDMETHOD(Output)
  (
    THIS_
      IN ULONG Mask,
    IN PCSTR Text)
  {
    printf("%s", Text);
    return S_OK;
  }
};

static EventCallbacks g_event_callbacks;
static OutputCallbacks g_output_callbacks;

void ld_debug_close()
{
  g_context.Client->EndSession(DEBUG_END_PASSIVE);
  if (g_context.Symbols) {
    g_context.Symbols->Release();
  }

  if (g_context.Registers) {
    g_context.Registers->Release();
  }

  if (g_context.DataSpaces) {
    g_context.DataSpaces->Release();
  }

  if (g_context.Control) {
    g_context.Control->Release();
  }

  if (g_context.Client) {
    g_context.Client->Release();
  }
}

static bool DLL_init()
{
  if (DLLDebugCreate)
    return true;
  HMODULE h = LoadLibrary(TEXT("dbgeng.dll"));
  if (!h)
    return false;

  DLLDebugCreate = (DebugCreate_t)GetProcAddress(h, "DebugCreate");
  if (!DLLDebugCreate)
    return false;
  return true;
}

static bool create_interfaces()
{
  if (DLLDebugCreate(
        __uuidof(IDebugClient),
        (void **)&g_context.Client)
      != S_OK) {
    goto fail;
  }

  if (g_context.Client->QueryInterface(
        __uuidof(IDebugControl),
        (void **)&g_context.Control)
      != S_OK) {
    goto fail;
  }

  if (g_context.Client->QueryInterface(
        __uuidof(IDebugDataSpaces),
        (void **)&g_context.DataSpaces)
      != S_OK) {
    goto fail;
  }

  if (g_context.Client->QueryInterface(
        __uuidof(IDebugRegisters),
        (void **)&g_context.Registers)
      != S_OK) {
    goto fail;
  }

  if (g_context.Client->QueryInterface(
        __uuidof(IDebugSymbols),
        (void **)&g_context.Symbols)
      != S_OK) {
    goto fail;
  }

  if (g_context.Client->SetEventCallbacks(&g_event_callbacks) != S_OK) {
    goto fail;
  }

  if (g_context.Client->SetOutputCallbacks(&g_output_callbacks) != S_OK) {
    goto fail;
  }

  return true;
fail:
  ld_debug_close();
  return false;
}

bool ld_debug_open(char *target)
{
  if (!DLL_init())
    return false;
  if (!create_interfaces())
    return false;
  if (g_context.Symbols->SetSymbolPath(
        "cache*;srv*https://msdl.microsoft.com/download/symbols")
      != S_OK) {
    return false;
  }

  if (g_context.Client->CreateProcess(
        0,
        target,
        DEBUG_ONLY_THIS_PROCESS | CREATE_NEW_CONSOLE)
      != S_OK) {
    return false;
  }
  return true;
}

int ld_debug_wait()
{
  HRESULT status;
  while ((status = g_context.Control->WaitForEvent(
            DEBUG_WAIT_DEFAULT,
            INFINITE))
         == S_FALSE)
    ;

  if (FAILED(status))
    return LD_STATUS_DEAD;

  ULONG type, process_id, thread_id;
  g_context.Control->GetLastEventInformation(
    &type,
    &process_id,
    &thread_id,
    NULL,
    0,
    NULL,
    NULL,
    0,
    NULL);

  int ret;

  switch (type) {
  case DEBUG_EVENT_BREAKPOINT:
    ret = LD_STATUS_BREAKPOINT;
    break;
  case DEBUG_EVENT_EXCEPTION:
    ret = LD_STATUS_EXCEPTION;
    break;
  case DEBUG_EVENT_CREATE_PROCESS:
    ret = LD_STATUS_CREATE_PROCESS;
    break;
  case DEBUG_EVENT_EXIT_PROCESS:
    ret = LD_STATUS_EXIT_PROCESS;
    break;
  case DEBUG_EVENT_LOAD_MODULE:
    ret = LD_STATUS_LOAD_MODULE;
    break;
  default:
    ret = LD_STATUS_ERROR;
  }

  return ret;
}

std::vector<ld_module> ld_get_modules()
{
  ULONG loaded, unloaded;
  std::vector<ld_module> ret;

  if (g_context.Symbols->GetNumberModules(&loaded, &unloaded) != S_OK) {
    return ret;
  }

  PDEBUG_MODULE_PARAMETERS params = new DEBUG_MODULE_PARAMETERS[loaded];

  if (g_context.Symbols->GetModuleParameters(
        loaded, NULL, 0, params)
      != S_OK) {
    return ret;
  }

  ULONG i;
  for (i = 0; i < loaded; i++) {
    char *modName = new char[params[i].ModuleNameSize];
    char *imageName = new char[params[i].ImageNameSize];
    if (g_context.Symbols->GetModuleNames(
          DEBUG_ANY_ID, params[i].Base, imageName, params[i].ImageNameSize, NULL, modName, params[i].ModuleNameSize, NULL, NULL, 0, NULL)
        != S_OK) {
      delete[] modName;
      delete[] imageName;
      break;
    }
    ld_module mod = { modName, imageName, params[i].Base, params[i].Size };
    ret.emplace_back(mod);
    delete[] modName;
    delete[] imageName;
  }
  delete[] params;
  return ret;
}

uintptr_t ld_get_process_base()
{
  return g_context.BaseAddress;
}

uint32_t ld_read_virtual(uintptr_t offset, uint8_t *buffer, size_t size)
{
  ULONG BytesRead = 0;
  if (g_context.DataSpaces->ReadVirtual(
        offset, buffer, (ULONG)size, &BytesRead)
      != S_OK) {
    return 0;
  }
  return BytesRead;
}

ld_breakpoint *ld_add_breakpoint(uintptr_t addr)
{
  static ULONG index = 0;
  if (g_context.breakpoints.count(addr) > 0) {
    return &g_context.breakpoints.at(addr);
  }

  HRESULT status;
  PDEBUG_BREAKPOINT bp;
  while ((status = g_context.Control->AddBreakpoint(
            DEBUG_BREAKPOINT_CODE, index, &bp))
         == E_INVALIDARG) {
    ++index;
  }
  if (status != S_OK) {
    return nullptr;
  }
  bp->AddFlags(DEBUG_BREAKPOINT_ENABLED);
  bp->SetOffset(addr);
  g_context.breakpoints[addr] = { index, addr, 0, true };
  return &g_context.breakpoints.at(addr);
}

}// namespace llama_debug

#include <llama_debug/debugger.h>

#include <Windows.h>
#include <DbgEng.h>

namespace llama_debug {

typedef HRESULT(__stdcall *DebugCreate_t)(
  _In_ REFIID InterfaceId,
  _Out_ PVOID *Interface);

static DebugCreate_t DLLDebugCreate = NULL;

struct ld_debugger::ld_debug_ctx
{
  ld_debug_ctx(ld_debugger *debugger)
    : debugger(debugger)
  {}
  ld_debugger *debugger;
  uintptr_t base_address;
  uint32_t module_size;
  IDebugClient *client;
  IDebugControl *control;
  IDebugDataSpaces *data_spaces;
  IDebugRegisters *registers;
  IDebugSymbols *symbols;
  IDebugEventCallbacks *event_callbacks;
  IDebugOutputCallbacks *output_callbacks;
  std::map<uintptr_t, ld_breakpoint> breakpoints;
};

class EventCallbacks : public DebugBaseEventCallbacks
{
public:
  EventCallbacks(ld_debugger::ld_debug_ctx *ctx)
    : m_ctx(ctx)
  {}

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
    if (m_ctx->debugger->get_breakpooint_cb()) {
      uintptr_t address;
      Bp->GetOffset(&address);
      m_ctx->debugger->get_breakpooint_cb()(m_ctx->breakpoints.at(address));
    }
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
    m_ctx->base_address = (uintptr_t)BaseOffset;
    m_ctx->module_size = ModuleSize;
    if (m_ctx->debugger->get_process_create_cb()) {
      m_ctx->debugger->get_process_create_cb()(ModuleName, BaseOffset, ModuleSize);
    }
    return DEBUG_STATUS_BREAK;
  }

  STDMETHOD(ExitProcess)
  (
    THIS_
      _In_ ULONG ExitCode)
  {
    if (m_ctx->debugger->get_process_exit_cb()) {
      m_ctx->debugger->get_process_exit_cb()(ExitCode);
    }
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
    if (m_ctx->debugger->get_module_load_cb()) {
      m_ctx->debugger->get_module_load_cb()(ModuleName, BaseOffset, ModuleSize);
    }
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

private:
  ld_debugger::ld_debug_ctx *m_ctx;
};

class OutputCallbacks : public IDebugOutputCallbacks
{
public:
  OutputCallbacks(ld_debugger::ld_debug_ctx *ctx)
    : m_ctx(ctx)
  {}

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
    if (m_ctx->debugger->get_output_cb()) {
      m_ctx->debugger->get_output_cb()(Text);
    }
    return S_OK;
  }

private:
  ld_debugger::ld_debug_ctx *m_ctx;
};

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

ld_debugger::ld_debugger()
  : m_ctx(new ld_debugger::ld_debug_ctx(this)),
    m_open(false)
{}

ld_debugger::~ld_debugger() {}

bool ld_debugger::open(char *target)
{
  if (!DLL_init()) {
    return false;
  }

  if (DLLDebugCreate(
        __uuidof(IDebugClient),
        (void **)&m_ctx->client)
      != S_OK) {
    goto fail;
  }

  if (m_ctx->client->QueryInterface(
        __uuidof(IDebugControl),
        (void **)&m_ctx->control)
      != S_OK) {
    goto fail;
  }

  if (m_ctx->client->QueryInterface(
        __uuidof(IDebugDataSpaces),
        (void **)&m_ctx->data_spaces)
      != S_OK) {
    goto fail;
  }

  if (m_ctx->client->QueryInterface(
        __uuidof(IDebugRegisters),
        (void **)&m_ctx->registers)
      != S_OK) {
    goto fail;
  }

  if (m_ctx->client->QueryInterface(
        __uuidof(IDebugSymbols),
        (void **)&m_ctx->symbols)
      != S_OK) {
    goto fail;
  }

  m_ctx->event_callbacks = new EventCallbacks(m_ctx.get());
  m_ctx->output_callbacks = new OutputCallbacks(m_ctx.get());

  if (m_ctx->client->SetEventCallbacks(m_ctx->event_callbacks) != S_OK) {
    goto fail;
  }

  if (m_ctx->client->SetOutputCallbacks(m_ctx->output_callbacks) != S_OK) {
    goto fail;
  }

  if (m_ctx->symbols->SetSymbolPath(
        "cache*;srv*https://msdl.microsoft.com/download/symbols")
      != S_OK) {
    return false;
  }

  if (m_ctx->client->CreateProcess(
        0,
        target,
        DEBUG_ONLY_THIS_PROCESS | CREATE_NEW_CONSOLE)
      != S_OK) {
    return false;
  }

  m_open = true;
  return true;

fail:
  close();
  return false;
}

void ld_debugger::close()
{
  if (m_ctx->event_callbacks) {
    delete m_ctx->event_callbacks;
  }

  if (m_ctx->output_callbacks) {
    delete m_ctx->output_callbacks;
  }

  if (m_ctx->client) {
    m_ctx->client->EndSession(DEBUG_END_PASSIVE);
  }

  if (m_ctx->symbols) {
    m_ctx->symbols->Release();
  }

  if (m_ctx->registers) {
    m_ctx->registers->Release();
  }

  if (m_ctx->data_spaces) {
    m_ctx->data_spaces->Release();
  }

  if (m_ctx->control) {
    m_ctx->control->Release();
  }

  if (m_ctx->client) {
    m_ctx->client->Release();
  }

  m_open = false;
}

ld_debug_status ld_debugger::wait()
{
  HRESULT status;
  while ((status = m_ctx->control->WaitForEvent(
            DEBUG_WAIT_DEFAULT,
            INFINITE))
         == S_FALSE)
    ;

  if (FAILED(status))
    return DEAD;

  ULONG type, process_id, thread_id;
  m_ctx->control->GetLastEventInformation(
    &type,
    &process_id,
    &thread_id,
    NULL,
    0,
    NULL,
    NULL,
    0,
    NULL);

  ld_debug_status ret;

#undef ERROR
  switch (type) {
  case DEBUG_EVENT_BREAKPOINT:
    ret = BREAKPOINT;
    break;
  case DEBUG_EVENT_EXCEPTION:
    ret = EXCEPTION;
    break;
  case DEBUG_EVENT_CREATE_PROCESS:
    ret = CREATE_PROCESS;
    break;
  case DEBUG_EVENT_EXIT_PROCESS:
    ret = EXIT_PROCESS;
    break;
  case DEBUG_EVENT_LOAD_MODULE:
    ret = LOAD_MODULE;
    break;
  default:
    ret = ERROR;
  }

  return ret;
}

std::vector<ld_module> ld_debugger::get_modules()
{
  ULONG loaded, unloaded;
  std::vector<ld_module> ret;

  if (m_ctx->symbols->GetNumberModules(&loaded, &unloaded) != S_OK) {
    return ret;
  }

  PDEBUG_MODULE_PARAMETERS params = new DEBUG_MODULE_PARAMETERS[loaded];

  if (m_ctx->symbols->GetModuleParameters(
        loaded, NULL, 0, params)
      != S_OK) {
    return ret;
  }

  ULONG i;
  for (i = 0; i < loaded; i++) {
    char *modName = new char[params[i].ModuleNameSize];
    char *imageName = new char[params[i].ImageNameSize];
    if (m_ctx->symbols->GetModuleNames(
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

uintptr_t ld_debugger::get_process_base()
{
  return m_ctx->base_address;
}

uint32_t ld_debugger::read_virtual(uintptr_t offset, uint8_t *buffer, size_t size)
{
  ULONG BytesRead = 0;
  if (m_ctx->data_spaces->ReadVirtual(
        offset, buffer, (ULONG)size, &BytesRead)
      != S_OK) {
    return 0;
  }
  return BytesRead;
}

ld_breakpoint *ld_debugger::add_breakpoint(uintptr_t addr)
{
  static ULONG index = 0;
  if (m_ctx->breakpoints.count(addr) > 0) {
    return &m_ctx->breakpoints.at(addr);
  }

  HRESULT status;
  PDEBUG_BREAKPOINT bp;
  while ((status = m_ctx->control->AddBreakpoint(
            DEBUG_BREAKPOINT_CODE, index, &bp))
         == E_INVALIDARG) {
    ++index;
  }
  if (status != S_OK) {
    return nullptr;
  }
  bp->AddFlags(DEBUG_BREAKPOINT_ENABLED);
  bp->SetOffset(addr);
  m_ctx->breakpoints[addr] = { index, addr, 0, true };
  return &m_ctx->breakpoints.at(addr);
}

void ld_debugger::set_breakpoint_cb(const breakpoint_cb &callback)
{
  m_breakpoint_cb = callback;
}

void ld_debugger::set_exception_cb(const exception_cb &callback)
{
  m_exception_cb = callback;
}

void ld_debugger::set_process_create_cb(const process_create_cb &callback)
{
  m_process_create_cb = callback;
}

void ld_debugger::set_process_exit_cb(const process_exit_cb &callback)
{
  m_process_exit_cb = callback;
}

void ld_debugger::set_module_load_cb(const module_load_cb &callback)
{
  m_module_load_cb = callback;
}

void ld_debugger::set_output_cb(const output_cb &callback)
{
  m_output_cb = callback;
}

}// namespace llama_debug

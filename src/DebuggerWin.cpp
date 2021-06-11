#include "LlamaDebug/Debugger.h"

#include <Windows.h>
#include <DbgEng.h>
#include <memory>
#include <cstdio>

namespace LlamaDebug 
{

class EventCallbacks : public DebugBaseEventCallbacks 
{
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

class Debugger::Impl
{
public:
    Impl() {}
    ~Impl() {}

    bool create_interfaces()
    {
        HRESULT status;
        if ((status = DebugCreate(__uuidof(IDebugClient), 
                                (void**)&m_client)) != S_OK) 
        {
            goto fail;
        }

        if ((status = m_client->QueryInterface(__uuidof(IDebugControl), 
                                            (void**)&m_control)) != S_OK) 
        {
            goto fail;
        }

        if ((status = m_client->QueryInterface(__uuidof(IDebugDataSpaces), 
                                            (void**)&m_data_spaces)) != S_OK) 
        {
            goto fail;
        }

        if ((status = m_client->QueryInterface(__uuidof(IDebugRegisters), 
                                            (void**)&m_registers)) != S_OK) 
        {
            goto fail;
        }

        if ((status = m_client->SetEventCallbacks(&m_event_callbacks)) != S_OK) 
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
        if (m_registers) 
        {
            m_registers->Release();
        }

        if (m_data_spaces) 
        {
            m_data_spaces->Release();
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

    bool create_process(char* target) 
    {
        if (m_client->CreateProcess(0, target, DEBUG_ONLY_THIS_PROCESS | CREATE_NEW_CONSOLE) != S_OK) 
        {
            return false;
        }
        return true;
    }

    int wait_for_event() 
    {
        HRESULT status;
        while ((status = m_control->WaitForEvent(DEBUG_WAIT_DEFAULT, 
                                                 INFINITE)) == S_FALSE);
        if (FAILED(status))
        {
            return LD_STATUS_DEAD;
        }

        ULONG type, process_id, thread_id, description_used;
        char description[256];
        m_control->GetLastEventInformation(&type, &process_id, &thread_id, NULL, 0, NULL, description, 256, &description_used);
        printf("type: %ul, pid: %ul, tid: %ul, desc: %.*s\n", type, process_id, thread_id, description_used, description);
        return LD_STATUS_ERROR;
    }

private:
    IDebugClient* m_client;
    IDebugControl* m_control;
    IDebugDataSpaces* m_data_spaces;
    IDebugRegisters* m_registers;
    EventCallbacks m_event_callbacks;
};

Debugger::Debugger() : p_impl{new Impl()} { }

bool Debugger::open(char *target) 
{
    if (!p_impl->create_interfaces()) return false;
    if (!p_impl->create_process(target)) return false;
    return true;
}

void Debugger::close() 
{
    p_impl->terminate();
}

int Debugger::wait() 
{
    return p_impl->wait_for_event();
}

} // namespace LlamaDebug
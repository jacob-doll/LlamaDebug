import llama_debug as ld

def breakpoint(bp):
    print("breakpoint", bp)

def exception(e):
    print("exception", e)

def process_create(name, base, size):
    print("process_create", name, hex(base), hex(size))

def process_exit(code):
    print("process_exit", hex(code))

def module_load(name, base, size):
    print("module_load", name, hex(base), hex(size))

def output(text):
    print("output", text, end="")

def __main__():
    debugger = ld.debugger()
    if (not debugger.open("cmd.exe")):
        print("Could not create debugger!");
    print("Opened process!")

    debugger.set_breakpoint_cb(breakpoint)
    debugger.set_exception_cb(exception)
    debugger.set_process_create_cb(process_create)
    debugger.set_process_exit_cb(process_exit)
    debugger.set_module_load_cb(module_load)
    debugger.set_output_cb(output)
    
    while True:
        status = debugger.wait()
        if status == ld.debug_status.ERROR or \
            status == ld.debug_status.DEAD:
            break
        if status == ld.debug_status.LOAD_MODULE:
            continue
        user_input = input("> ")

    debugger.close()

if __name__ == "__main__":
    __main__()
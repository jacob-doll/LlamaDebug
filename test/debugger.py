import llama_debug as ld

def __main__():
    if (not ld.debug_open("cmd.exe")):
        print("Could not create debugger!");
    
    while True:
        status = ld.debug_wait()
        if status is ld.STATUS_DEAD:
            break
        if status is ld.STATUS_LOAD_MODULE:
            continue
        user_input = input("> ")

    ld.debug_close()

if __name__ == "__main__":
    __main__()
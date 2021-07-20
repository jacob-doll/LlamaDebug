import llama_debug as ld

def __main__():
    debugger = ld.debugger()
    if (not debugger.open("cmd.exe")):
        print("Could not create debugger!");
    print("Opened process!")
    
    while True:
        status = debugger.wait()
        if status is ld.STATUS_DEAD:
            break
        if status is ld.STATUS_LOAD_MODULE:
            continue
        user_input = input("> ")

    debugger.close()

if __name__ == "__main__":
    __main__()
from LlamaDebug import *

def __main__():
    if (not Debugger.Instance().Open("cmd.exe")):
        print("Could not create debugger!");
    
    while True:
        status = Debugger.Instance().Wait()
        if status is LD_STATUS_DEAD:
            break
        if status is LD_STATUS_LOAD_MODULE:
            continue
        user_input = input("> ")

    Debugger.Instance().Close()

if __name__ == "__main__":
    __main__()
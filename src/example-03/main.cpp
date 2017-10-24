#include <csetjmp>
#include <csignal>
#include <cstdlib>
#include <iostream>
#include <Windows.h>
#include <process.h>

jmp_buf env;

void on_sigabrt(int signum)
{
    longjmp(env, 1);
}

void try_and_catch_abort(void (*func)(void))
{
    if (setjmp(env) == 0) {
        signal(SIGABRT, &on_sigabrt);
        (*func)();
    } else {
        std::cout << "aborted\n";
    }
}

void do_stuff()
{
    std::cout << "step 1\n";
    std::cout << "step 2\n";
}



void do_stuff_aborted()
{
    DWORD exceptionCode;
    __try
    {   
        std::cout << "step 1\n";
        int i = 13;  
        int j = 0;  
        int k = i / j;  

        std::cout << k << std::endl;
        //abort();
        std::cout << "step 2\n";
    }
    __except(exceptionCode = (DWORD)GetExceptionInformation(),
        EXCEPTION_EXECUTE_HANDLER)
    {
        ExitThread(exceptionCode);
    }

    
}

extern "C" UINT __stdcall run_lua_in_thread(LPVOID lParam)
{
    try_and_catch_abort(&do_stuff_aborted);
    try_and_catch_abort(&do_stuff);
    return 0;
}

int main()
{
    HANDLE hThread;
    hThread = (HANDLE)_beginthreadex(NULL, 1024 * 1024 * 2, run_lua_in_thread, NULL, 0, NULL);

    if (hThread) {
        SetThreadPriority(hThread, THREAD_PRIORITY_NORMAL);
        CloseHandle(hThread);
        hThread = NULL;
    }

    system("pause");
    return 0;
}
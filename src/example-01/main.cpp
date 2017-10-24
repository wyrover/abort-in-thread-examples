#if _MSC_VER >= 1400
//  Following 8 lines: workaround for a bug in some older SDKs
#pragma push_macro("_interlockedbittestandset")
#pragma push_macro("_interlockedbittestandreset")
#pragma push_macro("_interlockedbittestandset64")
#pragma push_macro("_interlockedbittestandreset64")
#define _interlockedbittestandset _local_interlockedbittestandset
#define _interlockedbittestandreset _local_interlockedbittestandreset
#define _interlockedbittestandset64 _local_interlockedbittestandset64
#define _interlockedbittestandreset64 _local_interlockedbittestandreset64
#include <intrin.h> // to force the header not to be included elsewhere
#pragma pop_macro("_interlockedbittestandreset64")
#pragma pop_macro("_interlockedbittestandset64")
#pragma pop_macro("_interlockedbittestandreset")
#pragma pop_macro("_interlockedbittestandset")
#endif

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <signal.h>
#include <csetjmp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>

jmp_buf env;

void SignalHandler(int signal)
{
    longjmp(env, 1);
}

extern "C" UINT __stdcall run_lua_in_thread(LPVOID lParam)
{
    if (setjmp(env) == 0) {
        signal(SIGABRT, &SignalHandler);
        std::cout << "step 1\n";
        abort();
        std::cout << "step 2\n";
    }

    return 0;
}



int main(int argc, char** argv)
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

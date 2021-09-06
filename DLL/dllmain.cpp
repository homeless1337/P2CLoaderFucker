#include <Windows.h>
#include <Wininet.h>
#include <iostream>

#include "MinHook.h"
#pragma comment(lib, "wininet.lib")

HINTERNET(*InternetOpenUrlAOrig)(
    HINTERNET hInternet,
    LPCSTR lpszUrl,
    LPCSTR lpszHeaders,
    DWORD dwHeadersLength,
    DWORD dwFlags,
    DWORD_PTR dwContext
);

HINTERNET InternetOpenUrlA_Hooked(
    HINTERNET hInternet,
    LPCSTR lpszUrl,
    LPCSTR lpszHeaders,
    DWORD dwHeadersLength,
    DWORD dwFlags,
    DWORD_PTR dwContext
)
{
    printf("function called! \n");
    printf("[+] DLLlink -> %s\n", lpszUrl);

    Sleep(-1);

    return InternetOpenUrlAOrig(hInternet, lpszUrl, lpszHeaders, dwHeadersLength, dwFlags, dwContext);
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{

    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        auto init = MH_Initialize();

        if(init != MH_OK)
            printf("Unable to hook.");

        auto base = GetModuleHandleA("Wininet.dll");
        auto addr = GetProcAddress(base, "InternetOpenUrlA");

        if (!base || !addr)
        {
            printf("Invalid base or address.");
        }

        std::cout << "Wininet base : " << base << std::endl;
        std::cout << "InternetOpenUrlA addr : " << addr << std::endl;

        MH_CreateHook(addr, InternetOpenUrlA_Hooked, (LPVOID*)&InternetOpenUrlAOrig);
        auto hook = MH_EnableHook(addr);
        if (hook != MH_OK)
            printf("Unable to hook.");
    }

    return TRUE;
}


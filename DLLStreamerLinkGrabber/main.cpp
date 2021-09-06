
#include <stdio.h>
#include <string>
#include <Windows.h>
#include <tchar.h>
#include <Wininet.h>
#include <filesystem>
#include <iostream>
#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "urlmon.lib")

std::string replaceAll(std::string subject, const std::string& search,
	const std::string& replace) {
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != std::string::npos) {
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
	return subject;
}

std::string DownloadString(std::string URL) {
	HINTERNET interwebs = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, NULL);
	HINTERNET urlFile;
	std::string rtn;
	if (interwebs) {
		urlFile = InternetOpenUrlA(interwebs, URL.c_str(), NULL, NULL, NULL, NULL);
		if (urlFile) {
			char buffer[2000];
			DWORD bytesRead;
			do {
				InternetReadFile(urlFile, buffer, 2000, &bytesRead);
				rtn.append(buffer, bytesRead);
				memset(buffer, 0, 2000);
			} while (bytesRead);
			InternetCloseHandle(interwebs);
			InternetCloseHandle(urlFile);
			std::string p = replaceAll(rtn, "|n", "\r\n");
			return p;
		}
	}
	InternetCloseHandle(interwebs);
	std::string p = replaceAll(rtn, "|n", "\r\n");
	return p;
}

int main()
{
	Sleep(2000);
	std::cout << DownloadString("https://pastebin.com/raw/K5ZvsHnk").c_str() << std::endl;

	/*auto bytes = DownloadString("HTTPS://example.COM/EOWEWOKEWOP.DLL").c_str();
	PBYTE* bytes = (PBYTE*)bytes; //example
*/

	getchar();
}
#include <iostream>
#include <cstring>
#include <fstream>

#include <windows.h>
#include <wincred.h>


void read_cred()
{
	PCREDENTIALW* cred = new PCREDENTIALW;
	LPCWSTR name = L"TPCI_TCG_APP_REF_TOK";
	CredRead(name, CRED_TYPE_GENERIC, 0, cred);
	wchar_t* valueW = (wchar_t*)(**cred).CredentialBlob;
	char* valueA = new char[4096];
	WideCharToMultiByte(CP_ACP, 0, valueW, -1, valueA, 4096, nullptr, nullptr);
	std::cout << "Token found. Run this program in Wine with the following arguments:" << std::endl << std::endl;
	std::cout << "Cred.exe write " << valueA << std::endl;
	delete cred;
	delete[] valueA;
}

wchar_t* convertCharArrayToLPCWSTR(const char* value)
{
	size_t len = strlen(value) + 1;
	wchar_t* wString = new wchar_t[4096];
	MultiByteToWideChar(CP_ACP, 0, value, -1, wString, 4096);
	return wString;
}

void write_cred(LPCWSTR value)
{
	LPWSTR name = convertCharArrayToLPCWSTR("TPCI_TCG_APP_REF_TOK");
	CREDENTIALW cred;
	cred.Flags = 0;
	cred.Type = CRED_TYPE_GENERIC;
	cred.TargetName = name;
	cred.Comment = 0;
	cred.LastWritten.dwLowDateTime = 3668410400;
	cred.LastWritten.dwHighDateTime = 30976481;
	cred.CredentialBlobSize = wcslen((wchar_t*)value) * 2;
	cred.CredentialBlob = (LPBYTE)value;
	cred.Persist = 2;
	cred.AttributeCount = 0;
	cred.Attributes = 0;
	cred.TargetAlias = 0;
	cred.UserName = 0;
	if (!CredWrite(&cred, 0))
	{
		std::cout << "write failed" << std::endl;
	}
}

int main(int argc, char *argv[])
{
    if (argc < 2 || argc > 3)
    {	
		std::cout << "Usage: " << argv[0] << "[read|write] [token]" << std::endl;
		return 1;
    }
	std::string cmd = argv[1];
	if (cmd == "read")
	{
		read_cred();
	} else if (cmd == "write")
	{
		if (argc != 3)
		{
			std::cout << "You need to pass a valid refreshToken when using the 'write' command." << std::endl;
			return 1;
		}
		LPCWSTR value = convertCharArrayToLPCWSTR(argv[2]);
		write_cred(value);
	}
}

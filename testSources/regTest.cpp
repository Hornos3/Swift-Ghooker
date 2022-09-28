#include <Windows.h>
#include <iostream>

const char* menu = "--------------------------------------\n"
	"Choose what you want to test:\n"
	"0. exit\n"
	"1. regedit monitor efficiency test\n"
	"2. opening nonexistent registry key\n"
	"3. opening, setting and deleting registry key related to self-starting programs\n"
	"4. closing registry key twice\n"
	"5. opening a registry key twice and set value\n";

HKEY regHandles[0x40];
wchar_t keys[0x800][0x11];

int main() {
	puts(menu);
	int choice;

	while (true) {
		puts("Your choice: ");
		std::cin >> choice;
		switch (choice) {
		case 0: {
			puts("Bye!");
			exit(0);
		}
		case 1: {
			puts("Start timing......");
			auto start_time = GetTickCount();
			srand(time(0));
			// 创建0x800个子key
			for (int idx = 0; idx < 0x800; idx++) {
				for (int i = 0; i < 0x10; i++) {
					char nextChar = rand() % 62;
					if (nextChar < 10)
						nextChar += 0x30;
					else if (nextChar < 36)
						nextChar += 0x41 - 10;
					else
						nextChar += 0x61 - 36;
					keys[idx][i] = nextChar;
				}
				keys[idx][0x11] = '\0';
			}
			// 创建0x40个key
			for (int i = 0; i < 0x40; i++) {
				wchar_t keyname[0x20];
				wsprintf(keyname, L"a_test_key\\key_%02d", i);
				RegCreateKeyEx(HKEY_CURRENT_USER, keyname, 0, NULL, REG_OPTION_NON_VOLATILE,
					KEY_ALL_ACCESS, NULL, regHandles + i, NULL);
			}
			for (int i = 0; i < 0x800; i++) {
				TCHAR Data[254];
				memcpy(Data, keys[i], 0x10);
				memset(Data, 0, sizeof(Data));
				size_t iLen = wcslen(Data);
				RegSetValueEx(regHandles[i % 0x40], keys[i], 0, REG_SZ, (CONST BYTE*)Data, sizeof(TCHAR) * iLen);
			}
			for (int i = 0; i < 0x800; i++) 
				RegDeleteValue(regHandles[i % 0x40], keys[i]);
			for (int i = 0; i < 0x40; i++) {
				wchar_t keyname[0x20];
				wsprintf(keyname, L"a_test_key\\key_%02d", i);
				RegCloseKey(regHandles[i]);
				RegDeleteKeyEx(HKEY_CURRENT_USER, keyname, KEY_WOW64_64KEY, 0);
			}
			RegDeleteKeyEx(HKEY_CURRENT_USER, L"a_test_key", KEY_WOW64_64KEY, 0);
			auto end_time = GetTickCount();
			std::cout << "Efficiency test completed.\n"
				"RegCreateKeyEx and RegDeleteKeyEx called individually for 64 times.\n"
				"RegSetValueEx and RegDeleteValue called individually for 2048 times.\n"
				"time cost: " << (end_time - start_time) << " ms.\n";
			break;
		}
		case 2: {
			HKEY hkey;
			RegOpenKeyEx(HKEY_CURRENT_USER, L"17657802543672524758021317890", 0, KEY_ALL_ACCESS, &hkey);
			break;
		}
		case 3: {
			HKEY hkey;
			RegOpenKeyEx(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_ALL_ACCESS, &hkey);
			BYTE* value = (BYTE*)L"C:\\Program Files (x86)\\Tencent\\QQ\\Bin\\QQ.exe";
			RegSetValueEx(hkey, L"QQ", 0, REG_SZ, value, lstrlen((LPCWSTR)value) * sizeof(WCHAR));
			RegDeleteValue(hkey, L"QQ");
			RegCloseKey(hkey);
			break;
		}
		case 4: {
			HKEY hkey;
			RegCreateKeyEx(HKEY_CURRENT_USER, L"a_test_key", 0, NULL, REG_OPTION_NON_VOLATILE, 
				KEY_ALL_ACCESS, NULL, &hkey, NULL);
			RegCloseKey(hkey);
			RegCloseKey(hkey);
			break;
		}
		case 5: {
			HKEY hkey[2];
			RegCreateKeyEx(HKEY_CURRENT_USER, L"a_test_key", 0, NULL, REG_OPTION_NON_VOLATILE,
				KEY_ALL_ACCESS, NULL, hkey, NULL);
			RegCreateKeyEx(HKEY_CURRENT_USER, L"a_test_key", 0, NULL, REG_OPTION_NON_VOLATILE,
				KEY_ALL_ACCESS, NULL, hkey + 1, NULL);
			const WCHAR* value[2] = { L"Software_Security" , L"https://github.com/Hornos3/Software_Security_Design"};
			RegSetValueEx(hkey[0], L"testKey", 0, REG_SZ, (BYTE*)value[0], lstrlen(value[0]) * 2);
			RegSetValueEx(hkey[1], L"testKey", 0, REG_SZ, (BYTE*)value[1], lstrlen(value[1]) * 2);
			RegDeleteValue(hkey[0], L"testKey");
			RegDeleteValue(HKEY_CURRENT_USER, L"a_test_key");
			RegDeleteKeyEx(HKEY_CURRENT_USER, L"a_test_key", KEY_WOW64_64KEY, 0);
			RegCloseKey(hkey[0]);
			RegCloseKey(hkey[1]);
			break;
		}
		}
	}
}
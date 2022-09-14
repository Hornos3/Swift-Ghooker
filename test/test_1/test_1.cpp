#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <tchar.h>
#include <windows.h>
using namespace std;

//typedef struct MessageBoxAArgs{
//	HWND hWnd;
//	LPCSTR lpText;
//	LPCSTR lpCaption;
//	UINT uType;
//}MessageBoxAArgs;
//
//typedef struct MessageBoxWArgs{
//	HWND hWnd;
//	LPCWSTR lpText;
//	LPCWSTR lpCaption;
//	UINT uType;
//}MessageBoxWArgs;
//
//MessageBoxAArgs getMessageBoxAArgs(){
//	HWND hWnd = nullptr;
//	LPCSTR lpText;
//	LPCSTR lpCaption;
//	UINT uType = MB_OK;
//	cout << "Please input some arguments of called function MessageBoxA (input '.' for default value):" << endl;
//	cout << "\tHWND hWnd = nullptr (you can't change)" << endl;
//	
//	cout << "\tLPCSTR lpText (MessageBoxA console): ";
//	string lpTextStr;
//	cin >> lpTextStr;
//	if(lpTextStr == ".")
//		lpTextStr = "MessageBoxA console";
//	lpText = lpTextStr.c_str();
//	
//	cout << "\tLPCSTR lpCaption (This is a test for MessageBoxA.): ";
//	string lpCaptionStr;
//	cin >> lpCaptionStr;
//	if(lpCaptionStr == ".")
//		lpCaptionStr = "This is a test for MessageBoxA.";
//	lpCaption = lpCaptionStr.c_str();
//	
//	cout << "\tUINT value = MB_OK (you can't change)" << endl;
//	return {hWnd, lpText, lpCaption, uType};
//} 
//
//MessageBoxWArgs getMessageBoxWArgs(){
//	HWND hWnd = nullptr;
//	LPCWSTR lpText;
//	LPCWSTR lpCaption;
//	UINT uType = MB_OK;
//	cout << "Please input some arguments of called function MessageBoxW (input '.' for default value):" << endl;
//	cout << "\tHWND hWnd = nullptr (you can't change)" << endl;
//	
//	cout << "\tLPCSTR lpText (MessageBoxW console): ";
//	string lpTextStr;
//	cin >> lpTextStr;
//	if(lpTextStr == ".")
//		lpTextStr = "MessageBoxW console";
//	lpText = (wchar_t*)(lpTextStr.c_str());
//	
//	cout << "\tLPCWSTR lpCaption (This is a test for MessageBoxW.)[at most 0x100 wide chars]: ";
//	wchar_t* lpCaptionStr = (wchar_t*)calloc(sizeof(wchar_t), 0x100);
//	wscanf(L"%256s", lpCaptionStr);
//	if(!strcmp((char*)lpCaptionStr, (char*)(L".")))
//		lpCaptionStr = L"This is a test for MessageBoxW.";
//	lpCaption = lpCaptionStr;
//	
//	cout << "\tUINT value = MB_OK (you can't change)" << endl;
//	return {hWnd, lpText, lpCaption, uType};
//} 

HANDLE hHeap = nullptr;
void* chunk = nullptr;
HANDLE fileHandle = INVALID_HANDLE_VALUE;

int main(){
	cout << "This is my test program for software security course design." << endl;
	cout << "Please choose what you want to test:" << endl;
	cout << "\t1. calling MessageBoxA" << endl;
	cout << "\t2. calling MessageBoxW" << endl;
	cout << "\t3. calling HeapCreate" << endl;
	cout << "\t4. calling HeapDestroy" << endl;
	cout << "\t5. calling HeapAlloc" << endl;
	cout << "\t6. calling HeapFree" << endl; 
	cout << "\t7. write anywhere" << endl;
	cout << "\t8. read anywhere" << endl;
	cout << "\t9. calling CreateFile" << endl;
	cout << "\t10. calling ReadFile" << endl;
	cout << "\t11. calling WriteFile" << endl;
	cout << "\t0. exit this program" << endl;
	printf("%p", malloc(0x123));
	int choice;
	while(true){
		cout << "Your choice: ";
		cin >> choice;
		switch(choice){
			case 1:{
				cout << "Ready to call MessageBoxA......" << endl;
//				MessageBoxAArgs args = getMessageBoxAArgs();
//				cout << "Argument selected, calling MessageBoxA......" << endl;
//				MessageBoxA(args.hWnd, args.lpText, args.lpCaption, args.uType);
				MessageBoxA(nullptr, "This is a test for MessageBoxA", "This is a title", MB_OK);
				break;
			}
			case 2:{
				cout << "Ready to call MessageBoxW......" << endl;
//				MessageBoxWArgs args = getMessageBoxWArgs();
//				cout << "Argument selected, calling MessageBoxW......" << endl;
//				MessageBoxW(args.hWnd, args.lpText, args.lpCaption, args.uType);
				MessageBoxW(nullptr, L"This is a test for MessageBoxW", L"This is a title", MB_OK);
				break;
			}
			case 3:{
				cout << "Ready to call HeapCreate......" << endl;
				hHeap = HeapCreate(0, 0x100, 0);
				break;
			}
			case 4:{
				cout << "Ready to call HeapDestroy......" << endl;
				if(!hHeap)
					cout << "Oops! You have no spare heap space," 
							"please call 'HeapCreate' first to get a heap space!" << endl;
				else{
					bool returnVal = HeapDestroy(hHeap);
					cout << "HeapDestroy called! return value: " << (returnVal ? "true" : "false") << endl;
					if(returnVal == 0){
						cout << GetLastError() << endl;
					}
					hHeap = nullptr;
				}
				break;
			}
			case 5:{
				cout << "Ready to call HeapAlloc......" << endl;
				if(!hHeap)
					cout << "Oops! You have no spare heap space," 
							"please call 'HeapCreate' first to get a heap space!" << endl;
				else{
					chunk = HeapAlloc(hHeap, 0, 0x80);
					cout << chunk << endl;
				}
				break;
			}
			case 6:{
				cout << "Ready to call HeapFree......" << endl;
				if(!chunk)
					cout << "Oops! You have no space heap chunk,"
							"please call 'HeapAlloc' first to get a heap chunk!" << endl;
				else{
					HeapFree(hHeap, 0, chunk);
					chunk = nullptr;
				}
				break;
			}
			case 7:{
				cout << "This is a backdoor for writing to any address." << endl;
				char* address = nullptr;
				scanf("%x", &address);
				char val = 0;
				cin >> hex >> val >> dec;
				*address = val;
				break;
			}
			case 8:{
				cout << "This is a backdoor for reading any address." << endl;
				char* address = nullptr;
				scanf("%x", &address);
				cout << hex << (int)*address << dec << endl;
				break;
			}
			case 9:{
				cout << "Ready to call CreateFileW......" << endl;
				fileHandle = CreateFileW(L"./exampleFile.txt", GENERIC_ALL, 0, nullptr, OPEN_ALWAYS, 0, nullptr);
				break;
			}
			case 10:{
				if(fileHandle == INVALID_HANDLE_VALUE){
					cout << "Oops! Invalid file handle." << endl;
					break;
				}
				cout << "Ready to call ReadFile......" << endl;
				char buf[0x10] = {0};
				DWORD bytesRead = 0;
				ReadFile(fileHandle, buf, 0xF, &bytesRead, nullptr);
				cout << "Content: " << buf << endl;
				break;
			}
			case 11:{
				if(fileHandle == INVALID_HANDLE_VALUE){
					cout << "Oops! Invalid file Handle." << endl;
					break;
				}
				cout << "Ready to call WriteFile......" << endl;
				char buf[] = "000000000000000";
				DWORD bytesWrite = 0;
				WriteFile(fileHandle, buf, 15, &bytesWrite, nullptr);
				break;
			} 
			case 0:{
				cout << "Bye!" << endl;
				exit(0);
			}
			default:{
				cout << "Invalid choice! Please input again." << endl; 
				break;
			}
		}
	}
	CloseHandle(fileHandle);
}

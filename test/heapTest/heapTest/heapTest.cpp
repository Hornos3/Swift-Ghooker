#include <cstdlib>
#include <random>
#include <iostream>
#include "windows.h"

void* chunks[0x1000];
void* largeChunks[0x1000];
int heapId[0x1000];
HANDLE heapHandles[0x100];

const char* menu = "--------------------------------------\n"
	"Choose what you want to test:\n"
	"0. exit\n"
	"1. heap monitor efficiency test\n"
	"2. double free detection <CAUSES CRUSH>\n"
	"3. double destroy detection <CAUSES CRUSH>\n"
	"4. heap spray through HeapAlloc\n"
	"5. freeing invalid chunk(no page fault) <CAUSES CRUSH>\n"
	"6. freeing invalid chunk with valid handle(page fault) <CAUSES CRUSH>\n"
	"7. freeing invalid chunk with invalid handle(page fault) <CAUSES CRUSH>\n"
	"8. freeing invalid chunk(damaging its control segment before HeapFree) <CAUSES CRUSH>\n"
	"9. destroying invalid handle(no page fault) <CAUSES CRUSH>\n"
	"10. destroying invalid handle(page fault) <CAUSES CRUSH>\n"
	"11. destroying invalid handle(damaging its control segment before HeapDestroy)\n"
	"12. destroying untracked handle(need input)\n"
	"--------------------------------------\n";

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
			for (int i = 0; i < 0x100; i++) 
				heapHandles[i] = HeapCreate(0, rand() % 0xFF000 + 0x1000, 0);
			for (int i = 0; i < 0x1000; i++) {
				int chunksize = rand() % 0xFA0 + 0x60;
				heapId[i] = rand() % 0x100;
				chunks[i] = HeapAlloc(heapHandles[heapId[i]], HEAP_ZERO_MEMORY, chunksize);
			}
			for (int i = 0; i < 0x1000; i++)
				HeapFree(heapHandles[heapId[i]], 0, chunks[i]);
			for (int i = 0; i < 0x100; i++)
				HeapDestroy(heapHandles[i]);
			auto end_time = GetTickCount();
			std::cout << "Efficiency test completed.\n"
				"HeapCreate and HeapDestroy called individually for 256 times.\n"
				"HeapAlloc and HeapFree called individually for 4096 times.\n"
				"time cost: " << (end_time - start_time) << " ms.\n";
			break;
		}
		case 2: {
			HANDLE handle = HeapCreate(0, rand() % 0xFF000 + 0x1000, 0);
			void* chunk = HeapAlloc(handle, 0, 0x100);
			HeapFree(handle, 0, chunk);
			HeapFree(handle, 0, chunk);		// free twice, will crush
			break;
		}
		case 3: {
			HANDLE handle = HeapCreate(0, 0x1000, 0);
			HeapDestroy(handle);
			HeapDestroy(handle);		// destroy twice, will crush
			break;
		}
		case 4: {
			int idx = 0;
			while (true) {
				largeChunks[idx++] = malloc(0x10000000);
				if (idx == 0x1000 || !largeChunks[idx-1])
					break;
			}
			while (true) {
				largeChunks[idx++] = malloc(0x100000);
				if (idx == 0x1000 || !largeChunks[idx - 1])
					break;
			}
			HANDLE lastHandle = HeapCreate(0, 0x100000, 0);
			std::cout << "Unable to alloc more memory!\n";
			for (int i = idx - 1; i >= 0; i--)
				free(largeChunks[i]);
			break;
		}
		case 5: {
			HANDLE handle = HeapCreate(0, 0x1000, 0);
			char* victimChunk = (char*)HeapAlloc(handle, 0, 0x100);
			HeapFree(handle, 0, victimChunk + 0x40);		// invalid address to free
			HeapDestroy(handle);
			break;
		}
		case 6: {
			HANDLE handle = HeapCreate(0, 0x1000, 0);
			HeapFree(handle, 0, (void*)0x123456);	// will crush
			break;
		}
		case 7: {
			HeapFree((void*)0x123456, 0, (void*)0x654321);
			break;
		}
		case 8: {
			HANDLE handle = HeapCreate(0, 0x1000, 0);
			char* victimChunk = (char*)HeapAlloc(handle, 0, 0x100);
			memset(victimChunk - 0x10, 0xcc, 0x10);
			HeapFree(handle, 0, victimChunk);
			break;
		}
		case 9: {
			HANDLE handle = HeapCreate(0, 0x5000, 0);
			HeapDestroy((char*)handle + 0x1000);		// invalid address to destroy
			HeapDestroy(handle);
			break;
		}
		case 10: {
			HeapDestroy((void*)0x123456);		// will crush
			break;
		}
		case 11: {
			HANDLE handle = HeapCreate(0, 0x1000, 0);
			memset(handle, 0xcc, 0x1000);
			HeapDestroy(handle);
		}
		case 12: {
			uint64_t address;
			std::cin >> std::hex >> address;
			HeapDestroy((void*)address);		// Destroy wherever you want
		}
		}
	}
}
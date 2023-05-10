#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <stdint.h>
#include <string.h>
#include <keyboard.h>

typedef int (*EntryPoint)();

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t pageSize = 0x1000;
static void* const sampleCodeModuleAddress = (void*)0x400000;
static void* const sampleDataModuleAddress = (void*)0x500000;

void
clearBSS(void* bssAddress, uint64_t bssSize)
{
	memset(bssAddress, 0, bssSize);
}

void*
getStackBase()
{
	return (void*)((uint64_t)&endOfKernel +
	               pageSize * 8        // The size of the stack itself, 32KiB
	               - sizeof(uint64_t)  // Begin at the top of the stack
	);
}

void*
initializeKernelBinary()
{
	char buffer[10];

	ncPrint("[x64BareBones]");
	ncNewline();

	ncPrint("CPU Vendor: ");
	ncPrint(cpuVendor(buffer));
	ncNewline();

	ncPrint("[Loading modules]");
	ncNewline();

	void* moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress,
	};
	loadModules(&endOfKernelBinary, moduleAddresses);

	ncPrint("[Done]");
	ncNewline();
	ncNewline();

	ncPrint("[Initializing kernel's binary]");
	ncNewline();

	clearBSS(&bss, &endOfKernel - &bss);

	ncPrint("  text: 0x");
	ncPrintHex((uint64_t)&text);
	ncNewline();
	ncPrint("  rodata: 0x");
	ncPrintHex((uint64_t)&rodata);
	ncNewline();
	ncPrint("  data: 0x");
	ncPrintHex((uint64_t)&data);
	ncNewline();
	ncPrint("  bss: 0x");
	ncPrintHex((uint64_t)&bss);
	ncNewline();

	ncPrint("[Done]");
	ncNewline();
	ncNewline();
	return getStackBase();
}

int
main()
{
	ncPrint("[Kernel Main]");
	ncNewline();
	ncPrint("  Sample code module at 0x");
	ncPrintHex((uint64_t)sampleCodeModuleAddress);
	ncNewline();
	ncPrint("  Calling the sample code module returned: ");
	ncPrintHex(((EntryPoint)sampleCodeModuleAddress)());
	ncNewline();
	ncNewline();

	ncPrint("  Sample data module at 0x");
	ncPrintHex((uint64_t)sampleDataModuleAddress);
	ncNewline();
	ncPrint("  Sample data module contents: ");
	ncPrint((char*)sampleDataModuleAddress);
	ncNewline();

	ncPrint("[Finished]");
	ncNewline();
	ncNewline();

	// ejercicio 2
	ncSetColor(0x2, 0xF);
	ncPrint("Arquitectura de las Computadoras");
	ncClearColor();
	ncNewline();

	// ejercicio 3
	ncPrintTime();

	// ejercicio 4
	// ncClear();
	// int i, teclahex = 0, teclahexant = teclahex;
	// for (int j = 1; j < 1000; j++) {
	// 	i = getKey();
	// 	teclahex = i;
	// 	if (teclahex != teclahexant) {
	// 		ncPrintHex(teclahex);
	// 		ncPrint(" ");
	// 		teclahexant = teclahex;
	// 	}
	// }
	for (int j = 1; j < 1000; j++)
		printChar();
	return 0;
}

#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <stdint.h>
#include <string.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t pageSize = 0x1000;

static void* const sampleCodeModuleAddress = (void*)0x400000;
static void* const sampleDataModuleAddress = (void*)0x500000;

typedef int (*EntryPoint)();

void
writeCharacter(unsigned char c,
               unsigned char fg,
               unsigned char bg,
               int x,
               int y)
{
	uint16_t attrib = (bg << 4) | (fg & 0x0F);
	volatile uint16_t* where;
	where = (volatile uint16_t*)0xB8000 + (y * 80 + x);
	*where = c | (attrib << 8);
}

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

	// ejercicio 1
	const char msg[] = "Arquitectura de las Computadoras";
	const unsigned int size = sizeof(msg) / sizeof(msg[0]);

	for (int i = 0; i < size; i++)
		writeCharacter(msg[i], 0x0 + i, 0xF, i, 24);

	return 0;
}

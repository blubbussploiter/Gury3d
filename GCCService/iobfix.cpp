#include <iostream>
#pragma comment(lib, "legacy_stdio_definitions.lib")
FILE iob[] = { *stdin, *stdout, *stderr };
extern "C" {
	FILE* __cdecl _iob(void) { return iob; }
}
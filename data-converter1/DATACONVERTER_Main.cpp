#include "DATACONVERTER_Internal.h"

int main(int argc, char **argv)
{
	InMap					inMap;
	OutMap				outMap;

	LOAD_Main(argv[1], &inMap);
	CONVERT_Main(&inMap, &outMap);
	PRINT_Main(argv[2], &outMap);

	return 0;
}

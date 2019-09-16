#include "DATACONVERTER_Internal.h"

int PRINT_Main(char *tarDir, OutMap *outMap)
{
	print_Vehicle(tarDir, &(outMap->vehicleList));
	printf("Print Vehicle\n");
	print_Link(tarDir, &(outMap->linkList));
	printf("Print Link\n");
	print_ConnectionCell(tarDir, &(outMap->connectionCellList));
	printf("Print ConnectionCell\n");


	return 0;
}

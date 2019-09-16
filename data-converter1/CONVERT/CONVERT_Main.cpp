#include "DATACONVERTER_Internal.h"

int CONVERT_Main(InMap	*inMap, OutMap *outMap)
{
	convert_ToVehicle(&(inMap->routeList), &(inMap->connectionMultiMap), &(inMap->edgeMap), &(outMap->vehicleList));
	convert_ToLink(&(inMap->edgeMap), &(inMap->netMap), &(outMap->linkList));
	convert_ToConnectionCell(&(inMap->nodeList), &(inMap->edgeMultiMap), &(inMap->connectionMultiMap), &(inMap->tlLogicMap), &(outMap->connectionCellList));

	printf("Complete Converting.\n");

	return 0;
}

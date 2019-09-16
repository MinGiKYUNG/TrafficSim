#include "DATACONVERTER_Internal.h"

int LOAD_Main(char *srcDir, InMap *inMap)
{

	load_Node(srcDir, &(inMap->nodeList));
	load_Edge(srcDir, &(inMap->edgeMap), &(inMap->edgeMultiMap));
	load_Net(srcDir, &(inMap->netMap));
	load_Connection(srcDir, &(inMap->connectionMultiMap));
	load_TlLogic(srcDir, &(inMap->tlLogicMap), &(inMap->connectionMultiMap));
	load_Route(srcDir, &(inMap->routeList));

	printf("Complete Loading.\n");
	return 0;
}

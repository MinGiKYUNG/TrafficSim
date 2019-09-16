#include "DATACONVERTER_Internal.h"

int convert_ToLink(EdgeMap *edgeMap, NetMap *netMap, LinkList *linkList)
{
	Edge			*edge;
	Link			*link;

	for (auto it = edgeMap->begin(); it != edgeMap->end(); it++)
	{
		edge = it->second;

		link = new Link;
		link->linkID = edge->id;
		link->numLanes = edge->numLanes;
		link->ffSpeed = edge->speed;

		if (netMap->find(edge->id) != netMap->end())
		{
			link->lenSection = netMap->find(edge->id)->second;
		}

		linkList->push_back(link);

	}

	return 0;
}

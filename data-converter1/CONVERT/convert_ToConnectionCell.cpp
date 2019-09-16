#include <string.h>
#include <limits.h>
#include "DATACONVERTER_Internal.h"


int convert_ToConnectionCell(NodeList *nodeList, EdgeMultiMap *edgeMultiMap, ConnectionMultiMap *connectionMultiMap, TlLogicMap *tlLogicMap, ConnectionCellList *connectionCellList)
{
	Node				*node;
	Edge				*edge;
	Connection		*connection;
	TlLogic			*tlLogic;
	ConnectionCell	*connectionCell;
	int				ccIDCount;	
	
	for (std::vector<Node*>::iterator it = nodeList->begin(); it != nodeList->end(); it++)
	{
		node = *it;
	
		ccIDCount = 0;

		auto rt = edgeMultiMap->equal_range(node->id);
		for (auto it = rt.first; it != rt.second; it++)
		{
			connectionCell = new ConnectionCell;
			memset(connectionCell, 0, sizeof(ConnectionCell));
			for (int i = 0; i < MAX_LANE; i++)
			{
				connectionCell->linkIndex[i] = INT_MAX;
				connectionCell->duration[i][0] = -1;
			}	
			

			edge = it->second;

			connectionCell->ccID = (node->id - ((node->id/100000000) *100000000)) * 10 + ccIDCount++;
			connectionCell->prevLinkID = edge->id;
			connectionCell->type = node->type;

			auto rrt = connectionMultiMap->equal_range(edge->id);
			for (auto iit = rrt.first; iit != rrt.second; iit++)
			{  
				connection = iit->second;
				connectionCell->fromLane[connectionCell->connectionNums] = connection->fromLane;																							               
				connectionCell->nextLinkID[connectionCell->connectionNums] = connection->to;
				connectionCell->nextLane[connectionCell->connectionNums++] = connection->toLane;

				if (connectionCell->type)
				{
					// process tl information
					if(tlLogicMap->find(node->id) != tlLogicMap->end())
					{
						tlLogic = tlLogicMap->find(node->id)->second;
						connectionCell->phaseNums = tlLogic->phaseNums;
						if (connection->linkIndex < connectionCell->linkIndex[connection->fromLane])
						{
							connectionCell->linkIndex[connection->fromLane] = connection->linkIndex;
							for (int i = 0; i < tlLogic->phaseNums; i++)
							{
								connectionCell->duration[connection->fromLane][i] = tlLogic->duration[i];
								if (tlLogic->state[i][connection->linkIndex] == 'G') connectionCell->state[connection->fromLane][i] = 1;
								else connectionCell->state[connection->fromLane][i] = 0;
							}

						}
					}

				}
			}
			for (int i = 0; i < MAX_LANE; i++)
			{
				if (connectionCell->duration[i][0] != -1) connectionCell->laneNums++;
			}
				
			connectionCellList->push_back(connectionCell);
		}
	}
	return 0;
}

#include <string.h>
#include <limits.h>
#include "DATACONVERTER_Internal.h"


int convert_ToVehicle(RouteList *routeList, ConnectionMultiMap *connectionMultiMap, EdgeMap *edgeMap, VehicleList *vehicleList)
{
	Route			*route;
	Connection	*connection;
	Vehicle 		*vehicle;

	for (std::vector<Route*>::iterator it = routeList->begin(); it != routeList->end(); it++)
	{
		route = *it;

		vehicle = new Vehicle;
		memset(vehicle, 0, sizeof(Vehicle));

		for (int i = 0; i < route->numofEdges; i++)
		{

			vehicle->id = route->id;
			vehicle->type = route->type;
			vehicle->depart = route->depart;
			vehicle->numofEdges = route->numofEdges;
			memcpy(vehicle->edges, route->edges, sizeof(int) * vehicle->numofEdges);

			vehicle->minTargetLane[i] = INT_MAX;
			vehicle->maxTargetLane[i] = INT_MIN;

			auto rt = connectionMultiMap->equal_range(route->edges[i]);
			if (rt.first != rt.second)
			{

				for (auto iit = rt.first; iit != rt.second; iit++)
				{
					connection = iit -> second;
					if (i == route->numofEdges - 1 || connection->to == route->edges[i+1])
					{
						if (connection->fromLane < vehicle->minTargetLane[i])
						{
							vehicle->minTargetLane[i] = connection->fromLane;
						}
						if (connection->fromLane > vehicle->maxTargetLane[i])
						{
							vehicle->maxTargetLane[i] = connection->fromLane;
						}
					}
				}
			}

			if (vehicle->minTargetLane[i] == INT_MAX) vehicle->minTargetLane[i] = 0;
			if (vehicle->maxTargetLane[i] == INT_MIN) 
			{
				if (edgeMap->find(route->edges[i]) != edgeMap->end())
				{
					vehicle->maxTargetLane[i] = edgeMap->find(route->edges[i])->second->numLanes;
				}
			}
		}
		vehicleList->push_back(vehicle);
	} 

	return 0;
}

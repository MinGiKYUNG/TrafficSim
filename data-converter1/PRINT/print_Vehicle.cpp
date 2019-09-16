#include <string.h>
#include "DATACONVERTER_Internal.h"

int print_Vehicle(char *tarDir, VehicleList *vehicleList)
{
	char			tarFile[MAX_FILENAME_LEN];
	Vehicle		*vehicle;
	FILE			*fp;

	strcpy(tarFile, tarDir);
	strcat(tarFile, "/Vehicle.dat");
	fp = fopen(tarFile, "w");

	for (std::vector<Vehicle*>::iterator it = vehicleList-> begin(); it != vehicleList->end(); it++)
	{
	 	vehicle = *it;
		fprintf(fp, "%d %d %d %d", vehicle->id, vehicle->type, vehicle->depart, vehicle->numofEdges);
		for (int i = 0; i < vehicle->numofEdges; i++)
		{
			fprintf(fp, " %d", vehicle->edges[i]);
		}

		for (int i = 0; i < vehicle->numofEdges; i++)
		{  
			fprintf(fp, " %d", vehicle->minTargetLane[i]);
		}  

      for (int i = 0; i < vehicle->numofEdges; i++)                     
		{                                   
			fprintf(fp, " %d", vehicle->maxTargetLane[i]);
		}

		fprintf(fp, "\n");

	}

	fclose(fp);

	return 0;
}

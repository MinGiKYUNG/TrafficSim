#include <string.h>
#include "DATACONVERTER_Internal.h"

int print_ConnectionCell(char *tarDir, ConnectionCellList *connectionCellList)
{
	char				tarFile[MAX_FILENAME_LEN];
	ConnectionCell	*connectionCell;
	FILE				*fp;

	strcpy(tarFile, tarDir);
	strcat(tarFile, "/ConnectionCell.dat");
	fp = fopen(tarFile, "w");

	for (std::vector<ConnectionCell*>::iterator it = connectionCellList-> begin(); it != connectionCellList->end(); it++)
	{
	 	connectionCell = *it;
		 
		fprintf(fp, "%d %d %d", connectionCell->ccID, connectionCell->prevLinkID, connectionCell->connectionNums);

		for (int i = 0; i < connectionCell->connectionNums; i++)
		{
			fprintf(fp, " %d %d %d", connectionCell->fromLane[i], connectionCell->nextLinkID[i], connectionCell->nextLane[i]);
		}  

		fprintf(fp, " %d", connectionCell-> type);

		if (connectionCell->type)
		{
			fprintf(fp, " %d", connectionCell->laneNums);
			fprintf(fp, " %d", connectionCell->phaseNums);
			for (int i = 0; i < MAX_LANE; i++)
			{
				if (connectionCell->duration[i][0] != -1)
				{
					fprintf(fp, " %d", i);
					for (int j = 0; j < connectionCell->phaseNums; j++)
					{
						fprintf(fp, " %d", connectionCell->duration[i][j]);
					}

					for (int j = 0; j < connectionCell->phaseNums; j++)
					{
						fprintf(fp, " %d", connectionCell->state[i][j]);
					}
				}
			}
		}

		fprintf(fp, "\n");

	}

	fclose(fp);

	return 0;
}

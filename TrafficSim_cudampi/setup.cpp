#include <iostream>
#include <vector>
#include <cstdlib>
#include <time.h>
#include <mpi.h>
#include <cstring>
#include <cmath>
#include "TrafficSim.h"

/*====================================================================*/
/// Setup Functions
/// Functions that setup components of the simulator
/// 1. void Setup_Vehicle(vehicle *v, char *srcDir, int* vehTotIDarr)
/// 2. void Setup_Link(vehicle*, int, link*, int)
/// 3. void Setup_SourceCell(source_cell*, int)
/// 4. void Setup_SinkCell(sink_cell*, int)
/// 5. void Setup_ConnectionCell(connection_cell*, int)
/// 6. int getNumLink(char *srcDir)
/// 7. int getNumVeh(char *srcDir)
/*====================================================================*/

/*--------------------------------------------------------------------*/
/// @fn      void Setup_Vehicle(vehicle*, int)
/// @brief   Function that setup the values of struct vehicle.
/// @param   vehicle* v, int numVeh
/// @return  None
/*--------------------------------------------------------------------*/

void Setup_Vehicle(vehicle *v, char *srcDir, int* vehTotIDarr, int numVeh)
{
	char        srcFile[MAX_FILENAME_LEN];
	FILE        *fp;

	strcpy(srcFile, srcDir);
	strcat(srcFile, "/Vehicle.dat");
	fp = fopen(srcFile, "r");

	//for (int i = 0; EOF != fscanf(fp, "%d", &(v[i].vehID)); i++)
	for (int i = 0; i < numVeh; i++)
	{
		fscanf(fp, "%d", &(v[i].vehID));

		//printf("veh[%d].VehID = %d\n", i, v[i].vehID);

		vehTotIDarr[i] = v[i].vehID;

		memset(v[i].path, -1, sizeof(int)*MAX_PATH);
		memset(v[i].minTargetLane, -1, sizeof(int)*MAX_PATH);
		memset(v[i].maxTargetLane, -1, sizeof(int)*MAX_PATH);

		int t;
		fscanf(fp, " %d %d %d", &(v[i].vehType), &(v[i].departTime), &(v[i].lenPath));
		for (int j = 0; j < v[i].lenPath; j++)
		{
			fscanf(fp, " %d", &(v[i].path[j]));
		}
		v[i].initLink = v[i].path[0];

		for (int j = 0; j < v[i].lenPath; j++)
		{
			fscanf(fp, " %d", &(v[i].minTargetLane[j]));
		}

		for (int j = 0; j < v[i].lenPath; j++)
		{
			fscanf(fp, " %d", &(v[i].maxTargetLane[j]));
		}

		v[i].path[v[i].lenPath] = -2;
		v[i].minTargetLane[v[i].lenPath] = -2;
		v[i].maxTargetLane[v[i].lenPath] = -2;		
	}

	fclose(fp);
}

void Setup_Vehicle_Test(vehicle *v, char *srcDir, int* vehTotIDarr, int numVeh, int* linkIDarr, int numLink)
{
	char        srcFile[MAX_FILENAME_LEN];
	FILE        *fp;

	strcpy(srcFile, srcDir);
	strcat(srcFile, "/Vehicle.dat");
	fp = fopen(srcFile, "r");

	//for (int i = 0; EOF != fscanf(fp, "%d", &(v[i].vehID)); i++)
	for (int i = 0; i < numVeh; i++)
	{
		fscanf(fp, "%d", &(v[i].vehID));

		//printf("veh[%d].VehID = %d\n", i, v[i].vehID);

		vehTotIDarr[i] = v[i].vehID;

		memset(v[i].path, -1, sizeof(int)*MAX_PATH);
		memset(v[i].minTargetLane, -1, sizeof(int)*MAX_PATH);
		memset(v[i].maxTargetLane, -1, sizeof(int)*MAX_PATH);

		fscanf(fp, " %d %d %d", &(v[i].vehType), &(v[i].departTime), &(v[i].lenPath));
		for (int j = 0; j < v[i].lenPath; j++)
		{
			fscanf(fp, " %d", &(v[i].path[j]));
			v[i].path[j] = linkIDarr[i % numLink];
		}
		v[i].initLink = v[i].path[0];

		for (int j = 0; j < v[i].lenPath; j++)
		{
			fscanf(fp, " %d", &(v[i].minTargetLane[j]));
		}

		for (int j = 0; j < v[i].lenPath; j++)
		{
			fscanf(fp, " %d", &(v[i].maxTargetLane[j]));
		}

		v[i].path[v[i].lenPath] = -2;
		v[i].minTargetLane[v[i].lenPath] = -2;
		v[i].maxTargetLane[v[i].lenPath] = -2;
	}

	fclose(fp);
}


/*--------------------------------------------------------------------*/
/// @fn      void Setup_Link(link*, char*)
/// @brief   Function that setup the values of struct link.
/// @param   vehicle* v, int numVeh, link* l, int numLink
/// @return  None
/*--------------------------------------------------------------------*/

void Setup_Link(link* l, char* srcDir, int* linkIDarr, int numLink)
{
	char    srcFile[MAX_FILENAME_LEN];
	FILE    *fp;
	//int     numLanes;
	double  length;

	strcpy(srcFile, srcDir);
	strcat(srcFile, "/Link.dat");
	fp = fopen(srcFile, "r");

	//for (int i = 0; EOF != fscanf(fp, "%d", &(l[i].linkID)); i++)
	for (int i = 0; i < numLink; i++)
	{
		fscanf(fp, "%d", &(l[i].linkID));

		//printf("link[%d].linkID = %d\n", i, l[i].linkID);

		linkIDarr[i] = l[i].linkID;

		fscanf(fp, " %d %lf %lf", &(l[i].numLane), &(l[i].ffSpeed), &(length));

		for (int j = 0; j < NUM_LANE; j++)
		{
			l[i].lenSection[j] = length / NUM_SECTION;
		}

		for (int j = 0; j < NUM_SECTION + 2; j++)
		{
			for (int k = 0; k < l[i].numLane; k++)
			{
				l[i].maxNumVeh[j][k] = MAX(JAMDENSITY * l[i].lenSection[j] / 1000, 1);
				l[i].maxNumCF[j][k] = ceil((double)SECONDS_PER_STEP / 2);
			}
			for (int k = l[i].numLane; k < NUM_LANE; k++)
			{
				l[i].maxNumVeh[j][k] = 0;
				l[i].maxNumCF[j][k] = 0;
			}

		}

		memset(l[i].numVehArr, 0, (NUM_SECTION + 2)*NUM_LANE * sizeof(int));

		memset(l[i].vehIDArr, -1, (NUM_SECTION + 2)*NUM_LANE*MAX_VEC * sizeof(int));
		memset(l[i].currLinkOrderArr, -1, (NUM_SECTION + 2)*NUM_LANE*MAX_VEC * sizeof(int));
		memset(l[i].nextLinkIDArr, -1, (NUM_SECTION + 2)*NUM_LANE*MAX_VEC * sizeof(int));
		memset(l[i].minTargetLaneArr, -1, (NUM_SECTION + 2)*NUM_LANE*MAX_VEC * sizeof(int));
		memset(l[i].maxTargetLaneArr, -1, (NUM_SECTION + 2)*NUM_LANE*MAX_VEC * sizeof(int));

		memset(l[i].speed, 0, (NUM_SECTION + 2)*NUM_LANE * sizeof(double));
		memset(l[i].density, 0, (NUM_SECTION + 2)*NUM_LANE * sizeof(double));

		memset(l[i].numMLCL, 0, (NUM_SECTION + 2)*NUM_LANE * sizeof(int));
		memset(l[i].numMLCR, 0, (NUM_SECTION + 2)*NUM_LANE * sizeof(int));
		memset(l[i].numCF, 0, (NUM_SECTION + 2)*NUM_LANE * sizeof(double));

		memset(l[i].vehMLC, 0, (NUM_SECTION + 2)*NUM_LANE*MAX_VEC * sizeof(int));
		memset(l[i].vehOLC, 0, (NUM_SECTION + 2)*NUM_LANE*MAX_VEC * sizeof(int));
		memset(l[i].vehCF, 0, (NUM_SECTION + 2)*NUM_LANE*MAX_VEC * sizeof(int));

		memset(l[i].tempIDArr, 0, (NUM_SECTION + 2)*NUM_LANE * sizeof(int));
		memset(l[i].tempNumArr, 0, (NUM_SECTION + 2)*NUM_LANE * sizeof(int));
	}

	fclose(fp);
}

/*--------------------------------------------------------------------*/
/// @fn      void Setup_SourceCell(source_cell*, int)s
/// @brief   Function that setup the values of struct source_cell.
/// @param   source_cell* sc, int numSC
/// @return  None
/*--------------------------------------------------------------------*/
void Setup_SourceCell(source_cell* sc, int numSC, int* linkIDarr) {
	/// (1) Setup source cell ID, and initialize values.
	for (int i = 0; i < numSC; i++) {
		sc[i].sourceID = linkIDarr[i];

		sc[i].numVeh = 0;
		for (int j = 0; j < MAX_SOURCE_VEC; j++) {
			sc[i].vehIDArr[j] = -1;
			sc[i].currLinkOrderArr[j] = -1;
			sc[i].nextLinkIDArr[j] = -1;
			sc[i].minTargetLaneArr[j] = -1;
			sc[i].maxTargetLaneArr[j] = -1;
		}
	}
}


/*--------------------------------------------------------------------*/
/// @fn      void Setup_SinkCell(sink_cell*, int)
/// @brief   Function that setup the values of struct sink_cell.
/// @param   sink_cell* sk, int numSK
/// @return  None
/*--------------------------------------------------------------------*/
void Setup_SinkCell(sink_cell* sk, int numSK, int* linkIDarr) {
	/// (1) Setup sink cell ID, and initialize values.
	for (int i = 0; i < numSK; i++) {
		sk[i].sinkID = linkIDarr[i];

		sk[i].numVeh = 0;
		for (int j = 0; j < MAX_SINK_VEC; j++) {
			sk[i].vehIDArr[j] = -1;
			sk[i].arrivalTime[j] = -1;
		}
	}
}


/*--------------------------------------------------------------------*/
/// @fn      void Setup_ConnectionCell(connection_cell*)
/// @brief   Function that setup the values of struct connection_cell.
/// @param   connection_cell* cc
/// @return  None
/*--------------------------------------------------------------------*/
void Setup_ConnectionCell(connection_cell *c, char *srcDir, int* ccIDarr, int numCC)
{
	char    srcFile[MAX_FILENAME_LEN];
	FILE    *fp;
	int     connectionNums;
	int     fromLane;
	int     type;
	int     laneNums;
	int     phaseNums;
	int     duration;
	int     state;
	int     trafficIndex;
	int     tmpnextlinkID;


	strcpy(srcFile, srcDir);
	strcat(srcFile, "/ConnectionCell.dat");
	fp = fopen(srcFile, "r");


	//for (int i = 0; EOF != fscanf(fp, "%d", &(c[i].ccID)); i++)
	for (int i = 0; i < numCC; i++)
	{
		fscanf(fp, "%d", &(c[i].ccID));

		//printf("c[%d].ccID = %d\n", i, c[i].ccID);

		ccIDarr[i] = c[i].ccID;
		memset(c[i].nextLinkID, -1, sizeof(int) * NUM_LANE * MAX_LEG);
		memset(c[i].nextLane, -1, sizeof(int) * NUM_LANE * MAX_LEG);

		int numLeg[NUM_LANE] = { 0 };

		fscanf(fp, " %d %d", &(c[i].prevLinkID), &(connectionNums));

		for (int j = 0; j < connectionNums; j++)
		{
			fscanf(fp, " %d", &fromLane);
			fscanf(fp, " %d", &tmpnextlinkID);

			c[i].nextLinkID[fromLane][numLeg[fromLane]] = tmpnextlinkID;

			//if (i == 453) printf("cc[%d].nextLinkID[%d][%d] = %d\n", i, fromLane, numLeg[fromLane], c[i].nextLinkID[fromLane][numLeg[fromLane]]);

			fscanf(fp, " %d", &(c[i].nextLane[fromLane][numLeg[fromLane]]));
			numLeg[fromLane]++;
		}

		fscanf(fp, " %d", &type);

		for (int lane = 0; lane < NUM_LANE; lane++) {
			c[i].trafficSignal[lane][0] = 1;
			for (int time = 1; time < MAX_CYCLE_DURATION; time++) {
				c[i].trafficSignal[lane][time] = -1;
			}
		}

		c[i].cycleDuration = 0;

		if (type == 1) {
			fscanf(fp, " %d %d", &laneNums, &phaseNums);

			//int durationArr[phaseNums];
			//int stateArr[phaseNums];
			int* durationArr = (int*)malloc(sizeof(int)*phaseNums);			
			int* stateArr = (int*)malloc(sizeof(int)*phaseNums);

			durationArr[0] = 0;

			for (int lane = 0; lane < laneNums; lane++) {
				fscanf(fp, " %d", &fromLane);
				for (int j = 0; j < phaseNums; j++)
				{
					fscanf(fp, " %d", &(durationArr[j]));
				}
				for (int j = 0; j < phaseNums; j++)
				{
					fscanf(fp, " %d", &(stateArr[j]));
				}


				trafficIndex = 0;
				int temp = 0;

				for (int j = 0; j <phaseNums; j++) {

					for (int k = trafficIndex; k < (int)(trafficIndex + ((double)durationArr[j] / SECONDS_PER_STEP) + 0.5); k++) {
						c[i].trafficSignal[fromLane][k] = stateArr[j];
						temp++;

					}
					trafficIndex = temp;

				}
				c[i].cycleDuration = temp;
				for (int k = c[i].cycleDuration; k < MAX_CYCLE_DURATION; k++) {
					c[i].trafficSignal[fromLane][k] = -1;
				}
			}

		}
		else {
			c[i].cycleDuration = 1;
		}

		for (int lane = 0; lane < NUM_LANE; lane++)
			for (int leg = 0; leg < MAX_LEG; leg++)
			{
				c[i].numVehArr[lane][leg] = 0;
				c[i].numCFArr[lane][leg] = 0;
				c[i].currLinkOrderArr[lane][leg] = 0;
				c[i].nextLinkIDArr[lane][leg] = 0;
				c[i].vehIDArr[lane][leg] = 0;
			}
	}

	fclose(fp);
}


void Setup_ConnectionCell_Test(connection_cell *c, char *srcDir, int* ccIDarr, int numCC, int *linkIDarr)
{
	char    srcFile[MAX_FILENAME_LEN];
	FILE    *fp;
	int     connectionNums;
	int     fromLane;
	int     type;
	int     laneNums;
	int     phaseNums;
	int     duration;
	int     state;
	int     trafficIndex;
	int     tmpnextlinkID;


	strcpy(srcFile, srcDir);
	strcat(srcFile, "/ConnectionCell.dat");
	fp = fopen(srcFile, "r");


	//for (int i = 0; EOF != fscanf(fp, "%d", &(c[i].ccID)); i++)
	for (int i = 0; i < numCC; i++)
	{
		fscanf(fp, "%d", &(c[i].ccID));

		//printf("c[%d].ccID = %d\n", i, c[i].ccID);

		ccIDarr[i] = c[i].ccID;
		memset(c[i].nextLinkID, -1, sizeof(int) * NUM_LANE * MAX_LEG);
		memset(c[i].nextLane, -1, sizeof(int) * NUM_LANE * MAX_LEG);

		int numLeg[NUM_LANE] = { 0 };

		fscanf(fp, " %d %d", &(c[i].prevLinkID), &(connectionNums));

		c[i].prevLinkID = linkIDarr[i];

		for (int j = 0; j < connectionNums; j++)
		{
			fscanf(fp, " %d", &fromLane);
			fscanf(fp, " %d", &tmpnextlinkID);

			c[i].nextLinkID[fromLane][numLeg[fromLane]] = tmpnextlinkID;

			c[i].nextLinkID[fromLane][numLeg[fromLane]] = linkIDarr[i];

			//if (i == 453) printf("cc[%d].nextLinkID[%d][%d] = %d\n", i, fromLane, numLeg[fromLane], c[i].nextLinkID[fromLane][numLeg[fromLane]]);

			fscanf(fp, " %d", &(c[i].nextLane[fromLane][numLeg[fromLane]]));			
			numLeg[fromLane]++;
		}

		fscanf(fp, " %d", &type);

		for (int lane = 0; lane < NUM_LANE; lane++) {
			c[i].trafficSignal[lane][0] = 1;
			for (int time = 1; time < MAX_CYCLE_DURATION; time++) {
				c[i].trafficSignal[lane][time] = -1;
			}
		}

		c[i].cycleDuration = 0;

		if (type == 1) {
			fscanf(fp, " %d %d", &laneNums, &phaseNums);

			//int durationArr[phaseNums];
			//int stateArr[phaseNums];
			int* durationArr = (int*)malloc(sizeof(int)*phaseNums);
			int* stateArr = (int*)malloc(sizeof(int)*phaseNums);

			durationArr[0] = 0;

			for (int lane = 0; lane < laneNums; lane++) {
				fscanf(fp, " %d", &fromLane);
				for (int j = 0; j < phaseNums; j++)
				{
					fscanf(fp, " %d", &(durationArr[j]));
				}
				for (int j = 0; j < phaseNums; j++)
				{
					fscanf(fp, " %d", &(stateArr[j]));
				}


				trafficIndex = 0;
				int temp = 0;

				for (int j = 0; j <phaseNums; j++) {

					for (int k = trafficIndex; k < (int)(trafficIndex + ((double)durationArr[j] / SECONDS_PER_STEP) + 0.5); k++) {
						c[i].trafficSignal[fromLane][k] = stateArr[j];
						temp++;

					}
					trafficIndex = temp;

				}
				c[i].cycleDuration = temp;
				for (int k = c[i].cycleDuration; k < MAX_CYCLE_DURATION; k++) {
					c[i].trafficSignal[fromLane][k] = -1;
				}
			}

		}
		else {
			c[i].cycleDuration = 1;
		}

		for (int lane = 0; lane < NUM_LANE; lane++)
			for (int leg = 0; leg < MAX_LEG; leg++)
			{
				c[i].numVehArr[lane][leg] = 0;
				c[i].numCFArr[lane][leg] = 0;
				c[i].currLinkOrderArr[lane][leg] = 0;
				c[i].nextLinkIDArr[lane][leg] = 0;
				c[i].vehIDArr[lane][leg] = 0;
			}
	}

	fclose(fp);
}



/*--------------------------------------------------------------------*/
/// @fn      int getNumLink(char *srcDir)
/// @brief   Function that calculates the number of links of input data.
/// @param   char* srcDir
/// @return  number of links
/*--------------------------------------------------------------------*/
int getNumLink(char *srcDir)
{
	char    srcFile[MAX_FILENAME_LEN];
	FILE    *fp;
	int     line = 0;
	char    c;

	strcpy(srcFile, srcDir);
	strcat(srcFile, "/Link.dat");
	fp = fopen(srcFile, "r");

	while ((c = fgetc(fp)) != EOF)
		if (c == '\n') line++;

	fclose(fp);
	return(line);
}

/*--------------------------------------------------------------------*/
/// @fn      int getNumVeh(char *srcDir)
/// @brief   Function that calculates the number of vehicles of input data.
/// @param   char* srcDir
/// @return  number of vehicles
/*--------------------------------------------------------------------*/
int getNumVeh(char *srcDir)
{
	char    srcFile[MAX_FILENAME_LEN];
	FILE    *fp;
	int     line = 0;
	char    c;

	strcpy(srcFile, srcDir);
	strcat(srcFile, "/Vehicle.dat");
	fp = fopen(srcFile, "r");

	while ((c = fgetc(fp)) != EOF)
		if (c == '\n') line++;

	fclose(fp);
	return(line);
}

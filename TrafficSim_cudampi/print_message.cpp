#include <iostream>
#include <vector>
#include <cstdlib>
#include <time.h>
#include <mpi.h>
#include <cstring>
#include <cmath>
#include "TrafficSim.h"

/*====================================================================*/
/// Output Functions
/// Functions to print or export results of the simulation
/// 1. double Get_Time_ms()
/// 2. int Print_Template(int, int, int)
/// 3. void Print_Vehicle(vehicle*, int);
/// 4. void Print_Link(link*, int)
/// 5. void Print_SourceCell(source_cell*, int)
/// 6. void Print_SinkCell(sink_cell*, int)
/// 7. void Print_ConnectionCell(connection_cell*, int)
/// 8. void Print_Signal(connection_cell*, int);
/// 9. void Print_All_Results(link*, source_cell*, sink_cell*, connection_cell*, int)
/// 10. int Export_All_Result(link*, int, int, int)
/*====================================================================*/

/*--------------------------------------------------------------------*/
/// @fn      double Get_Time_ms()
/// @brief   Function that returns current time.
/// @param   None
/// @return  currTime
/*--------------------------------------------------------------------*/
/*
double Get_Time_ms() {
	struct timeval t;
	gettimeofday(&t, NULL);
	int currTime = (t.tv_sec + (t.tv_usec / 1000000.0)) * 1000.0;

	return currTime;
}
*/

/*--------------------------------------------------------------------*/
/// @fn      int Print_Template(int, int, int)
/// @brief   Function that provides template for printf.
/// @param   int msgFlag, int msgType, int fncName
/// @return  TRUE or FALSE
/*--------------------------------------------------------------------*/
int Print_Template(int msgFlag, int msgType, int fncName) {
	if (msgFlag == TRUE) {
		printf("TrafficSim_Single.c:");

		switch (fncName) {
		case PRINT_VEHICLE:
			printf("Print_Vehicle: ");
			break;
		case PRINT_LINK:
			printf("Print_Link: ");
			break;
		case PRINT_SOURCECELL:
			printf("Print_SourceCell: ");
			break;
		case PRINT_SINKCELL:
			printf("Print_SinkCell: ");
			break;
		case PRINT_CONNECTIONCELL:
			printf("Print_ConnectionCell: ");
			break;
		case PRINT_ALL_RESULTS:
			printf("Print_All_Results: ");
			break;
		case SETUP_VEHICLE:
			printf("Setup_Vehicle: ");
			break;
		case SETUP_LINK:
			printf("Setup_Link: ");
			break;
		case SETUP_SOURCECELL:
			printf("Setup_SourceCell: ");
			break;
		case SETUP_SINKCELL:
			printf("Setup_SinkCell: ");
			break;
		case SETUP_CONNECTIONCELL:
			printf("Setup_ConnectionCell: ");
			break;
		case MOVELC:
			printf("MoveLC: ");
			break;
		case MOVECF:
			printf("MoveCF: ");
			break;
		case REMOVE_BLANK:
			printf("Remove_Blank: ");
			break;
		case FIND_INDEX:
			printf("Find_Index: ");
			break;
		case EVAULATE_PROB:
			printf("Evaluate_Prob: ");
			break;
		case UPDATE_SOURCE:
			printf("Update_Source: ");
			break;
		case ENTER_LINK:
			printf("Enter_Link: ");
			break;
		case EXIT_LINK:
			printf("Exit_Link: ");
			break;
		case EVAULATE_MLC:
			printf("Evaluate_MLC: ");
			break;
		case UPDATE_SPEED:
			printf("update_Speed: ");
			break;
		case SELECT_VEH:
			printf("Select_Veh: ");
			break;
		case EVALUATE_OLC:
			printf("Evaluate_OLC: ");
			break;
		case LCSIM:
			printf("LCSim: ");
			break;
		case EVALUATE_CF:
			printf("Evaluate_CF: ");
			break;
		case CFSIM:
			printf("CFsim: ");
			break;
		case UPDATE_TEMPARR:
			printf("Update_tempArr: ");
			break;
		case RELAY_NUMVEH:
			printf("Relay_numVeh: ");
			break;
		case RELAY_NUMCF:
			printf("Relay_numCF: ");
			break;
		case EVALUATE_EFF_NUMCF:
			printf("Evaluate_Eff_numCF: ");
			break;
		case UPDATE_NEXTLINK:
			printf("Update_nextLink: ");
			break;
		case RESET_LINK:
			printf("Reset_Link: ");
			break;
		case RESET_SINK:
			printf("Reset_Sink: ");
			break;
		case RESET_CONNECTIONCELL:
			printf("Reset_ConnectionCell: ");
			break;
		default:
			printf("Print_Template: ");
			printf("[error] ");
			printf("no matching function name");
		}

		switch (msgType) {
		case INFO:
			printf("[info] ");
			break;
		case WARNING:
			printf("[warning] ");
			break;
		case ERROR:
			printf("[error] ");
			break;
		default:
			printf("Print_Template: ");
			printf("[error] ");
			printf("no matching message type");
		}

		return TRUE;
	}

	return FALSE;
}


/*--------------------------------------------------------------------*/
/// @fn      void Print_Vehicle(vehicle*, int)
/// @brief   Function to print out vehicle info.
/// @param   vehicle* v, int numVeh
/// @return  None
/*--------------------------------------------------------------------*/
void Print_Vehicle(vehicle* v, int numVeh) {
	printf("====================================================================\n");

	numVeh = 5;

	for (int i = 0; i < numVeh; i++) {
		if (Print_Template(PRINT_INFO, INFO, PRINT_VEHICLE)) {
			printf("vehID:%d, ", v[i].vehID);
			printf("depart time:%d ", v[i].departTime);
			printf("lenPath: %d ", v[i].lenPath);
			printf("path(minTL,maxTL):");

			for (int j = 0; j < MAX_PATH; j++) {
				printf("%d", v[i].path[j]);
				printf("(%d,%d), ", v[i].minTargetLane[j], v[i].maxTargetLane[j]);
			}
			printf("\n");
		}
	}
}


/*--------------------------------------------------------------------*/
/// @fn      void Print_Link(link*, int)
/// @brief   Function to print out link variables during and after simulation.
/// @param   link* l, int numLink
/// @return  None
/*--------------------------------------------------------------------*/
void Print_Link(link* l, int numLink) {
	printf("====================================================================\n");

	for (int link = 0; link < numLink; link++) {
		//if (l[link].linkID == 572702238 || l[link].linkID == 572712738 || l[link].linkID == -572712737 )
		//if (l[link].linkID == 572718027 || l[link].linkID == 572718026 ) { //}|| l[link].linkID == -572718029 ) {
		for (int sect = 0; sect < NUM_SECTION + 2; sect++) {
			//for (int lane = 0 ; lane < NUM_LANE ; lane++) {
			for (int lane = 0; lane < l[link].numLane; lane++) {
				if (Print_Template(PRINT_INFO, INFO, PRINT_LINK)) {
					printf("linkID:%d, section:%d, lane:%d, numVeh:%d, numCF:%f, maxNumVeh:%d, density:%f, speed:%f, maxNumCF:%d ",
						l[link].linkID, sect, lane, l[link].numVehArr[sect][lane], l[link].numCF[sect][lane], l[link].maxNumVeh[sect][lane], l[link].density[sect][lane], l[link].speed[sect][lane], l[link].maxNumCF[sect][lane]);
					printf("vehID(nextLinkID):");

					if (l[link].numVehArr[sect][lane] == 0) printf("None");

					else {
						for (int i = 0; i < MAX_VEC; i++) { // l[link].numVehArr[sect][lane]
							if (l[link].vehIDArr[sect][lane][i] >= 0) {
								printf("%d", l[link].vehIDArr[sect][lane][i]);
								printf("(%d,%d,%d),", l[link].nextLinkIDArr[sect][lane][i], l[link].minTargetLaneArr[sect][lane][i], l[link].maxTargetLaneArr[sect][lane][i]);
							}
						}
					}

					printf("\n");
				}
			}
		}
	}
	//}
}

/*--------------------------------------------------------------------*/
/// @fn      void Print_SourceCell(source_cell*, int)
/// @brief   Function to print out source cell variables during and after simulation.
/// @param   source_cell* sc, int numLink
/// @return  None
/*--------------------------------------------------------------------*/
void Print_SourceCell(source_cell* sc, int numLink) {
	printf("====================================================================\n");

	for (int link = 0; link < numLink; link++) {
		if (Print_Template(PRINT_INFO, INFO, PRINT_SOURCECELL)) {
			printf("sourceID:%d, numVeh:%d, ", sc[link].sourceID, sc[link].numVeh);

			printf("vehIDArr:");

			if (sc[link].numVeh == 0) printf("None");

			else {
				for (int i = 0; i < MAX_SOURCE_VEC; i++) {
					if (sc[link].vehIDArr[i] >= 0) {
						printf("%d,", sc[link].vehIDArr[i]);
					}
				}
			}

			printf("\n");
		}

	}
}

/*--------------------------------------------------------------------*/
/// @fn      void Print_SinkCell(sink_cell*, int)
/// @brief   Function to print out sink cell variables during and after simulation.
/// @param   sink_cell* sk, int numLink
/// @return  None
/*--------------------------------------------------------------------*/
void Print_SinkCell(sink_cell* sk, int numLink) {
	printf("====================================================================\n");

	for (int link = 0; link < numLink; link++) {
		if (Print_Template(PRINT_INFO, INFO, PRINT_SINKCELL)) {
			printf("sinkID:%d, numVeh:%d ", sk[link].sinkID, sk[link].numVeh);
			printf("vehIDArr:");

			if (sk[link].numVeh == 0) printf("None");

			else {
				for (int i = 0; i < MAX_SOURCE_VEC; i++) {
					if (sk[link].vehIDArr[i] >= 0) {
						printf("%d,", sk[link].vehIDArr[i]);
					}
				}
			}

			printf("\n");
		}
	}
}


/*--------------------------------------------------------------------*/
/// @fn      void Print_ConnectionCell(connection_cell*, int)
/// @brief   Function to print out connection cell variables during and after simulation.
/// @param   connection_cell* cc, int numLink
/// @return  None
/*--------------------------------------------------------------------*/
void Print_ConnectionCell(connection_cell* cc, int numLink) {
	printf("====================================================================\n");

	for (int link = 0; link < numLink; link++) {
		if (Print_Template(PRINT_INFO, INFO, PRINT_CONNECTIONCELL)) {
			printf("ccID:%d, \n", cc[link].ccID);

			for (int lane = 0; lane < NUM_LANE; lane++) {
				for (int leg = 0; leg < MAX_LEG; leg++) {
					printf("cc[%d].nextLinkID[%d][%d] = [%d], cc[%d].nextLane[%d][%d] = [%d] \n", link, lane, leg, cc[link].nextLinkID[lane][leg], link, lane, leg, cc[link].nextLane[lane][leg]);
				}
			}

			printf("\n");
		}
	}
}

/*--------------------------------------------------------------------*/
/// @fn      void Print_Signal(connection_cell*, int)
/// @brief   Function to print out signal information of connection cells.
/// @param   connection_cell* cc, int numLink
/// @return  None
/*--------------------------------------------------------------------*/
void Print_Signal(connection_cell* cc, int numLink) {
	printf("====================================================================\n");

	for (int link = 0; link < numLink; link++) {
		if (Print_Template(PRINT_INFO, INFO, PRINT_CONNECTIONCELL)) {
			printf("ccID:%d, cycleDuration:%d \n", cc[link].ccID, cc[link].cycleDuration);

			for (int lane = 0; lane < NUM_LANE; lane++) {
				for (int cycle = 0; cycle < MAX_CYCLE_DURATION; cycle++) {
					printf("cc[%d].trafficSignal[%d][%d] = [%d], \n", link, lane, cycle, cc[link].trafficSignal[lane][cycle]);
				}
			}

			printf("\n");
		}
	}
}

/*--------------------------------------------------------------------*/
/// @fn      void Print_All_Results(link*, source_cell*, sink_cell*, connection_cell*, int)
/// @brief   Function to print out all results during and after the simulation.
/// @param   link* l, source_cell* sc, sink_cell* sk, connection_cell* cc, int numLink
/// @return  None
/*--------------------------------------------------------------------*/
void Print_All_Results(link* l, source_cell* sc, sink_cell* sk, connection_cell* cc, int numLink) {
	Print_Link(l, numLink);
	Print_SourceCell(sc, numLink);
	Print_SinkCell(sk, numLink);
	Print_ConnectionCell(cc, numLink);
}


/*--------------------------------------------------------------------*/
/// @fn      int Export_All_Result(link*, int, int, int)
/// @brief   Function to extract all results during and after the simulation.
/// @param   link* l, int numLink, int numLoop, int count
/// @return  0
/*--------------------------------------------------------------------*/
int Export_All_Result(link* l, int numLink, int numLoop, int count) {
	char txtFile[100];
	FILE *fp;

	int resultNum = count / UPDATE_INTERVAL;
	char resultNumChar[1];
	sprintf(resultNumChar, "%d", resultNum);

	strcpy(txtFile, "SimResult");
	strcat(txtFile, resultNumChar);
	strcat(txtFile, ".csv");

	fp = fopen(txtFile, "a");

	for (int link = 0; link < numLink; link++) {
		for (int sect = 1; sect < NUM_SECTION + 1; sect++) {
			for (int lane = 0; lane < NUM_LANE; lane++) {
				fprintf(fp, "%d\t\t", count);
				fprintf(fp, "%d_", l[link].linkID);
				fprintf(fp, "%d_", sect);
				fprintf(fp, "%d\t\t", lane);
				fprintf(fp, "%f\t", l[link].lenSection[sect] * NUM_SECTION);
				fprintf(fp, "%d\t", l[link].numVehArr[sect][lane]);
				fprintf(fp, "%f\t", l[link].ffSpeed);
				fprintf(fp, "%f\t", l[link].lenSection[sect]);
				fprintf(fp, "%f\t", l[link].density[sect][lane]);
				fprintf(fp, "%f", l[link].speed[sect][lane]);
				fprintf(fp, "\n");
			}
		}
	}

	fclose(fp);

	return 0;
}
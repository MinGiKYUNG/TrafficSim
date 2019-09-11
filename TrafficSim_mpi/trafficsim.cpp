#include <iostream>
#include <vector>
#include <cstdlib>
#include <time.h>
#include <mpi.h>
#include <cstring>
#include <cmath>

#include "TrafficSim.h"

using namespace std;


// Declare Data Types
link *mylink;
vehicle *myveh;
source_cell *mysource;
sink_cell *mysink;
connection_cell *mycon;



typedef struct _MyData{
	char name[8];
	int my_num;
}MyData;

extern void runGPU(vector<MyData>* v, int rank);

void Broadcast_Vehicle(vector<MyData>* v, int rank);
void Broadcast_Num(int* num, int rank);

int my_size;
int my_rank;

/*====================================================================*/
/// Utility Functions
/// Functions that are used in many functions to perform repetitive tasks
/// 1. void MoveLC(int*, int, int*, int, int)
/// 2. void MoveCF(int*, int, int*, int, int)
/// 3. void Remove_Blank(int*, int)
/// 4. int Find_Index(int*, int, int)
/// 5. int Evaluate_Prob(double)
/*====================================================================*/

/*--------------------------------------------------------------------*/
/// @fn      void MoveLC(int*, int, int*, int, int)
/// @brief   Function that moves elements of one array to another array for LC (It should lead the function Remove_Blank).
/// @param   int* fromArr, int fromArrSize, int* toArr, int toArrSize, int index
/// @return  None
/*--------------------------------------------------------------------*/
void MoveLC(int* fromArr, int fromArrSize, int* toArr, int toArrSize, int index) {
	/// (1) Print critical errors
	if (fromArrSize < 0) {
		if (Print_Template(PRINT_CRITICAL, CRITICAL, MOVELC)) {
			printf("fromArr is empty\n");

			exit(1);
		}
	}

	double fromArrLoc = ((double)(index + 1) / fromArrSize);
	double toArrLoc = (double)toArrSize * fromArrLoc;
	int toIndex = toArrLoc + 0.5;

	/// (2) Move toArr[i-1] to toArr[i] in order to provide space
	if (toArrSize > 0) {
		for (int i = toArrSize; i > toIndex; i--) {
			toArr[i] = toArr[i - 1];
		}
	}

	/// (3) Move fromArr[index] to toArr[toIndex]
	toArr[toIndex] = fromArr[index];
	fromArr[index] = -1;
}


/*--------------------------------------------------------------------*/
/// @fn      void MoveCF(int*, int, int*, int, int)
/// @brief   Function that moves elements of one array to another array.
/// @param   int* fromArr, int fromArrSize, int* toArr, int toArrSize, int index
/// @return  None
/*--------------------------------------------------------------------*/
void MoveCF(int* fromArr, int fromArrSize, int* toArr, int toArrSize, int index) {
	/// (1) Print critical errors
	if (fromArrSize < 0) {
		if (Print_Template(PRINT_CRITICAL, CRITICAL, MOVECF)) {
			printf(" fromArr is empty\n");

			exit(1);
		}
	}

	if (fromArrSize > 0) {
		/// (2) Move fromArr[index] to toArr[toArrSize]
		toArr[toArrSize] = fromArr[index];

		/// (3) Move romArr[i+1] to fromArr[i] in order to remove space
		for (int i = index; i < (fromArrSize - 1); i++) {
			fromArr[i] = fromArr[i + 1];
		}
		fromArr[fromArrSize - 1] = -1;
	}
}


/*--------------------------------------------------------------------*/
/// @fn      void Remove_Blank(int*, int)
/// @brief   Function that removes blank elements in an array.
/// @param   int* arr, int arrSize
/// @return  None
/*--------------------------------------------------------------------*/
void Remove_Blank(int* arr, int arrSize) {
	if (arrSize > 0) {
		for (int i = 0; i < arrSize; i++) {
			int index = Find_Index(arr, arrSize, -1);

			if (index != -1) {
				for (int j = index; j < arrSize; j++) {
					arr[j] = arr[j + 1];
				}
				arr[arrSize] = -1;
			}
		}
	}
}


/*--------------------------------------------------------------------*/
/// @fn      int Find_Index(int*, int, int)
/// @brief   Function that find the index of a value in an array.
/// @param   int* findArr, int findArrSize, int findValue, 
/// @return  index if the given value exists in the given array.
///          -1 if the given value doesn't exist in the givne array.
/*--------------------------------------------------------------------*/
int Find_Index(int* findArr, int findArrSize, int findValue) {
	for (int i = 0; i < findArrSize; i++) {
		if (findArr[i] == findValue) return i;
	}

	return -1;
}


/*--------------------------------------------------------------------*/
/// @fn      int Evaluate_Prob(double)
/// @brief   Function that randomly returns integer part or 
///          (integer part+1) of a rational number.
/// @param   double inputProb
/// @return  intPart+1 when random > probPart
///          intPart  when random <= probPart
/*--------------------------------------------------------------------*/
int Evaluate_Prob(double inputProb) {
	// int intPart = (int)inputProb;
	// double probPart = inputProb - (double)intPart;
	// double random = ((rand() % 10)/10.);
	// return random > probPart ? (intPart+1):intPart; 

	int intPart = (int)inputProb;

	return intPart;
}


/*====================================================================*/
/// Enter & Exit Link Functions
/// Functions that are involved in evaluation and performanace of CF
/// 1. void Update_Source(vehicle*, int, source_cell*, int)
/// 2. void Enter_Link(link*, source_cell*)
/// 3. void Exit_Link(link*, sink_cell*, int)
/*====================================================================*/

/*--------------------------------------------------------------------*/
/// @fn      void Update_Source(vehicle*, int, source_cell*, int)
/// @brief   Function that update source in every UPDATE_INTERVAL.
/// @param   vehicle* v, int numVeh, source_cell* sc, int currLoop
/// @return  None
/*--------------------------------------------------------------------*/
void Update_Source(vehicle* v, int numVeh, source_cell* sc, int currLoop, int numLink, int* linkIDarr) {
	/// (1) Scan vehicles that should be move to source cell
	for (int i = 0; i < numVeh; i++) {
		//printf("i = %d, v[%d].initLink = %d\n", i, i, v[i].initLink);
		
		if ((currLoop * SECONDS_PER_STEP) <= v[i].departTime && v[i].departTime < ((currLoop + UPDATE_INTERVAL) * SECONDS_PER_STEP)) {
			int initLink = v[i].initLink;

			int initLinkIndex = Find_Index(linkIDarr, numLink, initLink);
			sc[initLinkIndex].vehIDArr[sc[initLinkIndex].numVeh] = v[i].vehID;
			sc[initLinkIndex].currLinkOrderArr[sc[initLinkIndex].numVeh] = 0;
			sc[initLinkIndex].nextLinkIDArr[sc[initLinkIndex].numVeh] = v[i].path[1];
			sc[initLinkIndex].minTargetLaneArr[sc[initLinkIndex].numVeh] = v[i].minTargetLane[0];
			sc[initLinkIndex].maxTargetLaneArr[sc[initLinkIndex].numVeh] = v[i].maxTargetLane[0];

			sc[initLinkIndex].numVeh++;

			//printf("i = %d, initLinkIndex = %d\n", i, initLinkIndex);
			//if (initLinkIndex == 453) printf(">>>>> i = %d, initLinkIndex = %d\n", i, initLinkIndex);

			/// (2) Print critical errors
			if (sc[initLinkIndex].numVeh > MAX_SOURCE_VEC) {
				if (Print_Template(PRINT_CRITICAL, CRITICAL, UPDATE_SOURCE)) {
					printf("sc[initLinkIndex].numVeh is larger than MAX_SOURCE_VEC\n");

					exit(1);
				}
			}
		} 
		//else {
		//	return;
		//}
	}
}


/*--------------------------------------------------------------------*/
/// @fn      void Enter_Link(link*, source_cell*, int)
/// @brief   Function that moves vehicles from source cell to initial
///          link.
/// @param   link* l, source_cell* sc, int numLink
/// @return  None
/*--------------------------------------------------------------------*/
void Enter_Link(link* l, source_cell* sc) {
	int startSection = 1;
	int startLane = 0;  //l->numLane - 1;

	if (l->numVehArr[startSection][startLane] < l->maxNumVeh[startSection][startLane]) {
		int numEnter = MIN(MIN(l->maxNumCF[startSection - 1][startLane], (l->maxNumVeh[startSection][startLane] - l->numVehArr[startSection][startLane])), sc->numVeh);
		if (numEnter > 0) {
			for (int j = 0; j < numEnter; j++) {
				MoveCF(sc->vehIDArr, sc->numVeh, l->vehIDArr[startSection][startLane], l->numVehArr[startSection][startLane], 0);
				MoveCF(sc->currLinkOrderArr, sc->numVeh, l->currLinkOrderArr[startSection][startLane], l->numVehArr[startSection][startLane], 0);
				MoveCF(sc->nextLinkIDArr, sc->numVeh, l->nextLinkIDArr[startSection][startLane], l->numVehArr[startSection][startLane], 0);
				MoveCF(sc->minTargetLaneArr, sc->numVeh, l->minTargetLaneArr[startSection][startLane], l->numVehArr[startSection][startLane], 0);
				MoveCF(sc->maxTargetLaneArr, sc->numVeh, l->maxTargetLaneArr[startSection][startLane], l->numVehArr[startSection][startLane], 0);

				sc->numVeh--;
				l->numVehArr[startSection][startLane]++;
			}
		}
	}
}


/*--------------------------------------------------------------------*/
/// @fn      void Exit_Link(link*, sink_cell*, int)
/// @brief   Function that moves vehicles from the last link of their 
///          path to sink cell.
/// @param   link* l, sink_cell* sk, int numLink
/// @return  None
/*--------------------------------------------------------------------*/
void Exit_Link(link* l, sink_cell* sk, int numLoop) {
	int endSection = 3;
	int numVehSink = 0;

	for (int lane = 0; lane < l->numLane; lane++) {
		int vehNum = l->numVehArr[endSection][lane];
		for (int i = 0; i < vehNum; i++) {
			if (l->nextLinkIDArr[endSection][lane][i] == -2) {
				sk->vehIDArr[sk->numVeh] = l->vehIDArr[endSection][lane][i];
				l->vehIDArr[endSection][lane][i] = -1;
				sk->arrivalTime[sk->numVeh] = numLoop;
				l->currLinkOrderArr[endSection][lane][i] = -1;
				l->nextLinkIDArr[endSection][lane][i] = -1;
				l->minTargetLaneArr[endSection][lane][i] = -1;
				l->maxTargetLaneArr[endSection][lane][i] = -1;

				sk->numVeh++;
				l->numVehArr[endSection][lane]--;
			}
		}
	}

	for (int lane = 0; lane < l->numLane; lane++) {
		Remove_Blank(l->vehIDArr[endSection][lane], MAX_VEC - 1);
		Remove_Blank(l->currLinkOrderArr[endSection][lane], MAX_VEC - 1);
		Remove_Blank(l->nextLinkIDArr[endSection][lane], MAX_VEC - 1);
		Remove_Blank(l->minTargetLaneArr[endSection][lane], MAX_VEC - 1);
		Remove_Blank(l->maxTargetLaneArr[endSection][lane], MAX_VEC - 1);
	}
}





/*====================================================================*/
/// LC Functions
/// Functions that are involved in evaluation and performanace of LC
/// 1. void update_Speed(link*)
/// 2. void Evaluate_MLC(link*)
/// 3. void Select_Veh(link*, int, int, int, int)
/// 4. void Evaluate_OLC(link*) 
/// 5. void LCSim(link*)
/*====================================================================*/

/*--------------------------------------------------------------------*/
/// @fn      void update_Speed()
/// @brief   Function that calculate density and speed of link.
/// @param   link* l
/// @return  None
/*--------------------------------------------------------------------*/
void update_Speed(link* l) {
	double wSpeed = WAVESPEED * 1000 / 3600;

	for (int sect = 0; sect < NUM_SECTION + 2; sect++) {
		for (int lane = 0; lane < l->numLane; lane++) {
			double jamdensity = (double)JAMDENSITY / 1000;  //?\84로가 막히\EA\B8??\9C작?\98는 density (veh/km), 링크 ?\B9성(고정\EA\B0?

			l->density[sect][lane] = MIN(1, (double)l->numVehArr[sect][lane] / l->lenSection[sect]);  //?\84로???\84재 density (veh/m), ?\9C\EB??\88이???\A4텝마다 ?\85데?\B4트
			l->speed[sect][lane] = MAX(0, MIN(l->ffSpeed, wSpeed / l->density[sect][lane] * (jamdensity - l->density[sect][lane])));
		}
	}
}


/*--------------------------------------------------------------------*/
/// @fn      void Evaluate_MLC(link*)
/// @brief   Function that evaluate Mandatory Lane Change of a vehicle
///          and updates vehMLC Flag.
/// @param   link *l
/// @return  None
/*--------------------------------------------------------------------*/
void Evaluate_MLC(link *l) {
	if (l->numLane > 1) {
		for (int sect = 0; sect < NUM_SECTION + 2; sect++) {
			for (int lane = 0; lane < l->numLane; lane++) {
				for (int i = 0; i < l->numVehArr[sect][lane]; i++) {
					int minTL = l->minTargetLaneArr[sect][lane][i];
					int maxTL = l->maxTargetLaneArr[sect][lane][i];

					/// (1) Compare current lane with target lane and determine
					///     and update the value of vehMLC Flag.
					/// If vehicle should move left, set vehMLC Flag to -1
					if (lane > maxTL) {
						l->vehMLC[sect][lane][i] = -1;
						l->numMLCL[sect][lane]++;
					}

					/// If vehicle should move left, set vehMLC Flag to +1
					else if (lane < minTL) {
						l->vehMLC[sect][lane][i] = 1;
						l->numMLCR[sect][lane]++;
					}

					else {
						l->vehMLC[sect][lane][i] = 0;
					}

				}
			}
		}
	}
}


/*--------------------------------------------------------------------*/
/// @fn      void Select_Veh(link*, int, int, int, int)
/// @brief   Function that select vehicles of Optional Lane Change.
/// @param   link* l, int numOLC_L, int numOLC_R, int sect, int lane
/// @return  None
/*--------------------------------------------------------------------*/
void Select_Veh(link* l, int numOLC_L, int numOLC_R, int sect, int lane) {
	int numVeh = l->numVehArr[sect][lane];
	int possOLC[MAX_VEC] = { 0 };

	/// (1) Exclude vehMLC from candidates that can OLC.
	for (int i = 0; i < numVeh; i++) {
		if (l->vehMLC[sect][lane][i] != 0) possOLC[i] = 0;
		else possOLC[i] = 2;
	}

	/// (2) Consider when current lane is either maximum or minimum target lane.
	for (int i = 0; i < numVeh; i++) {
		int minTL = l->minTargetLaneArr[sect][lane][i];
		int maxTL = l->maxTargetLaneArr[sect][lane][i];

		if (lane == minTL && lane < maxTL) {
			if (possOLC[i] == 2) possOLC[i] = 1;
			//else if (possOLC[i] == -1) possOLC[i] = 0;
		}

		else if (lane == maxTL && lane > minTL) {
			if (possOLC[i] == 2) possOLC[i] = -1;
			//else if (possOLC[i] == 1) possOLC[i] = 0;
		}

		else if (lane == minTL && lane == maxTL) {
			if (possOLC[i] == 2) possOLC[i] = 0;
		}
	}

	/// (3) Calculate number of vehicles that can go left, right, or both.
	int possBoth = 0;
	int possLeft = 0;
	int possRight = 0;
	int possLeftArr[MAX_VEC] = { 0 };
	int possRightArr[MAX_VEC] = { 0 };

	for (int i = 0; i < numVeh; i++) {
		if (possOLC[i] == 2) {
			possLeftArr[possLeft] = i;
			possRightArr[possRight] = i;
			possLeft++;
			possRight++;
			possBoth++;
		}

		else if (possOLC[i] == -1) {
			possLeftArr[possLeft] = i;
			possLeft++;
		}

		else if (possOLC[i] == 1) {
			possRightArr[possRight] = i;
			possRight++;
		}
	}

	/// (4) Consider when number of OLC is larger than possible vehicle of OLC
	if (possLeft < numOLC_L) numOLC_L = possLeft;
	if (possRight < numOLC_R) numOLC_R = possRight;

	int possTotal = possLeft + possRight - possBoth;
	while (possTotal < numOLC_L + numOLC_R) {
		numOLC_R--;

		if (possTotal >= numOLC_L + numOLC_R) break;
		numOLC_L--;
	}

	/// (5) Update values of vehOLC flags.
	int count_R = numOLC_R;
	int count_L = numOLC_L;

	if (numOLC_L + numOLC_R > 0) {
		if (numOLC_L == 0) {
			while (count_R > 0) {
				int randVeh = rand() % possRight;

				if (l->vehOLC[sect][lane][possRightArr[randVeh]] == 0) {
					l->vehOLC[sect][lane][possRightArr[randVeh]] = 1;
					count_R--;
				}
			}
		}

		else if (numOLC_R == 0) {
			while (count_L > 0) {
				int randVeh = rand() % possLeft;

				if (l->vehOLC[sect][lane][possLeftArr[randVeh]] == 0) {
					l->vehOLC[sect][lane][possLeftArr[randVeh]] = -1;
					count_L--;
				}
			}
		}

		else if ((possLeft / numOLC_L) > (possRight / numOLC_R)) {
			while (count_R) {
				int randVeh = rand() % possRight;

				if (l->vehOLC[sect][lane][possRightArr[randVeh]] == 0) {
					l->vehOLC[sect][lane][possRightArr[randVeh]] = 1;
					count_R--;
				}
			}

			while (count_L) {
				int randVeh = rand() % possLeft;

				if (l->vehOLC[sect][lane][possLeftArr[randVeh]] == 0) {
					l->vehOLC[sect][lane][possLeftArr[randVeh]] = -1;
					count_L--;
				}
			}
		}

		else if ((possLeft / numOLC_L) <= (possRight / numOLC_R)) {
			while (count_L) {
				int randVeh = rand() % possLeft;

				if (l->vehOLC[sect][lane][possLeftArr[randVeh]] == 0) {
					l->vehOLC[sect][lane][possLeftArr[randVeh]] = -1;
					count_L--;
				}
			}

			while (count_R) {
				int randVeh = rand() % possRight;

				if (l->vehOLC[sect][lane][possRightArr[randVeh]] == 0) {
					l->vehOLC[sect][lane][possRightArr[randVeh]] = 1;
					count_R--;
				}
			}
		}
	}
}


/*--------------------------------------------------------------------*/
/// @fn      void Evaluate_OLC(link*)
/// @brief   Function that evaluate Optional Lane Change of a vehicle
///          and updates vehOLC Flag.
/// @param   link *l
/// @return  None
/*--------------------------------------------------------------------*/
void Evaluate_OLC(link *l) {
	if (l->numLane > 1) {
		for (int sect = 0; sect < NUM_SECTION + 2; sect++) {
			for (int lane = 0; lane < l->numLane; lane++) {
				int numMLC_L = l->numMLCL[sect][lane];
				int numMLC_R = l->numMLCR[sect][lane];
				double probLC_L, probLC_R;
				double diffSpeed_L;
				double diffSpeed_R;

				if (lane == 0) {
					diffSpeed_L = 0;
					diffSpeed_R = l->speed[sect][lane + 1] - l->speed[sect][lane];
				}
				else if (lane == l->numLane) {
					diffSpeed_L = l->speed[sect][lane - 1] - l->speed[sect][lane];
					diffSpeed_R = 0;
				}
				else {
					diffSpeed_L = l->speed[sect][lane - 1] - l->speed[sect][lane];
					diffSpeed_R = l->speed[sect][lane + 1] - l->speed[sect][lane];
				}


				/// (1) Set probLC to zero in special cases.
				/// When current lane is leftmost, probLC_L = 0
				/// In other words, the vehicle cannot move left.
				if (lane == 0) probLC_L = 0;
				else probLC_L = (diffSpeed_L / l->ffSpeed) * (double)l->numVehArr[sect][lane];

				/// When current lane is rightmost, probLC_R = 0
				/// In other words, the vehicle cannot move right.
				if (lane == l->numLane - 1) probLC_R = 0;
				else probLC_R = (diffSpeed_R / l->ffSpeed) * (double)l->numVehArr[sect][lane];

				/// (2) Evaluate number of OLC by subtrating number of MLC
				///     from the total number of LC.
				int numOLC_L = Evaluate_Prob(probLC_L) - numMLC_L;
				int numOLC_R = Evaluate_Prob(probLC_R) - numMLC_R;

				/// numOLC cannot be smaller than zero.
				if (numOLC_L < 0) numOLC_L = 0;
				if (numOLC_R < 0) numOLC_R = 0;

				/// (3) Select vehicle to change lane.
				if (numOLC_L != 0 && numOLC_R != 0) {
					Select_Veh(l, numOLC_L, numOLC_R, sect, lane);
				}
			}
		}
	}
}


/*--------------------------------------------------------------------*/
/// @fn      void LCSim(link*)
/// @brief   Function to move vehicles that perform lane change.
/// @param   link* l
/// @return  None
/*--------------------------------------------------------------------*/
void LCSim(link* l) {
	if (l->numLane > 1) {
		int numLC_L[NUM_SECTION + 2][NUM_LANE];
		int numLC_R[NUM_SECTION + 2][NUM_LANE];

		for (int sect = 1; sect < NUM_SECTION + 1; sect++) {
			for (int lane = 0; lane < l->numLane; lane++) {
				numLC_L[sect][lane] = 0;
				numLC_R[sect][lane] = 0;
			}
		}

		/// (1) MLC simulation
		for (int sect = 1; sect < NUM_SECTION + 1; sect++) {
			for (int lane = 0; lane < l->numLane; lane++) {
				for (int i = 0; i < MAX_VEC; i++) {
					/// (1-1) MLCR simulation
					if (l->vehMLC[sect][lane][i] == 1 && l->numVehArr[sect][lane + 1] < l->maxNumVeh[sect][lane]) {
						MoveLC(l->vehIDArr[sect][lane], l->numVehArr[sect][lane],
							l->vehIDArr[sect][lane + 1], l->numVehArr[sect][lane + 1] + 1, i);
						MoveLC(l->currLinkOrderArr[sect][lane], l->numVehArr[sect][lane],
							l->currLinkOrderArr[sect][lane + 1], l->numVehArr[sect][lane + 1] + 1, i);
						MoveLC(l->nextLinkIDArr[sect][lane], l->numVehArr[sect][lane],
							l->nextLinkIDArr[sect][lane + 1], l->numVehArr[sect][lane + 1] + 1, i);
						MoveLC(l->minTargetLaneArr[sect][lane], l->numVehArr[sect][lane],
							l->minTargetLaneArr[sect][lane + 1], l->numVehArr[sect][lane + 1] + 1, i);
						MoveLC(l->maxTargetLaneArr[sect][lane], l->numVehArr[sect][lane],
							l->maxTargetLaneArr[sect][lane + 1], l->numVehArr[sect][lane + 1] + 1, i);

						l->vehMLC[sect][lane][i] = 0;
						MoveLC(l->vehMLC[sect][lane], l->numVehArr[sect][lane],
							l->vehMLC[sect][lane + 1], l->numVehArr[sect][lane + 1] + 1, i);
						l->numVehArr[sect][lane + 1]++;
						//l->numVehArr[sect][lane]--;
						numLC_R[sect][lane]++;
					}

					/// (1-2) MLCL simulation
					else if (l->vehMLC[sect][lane][i] == -1 && l->numVehArr[sect][lane - 1] < l->maxNumVeh[sect][lane]) {
						MoveLC(l->vehIDArr[sect][lane], l->numVehArr[sect][lane],
							l->vehIDArr[sect][lane - 1], l->numVehArr[sect][lane - 1] + 1, i);
						MoveLC(l->currLinkOrderArr[sect][lane], l->numVehArr[sect][lane],
							l->currLinkOrderArr[sect][lane - 1], l->numVehArr[sect][lane - 1] + 1, i);
						MoveLC(l->nextLinkIDArr[sect][lane], l->numVehArr[sect][lane],
							l->nextLinkIDArr[sect][lane - 1], l->numVehArr[sect][lane - 1] + 1, i);
						MoveLC(l->minTargetLaneArr[sect][lane], l->numVehArr[sect][lane],
							l->minTargetLaneArr[sect][lane - 1], l->numVehArr[sect][lane - 1] + 1, i);
						MoveLC(l->maxTargetLaneArr[sect][lane], l->numVehArr[sect][lane],
							l->maxTargetLaneArr[sect][lane - 1], l->numVehArr[sect][lane - 1] + 1, i);

						l->vehMLC[sect][lane][i] = 0;
						MoveLC(l->vehMLC[sect][lane], l->numVehArr[sect][lane],
							l->vehMLC[sect][lane - 1], l->numVehArr[sect][lane - 1] + 1, i);
						l->numVehArr[sect][lane - 1]++;
						//l->numVehArr[sect][lane]--;
						numLC_L[sect][lane]++;
					}
				}
			}
		}


		/// (2) OLC simulation
		for (int sect = 1; sect < NUM_SECTION + 1; sect++) {
			for (int lane = 0; lane < l->numLane; lane++) {
				for (int i = 0; i < MAX_VEC; i++) {
					/// (2-1) OLCR simulation
					if (l->vehOLC[sect][lane][i] == 1 && l->numVehArr[sect][lane] < l->maxNumVeh[sect][lane]) {
						MoveLC(l->vehIDArr[sect][lane], l->numVehArr[sect][lane],
							l->vehIDArr[sect][lane + 1], l->numVehArr[sect][lane + 1] + 1, i);
						MoveLC(l->currLinkOrderArr[sect][lane], l->numVehArr[sect][lane],
							l->currLinkOrderArr[sect][lane + 1], l->numVehArr[sect][lane + 1] + 1, i);
						MoveLC(l->nextLinkIDArr[sect][lane], l->numVehArr[sect][lane],
							l->nextLinkIDArr[sect][lane + 1], l->numVehArr[sect][lane + 1] + 1, i);
						MoveLC(l->minTargetLaneArr[sect][lane], l->numVehArr[sect][lane],
							l->minTargetLaneArr[sect][lane + 1], l->numVehArr[sect][lane + 1] + 1, i);
						MoveLC(l->maxTargetLaneArr[sect][lane], l->numVehArr[sect][lane],
							l->maxTargetLaneArr[sect][lane + 1], l->numVehArr[sect][lane + 1] + 1, i);

						l->vehOLC[sect][lane][i] = 0;
						MoveLC(l->vehOLC[sect][lane], l->numVehArr[sect][lane],
							l->vehOLC[sect][lane + 1], l->numVehArr[sect][lane + 1] + 1, i);
						l->numVehArr[sect][lane + 1]++;
						//l->numVehArr[sect][lane]--;
						numLC_R[sect][lane]++;
					}

					/// (2-2) OLCL simulation
					else if (l->vehOLC[sect][lane][i] == -1 && l->numVehArr[sect][lane] < l->maxNumVeh[sect][lane]) {
						MoveLC(l->vehIDArr[sect][lane], l->numVehArr[sect][lane],
							l->vehIDArr[sect][lane - 1], l->numVehArr[sect][lane - 1] + 1, i);
						MoveLC(l->currLinkOrderArr[sect][lane], l->numVehArr[sect][lane],
							l->currLinkOrderArr[sect][lane - 1], l->numVehArr[sect][lane - 1] + 1, i);
						MoveLC(l->nextLinkIDArr[sect][lane], l->numVehArr[sect][lane],
							l->nextLinkIDArr[sect][lane - 1], l->numVehArr[sect][lane - 1] + 1, i);
						MoveLC(l->minTargetLaneArr[sect][lane], l->numVehArr[sect][lane],
							l->minTargetLaneArr[sect][lane - 1], l->numVehArr[sect][lane - 1] + 1, i);
						MoveLC(l->maxTargetLaneArr[sect][lane], l->numVehArr[sect][lane],
							l->maxTargetLaneArr[sect][lane - 1], l->numVehArr[sect][lane - 1] + 1, i);

						l->vehOLC[sect][lane][i] = 0;
						MoveLC(l->vehOLC[sect][lane], l->numVehArr[sect][lane],
							l->vehOLC[sect][lane - 1], l->numVehArr[sect][lane - 1] + 1, i);
						l->numVehArr[sect][lane - 1]++;
						//l->numVehArr[sect][lane]--;
						numLC_L[sect][lane]++;
					}
				}
			}
		}

		/// (3) Remove unwanted blanks
		for (int sect = 1; sect < NUM_SECTION + 1; sect++) {
			for (int lane = 0; lane < l->numLane; lane++) {
				if (l->numVehArr[sect][lane] > 0) {
					Remove_Blank(l->vehIDArr[sect][lane], MAX_VEC - 1);
					Remove_Blank(l->currLinkOrderArr[sect][lane], MAX_VEC - 1);
					Remove_Blank(l->nextLinkIDArr[sect][lane], MAX_VEC - 1);
					Remove_Blank(l->minTargetLaneArr[sect][lane], MAX_VEC - 1);
					Remove_Blank(l->maxTargetLaneArr[sect][lane], MAX_VEC - 1);
				}
			}
		}

		/// (4) Update numVehArr
		for (int sect = 1; sect < NUM_SECTION + 1; sect++) {
			for (int lane = 0; lane < l->numLane; lane++) {
				l->numVehArr[sect][lane] -= (numLC_L[sect][lane] + numLC_R[sect][lane]);
			}
		}

	}
}



/*====================================================================*/
/// CF Functions
/// Functions that are involved in evaluation and performanace of CF
/// 1. void Evaluate_CF(link*)
/// 2. void CFsim(link*)
/*====================================================================*/

/*--------------------------------------------------------------------*/
/// @fn      void Evaluate_CF(link*)
/// @brief   Function that evaluate CF of a vehicle and updates vehCF Flag.
/// @param   link* l
/// @return  None
/*--------------------------------------------------------------------*/
void Evaluate_CF(link* l) {
	double wSpeed = 4.2;

	for (int sect = 0; sect < NUM_SECTION; sect++) {
		for (int lane = 0; lane < l->numLane; lane++) {
			l->numCF[sect][lane] = MAX(0, MIN(l->numVehArr[sect][lane], MIN(l->maxNumCF[sect][lane], wSpeed / l->ffSpeed * (l->maxNumVeh[sect + 1][lane] - l->numVehArr[sect + 1][lane])))) * l->ffSpeed * SECONDS_PER_STEP / l->lenSection[sect];

			for (int i = 0; i < l->numCF[sect][lane]; i++) {
				l->vehCF[sect][lane][i] = 1;
			}
		}
	}
}


/*--------------------------------------------------------------------*/
/// @fn      void CFsim(link*)
/// @brief   Function to move vehicles that perform CF.
/// @param   link *l
/// @return  None
/*--------------------------------------------------------------------*/
void CFsim(link *l) {
	for (int sect = NUM_SECTION; sect > 0; sect--) {
		for (int lane = 0; lane < NUM_LANE; lane++) {
			if (l->numVehArr[sect][lane] > 0) {
				for (int i = 0; i < MAX_VEC; i++) {
					if (l->vehCF[sect][lane][0] == 1 && l->numVehArr[sect + 1][lane] < l->maxNumVeh[sect][lane]) {

						MoveCF(l->vehIDArr[sect][lane], l->numVehArr[sect][lane],
							l->vehIDArr[sect + 1][lane], l->numVehArr[sect + 1][lane], 0);
						MoveCF(l->currLinkOrderArr[sect][lane], l->numVehArr[sect][lane],
							l->currLinkOrderArr[sect + 1][lane], l->numVehArr[sect + 1][lane], 0);
						MoveCF(l->nextLinkIDArr[sect][lane], l->numVehArr[sect][lane],
							l->nextLinkIDArr[sect + 1][lane], l->numVehArr[sect + 1][lane], 0);
						MoveCF(l->minTargetLaneArr[sect][lane], l->numVehArr[sect][lane],
							l->minTargetLaneArr[sect + 1][lane], l->numVehArr[sect + 1][lane], 0);
						MoveCF(l->maxTargetLaneArr[sect][lane], l->numVehArr[sect][lane],
							l->maxTargetLaneArr[sect + 1][lane], l->numVehArr[sect + 1][lane], 0);
						MoveCF(l->vehCF[sect][lane], l->numVehArr[sect][lane], l->vehCF[sect + 1][lane], l->numVehArr[sect + 1][lane], 0);
						l->numVehArr[sect + 1][lane]++;
						l->numVehArr[sect][lane]--;
					}
				}
			}
		}
	}
}





/*====================================================================*/
/// Between-Link Functions
/// Functions that are involved in evaluation and performanace of CF
/// 1. void Update_tempArr(link*)
/// 2. void Relay_numVeh(link*, link*, int, connection_cell*, int, int, int)
/// 3. void Relay_numCF(link*, link*, int, connection_cell*, int, int, int)
/// 4. void Evaluate_Eff_numCF(link*)
/// 5. void Update_nextLink(vehicle*, link*, link*, int, connection_cell*, int)
/*====================================================================*/

/*--------------------------------------------------------------------*/
/// @fn      void Update_Link_TempArr(link*)
/// @brief   Function that update tempIDArr and tempNumArr.
/// @param   link* l
/// @return  None
/*--------------------------------------------------------------------*/
void Update_tempArr(link* l) {
	for (int lane = 0; lane < l->numLane; lane++) {
		int tempArrSize = 0;

		if (l->numVehArr[NUM_SECTION][lane] > 0) {
			for (int i = 0; i < l->numVehArr[NUM_SECTION][lane]; i++) {
				int tempArrIndex = Find_Index(l->tempIDArr[lane], MAX_LEG, l->nextLinkIDArr[NUM_SECTION][lane][i]);

				if (tempArrIndex == -1) {
					l->tempIDArr[lane][tempArrSize] = l->nextLinkIDArr[NUM_SECTION][lane][i];
					l->tempNumArr[lane][tempArrSize] += 1;
					tempArrSize += 1;
				}

				else l->tempNumArr[lane][tempArrIndex] += 1;
			}
		}
	}
}


/*--------------------------------------------------------------------*/
/// @fn      void Relay_numVeh(link*, link*, int, connection_cell*, int, int, int)
/// @brief   Function that relay numVeh of the last cell of a previous 
///          link to the virtual cell of the next link.
/// @param   link* prevl, link* nextl, int nextLane, connection_cell* cc, 
///			 int currLane, int leg, int currLink
/// @return  None
/*--------------------------------------------------------------------*/
void Relay_numVeh(link* prevl, link* nextl, int nextLane, connection_cell* cc, int currLane, int leg) {
	/// (1) Relay numVeh from tempArr of previous link to next link
	int index = Find_Index(prevl->tempIDArr[currLane], MAX_LEG, cc->nextLinkID[currLane][leg]);
	int index2 = Find_Index(cc->nextLinkID[currLane], MAX_LEG, prevl->tempIDArr[currLane][0]);

	if (index != -1) {
		/// (1-1) tempArr of previous link -> connection cell
		cc->numVehArr[currLane][leg] = prevl->tempNumArr[currLane][index];

		/// (1-2) connection cell -> virtual cell of next link
		nextl->numVehArr[0][nextLane] = cc->numVehArr[currLane][leg];
	}

	if (index2 == -1) {
		nextl->numVehArr[0][nextLane] = 0;
	}

}


/*--------------------------------------------------------------------*/
/// @fn      void Relay_numCF(link*, link*, int, connection_cell*, int, int, int)
/// @brief   Function that relay numCF of the virtual cell of a next 
///          link to the last cell of the previous link.
/// @param   link* prevl, link* nextl, int nextLane, connection_cell* cc, 
///			 int currLoop, int currLane, int leg
/// @return  None
/*--------------------------------------------------------------------*/
void Relay_numCF(link* prevl, link* nextl, int nextLane, connection_cell* cc, int currLoop, int currLane, int leg) {
	/// (1) virtual cell of next link -> connection cell
	int signalLoop = currLoop % cc->cycleDuration;
	cc->numCFArr[currLane][leg] = nextl->numCF[0][nextLane] * cc->trafficSignal[currLane][signalLoop];

	/// (2) connection cell -> previous link
	int index = Find_Index(prevl->tempIDArr[currLane], MAX_LEG, cc->nextLinkID[currLane][leg]);

	if (index != -1) {
		cc->numCFArr[currLane][leg] = ceil(cc->numCFArr[currLane][leg]);
		prevl->tempNumArr[currLane][index] = cc->numCFArr[currLane][leg];
	}
}


/*--------------------------------------------------------------------*/
/// @fn      void Evaluate_Eff_numCF(link*)
/// @brief   Function that evaluates effective numCF.
/// @param   link* l
/// @return  None
/*--------------------------------------------------------------------*/
void Evaluate_Eff_numCF(link* l) {
	for (int lane = 0; lane < l->numLane; lane++) {
		int tempArrSize = 0;
		int totalNumCF = 0;

		for (int i = 0; i < MAX_LEG; i++) {
			totalNumCF += l->tempNumArr[lane][i];

			if (l->tempIDArr[lane][i] == -1) {
				tempArrSize = i;
				break;
			}
		}

		int effNumCF = 0;

		for (int count = 0; count < MIN(totalNumCF, l->maxNumCF[NUM_SECTION][lane]); count++) {
			for (int i = 0; i < MAX_LEG; i++) {
				if (l->tempNumArr[lane][i] == 0) break;

				else {
					int currNextLinkID = l->nextLinkIDArr[NUM_SECTION][lane][i];
					int index = Find_Index(l->tempIDArr[lane], tempArrSize, currNextLinkID);

					l->tempNumArr[lane][index]--;
					effNumCF++;
				}
			}
		}

		l->numCF[NUM_SECTION][lane] = effNumCF;
	}

	for (int lane = 0; lane < l->numLane; lane++) {
		for (int i = 0; i < l->numCF[NUM_SECTION][lane]; i++) {
			l->vehCF[NUM_SECTION][lane][i] = 1;
		}
	}
}


/*--------------------------------------------------------------------*/
/// @fn      void Update_nextLink(vehicle*, link*, link*, int, connection_cell*, int)
/// @brief   Function update variables of first cell of a link using
///          variables of connection cell. 
/// @param   vehicle* v, link* prevl, link* nextl, int nextLane, connection_cell* cc, int currLane
/// @return  None
/*--------------------------------------------------------------------*/
void Update_nextLink(vehicle* v, link* prevl, link* nextl, int nextLane, connection_cell* cc, int currLane, int* vehTotIDarr, int numVeh) {
	/// (1) previous link -> connection cell
	for (int i = 0; i < prevl->numVehArr[NUM_SECTION + 1][currLane]; i++) {
		cc->currLinkOrderArr[currLane][i] = prevl->currLinkOrderArr[NUM_SECTION + 1][currLane][i];
		cc->nextLinkIDArr[currLane][i] = prevl->nextLinkIDArr[NUM_SECTION + 1][currLane][i];
		cc->vehIDArr[currLane][i] = prevl->vehIDArr[NUM_SECTION + 1][currLane][i];
	}

	/// (2) connection cell -> first cell of next link
	int index = nextl->numVehArr[1][nextLane];

	for (int i = 0; i < MAX_VEC; i++) {
		int currOrder = cc->currLinkOrderArr[currLane][i];
		int currVehID = Find_Index(vehTotIDarr, numVeh, cc->vehIDArr[currLane][i]);

		if (cc->nextLinkIDArr[currLane][i] == nextl->linkID) {
			nextl->vehIDArr[1][nextLane][index] = cc->vehIDArr[currLane][i];
			nextl->currLinkOrderArr[1][nextLane][index] = cc->currLinkOrderArr[currLane][i] + 1;
			nextl->nextLinkIDArr[1][nextLane][index] = v[currVehID].path[currOrder + 2];
			nextl->minTargetLaneArr[1][nextLane][index] = v[currVehID].minTargetLane[currOrder + 1];
			nextl->maxTargetLaneArr[1][nextLane][index] = v[currVehID].maxTargetLane[currOrder + 1];
			index++;
		}
	}

	nextl->numVehArr[1][nextLane] = index;
}





/*====================================================================*/
/// Reset Functions
/// Functions that are reset variables and flags
/// 1. void Reset_Link(link*)
/// 2. void Reset_Sink(sink_cell*)
/// 3. void Reset_ConnectionCell(connection_cell*)
/*====================================================================*/

/*--------------------------------------------------------------------*/
/// @fn      void Reset_Link(link*)
/// @brief   Function that resets flags and temp variables of link.
/// @param   link* l
/// @return  None
/*--------------------------------------------------------------------*/
void Reset_Link(link* l) {
	for (int sect = 0; sect < NUM_SECTION + 2; sect++) {
		for (int lane = 0; lane < l->numLane; lane++) {
			l->numVehArr[0][lane] = 0;
			l->numVehArr[NUM_SECTION + 1][lane] = 0;
		}
	}

	for (int sect = 0; sect < NUM_SECTION + 2; sect++) {
		for (int lane = 0; lane < l->numLane; lane++) {
			l->numMLCL[sect][lane] = 0;
			l->numMLCR[sect][lane] = 0;
			l->numCF[sect][lane] = 0;

			for (int i = 0; i < MAX_VEC; i++) {
				l->vehMLC[sect][lane][i] = 0;
				l->vehOLC[sect][lane][i] = 0;
				l->vehCF[sect][lane][i] = 0;

				l->vehIDArr[NUM_SECTION + 1][lane][i] = -1;
				l->currLinkOrderArr[NUM_SECTION + 1][lane][i] = -1;
				l->nextLinkIDArr[NUM_SECTION + 1][lane][i] = -1;
				l->minTargetLaneArr[NUM_SECTION + 1][lane][i] = -1;
				l->maxTargetLaneArr[NUM_SECTION + 1][lane][i] = -1;
			}
		}
	}

	for (int lane = 0; lane < l->numLane; lane++) {
		for (int i = 0; i < 3; i++) {
			l->tempIDArr[lane][i] = -1;
			l->tempNumArr[lane][i] = 0;
		}
	}
}


/*--------------------------------------------------------------------*/
/// @fn      void Reset_Sink(sink_cell* sk)
/// @brief   Function that resets flags and temp variables of sink.
/// @param   sink_cell* sk
/// @return  None
/*--------------------------------------------------------------------*/
void Reset_Sink(sink_cell* sk) {
	sk->numVeh = 0;

	for (int i = 0; i < MAX_SINK_VEC; i++) {
		sk->vehIDArr[i] = -1;
		sk->arrivalTime[i] = 0;
	}
}


/*--------------------------------------------------------------------*/
/// @fn      void Reset_ConnectionCell(connection_cell* cc)
/// @brief   Function that resets variables of connection cell.
/// @param   connection_cell* cc
/// @return  None
/*--------------------------------------------------------------------*/
void Reset_ConnectionCell(connection_cell* cc) {
	for (int lane = 0; lane < NUM_LANE; lane++) {
		for (int i = 0; i < 3; i++) {
			cc->numVehArr[lane][i] = 0;
			cc->numCFArr[lane][i] = 0;
		}

		for (int i = 0; i < MAX_VEC; i++) {
			cc->currLinkOrderArr[lane][i] = -1;
			cc->nextLinkIDArr[lane][i] = -1;
			cc->vehIDArr[lane][i] = -1;
		}
	}
}


void PrintSumLane(link l[], int numLink) {
	int nlane = 0;
	int maxLink = 100;
	if (numLink < maxLink) maxLink = numLink;

	for (int link = 0; link < maxLink; link++) {
		printf("link(%d): ", link);
		for (int sect = 0; sect < NUM_SECTION + 2; sect++) {
			for (int lane = 0; lane < NUM_LANE; lane++) {
				nlane += l[link].numVehArr[sect][lane];
			}
			printf("s%d: %d, ", sect, nlane);
			nlane = 0;
		}
		printf("\n");
	}
}

void PrintSumLaneN(link l[], int curlink) {
	int nlane = 0;
	
	printf("link(%d): ", curlink);
	for (int sect = 0; sect < NUM_SECTION + 2; sect++) {
		for (int lane = 0; lane < NUM_LANE; lane++) {
			nlane += l[curlink].numVehArr[sect][lane];
		}
		printf("s%d: %d, ", sect, nlane);
		nlane = 0;
	}
	printf("\n");	
}


/*--------------------------------------------------------------------*/
/// @fn      void SimulationStep(vehicle* v, int numVeh, link l[], 
/// 		 source_cell sc[], sink_cell sk[], connection_cell cc[], int numLink, int numLoop)
/// @brief   Function that manages the whole process of simulation.
/// @param   vehicle* v, int numVeh, link l[], source_cell sc[], sink_cell sk[], 
/// 		 connection_cell cc[], int numLink, int numLoop
/// @return  None
/*--------------------------------------------------------------------*/
void SimulationStep(vehicle* v, int numVeh, link l[], source_cell sc[], sink_cell sk[], connection_cell cc[], int numLink, int numLoop, int linkIDarr[], int vehTotIDarr[]) {
	for (int count = 0; count < numLoop; count++) {
		clock_t s_start =0;
		clock_t s_end=0;

		int sec = count * SECONDS_PER_STEP;
		printf("=====simulationStep:%d===== %02d:%02d:%02d\n", count, sec / 3600, (sec % 3600) / 60, (sec % 3600) % 60);
		
		//float wTime, wTime1, wTime2, wTime3, wTime4;
		//clock_t sTime, eTime;
		//sTime = clock();

		s_start = clock();
		int updateStep = count % UPDATE_INTERVAL;
		if (updateStep == 0) {
			for (int link = 0; link < numLink; link++) {
				Reset_Sink(&sk[link]);
			}

			Update_Source(v, numVeh, sc, count, numLink, linkIDarr);
		}
		s_end = clock();
		printf("ResetSink&Update_Source:%d\n", s_end-s_start);
		//eTime = clock();
		//wTime1 = eTime - sTime;
		//printf("=====Stage 1 :\t%fms=====\n", wTime1);
		//sTime = clock();

		s_start = clock();
		for (int link = 0; link < numLink; link++) {
			Exit_Link(&l[link], &sk[link], numLoop);
			Enter_Link(&l[link], &sc[link]);
		}
		s_end = clock();
		printf("Exit&Enter_Link:%d\n", s_end-s_start);

		//PrintSumLaneN(l, 453);

		s_start = clock();
		for (int link = 0; link < numLink; link++) {
			update_Speed(&l[link]);
		}
		s_end = clock();
		printf("update_speed: %d\n", s_end-s_start);

		s_start = clock();
		for (int link = 0; link < numLink; link++) {
			Evaluate_MLC(&l[link]);
			Evaluate_OLC(&l[link]);
			LCSim(&l[link]);
			Update_tempArr(&l[link]);
		}
		s_end = clock();
		printf("MLC,OLC,LCSim,Update_tempArr: %d\n", s_end-s_start);

	
		s_start = clock();
		for (int link = 0; link < numLink; link++) {
			update_Speed(&l[link]);
		}
		s_end = clock();
		printf("update_speed: %d\n", s_end-s_start);

		//eTime = clock();
		//wTime2 = eTime - sTime;
		//printf("=====Stage 2 :\t%fms=====\n", wTime2);
		//sTime = clock();

		s_start = clock();
		for (int link = 0; link < numLink; link++) {
			for (int lane = 0; lane < NUM_LANE; lane++) {
				for (int leg = MAX_LEG - 1; leg >= 0; leg--) {
					if (cc[link].nextLinkID[lane][leg] != -1) {
						if (cc[link].nextLinkID[lane][leg + 1] != -1 && cc[link].nextLinkID[lane][leg] == cc[link].nextLinkID[lane][leg + 1]) {

						}
						else {
							int prevIndex = Find_Index(linkIDarr, numLink, cc[link].prevLinkID);
							int nextIndex = Find_Index(linkIDarr, numLink, cc[link].nextLinkID[lane][leg]);
							Relay_numVeh(&l[prevIndex], &l[nextIndex], cc[link].nextLane[lane][leg], &cc[link], lane, leg);
						}
					}
				}
			}

			Reset_ConnectionCell(&cc[link]);
		}
		s_end = clock();
		printf("Relay_numVeh: %d\n", s_end-s_start);

		s_start = clock();
		for (int link = 0; link < numLink; link++) {
			Evaluate_CF(&l[link]);
		}
		s_end = clock();
		printf("Evlauate_CF: %d\n", s_end-s_start);

		s_start = clock();
		for (int link = 0; link < numLink; link++) {
			for (int lane = 0; lane < NUM_LANE; lane++) {
				for (int leg = MAX_LEG - 1; leg >= 0; leg--) {
					if (cc[link].nextLinkID[lane][leg] != -1) {
						if (cc[link].nextLinkID[lane][leg + 1] != -1 && cc[link].nextLinkID[lane][leg] == cc[link].nextLinkID[lane][leg + 1]) {

						}
						else {
							int prevIndex = Find_Index(linkIDarr, numLink, cc[link].prevLinkID);
							int nextIndex = Find_Index(linkIDarr, numLink, cc[link].nextLinkID[lane][leg]);

							Relay_numCF(&l[prevIndex], &l[nextIndex], cc[link].nextLane[lane][leg], &cc[link], count, lane, leg);
						}
					}
				}
			}

			Reset_ConnectionCell(&cc[link]);
		}
		s_end = clock();
		printf("Relay_numCF: %d\n", s_end-s_start);

		s_start = clock();
		for (int link = 0; link < numLink; link++) {
			Evaluate_Eff_numCF(&l[link]);
			CFsim(&l[link]);
		}
		s_end = clock();
		printf("CFSim: %d\n", s_end-s_start);

		s_start= clock();
		for (int link = 0; link < numLink; link++) {
			for (int lane = 0; lane < NUM_LANE; lane++) {
				for (int leg = MAX_LEG - 1; leg >= 0; leg--) {
					if (cc[link].nextLinkID[lane][leg] != -1) {
						if (cc[link].nextLinkID[lane][leg + 1] != -1 && cc[link].nextLinkID[lane][leg] == cc[link].nextLinkID[lane][leg + 1]) {

						}
						else {
							int prevIndex = Find_Index(linkIDarr, numLink, cc[link].prevLinkID);
							int nextIndex = Find_Index(linkIDarr, numLink, cc[link].nextLinkID[lane][leg]);

							Update_nextLink(v, &l[prevIndex], &l[nextIndex], cc[link].nextLane[lane][leg], &cc[link], lane, vehTotIDarr, numVeh);
						}
					}
				}
				Reset_ConnectionCell(&cc[link]);
			}
		}
		s_end = clock();
		printf("Update_nextLink: %d\n", s_end-s_start);
		//eTime = clock();
		//wTime3 = eTime - sTime;
		//printf("=====Stage 3 :\t%fms=====\n", wTime3);
		//sTime = clock();

		s_start = clock();
		for (int link = 0; link < numLink; link++) {
			update_Speed(&l[link]);
		}
		s_end = clock();
		printf("Update_Speed: %d\n", s_end-s_start);


		s_start = clock();
		for (int link = 0; link < numLink; link++) {
			Reset_Link(&l[link]);
		}
		s_end = clock();
		printf("Reset_Link: %d\n", s_end-s_start);

		//eTime = clock();
		//wTime4 = eTime - sTime;
		//printf("=====Stage 4 :\t%fms=====\n", wTime4);
		//wTime = wTime2 + wTime3 + wTime4;
		//printf("=====Stage 2~4 :\t%fms=====\n", wTime);
		Export_All_Result(mylink, numLink, numLoop, sec);

	}
}


int main(int argc, char **argv){
	
	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &my_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	vector<MyData> data_list;
	int num;
	int index = 0;

	if(my_rank == 0){
		
char* srcDir = "TARGET";
	int numLink = getNumLink(srcDir);
	int numVeh = getNumVeh(srcDir);

	//numLink = (int)atoi(argv[1]);
numLink = 1000;

	int* linkIDarr = (int* ) malloc(numLink * sizeof(int));
	memset(linkIDarr, 0, numLink * sizeof(int));
	int* vehTotIDarr = (int* ) malloc(numVeh * sizeof(int));
	memset(vehTotIDarr, 0, numVeh * sizeof(int));
	int* ccIDarr = (int* ) malloc(numLink * sizeof(int));
	memset(ccIDarr, 0, numLink * sizeof(int));

	myveh = (vehicle*)calloc(numVeh, sizeof(vehicle));
	mylink = (link*)calloc(numLink, sizeof(link));
	mycon = (connection_cell*)malloc(numLink * sizeof(connection_cell));
	mysource = (source_cell*)malloc(numLink * sizeof(source_cell));
	mysink = (sink_cell*)malloc(numLink * sizeof(sink_cell));

	printf("Setup_Link : %d\n", numLink);
	Setup_Link(mylink, srcDir, linkIDarr, numLink);
	printf("Setup_Vehicle : %d\n", numVeh);
	Setup_Vehicle_Test(myveh, srcDir, vehTotIDarr, numVeh, linkIDarr, numLink);
	printf("Setup_ConnectionCell : %d\n", numLink);
	Setup_ConnectionCell_Test(mycon, srcDir, ccIDarr, numLink, linkIDarr);
	printf("Setup_SourceCell\n");
	Setup_SourceCell(mysource, numLink, linkIDarr);
	printf("Setup_SinkCell\n");
	Setup_SinkCell(mysink, numLink, linkIDarr);

	int minLoop, maxLoop;
	minLoop = maxLoop = 100;

	float eTime[MAX_LOOP];

	for (int numLoop = minLoop; numLoop <= maxLoop; numLoop++) {

		printf("====================================================================\n");
		printf("# of simulation steps(Max): %d\n", maxLoop);
		printf("current simulation step: %d\n", numLoop);

		clock_t start = 0, stop = 0;
		float elapsed;

		start = clock();
		SimulationStep(myveh, numVeh, mylink, mysource, mysink, mycon, numLink, numLoop, linkIDarr, vehTotIDarr);
		stop = clock();

		printf("====================================================================\n");
		printf("After Simulation\n");
		elapsed = (float)(stop - start) / CLOCKS_PER_SEC;
		//printf("Simulation start : %u, stop: %u\n", start, stop);
		printf("Elapsed Time:\t%.3f s\n", elapsed);
		printf("====================================================================\n");

		eTime[numLoop - 1] = elapsed;
		Export_All_Result(mylink, numLink, numLoop, 100);
	}

	printf("Totla Elapsed Time(cuda)============================================\n");
	for (int l = minLoop; l <= maxLoop; l++) {
		printf("L%d = %.3f, ", l, eTime[l - 1]);
	}
	printf("\n====================================================================\n");


	free(linkIDarr);
	free(vehTotIDarr);
	free(ccIDarr);
	free(myveh);
	//free(mylink);
	free(mycon);
	free(mysource);
	free(mysink);

////



    } else {
	printf("Simulation Node 1 initialized.\n");
        /* Receive a message with a maximum length of 20 characters from process
           with rank 0. */
        //MPI_Recv(message, 20, MPI_CHAR, 0, tag, MPI_COMM_WORLD, &status);
        //printf("received %s\n", message);

char* srcDir = "TARGET";
	int numLink = getNumLink(srcDir);
	int numVeh = getNumVeh(srcDir);

	//numLink = (int)atoi(argv[1]);
numLink = 1000;

	int* linkIDarr = (int* ) malloc(numLink * sizeof(int));
	memset(linkIDarr, 0, numLink * sizeof(int));
	int* vehTotIDarr = (int* ) malloc(numVeh * sizeof(int));
	memset(vehTotIDarr, 0, numVeh * sizeof(int));
	int* ccIDarr = (int* ) malloc(numLink * sizeof(int));
	memset(ccIDarr, 0, numLink * sizeof(int));

	myveh = (vehicle*)calloc(numVeh, sizeof(vehicle));
	mylink = (link*)calloc(numLink, sizeof(link));
	mycon = (connection_cell*)malloc(numLink * sizeof(connection_cell));
	mysource = (source_cell*)malloc(numLink * sizeof(source_cell));
	mysink = (sink_cell*)malloc(numLink * sizeof(sink_cell));

	printf("Setup_Link : %d\n", numLink);
	Setup_Link(mylink, srcDir, linkIDarr, numLink);
	printf("Setup_Vehicle : %d\n", numVeh);
	Setup_Vehicle_Test(myveh, srcDir, vehTotIDarr, numVeh, linkIDarr, numLink);
	printf("Setup_ConnectionCell : %d\n", numLink);
	Setup_ConnectionCell_Test(mycon, srcDir, ccIDarr, numLink, linkIDarr);
	printf("Setup_SourceCell\n");
	Setup_SourceCell(mysource, numLink, linkIDarr);
	printf("Setup_SinkCell\n");
	Setup_SinkCell(mysink, numLink, linkIDarr);

	int minLoop, maxLoop;
	minLoop = maxLoop = 100;

	float eTime[MAX_LOOP];

	for (int numLoop = minLoop; numLoop <= maxLoop; numLoop++) {

		printf("====================================================================\n");
		printf("# of simulation steps(Max): %d\n", maxLoop);
		printf("current simulation step: %d\n", numLoop);

		clock_t start = 0, stop = 0;
		float elapsed;

		start = clock();
		SimulationStep(myveh, numVeh, mylink, mysource, mysink, mycon, numLink, numLoop, linkIDarr, vehTotIDarr);
		stop = clock();

		printf("====================================================================\n");
		printf("After Simulation\n");
		elapsed = (float)(stop - start) / CLOCKS_PER_SEC;
		//printf("Simulation start : %u, stop: %u\n", start, stop);
		printf("Elapsed Time:\t%.3f s\n", elapsed);
		printf("====================================================================\n");

		eTime[numLoop - 1] = elapsed;
		Export_All_Result(mylink, numLink, numLoop, 100);
	}

	printf("Totla Elapsed Time(cuda)============================================\n");
	for (int l = minLoop; l <= maxLoop; l++) {
		printf("L%d = %.3f, ", l, eTime[l - 1]);
	}
	printf("\n====================================================================\n");


	free(linkIDarr);
	free(vehTotIDarr);
	free(ccIDarr);
	free(myveh);
	//free(mylink);
	free(mycon);
	free(mysource);
	free(mysink);

	}

//	Broadcast_Vehicle(&data_list, 0);
//	runGPU(&data_list, my_rank);
	MPI_Finalize();
	return 0;
}

void Broadcast_Num(int* num, int rank){
	MPI_Bcast(num, 1, MPI_INT, rank, MPI_COMM_WORLD);
}

void Broadcast_Vehicle(vector<MyData>* v, int rank){
	int veh_num;

	if(rank == my_rank){
		veh_num = v->size();
	}

	Broadcast_Num(&veh_num, rank);

	if(rank != my_rank){
		v->resize(veh_num);
	}
	MPI_Bcast((void*)v->data(), sizeof(MyData) * veh_num, MPI_BYTE, rank, MPI_COMM_WORLD);

	
	FILE* fp = fopen("BroadCast.txt", "w");

	for(int i = 0 ; i < v->size(); i++){
		printf("Processor[%d] => data[%d]: %d \n", my_rank, i, v->at(i).my_num);
		fprintf(fp,"%d \n", v->at(i).my_num);
	}
	fclose(fp);
}

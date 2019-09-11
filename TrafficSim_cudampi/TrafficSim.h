#ifndef TRAFFICSIM_H_
#define TRAFFICSIM_H_

/*--------------------------------------------------------------------*/
/// NUM_LANE 				# of lane in one link
/// NUM_SECTION   			# of section in one link
/// MAX_VEC         		Maximum # of vehicle in one cell
/// MAX_SOURCE_VEC			Maximum # of vehicle in one source cell
/// MAX_SINK_VEC			Maximum # of vehicle in one sink cell
/// MAX_PATH				Maximum # of link in a path of a vehicle
/// MAX_LOOP				Maximum # of simulation step
/// MAX_LEG					Maximum # of leg in one intersection
/// MAX_CYCLE_DURATION		Maximum duration of one cycle of a traffic signal
/// SECONDS_PER_STEP		Seconds per one simulation step
/// UPDATE_INTERVAL			Time interval between source cell update
/// JAMDENSITY				Jam density (veh/km)
/// WAVESPEED 				Wave speed (km/h)
/// FFSPEED 				Free flow speed (km/h)
/*--------------------------------------------------------------------*/
#define NUM_LANE    		7
#define NUM_SECTION 		4
#define MAX_VEC     		20	
#define MAX_SOURCE_VEC     	100
#define MAX_SINK_VEC     	100	
#define MAX_PATH			120	
#define MAX_LOOP    		300
#define MAX_LEG				5
#define MAX_CYCLE_DURATION  40
#define SECONDS_PER_STEP   	5
#define UPDATE_INTERVAL 	60
#define JAMDENSITY          200
#define WAVESPEED           15
#define FFSPEED             60
#define MAX_FILENAME_LEN    50

/*--------------------------------------------------------------------*/
/// MIN(a,b) 				Returns smaller variable between a,b
/// MAX(a,b)				Returns larger variable between a,b
/*--------------------------------------------------------------------*/
#define MIN(a,b)			(((a)<(b)) ? (a):(b))
#define MAX(a,b)			(((a)>(b)) ? (a):(b))

/*--------------------------------------------------------------------*/
/// TRUE 					1
/// FALSE					0
/*--------------------------------------------------------------------*/
#define TRUE				1
#define FALSE				0

/*--------------------------------------------------------------------*/
/// PRINT_INFO 				Flag to decide whether to print info
/// PRINT_WARNING   		Flag to decide whether to print warnings
/// PRINT_ERROR         	Flag to decide whether to print errors
/// PRINT_CRITICAL        	Flag to decide whether to print critical errors
/*--------------------------------------------------------------------*/
#define PRINT_INFO			TRUE
/// TrafficSim_Single.c:<Function_Name>: [info] <Variable_Name> = <Variable_Value>
#define PRINT_WARNING		FALSE
/// TrafficSim_Single.c:<Function_Name>: [warning] <Warning_Name>::<Variable_Name> = <Variable_Value>
#define PRINT_ERROR			FALSE
/// TrafficSim_Single.c:<Function_Name>: [error] <Error_Name>::<Variable_Name> = <Variable_Value>
#define PRINT_CRITICAL		TRUE
/// TrafficSim_Single.c:<Function_Name>: [critical] <Error_Name>::<Variable_Name> = <Variable_Value>

/*--------------------------------------------------------------------*/
/// @enum 	 typedef enum message_categories
/// @brief 	 Enum of various categories of messages that can be printed.
/*--------------------------------------------------------------------*/
typedef enum {
	INFO = 0,
	WARNING,
	ERROR,
	CRITICAL,
} message_categories;

/*--------------------------------------------------------------------*/
/// @enum 	 typedef enum function_names
/// @brief 	 Enum of names of all functions of TrafficSim_Single.c.
/*--------------------------------------------------------------------*/
typedef enum {
	PRINT_VEHICLE = 0,
	PRINT_LINK,
	PRINT_SOURCECELL,
	PRINT_SINKCELL,
	PRINT_CONNECTIONCELL,
	PRINT_ALL_RESULTS,
	SETUP_VEHICLE,
	SETUP_LINK,
	SETUP_SOURCECELL,
	SETUP_SINKCELL,
	SETUP_CONNECTIONCELL,
	MOVELC,
	MOVECF,
	REMOVE_BLANK,
	FIND_INDEX,
	EVAULATE_PROB,
	UPDATE_SOURCE,
	ENTER_LINK,
	EXIT_LINK,
	UPDATE_SPEED,
	EVAULATE_MLC,
	SELECT_VEH,
	EVALUATE_OLC,
	LCSIM,
	EVALUATE_CF,
	CFSIM,
	UPDATE_TEMPARR,
	RELAY_NUMVEH,
	RELAY_NUMCF,
	EVALUATE_EFF_NUMCF,
	UPDATE_NEXTLINK,
	RESET_LINK,
	RESET_SINK,
	RESET_CONNECTIONCELL,

} function_names;


/*--------------------------------------------------------------------*/
/// @struct  typedef struct vehicle
/// @brief   Structure that holds all information of vehicles.
/// 
/// int vehIDArr		ID # of a vehicle
/// int vehType			Type of a vehicle (currently not used)
/// int path[]			Array of link ID # of a vehicle path
/// int lenPath			Length of a vehicle path
/// int minTargetLane[]	Minimum target lane of vehicles
/// int maxTargetLane[] Maximum target lane of vehicles
/// int initLink;		Initial link ID # of a vehicle
/// int departTime; 	Time step in which a vehicle departs from the 
///						source cell, or in other words, enters the link  
/*--------------------------------------------------------------------*/
typedef struct {

	int vehID;
	int vehType;

	int path[MAX_PATH];
	int lenPath;

	int minTargetLane[MAX_PATH];
	int maxTargetLane[MAX_PATH];

	int initLink;
	int departTime;

} vehicle;


/*--------------------------------------------------------------------*/
/// @struct  typedef struct link
/// @brief   Structure that holds information of links.
/// 
/// int linkID					ID # of a link
/// double ffSpeed 				Free flow speed of a link
/// double lenSection[]			length of a section
/// int maxNumVeh[][]	   		Maximum # of vehicles in one cell
/// double maxNumCF[][]			Maximum # of vehicles that perform CF
/// int numVehArr[][]			# of vehicles in a cell
/// int vehIDArr[][][]			ID # of vehicles in a cell
/// int currLinkOrderArr[][][]	Current link order in vehicle paths
/// int nextLinkIDArr[][][]		ID # of next link of vehicles
/// int minTargetLaneArr[][][]	Minimum target lane of vehicles 
/// int maxTargetLaneArr[][][]	Maximum target lane of vehicles
/// int speed[][]				Average speed of vehicles in one cell
/// int numMLCL[][]				# of vehicles that perform leftside MLC
/// int numMLCR[][]				# of vehicles that perform rightisde MLC
/// double numCF[][]			# of vehicles that perform CF
/// int vehMLC[][][]	   		Flag that indicates direction of MLC
/// 							(default: 0, left: -1, right: 1)
/// int vehOLC[][][] 			Flag that indicates direction of OLC
/// 							(default: 0, left: -1, right: 1)
/// int vehCF[][][]   			Flag that indicates whether a vehicle 
///								performs CF or not
/// int tempIDArr[][] 			Temporary storage for ID # infomation
/// int tempNumArr[][]			Temporary storage for # information
/*--------------------------------------------------------------------*/
typedef struct {

	int linkID;
	int numLane;

	double ffSpeed;
	double lenSection[NUM_SECTION + 2];
	int maxNumVeh[NUM_SECTION + 2][NUM_LANE];
	int maxNumCF[NUM_SECTION + 2][NUM_LANE];

	int numVehArr[NUM_SECTION + 2][NUM_LANE];
	int vehIDArr[NUM_SECTION + 2][NUM_LANE][MAX_VEC];
	int currLinkOrderArr[NUM_SECTION + 2][NUM_LANE][MAX_VEC];
	int nextLinkIDArr[NUM_SECTION + 2][NUM_LANE][MAX_VEC];
	int minTargetLaneArr[NUM_SECTION + 2][NUM_LANE][MAX_VEC];
	int maxTargetLaneArr[NUM_SECTION + 2][NUM_LANE][MAX_VEC];

	double speed[NUM_SECTION + 2][NUM_LANE];
	double density[NUM_SECTION + 2][NUM_LANE];

	int numMLCL[NUM_SECTION + 2][NUM_LANE];
	int numMLCR[NUM_SECTION + 2][NUM_LANE];
	double numCF[NUM_SECTION + 2][NUM_LANE];

	int vehMLC[NUM_SECTION + 2][NUM_LANE][MAX_VEC];
	int vehOLC[NUM_SECTION + 2][NUM_LANE][MAX_VEC];
	int vehCF[NUM_SECTION + 2][NUM_LANE][MAX_VEC];

	int tempIDArr[NUM_LANE][MAX_LEG];
	int tempNumArr[NUM_LANE][MAX_LEG];

} link;


/*--------------------------------------------------------------------*/
/// @struct  typedef struct source_cell
/// @brief   Structure that holds information of source cells.
/// 
/// int sourceID				ID # of a source cell
/// int numVehArr		  		# of vehicles in a source cell
/// int vehIDArr[]				ID # of vehicles in a source cell
/// int currLinkOrderArr[]		Current link order in vehicle paths, 
///								which is 0 in all cases
/// int nextLinkIDArr[]			ID # of next link of vehicles, which
/// 							equals to the initial link ID of a vehicle
/// int minTargetLaneArr[]		Minimum target lane of vehicles at the inital link 
/// int maxTargetLaneArr[]		Maximum target lane of vehicles at the inital link
/*--------------------------------------------------------------------*/
typedef struct {

	int sourceID;

	int numVeh;
	int vehIDArr[MAX_SOURCE_VEC];
	int currLinkOrderArr[MAX_SOURCE_VEC];
	int nextLinkIDArr[MAX_SOURCE_VEC];
	int minTargetLaneArr[MAX_SOURCE_VEC];
	int maxTargetLaneArr[MAX_SOURCE_VEC];

} source_cell;


/*--------------------------------------------------------------------*/
/// @struct  typedef struct sink_cell
/// @brief   Structure that holds information of sink cells.
///
/// int sinkID					ID # of a sink cell
///	int numVeh 					# of vehicles in a sink cell
/// int vehIDArr[]				ID # of vehicles in a sink cell
/// int arrivalTime[]			Time step in which a vehicle arrives to the 
///								sink cell, or in other words, leave the link  
/*--------------------------------------------------------------------*/
typedef struct {

	int sinkID;

	int numVeh;
	int vehIDArr[MAX_SINK_VEC];
	int arrivalTime[MAX_SINK_VEC];

} sink_cell;


/*--------------------------------------------------------------------*/
/// @struct  typedef struct connection_cell
/// @brief   Structure that holds information of connection cells.
///
/// int ccID					ID # of a connection cell
/// int prevLinkID;			 	ID # of previous link, which is equal as  
///								the ID # of a connection cell
/// int nextLinkID[][]			ID # of next link
/// int nextLane[][]			# of next link of next link
/// int numVehArr[][]			# of vehicles stored in a connection cell
/// double numCFArr[][]			# of vehicles that performs CF among 
/// 							vehicles stored in a connection cell
/// int currLinkOrderArr[][]	Current link order in vehicle paths
/// int nextLinkIDArr[][]		ID # of next link of vehicles
/// int vehIDArr[][]			ID # of vehicles
/// int trafficSignal[][]		Flag that indicates traffic signal information
///								(Stop: 0, Go: 1)
/// int cycleDuration 			Duration of time step of one traffic cycle
/*--------------------------------------------------------------------*/
typedef struct {

	int ccID;

	int prevLinkID;
	int nextLinkID[NUM_LANE][MAX_LEG];
	int nextLane[NUM_LANE][MAX_LEG];

	int numVehArr[NUM_LANE][MAX_LEG];
	double numCFArr[NUM_LANE][MAX_LEG];

	int currLinkOrderArr[NUM_LANE][MAX_VEC];
	int nextLinkIDArr[NUM_LANE][MAX_VEC];
	int vehIDArr[NUM_LANE][MAX_VEC];

	int trafficSignal[NUM_LANE][MAX_CYCLE_DURATION];
	int cycleDuration;

} connection_cell;


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
//double Get_Time_ms();
int Print_Template(int, int, int);
void Print_Vehicle(vehicle*, int);
void Print_Link(link*, int);
void Print_SourceCell(source_cell*, int);
void Print_SinkCell(sink_cell*, int);
void Print_ConnectionCell(connection_cell*, int);
void Print_Signal(connection_cell*, int);
void Print_All_Results(link*, source_cell*, sink_cell*, connection_cell*, int);
int Export_All_Result(link*, int, int, int);

/*====================================================================*/
/// Setup Functions
/// Functions that setup components of the simulator
/// 1. void Setup_Vehicle(vehicle*, int, int)
/// 2. void Setup_Link(vehicle*, int, link*, int)
/// 3. void Setup_SourceCell(source_cell*, int)
/// 4. void Setup_SinkCell(sink_cell*, int)
/// 5. void Setup_ConnectionCell(connection_cell*, int) 
/*====================================================================*/
void Setup_Vehicle(vehicle*, char*, int*, int);
void Setup_Vehicle_Test(vehicle*, char*, int*, int, int*, int);
void Setup_Link(link*, char*, int*, int);
void Setup_SourceCell(source_cell*, int, int*);
void Setup_SinkCell(sink_cell*, int, int*);
void Setup_ConnectionCell(connection_cell*, char*, int*, int);
void Setup_ConnectionCell_Test(connection_cell*, char*, int*, int, int*);

int getNumLink(char*);
int getNumVeh(char*);

/*====================================================================*/
/// Utility Functions
/// Functions that are used in many functions to perform repetitive tasks
/// 1. void MoveLC(int*, int, int*, int, int)
/// 2. void MoveCF(int*, int, int*, int, int)
/// 3. void Remove_Blank(int*, int)
/// 4. int Find_Index(int*, int, int)
/// 5. int Evaluate_Prob(double)
/*====================================================================*/
void MoveLC(int*, int, int*, int, int);
void MoveCF(int*, int, int*, int, int);
void Remove_Blank(int*, int);
int Find_Index(int*, int, int);
int Evaluate_Prob(double);

/*====================================================================*/
/// Enter & Exit Link Functions
/// Functions that are involved in evaluation and performanace of CF
/// 1. void Update_Source(vehicle*, int, source_cell*, int)
/// 2. void Enter_Link(link*, source_cell*)
/// 3. void Exit_Link(link*, sink_cell*, int)
/*====================================================================*/
void Update_Source(vehicle*, int, source_cell*, int, int, int*);
void Enter_Link(link*, source_cell*);
void Exit_Link(link*, sink_cell*, int);

/*====================================================================*/
/// LC Functions
/// Functions that are involved in evaluation and performanace of LC
/// 1. void update_Speed(link*)
/// 2. void Evaluate_MLC(link*)
/// 3. void Select_Veh(link*, int, int, int, int)
/// 4. void Evaluate_OLC(link*) 
/// 5. void LCSim(link*)
/*====================================================================*/
void update_Speed(link*);
void Evaluate_MLC(link*);
void Select_Veh(link*, int, int, int, int);
void Evaluate_OLC(link*);
void LCSim(link*);

/*====================================================================*/
/// CF Functions
/// Functions that are involved in evaluation and performanace of CF
/// 1. void Evaluate_CF(link*)
/// 2. void CFsim(link*)
/*====================================================================*/
void Evaluate_CF(link*);
void CFsim(link*);

/*====================================================================*/
/// Between-Link Functions
/// Functions that are involved in evaluation and performanace of CF
/// 1. void Update_tempArr(link*)
/// 2. void Relay_numVeh(link*, link*, int, connection_cell*, int, int, int)
/// 3. void Relay_numCF(link*, link*, int, connection_cell*, int, int, int)
/// 4. void Evaluate_Eff_numCF(link*)
/// 5. void Update_nextLink(vehicle*, link*, link*, int, connection_cell*, int)
/*====================================================================*/
void Update_tempArr(link*);
void Relay_numVeh(link*, link*, int, connection_cell*, int, int);
void Relay_numCF(link*, link*, int, connection_cell*, int, int, int);
void Evaluate_Eff_numCF(link*);
void Update_nextLink(vehicle*, link*, link*, int, connection_cell*, int, int*, int);

/*====================================================================*/
/// Reset Functions
/// Functions that are reset variables and flags
/// 1. void Reset_Link(link*)
/// 2. void Reset_Sink(sink_cell*)
/// 3. void Reset_ConnectionCell(connection_cell*)
/*====================================================================*/
void Reset_Link(link*);
void Reset_Sink(sink_cell*);
void Reset_ConnectionCell(connection_cell*);

void PrintSumLane(link l[], int);
void PrintSumLaneN(link l[], int);

void SimulationStep(vehicle*, int, link l[], source_cell sc[], sink_cell sk[], connection_cell cc[], int, int, int linkID[], int vehID[]);

#endif /* TRAFFICSIM_H_ */

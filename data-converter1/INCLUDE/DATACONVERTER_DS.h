#include <unordered_map>
#include <vector>

#define 	MAX_EDGE_NUMS 	500
#define	MAX_PHASE_NUMS 10
#define	MAX_STATE_LEN	20	
#define	MAX_LANE			6
#define	MAX_VEC			20

using namespace std;

//input data(XML)
typedef struct {
	int         id;
	double      x;
	double      y;
	int			type;
} Node;

typedef struct {
	int         id;
	int         from;
	int         to;
	int         numLanes;
	double      speed;
} Edge;


typedef struct {
	int			id;
	int			type;
	int			depart;
	int			numofEdges;
	int			edges[MAX_EDGE_NUMS];
} Route;

typedef struct {
	int			from;
	int			to;
	int			fromLane;
	int			toLane;
	int			tl;
	int			linkIndex;
} Connection;
	
typedef struct {
	int			id;
	int			phaseNums;
	int			duration[MAX_PHASE_NUMS];
	char			state[MAX_PHASE_NUMS][MAX_STATE_LEN];
} TlLogic;

typedef	vector<Node*>									NodeList;
typedef 	unordered_map<int, Edge*>					EdgeMap;
typedef  unordered_multimap<int, Edge*>  			EdgeMultiMap;				
typedef  unordered_map<int, double>       		NetMap;
typedef 	vector<Route*>									RouteList;
typedef 	unordered_multimap<int, Connection*>  	ConnectionMultiMap;
typedef	unordered_map<int, TlLogic*>				TlLogicMap;


typedef struct {
	NodeList					nodeList;
	EdgeMap					edgeMap;
	EdgeMultiMap			edgeMultiMap;
	NetMap					netMap;
	RouteList				routeList;
	ConnectionMultiMap	connectionMultiMap;
	TlLogicMap				tlLogicMap;
}	InMap;


//output data(text)
typedef struct {
	int         id;
	int         type;
	int         depart;
	int         numofEdges;
	int         edges[MAX_EDGE_NUMS];
	int         minTargetLane[MAX_EDGE_NUMS];
	int         maxTargetLane[MAX_EDGE_NUMS];
} Vehicle;

typedef struct {
	int			linkID;
	int			numLanes;
	double		ffSpeed;
	double		lenSection;
} Link;

typedef struct	{
	int 			ccID;
	int			prevLinkID;
	int			connectionNums;
	int			fromLane[MAX_LANE*MAX_VEC];
	int			nextLinkID[MAX_LANE*MAX_VEC];
	int			nextLane[MAX_LANE*MAX_VEC];
	int			type;
	int			laneNums;
	int			phaseNums;
	int			linkIndex[MAX_LANE];
	int			duration[MAX_LANE][MAX_PHASE_NUMS];
	int			state[MAX_LANE][MAX_PHASE_NUMS];
} ConnectionCell;

typedef 	vector<Vehicle*>							VehicleList;
typedef 	vector<Link*>								LinkList;
typedef	vector<ConnectionCell*>					ConnectionCellList;

typedef struct {
	VehicleList				vehicleList;
	LinkList					linkList;
	ConnectionCellList	connectionCellList;
} OutMap;

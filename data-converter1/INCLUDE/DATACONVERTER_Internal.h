#include <libxml/parser.h>
#include "DATACONVERTER_DS.h"

#define 		MAX_FILENAME_LEN		100

// Moudle Load
int load_Node(char*, NodeList*);
int load_Edge(char*, EdgeMap*, EdgeMultiMap*);
int load_Net(char*, NetMap*);
int load_Route(char*, RouteList*);
int load_Connection(char*, ConnectionMultiMap*);
int load_TlLogic(char*, TlLogicMap*, ConnectionMultiMap*);
int LOAD_Main(char*, InMap*);


// Module Convert
int convert_ToVehicle(RouteList*, ConnectionMultiMap*, EdgeMap*, VehicleList*);
int convert_ToLink(EdgeMap*, NetMap*, LinkList*);
int convert_ToConnectionCell(NodeList*, EdgeMultiMap*, ConnectionMultiMap*, TlLogicMap*, ConnectionCellList*);
int CONVERT_Main(InMap*, OutMap*);


// Module PRINT
int print_Vehicle(char*, VehicleList*);
int print_Link(char*, LinkList*);
int print_ConnectionCell(char*, ConnectionCellList*);
int PRINT_Main(char*, OutMap*);

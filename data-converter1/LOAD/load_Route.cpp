#include <string.h>
#include "DATACONVERTER_Internal.h"

int load_ParseRoute(xmlNode * a_node, RouteList *routeList)
{
	xmlNode		*cur_node = NULL;
	xmlNode		*route_node = NULL;
	Route			*route;
	char			*parseStr;
	char			*edgeToken;
	
	for (cur_node = a_node; cur_node; cur_node = cur_node->next){
		if (cur_node->type == XML_ELEMENT_NODE){
         if (!xmlStrcmp(cur_node->name , (const xmlChar *)"vehicle"))
			{
				// parse id
				parseStr = (char*)xmlGetProp(cur_node, (const xmlChar *)"id");
				if (parseStr != NULL)
				{
					route = new Route;
					routeList->push_back(route);
					memset(route, 0, sizeof(Route));

					//memcpy(parseStr, parseStr + 3, sizeof(parseStr) - 3);
					route->id = atoi(parseStr);
					xmlFree(parseStr);
					//printf("id:%d\n", route->id);

					// parse type
					parseStr = (char*)xmlGetProp(cur_node, (const xmlChar *)"type");
					if (parseStr != NULL)
					{
						if (!strcmp(parseStr, "passenger"))  route->type = 0;
						else  route->type = 1;
						xmlFree(parseStr);
					}

					// parse depart
					parseStr = (char*)xmlGetProp(cur_node, (const xmlChar *)"depart");
					if (parseStr != NULL)
					{
						route->depart = atoi(parseStr);
						xmlFree(parseStr);
						//printf("depart:%d\n", route->depart);
					}


					for (route_node = cur_node->children; route_node; route_node = route_node->next)
					{
						 if (!xmlStrcmp(route_node->name, (const xmlChar*)"route"))
						 {
							 parseStr = (char*)xmlGetProp(route_node, (const xmlChar *)"edges");
							 if (parseStr != NULL)
							 {
								 edgeToken = strtok(parseStr, " ");
								 while (edgeToken != NULL)
								 {
									 route->edges[route->numofEdges++] = atoi(edgeToken);
									 edgeToken = strtok(NULL, " ");
								 }
								 xmlFree(parseStr);
								 //printf("edges:%d\n", route->edges[0]);
							 }
						 }
					}
				}
			}
		}
		load_ParseRoute(cur_node->children, routeList);
	}

	return 0;
}

int load_Route(char *srcDir, RouteList *routeList)
{
	char					srcFile[MAX_FILENAME_LEN];
	xmlDoc				*doc = NULL;
	xmlNode				*root_element;


	strcpy(srcFile, srcDir);
	strcat(srcFile, "/Route.xml");
	
   /*parse the file and get the DOM */
	doc = xmlReadFile(srcFile, NULL, 0);
	
	if (doc == NULL) {
		printf("error: could not parse file %s\n", srcFile);
	}
	
	/*Get the root element node */
	root_element = xmlDocGetRootElement(doc);
	printf("Load Start\n");
	load_ParseRoute(root_element, routeList);
	printf("Load End\n");
	/*free the document */
	xmlFreeDoc(doc);

	return 0;
}

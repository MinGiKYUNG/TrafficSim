#include "DATACONVERTER_Internal.h"
#include <string.h>

int load_ParseEdge(xmlNode * a_node, EdgeMap *edgeMap, EdgeMultiMap *edgeMultiMap)
{
	xmlNode		*cur_node = NULL;
	Edge			*edge;	
	char			*parseStr;
	
	for (cur_node = a_node; cur_node; cur_node = cur_node->next){
		if (cur_node->type == XML_ELEMENT_NODE){
         if (!xmlStrcmp(cur_node->name , (const xmlChar *)"edge"))
			{
				// parse id
				parseStr = (char*)xmlGetProp(cur_node, (const xmlChar *)"id");
				if (parseStr != NULL)
				{
					edge = new Edge;
					edge->id = atoi(parseStr);
					xmlFree(parseStr);

					// parse from
					parseStr = (char*)xmlGetProp(cur_node, (const xmlChar *)"from"); 
					if(parseStr != NULL)                                                                                                 
					{                                                                                         
						edge->from = atoi(parseStr);
						xmlFree(parseStr);
					}                                                                                            

					// parse to
					parseStr = (char*)xmlGetProp(cur_node, (const xmlChar *)"to"); 
					if(parseStr != NULL)
					{
						edge->to = atoi(parseStr);                                        
						xmlFree(parseStr);
					}
					
					// parse numLanes
					parseStr = (char*)xmlGetProp(cur_node, (const xmlChar *)"numLanes");
					if (parseStr != NULL)
					{
						edge->numLanes = atoi(parseStr);
						xmlFree(parseStr);

					}

					// parse speed
					parseStr = (char*)xmlGetProp(cur_node, (const xmlChar *)"speed");
					if (parseStr != NULL)
					{
						edge->speed = atof(parseStr);
						xmlFree(parseStr);
					}


					// insert the edge into edgeMap
//					edgeMap->insert(make_pair<int, Edge*>(edge->id, edge));
					edgeMap->insert(make_pair(edge->id, edge));
//					edgeMultiMap->insert(make_pair<int, Edge*>(edge->to, edge));
					edgeMultiMap->insert(make_pair(edge->to, edge));

				}
			}
		}
		load_ParseEdge(cur_node->children, edgeMap, edgeMultiMap);
	}

	return 0;
}



int load_Edge(char *srcDir, EdgeMap *edgeMap, EdgeMultiMap *edgeMultiMap)
{
	char					srcFile[MAX_FILENAME_LEN];
	xmlDoc				*doc = NULL;
	xmlNode				*root_element;


	strcpy(srcFile, srcDir);
	strcat(srcFile, "/Edge.xml");
	
   /*parse the file and get the DOM */
	doc = xmlReadFile(srcFile, NULL, 0);
	
	if (doc == NULL) {
		printf("error: could not parse file %s\n", srcFile);
	}
	
	/*Get the root element node */
	root_element = xmlDocGetRootElement(doc);
	
	load_ParseEdge(root_element, edgeMap, edgeMultiMap);
	
	/*free the document */
	xmlFreeDoc(doc);

	return 0;
}

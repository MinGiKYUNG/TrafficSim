#include "DATACONVERTER_Internal.h"
#include <string.h>

int load_ParseTlLogic(xmlNode * a_node, TlLogicMap *tlLogicMap, ConnectionMultiMap *connectionMultiMap)
{
	xmlNode		*cur_node = NULL;
	xmlNode		*phase_node = NULL;
	Connection	*connection;	
	TlLogic 		*tlLogic;
	int			from, to, fromLane, toLane;
	char			*parseStr;
	
	for (cur_node = a_node; cur_node; cur_node = cur_node->next){
		if (cur_node->type == XML_ELEMENT_NODE){
			
			// parse tlLogic
         if (!xmlStrcmp(cur_node->name , (const xmlChar *)"tlLogic"))
			{	

				// parse id
				parseStr = (char*)xmlGetProp(cur_node, (const xmlChar *)"id");
				if(parseStr != NULL)
				{
					tlLogic = new TlLogic;

					tlLogic->id = atoi(parseStr);
					xmlFree(parseStr);
			
					tlLogic->phaseNums = 0; 
					for (phase_node = cur_node->children; phase_node; phase_node = phase_node->next)
					{
						if (!xmlStrcmp(phase_node->name, (const xmlChar*)"phase"))
						{
							// parse duration
							parseStr = (char*)xmlGetProp(phase_node, (const xmlChar *)"duration");
							if(parseStr != NULL)                               
							{                                                     
								tlLogic->duration[tlLogic->phaseNums] = atoi(parseStr);
								xmlFree(parseStr);                                                                                    
							}

							// parse state
							parseStr = (char*)xmlGetProp(phase_node, (const xmlChar *)"state");
							if(parseStr != NULL)
							{
								strcpy(tlLogic->state[tlLogic->phaseNums++], parseStr);
								xmlFree(parseStr);
							}
						}
					}
					tlLogicMap->insert(make_pair(tlLogic->id, tlLogic));
//					tlLogicMap->insert(make_pair<int, TlLogic*> (tlLogic->id, tlLogic));
				}
			}

			// parse connection
			else if (!xmlStrcmp(cur_node->name , (const xmlChar *)"connection"))
			{
				// parse from
				parseStr = (char*)xmlGetProp(cur_node, (const xmlChar *)"from"); 
				if(parseStr != NULL)                                                                                                 
				{                                                                                         
					from = atoi(parseStr);
					xmlFree(parseStr);
				}                                                                                            

				// parse to
				parseStr = (char*)xmlGetProp(cur_node, (const xmlChar *)"to"); 
				if(parseStr != NULL)
				{
					to = atoi(parseStr);                                        
					xmlFree(parseStr);
				}
					
				// parse fromLane
				parseStr = (char*)xmlGetProp(cur_node, (const xmlChar *)"fromLane");
				if (parseStr != NULL)
				{
					fromLane = atoi(parseStr);
					xmlFree(parseStr);
				}

				// parse toLane
				parseStr = (char*)xmlGetProp(cur_node, (const xmlChar *)"toLane");
				if(parseStr != NULL)
				{
					toLane = atoi(parseStr);
					xmlFree(parseStr);
				}
				
				// find connection
				auto rt = connectionMultiMap->equal_range(from);
				for (auto it = rt.first; it != rt.second; it++)
				{
					connection = it->second;
					if (connection->to == to && connection->fromLane == fromLane && connection->toLane == toLane)
					{
						// parse tl
						parseStr = (char*)xmlGetProp(cur_node, (const xmlChar *)"tl");
						if(parseStr != NULL)
						{
							connection->tl = atoi(parseStr);
							xmlFree(parseStr);
						}

						// parse linkIndex
						parseStr = (char*)xmlGetProp(cur_node, (const xmlChar *)"linkIndex");
						if(parseStr != NULL)
						{
							connection->linkIndex = atoi(parseStr);
							xmlFree(parseStr);
						}
					}
				}
			}
		}
		load_ParseTlLogic(cur_node->children, tlLogicMap, connectionMultiMap);
	}

	return 0;
}



int load_TlLogic(char *srcDir, TlLogicMap *tlLogicMap, ConnectionMultiMap *connectionMultiMap)
{
	char					srcFile[MAX_FILENAME_LEN];
	xmlDoc				*doc = NULL;
	xmlNode				*root_element;


	strcpy(srcFile, srcDir);
	strcat(srcFile, "/TlLogic.xml");
	
   /*parse the file and get the DOM */
	doc = xmlReadFile(srcFile, NULL, 0);
	
	if (doc == NULL) {
		printf("error: could not parse file %s\n", srcFile);
	}
	
	/*Get the root element node */
	root_element = xmlDocGetRootElement(doc);
	
	load_ParseTlLogic(root_element, tlLogicMap, connectionMultiMap);
	
	/*free the document */
	xmlFreeDoc(doc);

	return 0;
}

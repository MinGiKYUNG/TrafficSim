#include "DATACONVERTER_Internal.h"
#include <string.h>

int load_ParseConnection(xmlNode * a_node, ConnectionMultiMap *connectionMultiMap)
{
	xmlNode		*cur_node = NULL;
	Connection	*connection;	
	char			*parseStr;
	
	for (cur_node = a_node; cur_node; cur_node = cur_node->next){
		if (cur_node->type == XML_ELEMENT_NODE){
         if (!xmlStrcmp(cur_node->name , (const xmlChar *)"connection"))
			{
				connection = new Connection;

				// parse from
				parseStr = (char*)xmlGetProp(cur_node, (const xmlChar *)"from"); 
				if(parseStr != NULL)                                                                                                 
				{                                                                                         
					connection->from = atoi(parseStr);
					xmlFree(parseStr);
				}                                                                                            

				// parse to
				parseStr = (char*)xmlGetProp(cur_node, (const xmlChar *)"to"); 
				if(parseStr != NULL)
				{
					connection->to = atoi(parseStr);                                        
					xmlFree(parseStr);
				
					
					// parse fromLane
					parseStr = (char*)xmlGetProp(cur_node, (const xmlChar *)"fromLane");
					if (parseStr != NULL)
					{
						connection->fromLane = atoi(parseStr);
						xmlFree(parseStr);
					}

					// parse toLane 
					parseStr = (char*)xmlGetProp(cur_node, (const xmlChar *)"toLane");
					if(parseStr != NULL)
					{
						connection->toLane = atoi(parseStr);
						xmlFree(parseStr);
					}

					connectionMultiMap->insert(make_pair (connection->from, connection));
//					connectionMultiMap->insert(make_pair<int, Connection*> (connection->from, connection));
				}
			}
		}
		load_ParseConnection(cur_node->children, connectionMultiMap);
	}

	return 0;
}



int load_Connection(char *srcDir, ConnectionMultiMap *connectionMultiMap)
{
	char					srcFile[MAX_FILENAME_LEN];
	xmlDoc				*doc = NULL;
	xmlNode				*root_element;


	strcpy(srcFile, srcDir);
	strcat(srcFile, "/Connection.xml");
	
   /*parse the file and get the DOM */
	doc = xmlReadFile(srcFile, NULL, 0);
	
	if (doc == NULL) {
		printf("error: could not parse file %s\n", srcFile);
	}
	
	/*Get the root element node */
	root_element = xmlDocGetRootElement(doc);
	
	load_ParseConnection(root_element, connectionMultiMap);
	
	/*free the document */
	xmlFreeDoc(doc);

	return 0;
}

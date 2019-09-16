#include "DATACONVERTER_Internal.h"
#include <string.h>

int load_ParseNode(xmlNode * a_node, NodeList *nodeList)
{
	xmlNode		*cur_node = NULL;
	Node			*node;	
	char			*parseStr;

	for (cur_node = a_node; cur_node; cur_node = cur_node->next){
		if (cur_node->type == XML_ELEMENT_NODE){
         if (!xmlStrcmp(cur_node->name , (const xmlChar *)"node"))
			{
				// parse id
				parseStr = (char*)xmlGetProp(cur_node, (const xmlChar *)"id");
				if (parseStr != NULL)
				{
					node = new Node;
					if (strstr(parseStr, "cluster") == NULL)
					{
						node->id = atoi(parseStr);
					}
					else
					{
						strncpy(parseStr, parseStr + sizeof("cluster_"), 9);	
						node->id = 1000000000 + atoi(parseStr);
					}
					xmlFree(parseStr);

					// parse x
					parseStr = (char*)xmlGetProp(cur_node, (const xmlChar *)"x"); 
					if(parseStr != NULL)                                                                                                 
					{                                                                                         
						node->x = atof(parseStr);
						xmlFree(parseStr);
					}                                                                                            

					// parse y
					parseStr = (char*)xmlGetProp(cur_node, (const xmlChar *)"y"); 
					if(parseStr != NULL)
					{
						node->y = atof(parseStr);                                        
						xmlFree(parseStr);
					}

					// parse type
					parseStr = (char*)xmlGetProp(cur_node, (const xmlChar *)"type");
               if(!strcmp(parseStr, "traffic_light"))	node->type = 1;
					else	node->type = 0;
					xmlFree(parseStr);

					
					// insert the node into nodeList
					nodeList->push_back(node);

				}
			}
		}
		load_ParseNode(cur_node->children, nodeList);
	}

	return 0;
}



int load_Node(char *srcDir, NodeList *nodeList)
{
	char					srcFile[MAX_FILENAME_LEN];
	xmlDoc				*doc = NULL;
	xmlNode				*root_element;


	strcpy(srcFile, srcDir);
	strcat(srcFile, "/Node.xml");
	
   /*parse the file and get the DOM */
	doc = xmlReadFile(srcFile, NULL, 0);
	
	if (doc == NULL) {
		printf("error: could not parse file %s\n", srcFile);
	}
	
	/*Get the root element node */
	root_element = xmlDocGetRootElement(doc);
	
	load_ParseNode(root_element, nodeList);
	
	/*free the document */
	xmlFreeDoc(doc);

	return 0;
}

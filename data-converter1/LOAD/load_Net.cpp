#include <string.h>
#include "DATACONVERTER_Internal.h"

int load_ParseNet(xmlNode * a_node, NetMap *netMap)
{
	xmlNode		*cur_node = NULL;
	xmlNode		*lane_node = NULL;
	char			*parseStr;
	int			id;
	double		length;
	bool			skip = false;
	
	for (cur_node = a_node; cur_node; cur_node = cur_node->next){
		if (cur_node->type == XML_ELEMENT_NODE){
         if (!xmlStrcmp(cur_node->name , (const xmlChar *)"edge"))
			{
				// parse id
				parseStr = (char*)xmlGetProp(cur_node, (const xmlChar *)"id");
				if (parseStr != NULL)
				{
					id = atoi(parseStr);
					xmlFree(parseStr);
					for (lane_node = cur_node->children; lane_node; lane_node = lane_node->next)
					{
						 if (!xmlStrcmp(lane_node->name, (const xmlChar*)"lane"))
						 {
							 parseStr = (char*)xmlGetProp(lane_node, (const xmlChar *)"length");
							 if (parseStr != NULL)
							 {
								 length = atof(parseStr);
								 netMap ->insert(make_pair (id, length));
//								 netMap ->insert(make_pair<int, double> (id, length));

							 }
							 xmlFree(parseStr);
							 skip = true;
							 break;
																																																											                         
						 }
																																							                         
					}
				}
			}
		}
		if (!skip) load_ParseNet(cur_node->children, netMap);
		else skip = false;
	}

	return 0;
}

int load_Net(char *srcDir, NetMap *netMap)
{
	char					srcFile[MAX_FILENAME_LEN];
	xmlDoc				*doc = NULL;
	xmlNode				*root_element;


	strcpy(srcFile, srcDir);
	strcat(srcFile, "/Net.xml");
	
   /*parse the file and get the DOM */
	doc = xmlReadFile(srcFile, NULL, 0);
	
	if (doc == NULL) {
		printf("error: could not parse file %s\n", srcFile);
	}
	
	/*Get the root element node */
	root_element = xmlDocGetRootElement(doc);
	
	load_ParseNet(root_element, netMap);
	
	/*free the document */
	xmlFreeDoc(doc);

	return 0;
}

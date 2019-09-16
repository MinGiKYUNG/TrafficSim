#include <string.h>
#include "DATACONVERTER_Internal.h"

int print_Link(char *tarDir, LinkList *linkList)
{
	char			tarFile[MAX_FILENAME_LEN];
	Link			*link;
	FILE			*fp;

	strcpy(tarFile, tarDir);
	strcat(tarFile, "/Link.dat");
	fp = fopen(tarFile, "w");

	for (std::vector<Link*>::iterator it = linkList-> begin(); it != linkList->end(); it++)
	{
	 	link = *it;
		fprintf(fp, "%d %d %lf %lf\n", link->linkID, link->numLanes, link->ffSpeed, link->lenSection);

	}

	fclose(fp);

	return 0;
}

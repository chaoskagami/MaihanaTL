/********************************************************************
  (C) 2015 Jon Feldman

  This is a partially reversed way of reading the scripts from
  Sce files in Maihana ~ Ao no Makyou. It isn't complete...I just
  figured out enough to read the script data, and nothing more.
  
  This is licensed to you under GPLv3. Obey the license.
 ********************************************************************/

#include "structs.h"

char* vol2char (char* volname) {

	if (strlen(volname) < 30)
		return NULL;

	char* newda = (char*)calloc(sizeof(char), strlen(volname) - 30);

	memcpy(newda, &volname[27], strlen(volname) - 30);

	newda[strlen(newda) - 1] = 0;

	return newda;
}

int main(int c, char** v) {
	if (c <= 3) {
		fprintf(stderr, "Usage: %s inject|extract|simpleextract {SceFile.vol} {DumpFile.txt} [SceOut.vol]\n", v[0]);
		fprintf(stderr, "   inject          Inject previously extracted script.\n");
		fprintf(stderr, "   extract         Extract script from Sce/SceFile in engine\n");
		fprintf(stderr, "   simpleextract   Extract script from Sce/SceFile in engine without extra info\n");
		return 0;
	}

	char* buffer = NULL;
	size_t fsize = 0;

	//===========================================================
	// this is all just standard read-to-memory garbage, skip it.
	//===========================================================


	FILE* sceFile = fopen(v[2], "rb");

	if (!sceFile) {
		fprintf(stderr, "Invalid file.\n");
		return 1;
	}

	fseek(sceFile, 0, SEEK_END);
	fsize = ftell(sceFile);
	fseek(sceFile, 0, SEEK_SET);

	buffer = (char*) calloc(sizeof(char),fsize);

	if(buffer == NULL) {
		fprintf(stderr, "Memory error\n");
		return 1;		
	}

	int result = fread(buffer, 1, fsize, sceFile);
	if (result != fsize) {
		fprintf(stderr, "Mismatch on read size\n");
		return 1;
	}

	fclose(sceFile);

	if(!strcmp(v[1], "extract")) {
		fprintf(stderr, "[info] Using extractor.\n");
		extract(buffer, fsize, v[3], 0);
	} else if(!strcmp(v[1], "simpleextract")) {
		fprintf(stderr, "[info] Using extractor.\n");
		fprintf(stderr, "[info] Simple mode - not parsing characters, speaker, etc.\n");
		extract(buffer, fsize, v[3], 1);
	} else if(!strcmp(v[1], "inject")) {
		fprintf(stderr, "[info] Using injector.\n");
		if (c == 5) {
			fprintf(stderr, "[info] Alternate output specified. Not clobbering original.\n");
			inject(buffer, fsize, v[4], v[3]);
		} else {
			inject(buffer, fsize, v[2], v[3]);
		}
	}

}

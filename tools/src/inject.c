/********************************************************************
  (C) 2015 Jon Feldman

  This is a partially reversed way of reading the scripts from
  Sce files in Maihana ~ Ao no Makyou. It isn't complete...I just
  figured out enough to read the script data, and nothing more.
  
  This is licensed to you under GPLv3. Obey the license.
 ********************************************************************/

#include "structs.h"

void inject (char* buffer, size_t fsize, char* out, char* script) {

	// READING IN FILE JUNK. Skip it.

	FILE* script_f = fopen(script, "rb");

	if (!script_f) {
		fprintf(stderr, "Invalid file.\n");
		exit(1);
	}

	fseek(script_f, 0, SEEK_END);
	size_t ssize = ftell(script_f);
	fseek(script_f, 0, SEEK_SET);

	char* scrbuffer = (char*) calloc(sizeof(char),ssize);

	if(scrbuffer == NULL) {
		fprintf(stderr, "Memory error\n");
		exit(1);
	}

	int result = fread(scrbuffer, 1, ssize, script_f);
	if (result != ssize) {
		fprintf(stderr, "Mismatch on read size\n");
		exit(1);
	}

	fclose(script_f);

	// Actual code now.

	uint32_t offset = 0;
	char str[256];
	int str_loc = 0;

	char* str_split = scrbuffer;
	while(str_split < scrbuffer+ssize) {

		if (str_split[0] == '[') {
			// We need to write a block.
			if (offset != 0) {
				memcpy(((text_ent*)&buffer[offset])->TextData,str,256);
			}

			// Reset string.
			memset(str, 0, 256);
			str_loc = 0;

			// Control line.
			sscanf(str_split, "[text %x", &offset);
		} else {
			// Copy string.

			if(str_loc != 0) {
				str[str_loc] = '\n';
				str_loc += 1;
			}

			int i;
			for(i=0;;i++) {
				if(str_split[i] == '\n') {
					break;
				}
			}

			// Text line.
			memcpy(&str[str_loc], str_split, i);

			str_loc += i;
		}

		// Move pointer to next line.
		// Unfortunately, using strtok or strcmp explodes with SJIS.

		int i;
		for(i=0;;i++) {
			if(str_split[i] == '\n') {
				break;
			}
		}
		str_split = str_split+i+1;
	}

	FILE* wrt_out = fopen(out, "wb");

	fwrite(buffer,1,fsize,wrt_out);

	fflush(wrt_out);

	fclose(wrt_out);
}


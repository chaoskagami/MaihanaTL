/********************************************************************
  (C) 2015 Jon Feldman

  This is a partially reversed way of reading the scripts from
  Sce files in Maihana ~ Ao no Makyou. It isn't complete...I just
  figured out enough to read the script data, and nothing more.
  
  This is licensed to you under GPLv3. Obey the license.
 ********************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* =============================================================== */
/*  Miscellaneous Defines.                                         */
/* =============================================================== */

#define LEFT 0
#define RIGHT 1

#define EXPR1 0
#define EXPR2 0
#define EXPR3 0
#define EXPR4 0

/* =============================================================== */
/* Basic struct definitions.                                       */
/* =============================================================== */


typedef struct char_ent_2a {
	uint32_t 	Magic;			// 2A
	uint32_t 	Speaker;		// LEFT | RIGHT
	char		VolName[68];	// Data\Texture\TextureChara2D
	uint32_t	Expression;		// EXPR1 | EXPR2 | EXPR3 | EXPR4
	uint32_t	Unknown1;		// Meaning not known. Varies.
	uint32_t	Unknown2;		// ^
	uint32_t	Break;			// 0A
	uint32_t	Zero;			// 00
} char_ent_2a;

typedef struct char_ent_28 {
	uint32_t 	Magic;			// 28
	uint32_t 	Speaker;		// LEFT | RIGHT
	char		VolName[68];	// Data\Texture\TextureChara2D
	uint32_t	Expression;		// EXPR1 | EXPR2 | EXPR3 | EXPR4
	uint32_t	Unknown1;		// Meaning not known. Varies. Generally 00 00 00 01.
	uint32_t	Unknown2;		// ^ Same. Usually something like 00 00 0A 00.
} char_ent_28; // 88 bytes

typedef struct control_blk_05 {
	uint32_t	Magic;			// 05
	char		Reserved[32]; 	// No clue.
} control_blk_05;

typedef struct control_blk_40 {
	uint32_t	Magic;			// 40
	uint8_t		Reserved[32];	// Meaning not known.
} control_blk_40;

typedef struct text_ent {
	uint32_t	Magic;			// 1D
	uint32_t	Speaker;		// LEFT | RIGHT
	char		TextData[256];	// Textbox contents. 0x0A is newline.
	uint32_t	Reserved1;		// 01
	uint32_t	Reserved2;		// 00
} text_ent;

/* =============================================================== */
/* Block definitions - these are the meat.                         */
/* =============================================================== */

// These definitions are used in SceStageMarisa.vol and SceStageReimu.vol.
// They cover all of those files.

typedef struct block_1char_2a {
	char_ent_2a 	charA;
	control_blk_40	pre;
	text_ent		text;
} block_1char_2a;

typedef struct block_2char_2a {
	char_ent_2a 	charA;
	char_ent_2a 	charB;
	control_blk_40	pre;
	text_ent		text;
} block_2char_2a;

typedef struct block_1char_28 {
	char_ent_28		charA;
	control_blk_05	mid;
	control_blk_40	pre;
	text_ent		text;
} block_1char_28;

typedef struct block_2char_28 {
	char_ent_28		charA;
	char_ent_28		charB;
	control_blk_05	mid;
	control_blk_40	pre;
	text_ent		text;
} block_2char_28;

// SceHelp.vol doesn't contain a lot of the other structs.
// Thus it has it's own unique block types.

typedef struct block_help_2a {
	char_ent_2a		charA;
	text_ent		text;
} block_help_2a;

typedef struct block_help_28 {
	char_ent_28		charA;
	control_blk_05	mid;
	text_ent		text;
} block_help_28;


char* vol2char (char* volname) {

	if (strlen(volname) < 30)
		return NULL;

	char* newda = (char*)calloc(sizeof(char), strlen(volname) - 30);

	memcpy(newda, &volname[27], strlen(volname) - 30);

	newda[strlen(newda) - 1] = 0;

	return newda;
}

char* buffer = NULL;
size_t fsize = 0;

void inject (char* out, char* script) {

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

	uint32_t type = 0, block = 0, offset = 0;
	char str[256];
	int str_loc = 0;

	char* str_split = scrbuffer;
	while(str_split < scrbuffer+ssize) {

		if (str_split[0] == '[') {
			// We need to write a block.
			if (offset != 0) {
				switch(block) {
					case 0x28:
						switch(type) {
							case 0: // Two characters
								memcpy(((block_2char_28*)&buffer[offset])->text.TextData,str,256);
								break;
							case 1: // Help
								memcpy(((block_help_28*)&buffer[offset])->text.TextData,str,256);
								break;
							case 2: // One character
								memcpy(((block_1char_28*)&buffer[offset])->text.TextData,str,256);
								break;
						}
						break;
					case 0x2A:
						switch(type) {
							case 0: // Two characters
								memcpy(((block_2char_2a*)&buffer[offset])->text.TextData,str,256);
								break;
							case 1: // Help
								memcpy(((block_help_2a*)&buffer[offset])->text.TextData,str,256);
								break;
							case 2: // One character
								memcpy(((block_1char_2a*)&buffer[offset])->text.TextData,str,256);
								break;
						}
						break;
					case 0x1D:
						memcpy(((text_ent*)&buffer[offset])->TextData,str,256);
						break;
				}
			}

			// Reset string.
			memset(str, 0, 256);
			str_loc = 0;

			// Control line.
			sscanf(str_split, "[%x %x %d", &block, &offset, &type);
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

void extract (char* out) {
	FILE* export = fopen(out, "wb");

	//===================================================
	// Now the real code - search for instances of 0x2A.
	//===================================================
	size_t i;
	for(i=0; i < fsize; i++) {
		// 2A Control code, text block content type A.
		if (buffer[i] == 0x2A) {
			// MATCHED. Read a block out.
			// First...we have no clue whether it's a single character or
			// double until we fill a struct and check for mismatches.

			block_1char_2a* c1 = (block_1char_2a*)calloc(sizeof(block_1char_2a), 1);
			memcpy(c1, &buffer[i], sizeof(block_1char_2a));

			// If charA doesn't have a valid vol file, we ignore.
			if (strlen(c1->charA.VolName) > 28)
			{
				fprintf(export, "[2A 0x%08lX", i);

				if(c1->pre.Magic == 0x2A) {
					// This is a twosie. Oops.
					block_2char_2a* c2 = (block_2char_2a*)calloc(sizeof(block_2char_2a), 1);
					memcpy(c2, &buffer[i], sizeof(block_2char_2a));
	
					// Print script info.
					// [speakerflag][volfile]
					// [speakerflag][volfile]
					// [dialogue]
					fprintf(export, " 00 %s%s %s%s]\n%s\n", 	(c2->text.Speaker == 0 ? "*" : ""), vol2char(c2->charA.VolName),
													(c2->text.Speaker == 1 ? "*" : ""), vol2char(c2->charB.VolName),
													c2->text.TextData);

					free(c2);

					i += sizeof(block_2char_2a) - 1;
				} else if (c1->pre.Magic == 0x1D) {
					// This is a help vol. Not a regular Script vol.

					block_help_2a* c2 = (block_help_2a*)calloc(sizeof(block_help_2a), 1);
					memcpy(c2, &buffer[i], sizeof(block_help_2a));

					// Print script info.
					fprintf(export, " 01 *%s]\n%s\n", 		vol2char(c2->charA.VolName),
												c2->text.TextData);

					free(c2);

					i += sizeof(block_help_2a) - 1;
				} else if (c1->pre.Magic == 0x40) {
					// One character block.
					fprintf(export, " 02 *%s]\n%s\n", 		vol2char(c1->charA.VolName),
												c1->text.TextData);

					i += sizeof(block_1char_2a) - 1;
				}
				// Otherwise, IDK. Ignore it.
			}

			free(c1);
		} else if (buffer[i] == 0x28) {
			// Pretty much the same as 0x2A.

			block_1char_28* c1 = (block_1char_28*)calloc(sizeof(block_1char_28), 1);
			memcpy(c1, &buffer[i], sizeof(block_1char_28));

			// If charA doesn't have a valid vol file, we ignore.
			if (strlen(c1->charA.VolName) > 28)
			{
				fprintf(export, "[28 0x%08lX", i);

				if(c1->mid.Magic == 0x28) {
					// This is a twosie. Oops.
					block_2char_28* c2 = (block_2char_28*)calloc(sizeof(block_2char_28), 1);
					memcpy(c2, &buffer[i], sizeof(block_2char_28));
	
					// Print script info.
					fprintf(export, " 00 %s%s %s%s]\n%s\n", 	(c2->text.Speaker == 0 ? "*" : ""), vol2char(c2->charA.VolName),
													(c2->text.Speaker == 1 ? "*" : ""), vol2char(c2->charB.VolName),
													c2->text.TextData);

					free(c2);

					i += sizeof(block_2char_28) - 1;
				} else if (c1->pre.Magic == 0x1D) {
					// This is a help vol. Not a regular Script vol.

					block_help_28* c2 = (block_help_28*)calloc(sizeof(block_help_28), 1);
					memcpy(c2, &buffer[i], sizeof(block_help_28));

					// Print script info.
					fprintf(export, " 01 *%s]\n%s\n", 		vol2char(c2->charA.VolName),
												c2->text.TextData);

					free(c2);

					i += sizeof(block_help_28) - 1;
				} else if (c1->pre.Magic == 0x40) {
					fprintf(export, " 02 *%s]\n%s\n", 		vol2char(c1->charA.VolName),
												c1->text.TextData);

					i += sizeof(block_1char_28) - 1;
				}
				// Otherwise, IDK. Ignore it.
			}

			free(c1);
		} else if (buffer[i] == 0x1D) {
			// Raw text block - no formatting. This is a fallback for when there's no real formatting.

			text_ent* c1 = (text_ent*)calloc(sizeof(text_ent), 1);
			memcpy(c1, &buffer[i], sizeof(text_ent));
				
			fprintf(export, "[1D 0x%08lX 00]\n%s\n", i, c1->TextData);

			i += sizeof(text_ent) - 1;
		}
		// Anything not caught shouldn't be, unless I RE'd wrong.
	}

	fflush(export);
	fclose(export);
}

int main(int c, char** v) {
	if (c <= 3) {
		fprintf(stderr, "Usage: %s inject|extract {SceFile.vol} {DumpFile.txt} [SceOut.vol]\n", v[0]);
		return 0;
	}

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
		extract(v[3]);
	} else if(!strcmp(v[1], "inject")) {
		fprintf(stderr, "[info] Using injector.\n");
		if (c == 5) {
			fprintf(stderr, "[info] Alternate output specified. Not clobbering original.\n");
			inject(v[4], v[3]);
		} else {
			inject(v[2], v[3]);
		}
	}

}

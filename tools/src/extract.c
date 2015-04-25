/********************************************************************
  (C) 2015 Jon Feldman

  This is a partially reversed way of reading the scripts from
  Sce files in Maihana ~ Ao no Makyou. It isn't complete...I just
  figured out enough to read the script data, and nothing more.
  
  This is licensed to you under GPLv3. Obey the license.
 ********************************************************************/

#include "structs.h"

void extract (char* buffer, size_t fsize, char* out, int simple) {
	FILE* export = fopen(out, "wb");

	//===================================================
	// Now the real code - search for instances of 0x2A.
	//===================================================
	size_t i;
	for(i=0; i < fsize; i++) {
		// 2A Control code, text block content type A.
		if (buffer[i] == 0x2A && !simple) {
			// MATCHED. Read a block out.
			// First...we have no clue whether it's a single character or
			// double until we fill a struct and check for mismatches.

			block_1char_2a* c1 = (block_1char_2a*)calloc(sizeof(block_1char_2a), 1);
			memcpy(c1, &buffer[i], sizeof(block_1char_2a));

			// If charA doesn't have a valid vol file, we ignore.
			if (strlen(c1->charA.VolName) > 28)
			{
				if(c1->pre.Magic == 0x2A) {
					// This is a twosie. Oops.
					block_2char_2a* c2 = (block_2char_2a*)calloc(sizeof(block_2char_2a), 1);
					memcpy(c2, &buffer[i], sizeof(block_2char_2a));
	
					// Print script info.
					fprintf(export, "[text %08lX %s%s %s%s]\n%s\n", (size_t)(&((block_2char_2a*)&buffer[i])->text) - (size_t)buffer,
									(c2->text.Speaker == 0 ? "*" : ""), vol2char(c2->charA.VolName),
									(c2->text.Speaker == 1 ? "*" : ""), vol2char(c2->charB.VolName),
									 c2->text.TextData);
					free(c2);

					i += sizeof(block_2char_2a) - 1;
				} else if (c1->pre.Magic == 0x1D) {
					// This is a help vol. Not a regular Script vol.

					block_help_2a* c2 = (block_help_2a*)calloc(sizeof(block_help_2a), 1);
					memcpy(c2, &buffer[i], sizeof(block_help_2a));

					// Print script info.
					fprintf(export, "[text %08lX *%s]\n%s\n", (size_t)(&((block_help_2a*)&buffer[i])->text) - (size_t)buffer,
									vol2char(c2->charA.VolName),
									c2->text.TextData);
					free(c2);

					i += sizeof(block_help_2a) - 1;
				} else if (c1->pre.Magic == 0x40) {
					// One character block.
					fprintf(export, "[text %08lX *%s]\n%s\n", (size_t)(&((block_1char_2a*)&buffer[i])->text) - (size_t)buffer,
									vol2char(c1->charA.VolName),
									c1->text.TextData);

					i += sizeof(block_1char_2a) - 1;
				}
				// Otherwise, IDK. Ignore it.
			}

			free(c1);
		} else if (buffer[i] == 0x28 && !simple) {
			// Pretty much the same as 0x2A.

			block_1char_28* c1 = (block_1char_28*)calloc(sizeof(block_1char_28), 1);
			memcpy(c1, &buffer[i], sizeof(block_1char_28));

			// If charA doesn't have a valid vol file, we ignore.
			if (strlen(c1->charA.VolName) > 28)
			{
				if(c1->mid.Magic == 0x28) {
					// This is a twosie. Oops.
					block_2char_28* c2 = (block_2char_28*)calloc(sizeof(block_2char_28), 1);
					memcpy(c2, &buffer[i], sizeof(block_2char_28));
	
					// Print script info.
					fprintf(export, "[text %08lX %s%s %s%s]\n%s\n", (size_t)(&((block_2char_28*)&buffer[i])->text) - (size_t)buffer,
									(c2->text.Speaker == 0 ? "*" : ""), vol2char(c2->charA.VolName),
									(c2->text.Speaker == 1 ? "*" : ""), vol2char(c2->charB.VolName),
									 c2->text.TextData);

					free(c2);

					i += sizeof(block_2char_28) - 1;
				} else if (c1->pre.Magic == 0x1D) {
					// This is a help vol. Not a regular Script vol.

					block_help_28* c2 = (block_help_28*)calloc(sizeof(block_help_28), 1);
					memcpy(c2, &buffer[i], sizeof(block_help_28));

					// Print script info.
					fprintf(export, "[text %08lX *%s]\n%s\n", (size_t)(&((block_help_28*)&buffer[i])->text) - (size_t)buffer,
									vol2char(c2->charA.VolName),
									c2->text.TextData);

					free(c2);

					i += sizeof(block_help_28) - 1;
				} else if (c1->pre.Magic == 0x40) {
					fprintf(export, "[text %08lX *%s]\n%s\n", (size_t)(&((block_1char_28*)&buffer[i])->text) - (size_t)buffer,
									vol2char(c1->charA.VolName),
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
				
			fprintf(export, "[text %08lX]\n%s\n", i, c1->TextData);

			i += sizeof(text_ent) - 1;
		}
		// Anything not caught shouldn't be, unless I RE'd wrong.
	}

	fflush(export);
	fclose(export);
}


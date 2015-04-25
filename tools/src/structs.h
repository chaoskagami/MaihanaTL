/********************************************************************
  (C) 2015 Jon Feldman

  This is a partially reversed way of reading the scripts from
  Sce files in Maihana ~ Ao no Makyou. It isn't complete...I just
  figured out enough to read the script data, and nothing more.
  
  This is licensed to you under GPLv3. Obey the license.
 ********************************************************************/

#ifndef SCETOOL_STRUCTS_H
#define SCETOOL_STRUCTS_H

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

void extract (char* buffer, size_t fsize, char* out, int simple);
void inject (char* buffer, size_t fsize, char* out, char* script);
char* vol2char (char* volname);

#endif

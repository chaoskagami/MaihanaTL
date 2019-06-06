======================================================================
NOTICE - This repo is probably broken. You should just go buy the 
official release from the Nintendo eShop or PSN under the title
Azure Relfections instead of using this shit. It's left only here as
a reference to other programmers and people who are willing to
decipher my garbage with zero aid.
======================================================================
'Maihana Ao Makyou ~ Uniting Barrage Action' English Translation

           NOTE   The game needs to be patched to 1.12.
                  Do you like Flandre? I like Flandre.
        Sections
          S001) TL;DR
          S002) For Users
          S003) For Translators & Hackers
          S004) Copying Notice
          S005) TL Notes
          S006) Progress (Translation)
          S007) Progress (Tools & RE)
          S008) Progress (Cleanup / TLC)
== S001 ==============================================================

TL;DR - To install:
	0) Install Linux if you don't feel like waiting for .bat
           files. I hate cmd with such a passion, you have no clue.
           That said, cygwin should be capable. gcc, xdelta3 and
           bash are required.

	1) Copy everything inside the game's folder into data here.
           That's the folder with the exe and its own data folder.

	2) Run build.sh.

	3) It'll ask you what language you want. There's only jpn
           and en right now. jpn effectively reverts all
           translation sans the MaihanaConfig.exe binary.

	4) Type it and now you can have fun. In english.

== S002 ==============================================================

First off, until every line has been translated, run it in japanese
locale. It won't be necessary for the final - the game's structured
well to handle this stuff. I don't even need to switch fonts. For
windows, this means set the non-unicode to japanese. Alternatively, try
out localeemu. Or applocale. It only needs non-unicode.

To my friends in the linux-zone, using wine - run with LANG="ja_JP"
to run in japanese locale. Again, final won't need it. As another note,
you need to run winetricks xact. The default prefix will give zero
sound and an error on start. Also, some wine versions completely
ignore LANG. Can't help you if that's the case. Take it up with
your distro maintainers.

All of the text in-game is SHIFT-JIS, so otherwise untranslated things
will be garbled if you don't use japanese locale. Like I said, once
all is done, it won't make any difference.

If you're a pro and want to submit a better translation, do so.
Alternatively, if anyone wants to submit other translations than
english, submit a pull request put the scripts into the scripts folder
and name them accordingly. E.g. French StageMarisa would be
fr/SceStageMarisa.txt, etc.

== S003 ==============================================================

For translators/hackers (skip this if you don't fit):

These tools are incomplete. For what is complete, here's some
documentation.
                 
The format of the dumped scripts is like this, using an example to
illustrate what I mean, here's one entry of the translation:

[text 000002F0 *Patch]
I'll teach you all sorts of things.
Select an option you want to hear about.

What is the text should be obvious - the weird square brackets may
not be as clear. As a result of recent changes, contextual information
doesn't need to be considered on inject. The second field is the offset
in the data file as hex. After that, unless using simpleextract, is info
about speaker, emotion, context, etc meant as info for translators. I
may add emotion/expression listing later, but I can only show
information I know how to extract from what I have RE'd.

You have only 256 bytes to work with for dialogue. By default, sjis
can only have 128, but the program doesn't give a shit and both
does variable width and doesn't mind them being packed together.
That said, because only 256 bytes are available, reecoding the script as UTF-8 overflows. Don't try that. It must end zero terminated. Also, newlines are inserted as 0x0A, so
DON'T USE WINDOWS LINE FORMATTING. PERIOD. I have no clue what havok \r
could wreak on my code or the game, and it's a wasted byte. Also, you
need to figure where newlines go manually - they are not automatic.

Onto more technical stuff - The structs defined in tools/src/structs.h
should be considered the definitive documentation on the structures.
I'll try to keep it well commented.

Technically speaking, all strings start with the following hex:
1D 00 00 00 XX 00 00 00 String...

I can use this to find EVERY string, but it would be missing
contextual information like the characters. This is why extract is
the extractor rather than simpleextract. It scrapes more useful stuff.

More information which should probably be noted - images cannot be
modified the same way as scripts (e.g. cut-ins for spell cards, name
boxes, menus). The only way to go about this is to use a tool to dump
textures out of the files they're contained in, edit, and reinsert.

I'll recommend dragon unpacker or dd and bgrep if you're manly. All
textures are in DXT format, so luckily exporting images does not
require size carefulness - the size will always be the same like BMP.
You can edit them in GIMP with a plugin, I believe.

One more thing worth noting is that the software has symbols for lua
in it. I'm not sure how to separate the lua code from the rest of the
vol files, but theoretically one could alter the engine. I'm too busy
RE'ing things like scripts since it's evidently a custom structure.

== S004 ==============================================================

If asked by the creators, I'll remove the original japanese script
and any fragments from translation files.

The japanese script is provided only for people who would like to
translate the game, but may not be able to acquire a copy legally
and DON'T want peglegs, to head off in the seven seas drinking
a bottle o' rum and singing ye ol' sea shanties, and other shady
things of that sort.

I'm the one who translated the majority of the english script now,
and unless an official localization becomes available (highly
unlikely) then my github of this will remain. No content of the
game itself is within the script files here aside from text, and in
all honestly the data files with scripts can be acquired from the
1.12 update without having the game itself.

That said, no original data files will EVER be allowed here. Only
patches and script data.

I'm doing this for fun and for the sake of education - it involved a
shitload of RE and served as a way to brush up my crappy japanese.
Both of which are technical things which I greatly enjoy. The last
thing I want to do is step on the toes of the creators of this
great shmup! Keep making awesome shit, Souvenier Circle! I love
Croixluer Sigma, btw. Bought it in english before it was even sigma.
(/^_^)/

== S005 ==============================================================

TL Notes:
- There's some punny shit in the distorted zones. Komakan vs Komakyo.
  I've no clue how to capture the meaning of that, so expect
  'creativity'. 「紅魔郷」
- Terms lookup
	秘密
	紅魔郷

== S006 ==============================================================

Translation%:
	Reimu Route (SceStageReimu.vol): 
		Stage1				100%
		Ex1				~90%
	Marisa Route (SceStageMarisa.vol):
		*				0%
	Tutorial (SceCommon.vol): 		0%
	Help menu (SceHelp.vol): 
		entry#3 			100%
	Config Program(MaihanaConfig.exe): 	~80%
	Graphics: 				0%

== S007 ==============================================================

Tools:
	Readable:
		SceStageReimu.vol		100%
		SceStageMarisa.vol		100%
		SceHelp.vol			70% (I'm not sure if everything is picked up or not.)
						    (Needs a run through with a hex editor)
		SceCommon.vol			??% (Must use simpleextract)

== S008 ==============================================================

Cleanup & TLC:
	Reimu Route:
		0% TLC done

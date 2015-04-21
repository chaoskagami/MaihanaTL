======================================================================
'Maihana Ao Makyou ~ Uniting Barrage Action' English Translation

           NOTE   The game needs to be patched to 1.12.
                  Do you like Flandre? I like Flandre.
======================================================================

TL;DR - To install:
	0) Install Linux if you don't feel like waiting for .bat
           files. I hate cmd with such a passion, you have no clue.
	1) Copy everything inside the game's folder into Data here.
           That's the folder with the exe.
	2) Run build.sh.
	3) It'll ask you what language you want. There's only jpn
           and en right now.
	4) Type it and now you can have fun. In english.

======================================================================

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
and name them accordingly. E.g. French would be SceStageMarisa.fr.txt,
etc.

======================================================================

If asked by the creators, I'll remove the original japanese script.
Also, if this is ever localized (highly unlikely) then my github of
this will be removed permanently. Also, no original data files will
EVER be allowed here. Only patches. The japanese script is provided
only for people who would like to translate the game, but may not be
able to acquire a copy.

I'm doing this for fun and for the sake of education - it involved a
shitload of RE and I'm awful with japanese.

======================================================================

TL Notes:
- There's some punny shit in the distorted zones. Komakan vs Komakyo.
  I've no clue how to capture the meaning of that, so expect
  'creativity'.

======================================================================

Translation%:
	Reimu Route (SceStageReimu.vol): 
		Stage1				100%
		Ex1				~90%
		Stage2 				0%
	Marisa Route (SceStageMarisa.vol):
		*				0%
	Tutorial (SceCommon.vol): 		0%
	Help menu (SceHelp.vol): 
		entry#3 			100%
	Config Program(MaihanaConfig.exe): 	~80%
	Graphics: 				0%

======================================================================

Tools:
	Readable:
		SceStageReimu.vol		100%
		SceStageMarisa.vol		100%
		SceHelp.vol			70%
	Not readable:
		SceCommon.vol

======================================================================

Cleanup & TLC:
	Reimu Route:
		Stage1: 		Check me.
		Ex1:			Check me.

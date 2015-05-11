#!/bin/bash

if [ "$1" == "" ]; then
	echo "Please pass one of the following languages:"
	cd scripts
	for f in *; do
		echo "   $f"
	done
	cd ..
	exit 0
fi

if [ ! -e tools/scetool ]; then
	gcc -o tools/scetool tools/src/{scetool,inject,extract}.c
fi

cd scripts
if [ -d $1 ] && [ -d ../data/Data/Sce ]; then
	cd $1

	# Inject translation.
	for f in *.txt; do
		CURRENT_VOL=$(basename "$f" .txt)

		if [ -e $CURRENT_VOL.txt ]; then
			mv ../../data/Data/Sce/$CURRENT_VOL.vol{,.bak}
			../../tools/scetool inject ../../data/Data/Sce/$CURRENT_VOL.vol.bak $CURRENT_VOL.txt ../../data/Data/Sce/$CURRENT_VOL.vol
		fi
	done

	cd ../../patches/$1/

	# Apply deltas.

	for f in `ls`; do
		APPLYTO=$(basename $f .xdelta3)

		mv ../../data/${APPLYTO}{,.bak}
		xdelta3 -d -s ../../data/$APPLYTO.bak $f ../../data/$APPLYTO
	done
else
	echo "Not a valid translation or data not copied into data/ folder."
fi

#!/usr/bin/env bash

GOOD_SAMPLES="$(ls Samples/ -1)"
positive_result='successfully ended'

for f in $GOOD_SAMPLES
do
    OUTPUT="$(./out Samples/$f)"
    if [ "$OUTPUT" != "$positive_result" ]
    then
        echo "error in $f file (this should be correct file)"
    fi
done


echo ""


BAD_SAMPLES="$(ls BadSamples/ -1)"

for f in $BAD_SAMPLES
do
    OUTPUT="$(./out BadSamples/$f)"
    if [ "$OUTPUT" == "$positive_result" ]
    then
	echo "no error in $f file (but they myst be)"
    fi
done

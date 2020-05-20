#!/usr/bin/env bash


GOOD_FILES="$(ls Samples/ -1)"
positive_answer='success'

for f in $GOOD_FILES
do
    OUTPUT="$(./parser Samples/$f)"
    if [ "$OUTPUT" != "$positive_answer" ]
    then
        echo "error in $f file (this is correct file)"
    fi
done


echo ""


BAD_FILES="$(ls BadSamples/ -1)"

for f in $BAD_FILES
do
    OUTPUT="$(./parser BadSamples/$f)"
    if [ "$OUTPUT" == "$positive_answer" ]
    then
        if [[ $f == *"optional"* ]]
        then
            echo "no error in $f file (but they myst be), BUT IT IS OPTIONAL"
        else
            echo "no error in $f file (but they myst be)"
        fi
    fi
done
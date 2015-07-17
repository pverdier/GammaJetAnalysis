#!/bin/bash

for f in `ls *.pdf`;
do
    echo pdftopng $f `basename $f .pdf`
    pdftopng $f `basename $f .pdf`
done
#!/bin/bash

csvfile=$1

#cut -d',' -f1,10-13 $csvfile | 
cat $csvfile | sed -e 's/\r//g' -e 's/$/ \\\\ /g' -e 's/,/ \& /g' -e 's/_/\\_/g'

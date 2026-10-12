#/usr/bin/env bash

for i in results/128x128x128x128/2x2x2x2 results/128x128x128x128/3x3x3x3 results/128x128x128x128/4x4x4x4 ;
do
    echo $i

    for run in run{1..9} ;
    do
        echo $run

        for file in $i/$run/bit0/*.txt ; 
        do
            output=$i/$run/$(basename $file)
            utils/max-col-csv.py $i/$run/bit*/$(basename $file) >> $output
        done
    done
done

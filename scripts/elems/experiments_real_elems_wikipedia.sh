#!/bin/bash

module load devel/Boost/1.81.0-GCC-12.2.0


make -j

runs=5


dfile="samples/wikipedia/WIKIPEDIA-100K+_random-articles-all-revisions_[2020-2024).dat"

for el in 1 2 3 5 6 7 8 9 10 12 15; do

    qfile="${dfile}_10K_elems$el-extent0.1%.qry"

    ./query_tif.exec    -o HINTB -m 10              -r $runs  $dfile $qfile       &>> output_elems_wikipedia
    ./query_tif.exec    -o HINTG -m 5               -r $runs  $dfile $qfile       &>> output_elems_wikipedia
    ./query_tif.exec    -o HINTSLICING -m 5 -p 50   -r $runs  $dfile $qfile       &>> output_elems_wikipedia
    ./query_tif.exec    -o SHARDING -i 100 -x 5     -r $runs  $dfile $qfile       &>> output_elems_wikipedia
    ./query_tif.exec    -o SLICING -p 50            -r $runs  $dfile $qfile       &>> output_elems_wikipedia
    ./query_irhint.exec -o A -m 9                   -r $runs  $dfile $qfile       &>> output_elems_wikipedia
    ./query_irhint.exec -o B -m 10                  -r $runs  $dfile $qfile       &>> output_elems_wikipedia

done


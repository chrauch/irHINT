#!/bin/bash

module load devel/Boost/1.81.0-GCC-12.2.0


make -j

runs=5


dfile="samples/eclog/ECOM-LOG.dat"

for ex in 0.01 0.05 0.1 0.5 1.0 5.0 10.0 50.0 100.0; do

    qfile="${dfile}_10K_elems3-extent${ex}%.qry"

    ./query_tif.exec    -o HINTB -m 10              -r $runs  $dfile $qfile       &>> output_extent_eclog
    ./query_tif.exec    -o HINTG -m 5               -r $runs  $dfile $qfile       &>> output_extent_eclog
    ./query_tif.exec    -o HINTSLICING -m 5 -p 100  -r $runs  $dfile $qfile       &>> output_extent_eclog
    ./query_tif.exec    -o SHARDING -i 100 -x 5     -r $runs  $dfile $qfile       &>> output_extent_eclog
    ./query_tif.exec    -o SLICING -p 100           -r $runs  $dfile $qfile       &>> output_extent_eclog
    ./query_irhint.exec -o A -m 10                  -r $runs  $dfile $qfile       &>> output_extent_eclog
    ./query_irhint.exec -o B -m 10                  -r $runs  $dfile $qfile       &>> output_extent_eclog

done


#!/bin/bash

module load devel/Boost/1.81.0-GCC-12.2.0


make -j

runs=5


dfile="samples/eclog/ECOM-LOG.dat"

for m in {1..30}; do

    qfile="${dfile}_10K_elems3-extent0.1%.qry"

    ./query_tif.exec    -o HINTB -m $m              -r $runs  $dfile $qfile       &>> output_eclog_HINTB

done


for m in {1..30}; do

    qfile="${dfile}_10K_elems3-extent0.1%.qry"

    ./query_tif.exec    -o HINTG -m $m              -r $runs  $dfile $qfile       &>> output_eclog_HINTG

done


for m in {1..30}; do

    qfile="${dfile}_10K_elems3-extent0.1%.qry"

    ./query_tif.exec    -o HINTSLICING -m $m -p 50  -r $runs  $dfile $qfile       &>> output_eclog_HINTSLICING

done
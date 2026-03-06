#!/bin/bash

module load devel/Boost/1.81.0-GCC-12.2.0


make -j

runs=5


dir="samples/synthetic"
dfile="$dir/norm_zipf_w128000000_n_dev1000000_a1.2_n1000000.txt_terms50_dict100000_skew1.5"


for el in 7 8 9 10 11 12 13 14 15 20 50; do

    qfile="${dfile}_10K_elems${el}-extent0.1%.qry"

    ./query_irhint.exec -o A -m 13                   -r $runs    $dfile $qfile       &>> output_synthetic_elems
    ./query_irhint.exec -o B -m 15                   -r $runs    $dfile $qfile       &>> output_synthetic_elems
    ./query_tif.exec    -o SHARDING -i 200 -x 2      -r $runs    $dfile $qfile       &>> output_synthetic_elems
    ./query_tif.exec    -o SLICING -p 250            -r $runs    $dfile $qfile       &>> output_synthetic_elems
    ./query_tif.exec    -o HINTSLICING -p 250 -m 8   -r $runs    $dfile $qfile       &>> output_synthetic_elems

done


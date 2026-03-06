#!/bin/bash

module load devel/Boost/1.81.0-GCC-12.2.0


make -j

runs=5


dfile="samples/wikipedia/WIKIPEDIA-100K+_random-articles-all-revisions_[2020-2024).dat"

for re in "none" "less-than-0.001%" "0.001%-0.01%" "0.01%-0.1%" "0.1%-1.0%" "1.0%-10.0%" "more-than-10.0%"; do

    qfile="${dfile}_var_elems1-5-extent0.01%-10%-results[${re}].qry"

    ./query_tif.exec    -o SLICING -p 50            -r $runs  $dfile $qfile       &>> output_resultsmix_wikipedia
    ./query_tif.exec    -o SHARDING -i 100 -x 5     -r $runs  $dfile $qfile       &>> output_resultsmix_wikipedia
    ./query_tif.exec    -o HINTB -m 10              -r $runs  $dfile $qfile       &>> output_resultsmix_wikipedia
    ./query_tif.exec    -o HINTG -m 5               -r $runs  $dfile $qfile       &>> output_resultsmix_wikipedia
    ./query_tif.exec    -o HINTSLICING -m 5 -p 50   -r $runs  $dfile $qfile       &>> output_resultsmix_wikipedia
    ./query_irhint.exec -o A -m 9                   -r $runs  $dfile $qfile       &>> output_resultsmix_wikipedia
    ./query_irhint.exec -o B -m 10                  -r $runs  $dfile $qfile       &>> output_resultsmix_wikipedia

done


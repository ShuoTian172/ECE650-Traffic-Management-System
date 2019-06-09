#!/bin/bash

# calculate mean std-var
# data.txt should be one entry per line

if [[ ! -f data.tar.gz ]]; then
  for V in 5 10 15 17; do
    for i in {1..10}; do
      ./graphGen ${V} > in_${V}_${i}.txt
    done
  done
else
  tar xvf data.tar.gz
  cp data/*.txt .
fi

for V in 5 10 15 17; do
  for i in {1..10}; do
    # in total will test 100 times
    cat ./in_${V}_${i}.txt | ./prj-measure >> out_${V}_${i}.txt
  done
done


## eg. filter = cnf_runtime, dataFileName=cnftime.dat
function cal_mean_std() {
  filter=$1
  dataFileName=$2
  for V in 5 10 15 17; do
    echo -n "${V} " >> ${dataFileName}
    (cat out_${V}_*.txt | grep ${filter} | awk '{print $2}'|  awk '{for(i=1;i<=NF;i++) {sum[i] += $i; sumsq[i] += ($i)^2}}
       END {for (i=1;i<=NF;i++) {
       print sum[i]/NR, sqrt((sumsq[i]-sum[i]^2/NR)/NR)}
       }') >> ${dataFileName}
  done
}

cal_mean_std "cnf_runtime" "cnftime.dat"
cal_mean_std "a1_runtime" "a1time.dat"
cal_mean_std "a2_runtime" "a2time.dat"

cal_mean_std "a1_ratio" "approx1.dat"
cal_mean_std "a2_ratio" "approx2.dat"


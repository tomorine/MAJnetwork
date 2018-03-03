#!/bin/sh

BLIF=/home/tomorow/Downloads/alanmi-abc-c3ebee81b8d8/data/*.blif
OUT=/home/tomorow/NGCNetwork/NGCnetwork_kai/main/data/
RUN_FILE=/home/tomorow/NGCNetwork/NGCnetwork_kai/main/data/*.blif

for file in $BLIF
do
    tmp=${file%.*}
    tmp=${tmp##*/}
    echo ${tmp}_out.blif
    #_out${file##*.}
    ./abc << EOF
read_blif $file
resyn
resyn2
if -K 3
choice
if -K 3
write_blif ${OUT}${tmp}_out.blif
quit 
EOF
    
done

ls

cd $OUT
cd ..
echo "frg-------------"
ls

echo '' > test_out
for run_f in $RUN_FILE
do
    echo ${run_f##*/} >> test_out
    ./ger-program $run_f >> test_out
done

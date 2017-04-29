m=64
for i in 1 2 4 8 16 32
do
       	x=$(($m/$i))
	qsub -q \*@@dwarves -l mem=${x}G -l h_rt=24:0:0 -pe single $i ./twentyfourthread.sh
done

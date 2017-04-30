m=64
echo 4 thread

for i in 1 2 4 8 16 32
do
        x=$(($m/$i))
        qsub -q \*@@dwarves -l mem=${x}G -l h_rt=24:0:0 -pe single $i ./fopen_qsub.sh
done

echo 8 thread

for i in 1 2 4 8 16 32
do
	x=$(($m/$i))
	qsub -q \*@@dwarves -l mem=${x}G -l h_rt=24:0:0 -pe single $i ./eopen_qsub.sh
done

echo 16 thread

for i in 1 2 4 8 16 32
do
        x=$(($m/$i))
        qsub -q \*@@dwarves -l mem=${x}G -l h_rt=24:0:0 -pe single $i ./stopen_qsub.sh
done

echo 24 thread

for i in 1 2 4 8 16 32
do
        x=$(($m/$i))
        qsub -q \*@@dwarves -l mem=${x}G -l h_rt=24:0:0 -pe single $i ./tfopen_qsub.sh
done

echo 32 thread

for i in 1 2 4 8 16 32
do 
	x=$(($m/$i))
	qsub -q \*@@dwarves -l mem=${x}G -l h_rt=24:0:0 -pe single $i ./ttopen_qsub.sh
done

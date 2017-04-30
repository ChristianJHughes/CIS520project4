m=64
echo 1 thread

for i in 1 2 4 8 16 32
do
        x=$(($m/$i))
        qsub -q \*@@dwarves -l mem=${x}G -l h_rt=24:0:0 -pe single $i ./pthread_qsub.sh
done

echo 2 thread

for i in 1 2 4 8 16 32
do
	x=$(($m/$i))
	qsub -q \*@@dwarves -l mem=${x}G -l h_rt=24:0:0 -pe single $i ./doublethread.sh
done

echo 4 thread

for i in 1 2 4 8 16 32
do
        x=$(($m/$i))
        qsub -q \*@@dwarves -l mem=${x}G -l h_rt=24:0:0 -pe single $i ./quadthread.sh
done

echo 8 thread

for i in 1 2 4 8 16 32
do
        x=$(($m/$i))
        qsub -q \*@@dwarves -l mem=${x}G -l h_rt=24:0:0 -pe single $i ./octathread.sh
done

echo 16 thread

for i in 1 2 4 8 16 32
do 
	x=$(($m/$i))
	qsub -q \*@@dwarves -l mem=${x}G -l h_rt=24:0:0 -pe single $i ./sixteenthread.sh
done

echo 24 thread
for i in 1 2 4 8 16 32
do
        x=$(($m/$i))
        qsub -q \*@@dwarves -l mem=${x}G -l h_rt=24:0:0 -pe single $i ./twentyfourthread.sh
done

echo 32 thread
for i in 1 2 4 8 16 32
do
        x=$(($m/$i))
        qsub -q \*@@dwarves -l mem=${x}G -l h_rt=24:0:0 -pe single $i ./thirtytwothread.sh
done


To make a shell script executable run this:
chmod u+x <script_name>



qsub -q \*@@dwarves -l mem=64G -l h_rt=24:0:0 -pe single 1 ./base_qsub.sh 

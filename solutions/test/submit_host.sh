#!/bin/sh
qsub -l h_rt=60 -l mem=512M -pe single 1 ./myhost.sh

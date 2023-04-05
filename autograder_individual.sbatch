#!/bin/bash
# Choose the partition that you will use.
# You can choose: oucspdn_cpu OR debug_5min OR debug OR others 
#
#
#SBATCH --partition=oucspdn_cpu
#
#SBATCH --ntasks=8
#SBATCH --ntasks-per-node=8
#
#SBATCH --time=00:15:00
#SBATCH --job-name=auto
#
#
# --------------------- Change the items below here! ---------------------
#
#SBATCH --mail-user=mariadoan247@ou.edu
#SBATCH --mail-type=ALL
#
#SBATCH --chdir=/home/oucspdn086/Doan_Maria_Project_2/
#SBATCH --output=/home/oucspdn086/Doan_Maria_Project_2/autograding_%J_stdout.txt
#SBATCH --error=/home/oucspdn086/Doan_Maria_Project_2/autograding_%J_stderr.txt

#################################################

module purge

module load SciPy-bundle/2020.03-foss-2020a-Python-3.8.2

python3 -u autograding_individual_2f.py
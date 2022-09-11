#!/bin/bash

# Partition for the job:
#SBATCH --partition=physical

# Multithreaded (SMP) job: must run on one node 
#SBATCH --nodes=1

# The name of the job:
#SBATCH --job-name="parallel"

# Maximum number of tasks/CPU cores used by the job:
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=32

#SBATCH --output=stat.txt
#SBATCH --job-name=parallel

# The maximum running time of the job in days-hours:mins:sec
#SBATCH --time=0-00:30:00

# The project ID which this job should run under:
#SBATCH --account="punim0520"

# Use this email address:
#SBATCH --mail-user=lingxiao1@student.unimelb.edu.au

# Send yourself an email when the job:
# aborts abnormally (fails)
#SBATCH --mail-type=FAIL
# ends successfully
#SBATCH --mail-type=END

# The modules to load:
module load gcc/11.2.0

# compile
gcc -fopenmp solution.c -o solution

#parallel 32 core
for i in {1..10}  
do
./solution 1 32 < input/input400.txt >> output_single.txt
done


my-job-stats -a -n -s

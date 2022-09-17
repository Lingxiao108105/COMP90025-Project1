#!/bin/bash

# Partition for the job:
#SBATCH --partition=physical

# Multithreaded (SMP) job: must run on one node 
#SBATCH --nodes=1

# The name of the job:
#SBATCH --job-name="core5_10"

# Maximum number of tasks/CPU cores used by the job:
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=10

#SBATCH --output=output5_10CoreDynamic.txt

# The maximum running time of the job in days-hours:mins:sec
#SBATCH --time=0-04:00:00

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

#parallel
for core in {5..10}
do
    thread=$(($core * 2))
    for i in {1..10}  
    do
    ./solution 2 $thread < input/input200.txt >> output5_10CoreDynamic.txt
    done
done

my-job-stats -a -n -s

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

#output file
#SBATCH --output=output64.txt

# The maximum running time of the job in days-hours:mins:sec
#SBATCH --time=0-03:00:00

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

#sequential
# for num in {10..100..10}  
# do  
#     for i in {1..10}  
#     do
#     ./solution 0 1 < input/input$num.txt >> output.txt
#     done
# done 

#parallel
# for num in {10..100..10}  
# do  
#     for thread in {1..32}
#     do
#         for i in {1..10}  
#         do
#         ./solution 1 $thread < input/input$num.txt >> output.txt
#         done
#     done
# done 

#parallel 32 core
# for num in {110..400..10}  
# do  
#     for i in {1..10}  
#     do
#     ./solution 1 32 < input/input$num.txt >> output.txt
#     done
# done 

#parallel 1 core
# for i in {1..10}  
# do
# ./solution 1 1 < input/input200.txt >> output.txt
# done

#parallel 64 core
for i in {1..10}  
do
./solution 1 64 < input/input200.txt >> output.txt
done


my-job-stats -a -n -s

#!/bin/bash
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task 32
#SBATCH --output=output.txt
#SBATCH --job-name=parallel
#SBATCH --time=30:00
#SBATCH --account=punim0520
#SBATCH --partition=bigmem


# compile
gcc -fopenmp solution.c -o solution

#sequential
for num in {10..50..10}  
do  
    for i in {1..10}  
    do
    ./solution 0 1 < input/input$num.txt >> output.txt
    done
done 

#parallel
for num in {10..50..10}  
do  
    for thread in {1..32}
    do
        for i in {1..10}  
        do
        ./solution 1 $thread < input/input$num.txt >> output.txt
        done
    done
done 

#!/bin/bash
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task 32
#SBATCH --output=output.txt
#SBATCH --job-name=parallel
#SBATCH --time=10:00
#SBATCH --account=punim0502
#SBATCH --partition=bigmem


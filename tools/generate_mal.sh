#!/bin/bash

# Exit immediately if a command exits with a non-zero status
set -e

EXE_FILE="../build/prepare_malicious"


# 2. Reusable function to generate files
# Usage: generate_data <prefix_name> <repeats> <msprepeats> [maker] [col1] [col2]
generate_data() {
    local base_name=$1
    local repeats=$2
    local msprepeats=$3
    local maker=${4:-""}
    local col1=${5:-""}
    local col2=${6:-""}

    local txt_file="${base_name}_sample.txt"
    local csv_file="${base_name}_dataset.csv"

    echo "----------------------------------------"
    echo "Running generation for: $base_name"

    # Build the command array dynamically
    local cmd=("$EXE_FILE" --text "$txt_file" --csv "$csv_file" --repeats "$repeats" --msprepeats "$msprepeats")

    if [ -n "$maker" ]; then
        cmd+=(--matchingSubPatternMaker "$maker")
    fi
    if [ -n "$col1" ]; then
        cmd+=(--collidingPattern1 "$col1")
    fi
    if [ -n "$col2" ]; then
        cmd+=(--collidingPattern2 "$col2")
    fi

    # Execute the C++ binary
    "${cmd[@]}"
}

# 3. Define your required data generation jobs
# Note: Adjust the 'repeats' (Argument 2) and 'msprepeats' (Argument 3) as per your exact mathematical needs for these sizes.

# Job 1: malicious_one_150000
# Defaults to the long alphanumeric string inside your C++ code
generate_data "malicious_one_18600" 1 100

# Job 2: malicious_one_abc_18600000
# Overrides matchingSubPatternMaker with "abc"
generate_data "malicious_one_abc_1860000" 1 10000 

# Job 3: malicious_one_abc_186000000
# Overrides matchingSubPatternMaker with "abc"
generate_data "malicious_one_abc_186000000" 1 1000000

echo "----------------------------------------"
echo "All data generation completed successfully!"
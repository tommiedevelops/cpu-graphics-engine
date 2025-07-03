#!/bin/bash

if [ $# -ne 2 ]; then
    echo "Usage: $0 input.obj output.obj"
    exit 1
fi

input="$1"
output="$2"

> "$output"  # Clear the output file

while IFS= read -r line; do
    if [[ "$line" =~ ^f[[:space:]]+([^[:space:]]+)[[:space:]]+([^[:space:]]+)[[:space:]]+([^[:space:]]+)[[:space:]]+([^[:space:]]+) ]]; then
        # Quad face
        v1="${BASH_REMATCH[1]}"
        v2="${BASH_REMATCH[2]}"
        v3="${BASH_REMATCH[3]}"
        v4="${BASH_REMATCH[4]}"
        echo "f $v1 $v2 $v3" >> "$output"
        echo "f $v1 $v3 $v4" >> "$output"
    else
        echo "$line" >> "$output"
    fi
done < "$input"

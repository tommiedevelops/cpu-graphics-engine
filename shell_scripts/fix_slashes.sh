#!/bin/bash

# Usage: ./replace_double_slash.sh filename.obj

file="$1"

if [ -z "$file" ]; then
    echo "Usage: $0 filename"
    exit 1
fi

# Replace all occurrences of '//' with '/'
sed -i 's|//|/|g' "$file"

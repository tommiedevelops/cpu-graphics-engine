#!/bin/bash

# Usage: ./add_obj_counts.sh path/to/file.obj

FILE="$1"

if [[ ! -f "$FILE" ]]; then
    echo "File not found: $FILE"
    exit 1
fi

VERTEX_COUNT=$(grep -c '^v ' "$FILE")
FACE_COUNT=$(grep -c '^f ' "$FILE")
UV_COUNT=$(grep -c '^vt ' "$FILE")

# Create a temporary file with the new header
TMP_FILE=$(mktemp)

{
    echo "# vertex count = $VERTEX_COUNT"
    echo "# face count = $FACE_COUNT"
    echo "# uv count = = $UV_COUNT"
    cat "$FILE"
} > "$TMP_FILE"

# Replace original file with updated version
mv "$TMP_FILE" "$FILE"

echo "Added vertex and face count to $FILE"

#!/bin/bash

# 1. Move to the directory where the script is located
cd "$(dirname "$0")"

# 2. Define paths relative to the project root
SOURCE_FILE="main.c"
OUTPUT_DIR="obj/Release"
OUTPUT_FILE="$OUTPUT_DIR/main.o"

# 3. Create the directory if it doesn't exist
# -p ensures it creates parents and doesn't error if it already exists
mkdir -p "$OUTPUT_DIR"

# 4. Run the compiler
echo "Building $OUTPUT_FILE..."
gcc -c "$SOURCE_FILE" -o "$OUTPUT_FILE" -O2

# Check the exit status of the last command ($?)
if [ $? -ne 0 ]; then
    echo "[ERROR] Build failed. Check if $SOURCE_FILE exists in $(pwd)"
    exit 1
fi

echo "[SUCCESS] File built: $OUTPUT_FILE"

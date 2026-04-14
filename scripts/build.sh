cd "$(dirname "$0")"

SOURCE_FILE="main.c"
OUTPUT_DIR="obj/Release"
OUTPUT_FILE="$OUTPUT_DIR/main.o"

mkdir -p "$OUTPUT_DIR"

echo "Building $OUTPUT_FILE..."
gcc -c "$SOURCE_FILE" -o "$OUTPUT_FILE" -O2

if [ $? -ne 0 ]; then
    echo "[ERROR] Build failed. Check if $SOURCE_FILE exists in $(pwd)"
    exit 1
fi

echo "[SUCCESS] File built: $OUTPUT_FILE"

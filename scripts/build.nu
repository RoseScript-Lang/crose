# 1. Move to the directory where the script is located
# Nushell uses $env.FILE_PWD to track the script's location
cd $env.FILE_PWD

# 2. Define paths relative to the project root
let source_file = "main.c"
let output_dir = "obj/Release"
let output_file = ($output_dir | path join "main.o")

# 3. Create the directory if it doesn't exist
# 'mkdir' in Nushell behaves like 'mkdir -p' by default
if not ($output_dir | path exists) {
    mkdir $output_dir
}

# 4. Run the compiler
print $"Building ($output_file)..."

# External commands are called directly; we check the exit code afterward
gcc -c $source_file -o $output_file -O2

if $env.LAST_EXIT_CODE != 0 {
    print -e $"(ansi red_bold)[ERROR](ansi reset) Build failed. Check if ($source_file) exists in ($env.PWD)"
    exit $env.LAST_EXIT_CODE
}

print $"(ansi green_bold)[SUCCESS](ansi reset) File built: ($output_file)"

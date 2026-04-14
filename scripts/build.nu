cd $env.FILE_PWD

let source_file = "main.c"
let output_dir = "obj/Release"
let output_file = ($output_dir | path join "main.o")

if not ($output_dir | path exists) {
    mkdir $output_dir
}

print $"Building ($output_file)..."

gcc -c $source_file -o $output_file -O2

if $env.LAST_EXIT_CODE != 0 {
    print -e $"(ansi red_bold)[ERROR](ansi reset) Build failed. Check if ($source_file) exists in ($env.PWD)"
    exit $env.LAST_EXIT_CODE
}

print $"(ansi green_bold)[SUCCESS](ansi reset) File built: ($output_file)"

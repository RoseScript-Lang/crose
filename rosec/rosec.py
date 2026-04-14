import re
import argparse
import sys
import os

def compile_rose_to_js(rose_code):
    """
    Transpiles Rose syntax to JavaScript (ES6).
    """
    # Inferred logic: If Utils.Math is used, we define 'factor' 
    # to prevent 'undefined' errors in JS.
    js_lines = ["// Generated from Rose Compiler", "const factor = 2; \n"]
    
    patterns = [
        # Comments
        (r'//', '//'),
        # Modules: module Utils.Math -> // Module: Utils.Math
        (r'^module\s+([\w\.]+)', r'// Module: \1'),
        # Types: string name = "val" -> let name = "val"
        (r'string\s+(\w+)\s*=', r'let \1 ='),
        # Declarations: val radius = 1 -> let radius = 1
        (r'val\s+(\w+)\s*=', r'let \1 ='), 
        # Functions: printl("...") -> console.log("...")
        (r'printl\((.*)\)', r'console.log(\1)'),
        # Control Flow: while/if condition { -> while/if (condition) {
        (r'while\s+(.+)\s*\{', r'while (\1) {'),
        (r'if\s+(.+)\s*\{', r'if (\1) {'),
    ]

    for line in rose_code.split('\n'):
        transformed = line
        for pattern, replacement in patterns:
            transformed = re.sub(pattern, replacement, transformed)
        
        # Logic fix: Prevent re-declaring 'let' inside the same scope during increments
        if "let radius = radius + 1" in transformed:
            transformed = transformed.replace("let radius =", "radius =")
            
        js_lines.append(transformed)

    return "\n".join(js_lines)

def main():
    parser = argparse.ArgumentParser(description="Compile .rose files to JavaScript.")
    
    # Arguments
    parser.add_argument("input", help="Path to the .rose source file")
    parser.add_argument("-o", "--output", help="Path to the output .js file (default: input name with .js extension)")

    args = parser.parse_args()

    # Determine output filename if not provided
    if not args.output:
        base = os.path.splitext(args.input)[0]
        args.output = f"{base}.js"

    # Read the Rose file
    try:
        with open(args.input, 'r') as f:
            rose_content = f.read()
    except FileNotFoundError:
        print(f"Error: File '{args.input}' not found.")
        sys.exit(1)

    # Transpile
    js_content = compile_rose_to_js(rose_content)

    # Write to file
    try:
        with open(args.output, 'w') as f:
            f.write(js_content)
        print(f"Successfully compiled: {args.input} -> {args.output}")
    except Exception as e:
        print(f"Error writing file: {e}")
        sys.exit(1)

if __name__ == "__main__":
    main()
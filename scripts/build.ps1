# 1. Move to the directory where the script is located
# $PSScriptRoot is the automatic variable for the script's folder
Set-Location -Path $PSScriptRoot

# 2. Define paths relative to the project root
$SourceFile = "main.c"
$OutputDir = "obj\Release"
$OutputFile = Join-Path $OutputDir "main.o"

# 3. Create the directory if it doesn't exist
if (-not (Test-Path $OutputDir)) {
    New-Item -Path $OutputDir -ItemType Directory | Out-Null
}

# 4. Run the compiler
Write-Host "Building $OutputFile..."
gcc -c $SourceFile -o $OutputFile -O2

# Check the automatic variable $LASTEXITCODE
if ($LASTEXITCODE -ne 0) {
    Write-Error "[ERROR] Build failed. Check if $SourceFile exists in $((Get-Location).Path)"
    exit $LASTEXITCODE
}

Write-Host "[SUCCESS] File built: $OutputFile" -ForegroundColor Green

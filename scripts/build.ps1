Set-Location -Path $PSScriptRoot

$SourceFile = "main.c"
$OutputDir = "obj\Release"
$OutputFile = Join-Path $OutputDir "main.o"

if (-not (Test-Path $OutputDir)) {
    New-Item -Path $OutputDir -ItemType Directory | Out-Null
}

Write-Host "Building $OutputFile..."
gcc -c $SourceFile -o $OutputFile -O2

if ($LASTEXITCODE -ne 0) {
    Write-Error "[ERROR] Build failed. Check if $SourceFile exists in $((Get-Location).Path)"
    exit $LASTEXITCODE
}

Write-Host "[SUCCESS] File built: $OutputFile" -ForegroundColor Green

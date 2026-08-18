param (
    [string]$filename
)

if (-not $filename) {
    Write-Host "Please provide a filename without extension."
    exit
}

g++ "$filename.cpp" -o "$filename.exe" -lfreeglut -lglew32 -lopengl32 -lglu32
if ($LASTEXITCODE -ne 0) {
    Write-Host "Compilation failed."
    exit
}

Start-Process "$filename.exe"

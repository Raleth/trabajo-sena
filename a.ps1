# Obtener la ruta del directorio donde está guardado este script
$scriptPath = Split-Path -Parent $MyInvocation.MyCommand.Definition

# Moverse a ese directorio
Set-Location -Path $scriptPath

Write-Host "Ejecutando 'make'..."
make

if ($LASTEXITCODE -eq 0) {
    Write-Host "Compilación exitosa. Ejecutando bin/prueba.exe..."
    & ".\bin\prueba.exe"
}
else {
    Write-Host "Error al ejecutar 'make'." -ForegroundColor Red
}

Set-Location -Path "C:\Users\dalel\Desktop\Programas C\Ana_Transcriptor"
Write-Host "Ejecutando 'make'..."
make

if ($LASTEXITCODE -eq 0) {
    Write-Host "Compilación exitosa. Ejecutando bin/tu_programa.exe..."
    & ".\bin\prueba.exe"
}
else {
    Write-Host "Error al ejecutar 'make'." -ForegroundColor Red
}
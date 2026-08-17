@echo off

echo [1/2] Configurando...
cmake -S . -B build >nul
if errorlevel 1 exit /b %errorlevel%

echo [2/2] Compilando...
cmake --build build >nul
if errorlevel 1 exit /b %errorlevel%

echo.
echo Executando graphs...
echo.

if exist ".\build\Debug\graphs.exe" (
    .\build\Debug\graphs.exe
) else if exist ".\build\graphs.exe" (
    .\build\graphs.exe
) else (
    echo ERRO: graphs.exe nao encontrado.
    exit /b 1
)
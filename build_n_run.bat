@echo off
echo [1/2] Configurando...
cmake -S . -B build
if errorlevel 1 (
    echo.
    echo ERRO ao configurar o projeto.
    pause
    exit /b 1
)

echo.
echo [2/2] Compilando...
cmake --build build
if errorlevel 1 (
    echo.
    echo ERRO ao compilar o projeto.
    pause
    exit /b 1
)

echo.
echo Executando graphs...
echo.

if exist ".\build\Debug\graphs.exe" (
    ".\build\Debug\graphs.exe"
) else if exist ".\build\graphs.exe" (
    ".\build\graphs.exe"
) else (
    echo ERRO: graphs.exe nao encontrado.
    echo.
    echo Arquivos encontrados em build:
    dir /s /b build\*.exe
    pause
    exit /b 1
)

pause
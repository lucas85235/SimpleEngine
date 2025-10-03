@echo off
setlocal enabledelayedexpansion

:: Enable ANSI color support
for /f "tokens=2 delims=[]" %%a in ('ver') do set "winver=%%a"
reg add HKCU\Console /v VirtualTerminalLevel /t REG_DWORD /d 1 /f >nul 2>&1

for /f %%a in ('echo prompt $E ^| cmd') do set "ESC=%%a"
set "C_GREEN=%ESC%[92m"
set "C_YELLOW=%ESC%[93m"
set "C_CYAN=%ESC%[96m"
set "C_RESET=%ESC%[0m"

echo.
echo %C_CYAN%========================================
echo    FORMATTING ALL SOURCE FILES
echo ========================================%C_RESET%
echo.

:: Navigate to project root
cd /d "%~dp0\.."

:: Check if clang-format exists
where clang-format >nul 2>&1
if %errorlevel% neq 0 (
    echo %C_YELLOW%[WARN] clang-format not found in PATH%C_RESET%
    echo Please install LLVM or use Visual Studio's clang-format
    echo.
    echo Trying Visual Studio's clang-format...
    set "CLANG_FORMAT=C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\Llvm\bin\clang-format.exe"
    if not exist "!CLANG_FORMAT!" (
        echo %C_RED%[ERROR] Could not find clang-format%C_RESET%
        pause
        exit /b 1
    )
) else (
    set "CLANG_FORMAT=clang-format"
)

echo %C_YELLOW%Formatting C/C++ files...%C_RESET%
echo.

set count=0

:: Format all source files
for /r %%f in (*.cpp *.hpp *.h *.c *.cc *.cxx) do (
    :: Skip build and third_party directories
    echo %%f | find /i "\build\" >nul
    if errorlevel 1 (
        echo %%f | find /i "\third_party\" >nul
        if errorlevel 1 (
            echo %C_GREEN%[FORMAT]%C_RESET% %%f
            "!CLANG_FORMAT!" -i "%%f"
            set /a count+=1
        )
    )
)

echo.
echo %C_GREEN%========================================
echo    DONE! Formatted !count! files
echo ========================================%C_RESET%
echo.
pause

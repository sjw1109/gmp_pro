@echo off


:: Check if Git has installed correctly.
git --version >nul 2>&1
if %errorlevel% neq 0 (
    echo Git is not installed correctly.
    echo You may find the installer of Git from the official website：
    echo https://git-scm.com/downloads
    exit /b
) else (
    echo Git has installed, 
    git --version
)

:: Check if Python has installed correctly.
where python >nul 2>&1
if %ERRORLEVEL% neq 0 (
    echo Python is not installed.
    echo You can find Python from the official website: https://www.python.org/downloads/
    exit /b
) else (
    echo Python is installed.
    echo Version: python --version
    python --version
)
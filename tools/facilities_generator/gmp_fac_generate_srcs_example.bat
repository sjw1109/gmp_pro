:: User should copy this file to your project folder


@echo off

:: Check if gmp_src had exist
if exist "gmp_src\" (
    echo gmp_src folder exists, deleting...
    :: remove gmp_src folder
    rmdir /s /q "gmp_src"
    echo gmp_src folder has been deleted.
) else (
    echo gmp_src folder does not exist.
)


:: Record Script Path
set "SCRIPT_DIR=%~dp0"
set "SCRIPT_DIR=%SCRIPT_DIR:~0,-1%"

:: Check Environment variable
if "%GMP_PRO_LOCATION%"=="" (
    echo Error: GMP_PRO_LOCATION environment variable is not set.
    exit /b 1
)

:: call source generator
python "%GMP_PRO_LOCATION%/tools/facilities_generator/gmp_fac_generate_srcs.py" %SCRIPT_DIR%/json/facility_cfg.json .

echo source code generate done.


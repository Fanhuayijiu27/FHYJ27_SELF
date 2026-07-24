@echo off
:: tools\vm_build_watcher.bat
:: VM background listener script for auto-build triggered by host

:: ================= CONFIGURATION =================
set "SHARED_ROOT=Z:\VMwareSHARE\HRproject\31project"
set "LOCAL_SOURCE=C:\Users\ug12\Desktop\HuNXTool\_Source\FeatureSpy260722"
set "VCVARSALL_PATH=C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat"
set "MSBUILD_PATH=C:\Program Files (x86)\MSBuild\14.0\Bin\MSBuild.exe"
set "SLN_PATH=%LOCAL_SOURCE%\FeatureSpy260722.sln"

set "BUILD_CONFIG=Debug"
set "BUILD_PLATFORM=x64"
:: ============================================

set "SHARED_SOURCE_DIR=%SHARED_ROOT%\shared_source_autoVS2015\FeatureSpy260722"
set "BUILD_BRIDGE_DIR=%SHARED_ROOT%\build_bridge_autoVS2015"

set "FILE_REQUEST=%BUILD_BRIDGE_DIR%\build.request"
set "FILE_RUNNING=%BUILD_BRIDGE_DIR%\build.running"
set "FILE_DONE=%BUILD_BRIDGE_DIR%\build.done"
set "FILE_FAILED=%BUILD_BRIDGE_DIR%\build.failed"
set "FILE_LOG=%BUILD_BRIDGE_DIR%\build_latest.log"

echo ===================================================
echo Listening for build requests...
echo Shared Root: %SHARED_ROOT%
echo Local Source: %LOCAL_SOURCE%
echo ===================================================

:LISTEN_LOOP
if exist "%FILE_REQUEST%" (
    goto PROCESS_BUILD
)
ping 127.0.0.1 -n 3 >nul 2>nul
goto LISTEN_LOOP

:PROCESS_BUILD
echo.
echo [%date% %time%] Build request found, starting process...

set /p BUILD_ID=<"%FILE_REQUEST%"
echo BuildId: %BUILD_ID%

if exist "%FILE_DONE%" del /f /q "%FILE_DONE%"
if exist "%FILE_FAILED%" del /f /q "%FILE_FAILED%"

move /y "%FILE_REQUEST%" "%FILE_RUNNING%" >nul 2>nul

echo =================================================== > "%FILE_LOG%"
echo [BUILD START] >> "%FILE_LOG%"
echo BuildId: %BUILD_ID% >> "%FILE_LOG%"
echo StartTime: %date% %time% >> "%FILE_LOG%"
echo SharedSource: %SHARED_SOURCE_DIR% >> "%FILE_LOG%"
echo LocalSource: %LOCAL_SOURCE% >> "%FILE_LOG%"
echo Solution: %SLN_PATH% >> "%FILE_LOG%"
echo Configuration: %BUILD_CONFIG% >> "%FILE_LOG%"
echo Platform: %BUILD_PLATFORM% >> "%FILE_LOG%"
echo =================================================== >> "%FILE_LOG%"

echo [%date% %time%] Syncing source code...
echo [%date% %time%] [SYNC] Syncing source code to VM... >> "%FILE_LOG%"
robocopy "%SHARED_SOURCE_DIR%" "%LOCAL_SOURCE%" /MIR /XD .git .vs x64 Debug Release /XF *.sdf *.opensdf *.user *.log >> "%FILE_LOG%" 2>&1
set ROBO_EXIT=%ERRORLEVEL%

if "%ROBO_EXIT%"=="8" goto SYNC_FAIL
if "%ROBO_EXIT%"=="9" goto SYNC_FAIL
if "%ROBO_EXIT%"=="10" goto SYNC_FAIL
if "%ROBO_EXIT%"=="11" goto SYNC_FAIL
if "%ROBO_EXIT%"=="12" goto SYNC_FAIL
if "%ROBO_EXIT%"=="13" goto SYNC_FAIL
if "%ROBO_EXIT%"=="14" goto SYNC_FAIL
if "%ROBO_EXIT%"=="15" goto SYNC_FAIL
if "%ROBO_EXIT%"=="16" goto SYNC_FAIL
goto SYNC_SUCCESS

:SYNC_FAIL
    echo [%date% %time%] Source sync failed (ExitCode: %ROBO_EXIT%)
    echo [%date% %time%] [SYNC FAILED] Source sync failed (ExitCode: %ROBO_EXIT%) >> "%FILE_LOG%"
    set BUILD_STATUS=FAILED
    set MSBUILD_EXIT=%ROBO_EXIT%
    goto BUILD_FINISH

:SYNC_SUCCESS

echo [%date% %time%] Building code...
echo [%date% %time%] [BUILD] Calling MSBuild... >> "%FILE_LOG%"

call "%VCVARSALL_PATH%" amd64 >> "%FILE_LOG%" 2>&1
call "%MSBUILD_PATH%" "%SLN_PATH%" /t:Rebuild /p:Configuration="%BUILD_CONFIG%" /p:Platform="%BUILD_PLATFORM%" /m:1 >> "%FILE_LOG%" 2>&1
set MSBUILD_EXIT=%ERRORLEVEL%

if %MSBUILD_EXIT% EQU 0 (
    echo [%date% %time%] Build SUCCESS!
    set BUILD_STATUS=SUCCESS
) else (
    echo [%date% %time%] Build FAILED! ExitCode: %MSBUILD_EXIT%
    set BUILD_STATUS=FAILED
)

:BUILD_FINISH
echo =================================================== >> "%FILE_LOG%"
echo [BUILD FINISH] >> "%FILE_LOG%"
if "%BUILD_STATUS%"=="SUCCESS" (
    echo Result: BUILD_SUCCESS >> "%FILE_LOG%"
) else (
    echo Result: BUILD_FAILED >> "%FILE_LOG%"
)
echo ExitCode: %MSBUILD_EXIT% >> "%FILE_LOG%"
echo EndTime: %date% %time% >> "%FILE_LOG%"
echo =================================================== >> "%FILE_LOG%"

if "%BUILD_STATUS%"=="SUCCESS" (
    echo %BUILD_ID%> "%FILE_DONE%"
) else (
    echo %BUILD_ID% %MSBUILD_EXIT%> "%FILE_FAILED%"
)

if exist "%FILE_RUNNING%" del /f /q "%FILE_RUNNING%"

echo [%date% %time%] Task finished, waiting for next request...
echo.

goto LISTEN_LOOP
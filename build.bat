@echo off
:: Build script for DemoTape using CMake on Windows

:: Store current directory and script directory
set "ORIGINAL_DIR=%CD%"
set "SCRIPT_DIR=%~dp0"
echo Script directory: %SCRIPT_DIR%

:: Change to the script directory
cd /d "%SCRIPT_DIR%"
echo Current directory: %CD%

:: Create build directory
if not exist build mkdir build
cd build

:: Remove the trailing backslash from SCRIPT_DIR to avoid issues
set "SOURCE_DIR=%SCRIPT_DIR:~0,-1%"
echo Source directory: %SOURCE_DIR%

:: Create logs directory if it doesn't exist
if not exist logs mkdir logs

:: Enable verbose makefile output and debug log with more detailed output
echo.
echo ========== RUNNING CMAKE CONFIGURE ==========
cmake -G "Visual Studio 17 2022" -A x64 -T ClangCL -DCMAKE_VERBOSE_MAKEFILE=ON -DCMAKE_MESSAGE_LOG_LEVEL=VERBOSE "%SOURCE_DIR%" > logs\cmake_configure.log 2>&1
if %ERRORLEVEL% NEQ 0 (
  echo [ERROR] CMake configure FAILED! See logs\cmake_configure.log for details.
  type logs\cmake_configure.log | findstr /i "error warning"
  goto build_failed
) else (
  echo [SUCCESS] CMake configure completed successfully.
)

:: Build with verbose output
echo.
echo ========== RUNNING CMAKE BUILD ==========
cmake --build . --config Debug --verbose > logs\cmake_build.log 2>&1
if %ERRORLEVEL% NEQ 0 (
  echo [ERROR] CMake build FAILED! See logs\cmake_build.log for details.
  echo.
  echo === Last few lines of build log: ===
  type logs\cmake_build.log | findstr /i "error warning failed"
  goto build_failed
) else (
  echo [SUCCESS] CMake build completed successfully.
)

:: Check if VST3 plugin was actually built
set "VST3_PATH=%CD%\DemoTape_artefacts\Debug\VST3\DemoTape.vst3\Contents\x86_64-win\DemoTape.vst3"
if exist "%VST3_PATH%" (
  echo.
  echo [SUCCESS] VST3 plugin was successfully built!
  echo Plugin location: %VST3_PATH%
) else (
  echo.
  echo [WARNING] VST3 plugin file was not found at expected location:
  echo %VST3_PATH%
  echo Build may have failed silently. Check logs for errors.
  goto build_failed
)

echo.
echo ========== BUILD SUMMARY ==========
echo BUILD STATUS: SUCCESS
echo.
echo All build logs are available in: %CD%\logs\
goto end

:build_failed
echo.
echo ========== BUILD SUMMARY ==========
echo BUILD STATUS: FAILED
echo.
echo Examine the log files for more details:
echo CMake Output Log: %CD%\logs\CMakeOutput.log
echo CMake Error Log: %CD%\logs\CMakeError.log
echo CMake Configure Log: %CD%\logs\cmake_configure.log
echo CMake Build Log: %CD%\logs\cmake_build.log

:end
:: Copy CMake's internal logs to our logs directory for easier access
if exist CMakeFiles\CMakeOutput.log copy /Y CMakeFiles\CMakeOutput.log logs\
if exist CMakeFiles\CMakeError.log copy /Y CMakeFiles\CMakeError.log logs\

:: Return to original directory
cd /d "%ORIGINAL_DIR%"

echo.
pause 
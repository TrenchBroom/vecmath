REM Init submodules
git submodule update --init

REM Check versions
cmake --version
cppcheck --version

set SOURCE_DIR="%cd%"
mkdir cmakebuild
cd cmakebuild

cmake .. -G"Visual Studio 15 2017" -T v141 -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="/WX"

REM  -DCMAKE_CXX_FLAGS=/WX

IF ERRORLEVEL 1 GOTO ERROR

REM cmake --build . --target cppcheck

IF ERRORLEVEL 1 GOTO ERROR_CPPCHECK

cmake --build . --config Release

IF ERRORLEVEL 1 GOTO ERROR

set BUILD_DIR="%cd%"

cd test\Release
vecmath-test.exe
IF ERRORLEVEL 1 GOTO ERROR
cd "%BUILD_DIR%"

REM run compilation benchmark
cd "%SOURCE_DIR%"
cd compilation_benchmark
REM this is necessary to add CL.exe to PATH (see: https://www.appveyor.com/docs/lang/cpp/)
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars64.bat"
powershell ./compilation_benchmark.ps1

GOTO END

:ERROR_CPPCHECK

echo.
echo "cppcheck detected issues, see below"
echo.

type cppcheck-errors.txt

echo.

:ERROR

echo "Building libvecmath failed"
exit /b 1

:END

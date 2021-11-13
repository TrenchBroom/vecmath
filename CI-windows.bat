REM Init submodules
git submodule update --init

REM Check versions
cmake --version

set SOURCE_DIR="%cd%"
mkdir cmakebuild
cd cmakebuild

cmake .. -G"Visual Studio 16 2019" -T v142 -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="/WX"

REM  -DCMAKE_CXX_FLAGS=/WX

IF ERRORLEVEL 1 GOTO ERROR

cmake --build . --config Release

IF ERRORLEVEL 1 GOTO ERROR

set BUILD_DIR="%cd%"

cd test\Release
vecmath-test.exe
IF ERRORLEVEL 1 GOTO ERROR

GOTO END

echo.

:ERROR

echo "Building libvecmath failed"
exit /b 1

:END

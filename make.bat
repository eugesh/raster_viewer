set CC_BIN_DIR=C:\MSVS\2010\VC\bin
call %CC_BIN_DIR%/VCVARS32.BAT

set QT_DIR_VER=C:/qt/4.8.5
call %QT_DIR_VER%/bin/qtvars.bat

set path=%path%;%CUDA_DIR%/open64/bin;%CUDA_DIR%/bin;%CUDA_DIR%/open64/lib;
echo %path%

set BASE_DIR=%CD%

C:/mingw/make/bin/mingw32-make.exe  -f  makefile.mak all -e MAKE_FOR_SYSTEM=make_win.mak %1
rem mingw32-make.exe  -f  makefile.mak program -e MAKE_FOR_SYSTEM=make_win.mak %1


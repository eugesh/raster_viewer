rem set QT_DIR_VER=C:\Qt_4\4_7_1
rem set QT_DIR_VER=C:\Qt\4.7.1
set QT_DIR_VER=C:/qt/4.8.5

set path=%path%;%QT_DIR_VER%\bin;

rem set CUDA_DIR=C:/CUDA/tkt3_2_32/v3.2
rem C:\CUDA\tkt3_2_32\v3.2\bin
rem set path=%path%;%CUDA_DIR%/open64/bin;%CUDA_DIR%/bin;%CUDA_DIR%/open64/lib;

echo %path%
call exe\interior_orientation.exe


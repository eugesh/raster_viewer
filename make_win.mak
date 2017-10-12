CPP=$(CC_BIN_DIR)/cl
LINK=link
LIB_CR=$(CC_BIN_DIR)\lib
MAKE=mingw32-make.exe
#CPP=$(ICC_BIN_DIR)\icl
#LINK=$(ICC_BIN_DIR)\xilink.exe
MOC=$(QT_DIR_VER)/bin/moc.exe
UIC=$(QT_DIR_VER)/bin/uic.exe

#TIFF_DIR="C:/GnuWin32"
#GDAL_DIR="C:/gdalwin32-1.4.1"
#GIS_DIR="C:/Integracia8"
#IPPLIB_DIR = "C:/Intel/IPP/5.0/ia32"
#LIBINTEL = "C:/Intel/Compiler/C++/10.0.026/IA32/lib"
#CVLIB_DIR = "C:/OpenCV2.1"
#MKLLIB_DIR = "C:/mkl/MKL/8.1"

GDAL_DIR="C:/warmerda/bld"

############################################################
# Author: Anton 
# Date: 10/04/09
############################################################
MYLIB_DIR = D:/projects/Anton/include
############################################################

# расширение для объектных файлов
ext_obj=obj

# команда удаления файлов
command_del= rm -f

SD=\\


# знак для задания имени файла в include make
opt_f_b=<
opt_f_e=>
# опция для задания директорий с заголовочными файлами
opt_inc=/I
# опиция f для make
opt_make_f=-f

# основные библиотеки windows
library=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib imm32.lib winmm.lib wsock32.lib opengl32.lib glu32.lib 

#DEBUG=yes
#USE_MKL=yes
#USE_IPP=yes
#USE_OPENCV=yes
#USE_DEVCONTROL=yes
#USE_CUDA=yes
#USE_LIBTIFF=yes
USE_GDAL=yes

ifdef DEBUG
library+=\
	$(QT_DIR_VER)/lib/qtmaind.lib $(QT_DIR_VER)/lib/QtCored4.lib $(QT_DIR_VER)/lib/QtGuid4.lib $(QT_DIR_VER)/lib/QtNetworkd4.lib 
#	$(QT_DIR_VER)\lib\Qt3Supportd4.lib $(QT_DIR_VER)\lib\QtOpenGLd4.lib $(QT_DIR_VER)\lib\QtXmld4.lib
else
	library+=\
	$(QT_DIR_VER)/lib/qtmain.lib $(QT_DIR_VER)/lib/QtCore4.lib $(QT_DIR_VER)/lib/QtGui4.lib $(QT_DIR_VER)/lib/QtNetwork4.lib 
#	$(QT_DIR_VER)\lib\Qt3Support4.lib $(QT_DIR_VER)\lib\QtOpenGL4.lib $(QT_DIR_VER)\lib\QtXml4.lib

endif
# библиотеки Qt 3.x
#library=$(library) \
#			$(QT_DIR_VER)\lib\qtmain.lib $(QT_DIR_VER)\lib\qt-mt.lib $(QT_DIR_VER)\lib\qui.lib

ifdef USE_GDAL
library+=\
			$(GDAL_DIR)/lib/gdal_i.lib
endif

ifdef USE_LIBTIFF			
library+=\
			$(TIFF_DIR)/lib/libtiff.lib
endif

ifdef USE_MKL
library+=\
			$(MKLLIB_DIR)/ia32/lib/mkl_c.lib $(MKLLIB_DIR)/ia32/lib/mkl_s.lib \
			$(MKLLIB_DIR)/ia32/lib/mkl_lapack.lib $(MKLLIB_DIR)/ia32/lib/mkl_ia32.lib \
			$(MKLLIB_DIR)/ia32/lib/mkl_solver.lib $(MKLLIB_DIR)/ia32/lib/libguide.lib \
			$(MKLLIB_DIR)/ia32/lib/libguide40.lib
endif

ifdef USE_IPP
library+=\
			$(IPPLIB_DIR)/stublib/ippi.lib\
			$(IPPLIB_DIR)/stublib/ipps.lib\
			$(IPPLIB_DIR)/stublib/ippcore.lib\
			$(IPPLIB_DIR)/stublib/ippcv.lib
endif

ifdef USE_OPENCV
library+=\
			$(CVLIB_DIR)/lib/cv.lib\
			$(CVLIB_DIR)/lib/highgui.lib\
			$(CVLIB_DIR)/lib/cxcore.lib
endif			

# опиции для компилятора С++  /RTC1 /Qopenmp
ifdef DEBUG
cpp_options=  /EHs /GR /D "WIN32"  /D "DEBUG" /D "_CONSOLE"  /D "WIN32API"  \
				/D "_CRT_SECURE_NO_DEPRECATE"   \
				/D "__MSVC_RUNTIME_CHECKS"  \
				/D "USE_IPP" /D "USE_OPENCV" \
				/Zc:wchar_t- /MDd   /Zp16 /RTC1
else
cpp_options=  /EHs /GR /D "WIN32"   /D "_CONSOLE"  /D "WIN32API"  \
				/D "_CRT_SECURE_NO_DEPRECATE"   \
				/D "__MSVC_RUNTIME_CHECKS"  \
				/Zc:wchar_t- /MD  /QaxNPT /Zp16 /Qparallel /Qpar-report1  /Qvec-report1 /Qopenmp
endif

ifdef USE_DEVCONTROL
cpp_options += /D "USE_DEVCONTROL"
endif

#опиции для компилятора С++ для работы с Qt 4.x
qt_options= /D "QT3_SUPPORT" /D "QT_DLL" /D "QT_THREAD_SUPPORT" \
	/D "QT_LARGEFILE_SUPPORT" /D "QT_EDITION=QT_EDITION_DESKTOP" /D "QT_GUI_LIB" /D "QT_CORE_LIB" 	

qt_options+= \
	$(opt_inc) "$(QT_DIR_VER)/include" \
	$(opt_inc) "$(QT_DIR_VER)/include/QtGui" \
	$(opt_inc) "$(QT_DIR_VER)/include/QtCore" \
	$(opt_inc) "$(QT_DIR_VER)/include/Qt3Support" \
	$(opt_inc) "$(QT_DIR_VER)/include/Qt"  \
	$(opt_inc) "$(QT_DIR_VER)/include/QtNetwork" \
	$(opt_inc) "$(QT_DIR_VER)/include/QtXml" 

# установки для cuda
cpp_options_cuda=$(opt_inc) "$(CUDA_DIR)/include"

ifdef USE_CUDA
library+=$(CUDA_DIR)/lib/Win32/cuda.lib $(CUDA_DIR)/lib/Win32/cudart.lib $(CUDA_DIR)/lib/Win32/cufft.lib $(CUDA_DIR)/lib/Win32/cublas.lib $(CUDA_DIR)/lib/Win32/curand.lib $(CUDA_DIR)/lib/Win32/cusparse.lib $(CUDA_DIR)/lib/Win32/nvcuvenc.lib $(CUDA_DIR)/lib/Win32/nvcuvid.lib $(CUDA_DIR)/lib/Win32/OpenCL.lib
endif

############################################################

cuda_options_include = -I"$(CUDA_DIR)/include" \
						-I"$(source_mylib_cuda_dir)" \
						-I"$(source_mylib_matching_dir)" \
						-I"$(source_mylib_devraster_dir)" \
						-I"$(source_mylib_types_dir)"

cuda_options_mylib = -I"$(source_mylib_geometry_dir)"
						
command_comp_cuda=$(CUDA_DIR)/bin/nvcc.exe  -m32 -arch sm_13 -c -ccbin=$(CC_BIN_DIR) $(cuda_options_include) $(cuda_options_mylib) -o=$@ $?

command_comp=$(CPP) /c $(cpp_options) $(cpp_options_include) $(qt_options) $(cpp_options_cuda) $(mylib_options_include) /Fo$@ $?

############################################################

command_touch=touch $@
command_link=$(LINK) /OUT:$@ /SUBSYSTEM:windows /machine:X86

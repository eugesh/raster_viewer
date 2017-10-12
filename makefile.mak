
include $(MAKE_FOR_SYSTEM)
	
base_dir=$(BASE_DIR)
name_exe=interior_orientation_test.exe
##############################################

source_mylib_aux_dir = $(MYLIB_DIR)/auxdir
source_mylib_cuda_dir = $(MYLIB_DIR)/cuda
source_mylib_devraster_dir = $(MYLIB_DIR)/devraster
source_mylib_math_la_dir = $(MYLIB_DIR)/math/LA
source_mylib_matching_dir = $(MYLIB_DIR)/matching
source_mylib_types_dir = $(MYLIB_DIR)/types
source_mylib_camera_dir = $(MYLIB_DIR)/camera
source_mylib_geometry_dir = $(MYLIB_DIR)/math/geometry
source_mylib_video_analysis_dir = $(MYLIB_DIR)/video_analysis

##############################################

	
# директория с исходными файлами
source_dir=$(base_dir)/source
source_shot_dir=$(base_dir)/shot
source_geom_dir=$(SHARED_MODULE_DIR)/geom
source_raster_dir=$(SHARED_MODULE_DIR)/raster
# директория с исходными файлами модуля работы с tiff-файлами
source_tiff_dir=$(SHARED_MODULE_DIR)/source_tiff
# директория с исходными файлами с сетью
source_network_dir=$(SHARED_MODULE_DIR)/source_network
# директория с исходыми файлами фотограмметрического модуля
source_photogrammetry_dir=$(SHARED_MODULE_DIR)/source_photogrammetry
# директория с исходными файлами модуля работы с картографическими СК
source_cartographic_sc_dir=$(SHARED_MODULE_DIR)/cartographic_sc
# дирктория с исходными файлами для работы с модулем ГИС-Интеграция
source_gis_dir=$(SHARED_MODULE_DIR)/source_gis
# директория с исходными файлами для работы с многообъективными камерами
source_multi_shot_dir=$(SHARED_MODULE_DIR)/source_multi_shot
source_equiv_shot_dir=$(SHARED_MODULE_DIR)/source_equiv_shot
#
source_module_link=$(SHARED_MODULE_DIR)/source_module_link
# директория с исходными файлами для работы с моделями рельефа
source_relief_dir=$(SHARED_MODULE_DIR)/source_relief
# директория с исходными файлами для работы с КСОИ
source_ksoi_dir=$(SHARED_MODULE_DIR)/source_ksoi
# директория с h-файлами оконных форм

source_graphic_item_image=$(base_dir)/graphic_item_image

source_header_form_dir=$(base_dir)/form_h
# директория с ui-файлами оконных форм
source_form_dir=$(base_dir)/form_ui

# директория для объектных файлов
obj_dir=$(base_dir)/obj
# директория для moc-файлов
moc_dir=$(base_dir)/moc
# директория для выполняемого файла
proc_dir=$(base_dir)/exe


# директории с библиотечными файлами
library_source_dir=$(base_dir)/library_source
library_obj_dir=$(base_dir)/library_obj

# директория с заголовочными файлами
cpp_options_include=$(opt_inc) "$(NIITP_CPP)/include" \
							$(opt_inc) "$(TIFF_DIR)/include" \
							$(opt_inc) "$(GDAL_DIR)/include" \
							$(opt_inc) "$(source_geom_dir)" \
							$(opt_inc) "$(source_cartographic_sc_dir)" \
							$(opt_inc) "$(source_dir)" \
							$(opt_inc) "$(source_header_form_dir)"

##############################################
mylib_options_include =	$(opt_inc) "$(source_mylib_aux_dir)" \
						$(opt_inc) "$(source_mylib_cuda_dir)" \
						$(opt_inc) "$(source_mylib_devraster_dir)" \
						$(opt_inc) "$(source_mylib_math_la_dir)" \
						$(opt_inc) "$(source_mylib_matching_dir)" \
						$(opt_inc) "$(source_mylib_types_dir)" \
						$(opt_inc) "$(source_mylib_camera_dir)" \
						$(opt_inc) "$(source_mylib_geometry_dir)" \
						$(opt_inc) "$(source_mylib_video_analysis_dir)"

ifdef USE_MKL
cpp_options_include+=\
						$(opt_inc) "$(MKLLIB_DIR)/include"
endif


ifdef USE_IPP
cpp_options_include+=\
						$(opt_inc) "$(IPPLIB_DIR)/include"
endif

ifdef USE_OPENCV
cpp_options_include+=\
						$(opt_inc) "$(CVLIB_DIR)/CV/include" \
						$(opt_inc) "$(CVLIB_DIR)/OtherLibs/_Ipl/include" \
						$(opt_inc) "$(CVLIB_DIR)/cxcore/include" \
						$(opt_inc) "$(CVLIB_DIR)/otherlibs/highgui"
endif
						
##############################################							
 
# задание правил 
.SUFFIXES :

# h-файлы с формами для окон
h_file_widget=$(source_header_form_dir)/form_mainwindow_krest.h

# объектные файла за исключением moc-файлов
obj_file=\
	$(obj_dir)/main.$(ext_obj)\
	$(obj_dir)/Krest_window.$(ext_obj)\
	$(obj_dir)/krestview.$(ext_obj)\
	$(obj_dir)/krest.$(ext_obj)\
	$(obj_dir)/RubberRect.$(ext_obj)\
	$(obj_dir)/Krest_scene.$(ext_obj)\
	$(obj_dir)/cthread.$(ext_obj)

# объектные файлы полученные из moc-файлов						
obj_moc_file=\
	$(obj_dir)/krest_window_moc.$(ext_obj)\
	$(obj_dir)/krest_scene_moc.$(ext_obj)\
	$(obj_dir)/cthread_moc.$(ext_obj)

# библиотечные модули
library_obj_file=

# объектные файлы с cuda-кодом
obj_file_cuda=

all:
	$(MAKE) $(opt_make_f) makefile.mak clean
	$(MAKE) $(opt_make_f) makefile.mak program

clean:
#	$(command_del) $(base_dir)$(SD)obj$(SD)*.$(ext_obj)
#	$(command_del) $(base_dir)$(SD)library_obj$(SD)*.$(ext_obj)
	$(command_del) $(base_dir)$(SD)moc$(SD)*moc.cpp
	$(command_del) $(obj_file)
	$(command_del) $(obj_moc_file)
	$(command_del) $(library_obj_file)
	$(command_del) $(obj_file_cuda)
	$(command_del) $(proc_dir)/$(name_exe)

# описание цели "построение приложения"
program :  $(h_file_widget) $(proc_dir)/$(name_exe)

# создание h-файлов на основе ui-файлов
$(source_header_form_dir)/form_mainwindow_krest.h : $(source_form_dir)/form_mainwindow_krest.ui
	$(UIC) -o $@ $?

# создание moc-файлов
$(moc_dir)/krest_window_moc.cpp : $(source_dir)/Krest_window.h
	$(MOC) -o $@ $?

$(moc_dir)/krest_scene_moc.cpp : $(source_dir)/Krest_scene.h
	$(MOC) -o $@ $?

$(moc_dir)/cthread_moc.cpp : $(source_dir)/cthread.h
	$(MOC) -o $@ $?

# .moc files compilation

$(obj_dir)/krest_window_moc.$(ext_obj) : $(moc_dir)/krest_window_moc.cpp
	$(command_comp)

$(obj_dir)/krest_scene_moc.$(ext_obj) : $(moc_dir)/krest_scene_moc.cpp
	$(command_comp)

$(obj_dir)/cthread_moc.$(ext_obj) : $(moc_dir)/cthread_moc.cpp
	$(command_comp)

$(obj_dir)/Krest_window.$(ext_obj) : $(source_dir)/Krest_window.cpp
	$(command_comp)	

$(obj_dir)/main.$(ext_obj) : $(source_dir)/main.cpp
	$(command_comp)

$(obj_dir)/krestview.$(ext_obj) : $(source_dir)/krestview.cpp
	$(command_comp)

$(obj_dir)/RubberRect.$(ext_obj) : $(source_dir)/RubberRect.cpp
	$(command_comp)	

$(obj_dir)/krest.$(ext_obj) : $(source_dir)/krest.cpp
	$(command_comp)	

$(obj_dir)/Krest_scene.$(ext_obj) : $(source_dir)/Krest_scene.cpp
	$(command_comp)

$(obj_dir)/cthread.$(ext_obj) : $(source_dir)/cthread.cpp
	$(command_comp)	

######################################################

include $(source_graphic_item_image)/graphic_item_image.mak

# создание приложения с помощью линковщика
$(proc_dir)/$(name_exe)  : $(obj_file) $(obj_moc_file) $(library_obj_file) $(obj_file_cuda)
	$(command_link)  $(obj_file) $(obj_moc_file)   $(library_obj_file) $(obj_file_cuda) $(library)


# объектные файла за исключением moc-файлов
obj_file+=\
	$(obj_dir)/graphic_item_image.$(ext_obj)

# исходные файлы
$(obj_dir)/graphic_item_image.$(ext_obj) : $(source_graphic_item_image)/graphic_item_image.cpp
	$(command_comp)

$(source_graphic_item_image)/graphic_item_image.cpp : $(source_graphic_item_image)/graphic_item_image.h
	$(command_touch)
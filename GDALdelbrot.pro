QT += core gui widgets

qtHaveModule(printsupport): QT += printsupport
qtHaveModule(opengl): QT += opengl

unix{
INCLUDEPATH += /usr/include/gdal
#LIB += -L/path_to_lib -lname_of_lib
#LIB += -L/usr/lib/x86_64-linux-gnu/perl5/5.22/auto/Geo/GDAL -lGDAL
#LIB += -L/usr/lib -libgdal
#LIB += -L/usr/lib -lgdal
LIBS += /usr/lib/libgdal.so
}

HEADERS       = mandelbrotwidget.h \
                affine_tr.hpp \
                ehmath.h \
                gdal-raster-item.h \
                gdal-wrapper.hpp \
                mainwindow.h \
                renderthread.h
SOURCES       = main.cpp \
                gdal-raster-item.cpp \
                gdal-wrapper.cpp \
                mainwindow.cpp \
                mandelbrotwidget.cpp \
                renderthread.cpp

unix:!mac:!vxworks:!integrity:!haiku:LIBS += -lm

# install
target.path = $$[QT_INSTALL_EXAMPLES]/corelib/threads/mandelbrot
INSTALLS += target

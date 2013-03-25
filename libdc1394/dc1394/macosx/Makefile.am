pkgmacosxincludedir = $(pkgincludedir)/macosx

if HAVE_MACOSX
noinst_LTLIBRARIES = libdc1394-macosx.la

pkgmacosxinclude_HEADERS =
endif

AM_CFLAGS = -I..
libdc1394_macosx_la_LDFLAGS = -framework CoreFoundation
libdc1394_macosx_la_LIBADD = -lIOKit
libdc1394_macosx_la_SOURCES =  \
	control.c \
	capture.c \
	macosx.h



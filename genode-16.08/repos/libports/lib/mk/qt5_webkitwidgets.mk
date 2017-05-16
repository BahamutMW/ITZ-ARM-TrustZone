include $(REP_DIR)/lib/import/import-qt5_webkitwidgets.mk

SHARED_LIB = yes

# use default warning level to avoid noise when compiling contrib code
CC_WARN =

include $(REP_DIR)/lib/mk/qt5_webkitwidgets_generated.inc

QT_INCPATH += qtwebkit/Source/WebCore/generated

QT_VPATH += qtwebkit/Source/WebKit/qt/Api \
  
# remove unneeded files to prevent moc warnings
COMPILER_MOC_HEADER_MAKE_ALL_FILES_FILTER_OUT = \
  moc_DefaultFullScreenVideoHandler.cpp \

COMPILER_MOC_SOURCE_MAKE_ALL_FILES_FILTER_OUT = \

include $(REP_DIR)/lib/mk/qt5.inc

LIBS += qt5_webkit qt5_widgets qt5_printsupport qt5_core icu

APPNAME = CQSVGParser

include($$(MAKE_DIR)/qt_app.mk)

QT += svg xml

SOURCES += \
main.cpp \
CQSVGParser.cpp \

HEADERS += \
CQSVGParser.h \

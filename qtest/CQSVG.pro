TEMPLATE = app

QT += widgets

TARGET = CQSVG

DEPENDPATH += .

QMAKE_CXXFLAGS += -std=c++14

MOC_DIR = .moc

CONFIG += debug
CONFIG += silent

# Input
SOURCES += \
CQSVGMain.cpp \

HEADERS += \

DESTDIR     = ../bin
OBJECTS_DIR = ../obj
LIB_DIR     = ../lib

INCLUDEPATH += \
. \
../qinclude \
../include \
../../CSVG/include \
../../CQApp/include \
../../CQUtil/include \
../../CQPropertyTree/include \
../../CFont/include \
../../CImageLib/include \
../../CReadLine/include \
../../CFileUtil/include \
../../CFile/include \
../../COS/include \
../../CUtil/include \
../../CMath/include \
../../CStrUtil/include \
../../CRegExp/include \
../../CRGBName/include \
../../CCSS/include \
../../CXML/include \
../../CJavaScript/include \

unix:LIBS += \
-L$$LIB_DIR \
-L../../CQUtil/lib \
-L../../CQPropertyTree/lib \
-L../../CCSS/lib \
-L../../CXML/lib \
-L../../CJavaScript/lib \
-L../../CJson/lib \
-L../../CReadLine/lib \
-L../../CFont/lib \
-L../../CUtil/lib \
-L../../CImageLib/lib \
-L../../CConfig/lib \
-L../../CFileUtil/lib \
-L../../CFile/lib \
-L../../CMath/lib \
-L../../COS/lib \
-L../../CStrUtil/lib \
-L../../CRegExp/lib \
-L../../CRGBName/lib \
-lCQSVG -lCSVG -lCQPropertyTree -lCQUtil \
-lCCSS -lCXML -lCJavaScript -lCJson \
-lCReadLine -lCConfig -lCUtil -lCFont -lCImageLib \
-lCRGBName -lCFileUtil -lCFile -lCMath -lCOS -lCStrUtil -lCRegExp \
-lpng -ljpeg -ltre -lreadline

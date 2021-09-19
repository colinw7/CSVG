TEMPLATE = app

QT += widgets

TARGET = CQInkscape

DEPENDPATH += .

QMAKE_CXXFLAGS += -std=c++14

MOC_DIR = .moc

CONFIG += debug
CONFIG += silent

SOURCES += \
CQInkscapeCanvas.cpp \
CQInkscapeColorBar.cpp \
CQInkscapeConsole.cpp \
CQInkscapeFillStroke.cpp \
CQInkscapeMain.cpp \
CQInkscapeModeToolBar.cpp \
CQInkscapeMouseToolBar.cpp \
CQInkscapeObjectProperties.cpp \
CQInkscapePaletteArea.cpp \
CQInkscapePalette.cpp \
CQInkscapeSettings.cpp \
CQInkscapeStatusBar.cpp \
CQInkscapeTclCmd.cpp \
CQInkscapeWindow.cpp \
\
CQCommand.cpp \
CQLineCap.cpp \
CQLineJoin.cpp \
CQRubberBand.cpp \
CQTclCmd.cpp \
CQTclCommand.cpp \
CQTclUtil.cpp \
CTclParse.cpp \

HEADERS += \
CQInkscapeCanvas.h \
CQInkscapeColorBar.h \
CQInkscapeConsole.h \
CQInkscapeFillStroke.h \
CQInkscapeModeToolBar.h \
CQInkscapeMouseToolBar.h \
CQInkscapeObjectProperties.h \
CQInkscapePaletteArea.h \
CQInkscapePalette.h \
CQInkscapeSettings.h \
CQInkscapeStatusBar.h \
CQInkscapeTclCmd.h \
CQInkscapeWindow.h \
\
CQCommand.h \
CQLineCap.h \
CQLineJoin.h \
CQRubberBand.h \
CQTclCmd.h \
CQTclCommand.h \
CQTclUtil.h \
CTclParse.h \
CTclUtil.h \

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
/usr/include/tcl \

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
-lpng -ljpeg -ltre -lreadline -ltcl

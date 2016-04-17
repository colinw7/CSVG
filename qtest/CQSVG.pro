TEMPLATE = app

QT += widgets

TARGET = CQSVG

DEPENDPATH += .

QMAKE_CXXFLAGS += -std=c++11

MOC_DIR = .moc

CONFIG += debug

# Input
SOURCES += \
CQSVGAnchor.cpp \
CQSVGBlock.cpp \
CQSVGBufferView.cpp \
CQSVGCanvas.cpp \
CQSVGCircle.cpp \
CQSVG.cpp \
CQSVGDefs.cpp \
CQSVGDesc.cpp \
CQSVGEllipse.cpp \
CQSVGEnum.cpp \
CQSVGFeBlend.cpp \
CQSVGFeColorMatrix.cpp \
CQSVGFeComposite.cpp \
CQSVGFeDiffuseLighting.cpp \
CQSVGFeDisplacementMap.cpp \
CQSVGFeDistantLight.cpp \
CQSVGFeFlood.cpp \
CQSVGFeGaussianBlur.cpp \
CQSVGFeImage.cpp \
CQSVGFeMerge.cpp \
CQSVGFeMergeNode.cpp \
CQSVGFeMorphology.cpp \
CQSVGFeOffset.cpp \
CQSVGFePointLight.cpp \
CQSVGFeSpecularLighting.cpp \
CQSVGFeSpotLight.cpp \
CQSVGFeTile.cpp \
CQSVGFeTurbulence.cpp \
CQSVGFilter.cpp \
CQSVGGroup.cpp \
CQSVGImage.cpp \
CQSVGLinearGradient.cpp \
CQSVGLine.cpp \
CQSVGMarker.cpp \
CQSVGMask.cpp \
CQSVGMPath.cpp \
CQSVGObject.cpp \
CQSVGPath.cpp \
CQSVGPattern.cpp \
CQSVGPolygon.cpp \
CQSVGPolyLine.cpp \
CQSVGPropertiesDlg.cpp \
CQSVGRadialGradient.cpp \
CQSVGRect.cpp \
CQSVGSet.cpp \
CQSVGStop.cpp \
CQSVGSymbol.cpp \
CQSVGText.cpp \
CQSVGTextPath.cpp \
CQSVGTitle.cpp \
CQSVGTSpan.cpp \
CQSVGUse.cpp \
CQSVGWindow.cpp \
\
CQSVGRenderer.cpp \
\
CMathGeom2D.cpp \
CArcToBezier.cpp \

HEADERS += \
CEnumConv.h \
CQSVGAnchor.h \
CQSVGBlock.h \
CQSVGBufferView.h \
CQSVGCanvas.h \
CQSVGCircle.h \
CQSVGDefs.h \
CQSVGDesc.h \
CQSVGEllipse.h \
CQSVGEnum.h \
CQSVGFeBlend.h \
CQSVGFeColorMatrix.h \
CQSVGFeComposite.h \
CQSVGFeDiffuseLighting.h \
CQSVGFeDisplacementMap.h \
CQSVGFeDistantLight.h \
CQSVGFeFlood.h \
CQSVGFeGaussianBlur.h \
CQSVGFeImage.h \
CQSVGFeMerge.h \
CQSVGFeMergeNode.h \
CQSVGFeMorphology.h \
CQSVGFeOffset.h \
CQSVGFePointLight.h \
CQSVGFeSpecularLighting.h \
CQSVGFeSpotLight.h \
CQSVGFeTile.h \
CQSVGFeTurbulence.h \
CQSVGFilter.h \
CQSVGGroup.h \
CQSVG.h \
CQSVGImage.h \
CQSVGLinearGradient.h \
CQSVGLine.h \
CQSVGMarker.h \
CQSVGMask.h \
CQSVGMPath.h \
CQSVGObject.h \
CQSVGPath.h \
CQSVGPattern.h \
CQSVGPolygon.h \
CQSVGPolyLine.h \
CQSVGPropertiesDlg.h \
CQSVGRadialGradient.h \
CQSVGRect.h \
CQSVGSet.h \
CQSVGStop.h \
CQSVGSymbol.h \
CQSVGText.h \
CQSVGTextPath.h \
CQSVGTitle.h \
CQSVGTSpan.h \
CQSVGUse.h \
CQSVGWindow.h \
\
CQSVGRenderer.h \
\

DESTDIR     = ../bin
OBJECTS_DIR = ../obj
LIB_DIR     = ../lib

INCLUDEPATH += \
. \
../include \
../../CSVG/include \
../../CQApp/include \
../../CQUtil/include \
../../CQPropertyTree/include \
../../CFont/include \
../../CImageLib/include \
../../CReadLine/include \
../../CFile/include \
../../COS/include \
../../CUtil/include \
../../CMath/include \
../../CStrUtil/include \
../../CRGBName/include \
../../CXML/include \

unix:LIBS += \
-L$$LIB_DIR \
-L../../CQUtil/lib \
-L../../CQPropertyTree/lib \
-L../../CXML/lib \
-L../../CReadLine/lib \
-L../../CFont/lib \
-L../../CUtil/lib \
-L../../CImageLib/lib \
-L../../CConfig/lib \
-L../../CFile/lib \
-L../../COS/lib \
-L../../CStrUtil/lib \
-L../../CRegExp/lib \
-L../../CRGBName/lib \
-lCSVG -lCQPropertyTree -lCQUtil \
-lCXML -lCReadLine -lCUtil -lCFont -lCImageLib -lCConfig \
-lCRGBName -lCFile -lCOS -lCStrUtil -lCRegExp \
-lpng -ljpeg -ltre -lreadline
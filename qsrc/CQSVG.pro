TEMPLATE = lib

TARGET = CQSVG

QT += widgets

DEPENDPATH += .

CONFIG += staticlib

QMAKE_CXXFLAGS += -std=c++14

MOC_DIR = .moc

CONFIG += debug
CONFIG += silent

# Input
SOURCES += \
CQSVG.cpp \
CQSVGAnchor.cpp \
CQSVGAnimate.cpp \
CQSVGAnimateBase.cpp \
CQSVGAnimateColor.cpp \
CQSVGAnimateMotion.cpp \
CQSVGAnimateTransform.cpp \
CQSVGBlock.cpp \
CQSVGBufferView.cpp \
CQSVGCanvas.cpp \
CQSVGCircle.cpp \
CQSVGClipPath.cpp \
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
CQSVGJSDialog.cpp \
CQSVGLinearGradient.cpp \
CQSVGLine.cpp \
CQSVGMarker.cpp \
CQSVGMask.cpp \
CQSVGMPath.cpp \
CQSVGObject.cpp \
CQSVGPath.cpp \
CQSVGPathPart.cpp \
CQSVGPattern.cpp \
CQSVGPolygon.cpp \
CQSVGPolyLine.cpp \
CQSVGPropertiesDlg.cpp \
CQSVGRadialGradient.cpp \
CQSVGRect.cpp \
CQSVGSet.cpp \
CQSVGStop.cpp \
CQSVGStyleTree.cpp \
CQSVGSymbol.cpp \
CQSVGText.cpp \
CQSVGTextPath.cpp \
CQSVGTitle.cpp \
CQSVGTSpan.cpp \
CQSVGUse.cpp \
CQSVGWindow.cpp \
\
CQSVGRenderer.cpp \
CQSVGImageData.cpp \
CQSVGFontObj.cpp \
CQSVGUtil.cpp \
\
CQHistoryLineEdit.cpp \

HEADERS += \
../qinclude/CEnumConv.h \
../qinclude/CQSVGAnchor.h \
../qinclude/CQSVGAnimate.h \
../qinclude/CQSVGAnimateBase.h \
../qinclude/CQSVGAnimateColor.h \
../qinclude/CQSVGAnimateMotion.h \
../qinclude/CQSVGAnimateTransform.h \
../qinclude/CQSVGBlock.h \
../qinclude/CQSVGBufferView.h \
../qinclude/CQSVGCanvas.h \
../qinclude/CQSVGCircle.h \
../qinclude/CQSVGClipPath.h \
../qinclude/CQSVGDefs.h \
../qinclude/CQSVGDesc.h \
../qinclude/CQSVGEllipse.h \
../qinclude/CQSVGEnum.h \
../qinclude/CQSVGFeBlend.h \
../qinclude/CQSVGFeColorMatrix.h \
../qinclude/CQSVGFeComposite.h \
../qinclude/CQSVGFeDiffuseLighting.h \
../qinclude/CQSVGFeDisplacementMap.h \
../qinclude/CQSVGFeDistantLight.h \
../qinclude/CQSVGFeFlood.h \
../qinclude/CQSVGFeGaussianBlur.h \
../qinclude/CQSVGFeImage.h \
../qinclude/CQSVGFeMerge.h \
../qinclude/CQSVGFeMergeNode.h \
../qinclude/CQSVGFeMorphology.h \
../qinclude/CQSVGFeOffset.h \
../qinclude/CQSVGFePointLight.h \
../qinclude/CQSVGFeSpecularLighting.h \
../qinclude/CQSVGFeSpotLight.h \
../qinclude/CQSVGFeTile.h \
../qinclude/CQSVGFeTurbulence.h \
../qinclude/CQSVGFilter.h \
../qinclude/CQSVGGroup.h \
../qinclude/CQSVG.h \
../qinclude/CQSVGImage.h \
../qinclude/CQSVGJSDialog.h \
../qinclude/CQSVGLinearGradient.h \
../qinclude/CQSVGLine.h \
../qinclude/CQSVGMarker.h \
../qinclude/CQSVGMask.h \
../qinclude/CQSVGMPath.h \
../qinclude/CQSVGObject.h \
../qinclude/CQSVGPath.h \
../qinclude/CQSVGPathPart.h \
../qinclude/CQSVGPattern.h \
../qinclude/CQSVGPolygon.h \
../qinclude/CQSVGPolyLine.h \
../qinclude/CQSVGPropertiesDlg.h \
../qinclude/CQSVGRadialGradient.h \
../qinclude/CQSVGRect.h \
../qinclude/CQSVGSet.h \
../qinclude/CQSVGStop.h \
../qinclude/CQSVGStyleTree.h \
../qinclude/CQSVGSymbol.h \
../qinclude/CQSVGText.h \
../qinclude/CQSVGTextPath.h \
../qinclude/CQSVGTitle.h \
../qinclude/CQSVGTSpan.h \
../qinclude/CQSVGUse.h \
../qinclude/CQSVGWindow.h \
\
../qinclude/CQSVGRenderer.h \
../qinclude/CQSVGImageData.h \
../qinclude/CQSVGFontObj.h \
../qinclude/CQSVGUtil.h \
\
CQHistoryLineEdit.h \

DESTDIR     = ../lib
OBJECTS_DIR = ../obj

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

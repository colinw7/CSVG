CC = g++
AR = ar
RM = rm

CDEBUG = -g

INC_DIR = ../include
OBJ_DIR = ../obj
LIB_DIR = ../lib

all: dirs $(LIB_DIR)/libCSVG.a

dirs:
	@if [ ! -e ../obj ]; then mkdir ../obj; fi
	@if [ ! -e ../lib ]; then mkdir ../lib; fi
	@if [ ! -e ../bin ]; then mkdir ../bin; fi

ifeq ($(CSVG_JAVASCRIPT),1)
JAVASCRIPT_SRC = \
CSVGJavaScript.cpp \
CSVGJAnimated.cpp \
CSVGJDocument.cpp \
CSVGJElement.cpp \
CSVGJEvent.cpp \
CSVGJTypes.cpp \

JAVASCRIPT_CFLAGS = \
-I../../CJavaScript/include \
-DCSVG_JAVASCRIPT
else
JAVASCRIPT_SRC =

JAVASCRIPT_CFLAGS =
endif

SRC = \
CSVG.cpp \
CSVGAnchor.cpp \
CSVGAnimate.cpp \
CSVGAnimateBase.cpp \
CSVGAnimateColor.cpp \
CSVGAnimateMotion.cpp \
CSVGAnimateTransform.cpp \
CSVGAnimation.cpp \
CSVGAudio.cpp \
CSVGBlock.cpp \
CSVGBuffer.cpp \
CSVGCircle.cpp \
CSVGClip.cpp \
CSVGClipPath.cpp \
CSVGColorProfile.cpp \
CSVGDefs.cpp \
CSVGDesc.cpp \
CSVGEllipse.cpp \
CSVGFeBlend.cpp \
CSVGFeColorMatrix.cpp \
CSVGFeComponentTransfer.cpp \
CSVGFeComposite.cpp \
CSVGFeConvolveMatrix.cpp \
CSVGFeDiffuseLighting.cpp \
CSVGFeDisplacementMap.cpp \
CSVGFeDistantLight.cpp \
CSVGFeFlood.cpp \
CSVGFeFunc.cpp \
CSVGFeGaussianBlur.cpp \
CSVGFeImage.cpp \
CSVGFeLighting.cpp \
CSVGFeMerge.cpp \
CSVGFeMergeNode.cpp \
CSVGFeMorphology.cpp \
CSVGFeOffset.cpp \
CSVGFePointLight.cpp \
CSVGFeSpecularLighting.cpp \
CSVGFeSpotLight.cpp \
CSVGFeTile.cpp \
CSVGFeTurbulence.cpp \
CSVGFill.cpp \
CSVGFilter.cpp \
CSVGFilterBase.cpp \
CSVGFont.cpp \
CSVGFontDef.cpp \
CSVGFontFace.cpp \
CSVGFontFaceSrc.cpp \
CSVGFontFaceUri.cpp \
CSVGFontObj.cpp \
CSVGGlyph.cpp \
CSVGGroup.cpp \
CSVGHKern.cpp \
CSVGImage.cpp \
CSVGImageData.cpp \
CSVGLine.cpp \
CSVGLinearGradient.cpp \
CSVGMarker.cpp \
CSVGMask.cpp \
CSVGMetaData.cpp \
CSVGMissingGlyph.cpp \
CSVGMPath.cpp \
CSVGObject.cpp \
CSVGPath.cpp \
CSVGPathPart.cpp \
CSVGPattern.cpp \
CSVGPolygon.cpp \
CSVGPolyLine.cpp \
CSVGRadialGradient.cpp \
CSVGRect.cpp \
CSVGScript.cpp \
CSVGSet.cpp \
CSVGStop.cpp \
CSVGStroke.cpp \
CSVGStrokeDash.cpp \
CSVGStyle.cpp \
CSVGStyleData.cpp \
CSVGSwitch.cpp \
CSVGSymbol.cpp \
CSVGTBreak.cpp \
CSVGText.cpp \
CSVGTextPath.cpp \
CSVGTitle.cpp \
CSVGTSpan.cpp \
CSVGUse.cpp \
CSVGVisitor.cpp \
CSVGWrite.cpp \
CSVGXLink.cpp \
\
CSVGUtil.cpp \
\
$(JAVASCRIPT_SRC) \
\
CSVGRenderer.cpp \
CSVGImageRenderer.cpp \
CSVGLogRenderer.cpp \
\
CLog.cpp \

OBJS = $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SRC))

CPPFLAGS = \
-std=c++17 \
$(CDEBUG) \
-I. \
-I$(INC_DIR) \
-I../../CXML/include \
-I../../CRenderer/include \
-I../../CCSS/include \
-I../../CConfig/include \
-I../../CImageLib/include \
-I../../CFileUtil/include \
-I../../CFile/include \
-I../../CFont/include \
-I../../CRegExp/include \
-I../../CStrUtil/include \
-I../../COS/include \
-I../../CMath/include \
-I../../CUtil/include \
-I../../CRGBName/include \
\
$(JAVASCRIPT_CFLAGS) \

clean:
	$(RM) -f $(OBJ_DIR)/*.o
	$(RM) -f $(LIB_DIR)/libCSVG.a

.SUFFIXES: .cpp

$(OBJS): $(OBJ_DIR)/%.o: %.cpp
	$(CC) -c $< -o $(OBJ_DIR)/$*.o $(CPPFLAGS)

$(LIB_DIR)/libCSVG.a: $(OBJS)
	$(AR) crv $(LIB_DIR)/libCSVG.a $(OBJS)

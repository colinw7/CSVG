CC = g++
AR = ar
RM = rm

CDEBUG = -g

INC_DIR = ../include
OBJ_DIR = ../obj
LIB_DIR = ../lib
BIN_DIR = ../bin

all: dirs $(LIB_DIR)/libCSVG.a

dirs:
	@if [ ! -e ../obj ]; then mkdir ../obj; fi
	@if [ ! -e ../lib ]; then mkdir ../lib; fi

SRC = \
CSVGAnchor.cpp \
CSVGAnimateBase.cpp \
CSVGAnimateColor.cpp \
CSVGAnimate.cpp \
CSVGAnimateMotion.cpp \
CSVGAnimateTransform.cpp \
CSVGAnimation.cpp \
CSVGBlock.cpp \
CSVGBuffer.cpp \
CSVGCircle.cpp \
CSVGClip.cpp \
CSVGClipPath.cpp \
CSVGColorProfile.cpp \
CSVG.cpp \
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
CSVGFilterBase.cpp \
CSVGFilter.cpp \
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
CSVGFeLighting.cpp \
CSVGLinearGradient.cpp \
CSVGLine.cpp \
CSVGMarker.cpp \
CSVGMask.cpp \
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
CSVGRenderer.cpp \
CSVGScript.cpp \
CSVGSet.cpp \
CSVGStop.cpp \
CSVGStroke.cpp \
CSVGStrokeDash.cpp \
CSVGStyle.cpp \
CSVGStyleData.cpp \
CSVGSwitch.cpp \
CSVGSymbol.cpp \
CSVGText.cpp \
CSVGTextPath.cpp \
CSVGTitle.cpp \
CSVGTSpan.cpp \
CSVGUse.cpp \
CSVGUtil.cpp \
CSVGXLink.cpp \
\
CSVGJavaScript.cpp \
CSVGJDocument.cpp \
CSVGJEvent.cpp \
CSVGJObject.cpp \
CSVGJTransform.cpp \
\
CLog.cpp \
CEncode64.cpp \
CMathGeom2D.cpp \
CArcToBezier.cpp \

OBJS = $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SRC))

CPPFLAGS = \
-std=c++11 \
$(CDEBUG) \
-I. \
-I$(INC_DIR) \
-I../../CXML/include \
-I../../CJavaScript/include \
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

clean:
	$(RM) -f $(OBJ_DIR)/*.o
	$(RM) -f $(LIB_DIR)/libCSVG.a

.SUFFIXES: .cpp

$(OBJS): $(OBJ_DIR)/%.o: %.cpp
	$(CC) -c $< -o $(OBJ_DIR)/$*.o $(CPPFLAGS)

$(LIB_DIR)/libCSVG.a: $(OBJS)
	$(AR) crv $(LIB_DIR)/libCSVG.a $(OBJS)

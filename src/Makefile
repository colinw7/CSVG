CC = g++
AR = ar
RM = rm

CDEBUG = -g

INC_DIR = ../include
OBJ_DIR = ../obj
LIB_DIR = ../lib
BIN_DIR = ../bin

all: $(LIB_DIR)/libCSVG.a

SRC = \
CSVGBlock.cpp \
CSVGBuffer.cpp \
CSVGCircle.cpp \
CSVGClip.cpp \
CSVGClipPath.cpp \
CSVG.cpp \
CSVGDefs.cpp \
CSVGDesc.cpp \
CSVGEllipse.cpp \
CSVGFeBlend.cpp \
CSVGFeColorMatrix.cpp \
CSVGFeComponentTransfer.cpp \
CSVGFeComposite.cpp \
CSVGFeDiffuseLighting.cpp \
CSVGFeDistantLight.cpp \
CSVGFeFlood.cpp \
CSVGFeFunc.cpp \
CSVGFeGaussianBlur.cpp \
CSVGFeImage.cpp \
CSVGFeMerge.cpp \
CSVGFeMergeNode.cpp \
CSVGFeOffset.cpp \
CSVGFePointLight.cpp \
CSVGFeSpecularLighting.cpp \
CSVGFeTile.cpp \
CSVGFeTurbulence.cpp \
CSVGFill.cpp \
CSVGFilter.cpp \
CSVGFont.cpp \
CSVGFontDef.cpp \
CSVGFontFace.cpp \
CSVGGlyph.cpp \
CSVGGroup.cpp \
CSVGHKern.cpp \
CSVGImage.cpp \
CSVGLinearGradient.cpp \
CSVGLine.cpp \
CSVGMarker.cpp \
CSVGMask.cpp \
CSVGMissingGlyph.cpp \
CSVGObject.cpp \
CSVGPath.cpp \
CSVGPattern.cpp \
CSVGPolygon.cpp \
CSVGPolyLine.cpp \
CSVGRadialGradient.cpp \
CSVGRect.cpp \
CSVGStop.cpp \
CSVGStroke.cpp \
CSVGStyle.cpp \
CSVGSymbol.cpp \
CSVGText.cpp \
CSVGTitle.cpp \
CSVGTSpan.cpp \
CSVGUse.cpp \

OBJS = $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SRC))

CPPFLAGS = \
$(CDEBUG) \
-I$(INC_DIR) \
-I../../CXML/include \
-I../../CConfig/include \
-I../../CImageLib/include \
-I../../CFile/include \
-I../../CFont/include \
-I../../CRegExp/include \
-I../../CStrUtil/include \
-I../../COS/include \
-I../../CMath/include \
-I../../CUtil/include \
-I. \

clean:
	$(RM) -f $(OBJ_DIR)/*.o
	$(RM) -f $(LIB_DIR)/libCSVG.a

.SUFFIXES: .cpp

$(OBJS): $(OBJ_DIR)/%.o: %.cpp
	$(CC) -c $< -o $(OBJ_DIR)/$*.o $(CPPFLAGS)

$(LIB_DIR)/libCSVG.a: $(OBJS)
	$(AR) crv $(LIB_DIR)/libCSVG.a $(OBJS)

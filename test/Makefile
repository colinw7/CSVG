CC = g++
RM = rm

CDEBUG = -g
LDEBUG = -g

INC_DIR = ../include
OBJ_DIR = .
LIB_DIR = ../lib
BIN_DIR = ../bin

all: dirs $(BIN_DIR)/CSVGTest

dirs:
	@if [ ! -e ../bin ]; then mkdir ../bin; fi

SRC = \
CSVGTest.cpp \
CImageRenderer2D.cpp \
CRenderer2D.cpp \
CRendererRegion2D.cpp \
CPath2D.cpp \
CImagePixelRenderer.cpp \
CPixelRenderer.cpp \
CPixelClip.cpp \
CFreeType.cpp \
CSymbol2D.cpp \

OBJS = $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SRC))

CPPFLAGS = \
$(CDEBUG) \
-std=c++11 \
-I. \
-I$(INC_DIR) \
-I../../CConfig/include \
-I../../CImageLib/include \
-I../../CFont/include \
-I../../CFile/include \
-I../../CStrUtil/include \
-I../../CUtil/include \
-I../../CMath/include \
-I/usr/include/freetype2 \

LFLAGS = \
$(LDEBUG) \
-L$(LIB_DIR) \
-L../../CSVG/lib \
-L../../CXML/lib \
-L../../CImageLib/lib \
-L../../CRGBName/lib \
-L../../CFont/lib \
-L../../CConfig/lib \
-L../../CFile/lib \
-L../../COS/lib \
-L../../CRegExp/lib \
-L../../CStrUtil/lib \
-L../../CUtil/lib \

LIBS = \
-lCSVG -lCXML -lCImageLib -lCFont -lCConfig -lCFile \
-lCUtil -lCOS -lCRGBName -lCRegExp -lCStrUtil \
-ltre -lpng -ljpeg -lfreetype

clean:
	$(RM) -f $(OBJ_DIR)/*.o
	$(RM) -f $(BIN_DIR)/CSVGTest

.SUFFIXES: .cpp

.cpp.o:
	$(CC) -c $< -o $(OBJ_DIR)/$*.o $(CPPFLAGS)

$(BIN_DIR)/CSVGTest: $(OBJS) $(LIB_DIR)/libCSVG.a
	$(CC) $(LDEBUG) -o $(BIN_DIR)/CSVGTest $(OBJS) $(LFLAGS) $(LIBS)

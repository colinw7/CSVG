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

CSVG_JAVASCRIPT = 0

SRC = \
CSVGTest.cpp \

ifeq ($(CSVG_JAVASCRIPT),1)
JAVASCRIPT_CFLAGS = \
-I../../include/CJavaScript \
-DCSVG_JAVASCRIPT

JAVASCRIPT_LFLAGS = \
-L../../CJson/lib \
-L../../CJavaScript/lib \

JAVASCRIPT_LIBS = \
-lCJavaScript -lCJson
else
JAVASCRIPT_CFLAGS =

JAVASCRIPT_LFLAGS =

JAVASCRIPT_LIBS =
endif

OBJS = $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SRC))

CPPFLAGS = \
$(CDEBUG) \
-std=c++14 \
-I. \
-I$(INC_DIR) \
-I../../CCSS/include \
-I../../CConfig/include \
-I../../CRenderer/include \
-I../../CImageLib/include \
-I../../CFont/include \
-I../../CFileUtil/include \
-I../../CFile/include \
-I../../CStrUtil/include \
-I../../CUtil/include \
-I../../CMath/include \
-I../../COS/include \
-I/usr/include/freetype2 \
\
$(JAVASCRIPT_CFLAGS) \

LFLAGS = \
$(LDEBUG) \
-L$(LIB_DIR) \
-L../../CSVG/lib \
-L../../CCSS/lib \
-L../../CXML/lib \
-L../../CRenderer/lib \
-L../../CImageLib/lib \
-L../../CRGBName/lib \
-L../../CFont/lib \
-L../../CConfig/lib \
-L../../CFileUtil/lib \
-L../../CFile/lib \
-L../../CMath/lib \
-L../../COS/lib \
-L../../CRegExp/lib \
-L../../CStrUtil/lib \
-L../../CUtil/lib \
\
$(JAVASCRIPT_LFLAGS) \

LIBS = \
-lCSVG -lCCSS -lCXML $(JAVASCRIPT_LIBS) \
-lCRenderer -lCImageLib -lCFont -lCConfig -lCFileUtil -lCFile \
-lCUtil -lCMath -lCOS -lCRGBName -lCRegExp -lCStrUtil \
-ltre -lpng -ljpeg -lfreetype

clean:
	$(RM) -f $(OBJ_DIR)/*.o
	$(RM) -f $(BIN_DIR)/CSVGTest

.SUFFIXES: .cpp

.cpp.o:
	$(CC) -c $< -o $(OBJ_DIR)/$*.o $(CPPFLAGS)

$(BIN_DIR)/CSVGTest: $(OBJS) $(LIB_DIR)/libCSVG.a
	$(CC) $(LDEBUG) -o $(BIN_DIR)/CSVGTest $(OBJS) $(LFLAGS) $(LIBS)

#ifndef CSYMBOL_2D_H
#define CSYMBOL_2D_H

#include <vector>

class CSymbol2DRenderer;

enum CSymbolType {
  CSYMBOL_NONE,
  CSYMBOL_CROSS,
  CSYMBOL_PLUS,
  CSYMBOL_Y,
  CSYMBOL_TRIANGLE,
  CSYMBOL_ITRIANGLE,
  CSYMBOL_BOX,
  CSYMBOL_DIAMOND,
  CSYMBOL_STAR,
  CSYMBOL_STAR1,
  CSYMBOL_PENTAGON
};

struct CSymbol2D {
  enum Connect {
    NONE,
    LINE,
    CLOSE,
    STROKE
  };

  struct Line {
    double  x1, y1;
    double  x2, y2;
    Connect connect;

    Line(double x11, double y11, double x21, double y21, Connect connect1=NONE) :
     x1(x11), y1(y11), x2(x21), y2(y21), connect(connect1) {
    }
  };

  CSymbolType       type;
  std::vector<Line> lines;

  CSymbol2D(CSymbolType type1, std::initializer_list<Line> lines1) :
   type(type1), lines(lines1) {
  }
};

namespace CSymbol2DMgr {
  bool isSymbol(CSymbolType type);

  const CSymbol2D &getSymbol(CSymbolType type);

  void drawSymbol(CSymbolType type, CSymbol2DRenderer *renderer);
}

class CSymbol2DRenderer {
 public:
  CSymbol2DRenderer() { }

  virtual void moveTo(double x, double y) = 0;
  virtual void lineTo(double x, double y) = 0;

  virtual void closePath() = 0;

  virtual void stroke() = 0;

  void drawSymbol(CSymbolType type);
};

#endif

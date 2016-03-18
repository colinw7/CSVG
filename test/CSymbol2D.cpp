#include <CSymbol2D.h>
#include <cassert>

class CSymbol2DList {
 public:
  typedef std::vector<CSymbol2D> Symbols;

 public:
  CSymbol2DList(std::initializer_list<CSymbol2D> symbols) :
   symbols_(symbols) {
  }

  const Symbols &symbols() const { return symbols_; }

  bool isSymbol(CSymbolType type) const {
    for (const auto &s : symbols_)
      if (s.type == type)
        return true;

    return false;
  }

  const CSymbol2D &getSymbol(CSymbolType type) const {
    for (const auto &s : symbols_)
      if (s.type == type)
        return s;

    assert(false);

    return symbols_[0];
  }

  void drawSymbol(CSymbolType type, CSymbol2DRenderer *renderer) const {
    const CSymbol2D &s = getSymbol(type);

    bool connect = false;

    for (const auto &l : s.lines) {
      if (! connect)
        renderer->moveTo(l.x1, l.y1);
      else
        renderer->lineTo(l.x1, l.y1);

      if      (l.connect == CSymbol2D::CLOSE) {
        renderer->closePath();

        renderer->stroke();

        connect = false;
      }
      else if (l.connect == CSymbol2D::STROKE) {
        renderer->lineTo(l.x2, l.y2);

        renderer->stroke();

        connect = false;
      }
      else
        connect = true;
    }
  }

 private:
  Symbols symbols_;
};

CSymbol2DList symbols({
  { CSYMBOL_CROSS    , {{-1.0, -1.0,  1.0,  1.0, CSymbol2D::STROKE},
                        {-1.0,  1.0,  1.0, -1.0, CSymbol2D::STROKE}} },
  { CSYMBOL_PLUS     , {{ 0.0, -1.0,  0.0,  1.0, CSymbol2D::STROKE},
                        {-1.0,  0.0,  1.0,  0.0, CSymbol2D::STROKE}} },
  { CSYMBOL_Y        , {{ 0.0,  0.0,  0.0, -1.0, CSymbol2D::STROKE},
                        { 0.0,  0.0,  1.0,  1.0, CSymbol2D::STROKE},
                        { 0.0,  0.0, -1.0,  1.0, CSymbol2D::STROKE}} },
  { CSYMBOL_TRIANGLE , {{ 0.0,  1.0, -1.0, -1.0, CSymbol2D::LINE},
                        {-1.0, -1.0,  1.0, -1.0, CSymbol2D::LINE},
                        { 1.0, -1.0,  0.0,  1.0, CSymbol2D::CLOSE},
                        { 0.0,  0.0,  0.0,  1.0, CSymbol2D::STROKE}} },
  { CSYMBOL_ITRIANGLE, {{ 0.0, -1.0, -1.0,  1.0, CSymbol2D::LINE},
                        {-1.0,  1.0,  1.0,  1.0, CSymbol2D::LINE},
                        { 1.0,  1.0,  0.0, -1.0, CSymbol2D::CLOSE},
                        { 0.0,  0.0,  0.0, -1.0, CSymbol2D::STROKE}} },
  { CSYMBOL_BOX      , {{-1.0,  1.0,  1.0,  1.0, CSymbol2D::LINE},
                        { 1.0,  1.0,  1.0, -1.0, CSymbol2D::LINE},
                        { 1.0, -1.0, -1.0, -1.0, CSymbol2D::LINE},
                        {-1.0, -1.0, -1.0,  1.0, CSymbol2D::CLOSE},
                        { 0.0,  0.0,  0.0,  1.0, CSymbol2D::STROKE}} },
  { CSYMBOL_DIAMOND  , {{-1.0,  0.0,  0.0,  1.0, CSymbol2D::LINE},
                        { 0.0,  1.0,  1.0,  0.0, CSymbol2D::LINE},
                        { 1.0,  0.0,  0.0, -1.0, CSymbol2D::LINE},
                        { 0.0, -1.0, -1.0,  0.0, CSymbol2D::CLOSE},
                        { 0.0,  0.0,  0.0,  1.0, CSymbol2D::STROKE}} },
  { CSYMBOL_STAR     , {{ 0.0,  0.0,  0.0,  1.0, CSymbol2D::STROKE},
                        { 0.0,  0.0, -1.0,  0.0, CSymbol2D::STROKE},
                        { 0.0,  0.0,  1.0,  0.0, CSymbol2D::STROKE},
                        { 0.0,  0.0, -1.0, -1.0, CSymbol2D::STROKE},
                        { 0.0,  0.0,  1.0, -1.0, CSymbol2D::STROKE}} },
  { CSYMBOL_STAR1    , {{-1.0,  0.0,  1.0,  0.0, CSymbol2D::STROKE},
                        { 0.0, -1.0,  0.0,  1.0, CSymbol2D::STROKE},
                        {-1.0, -1.0,  1.0,  1.0, CSymbol2D::STROKE},
                        {-1.0,  1.0,  1.0, -1.0, CSymbol2D::STROKE}} },
  { CSYMBOL_PENTAGON , {{  0.000000, -1.000000,  0.951057, -0.309017, CSymbol2D::LINE},
                        {  0.951057, -0.309017,  0.587785,  0.809017, CSymbol2D::LINE},
                        {  0.587785,  0.809017, -0.587785,  0.809017, CSymbol2D::LINE},
                        { -0.587785,  0.809017, -0.951057, -0.309017, CSymbol2D::LINE},
                        { -0.951057, -0.309017,  0.000000, -1.000000, CSymbol2D::CLOSE},
                        {  0.000000, -1.000000,  0.000000,  0.000000, CSymbol2D::STROKE}} }
});

bool
CSymbol2DMgr::
isSymbol(CSymbolType type)
{
  return symbols.isSymbol(type);
}

const CSymbol2D &
CSymbol2DMgr::
getSymbol(CSymbolType type)
{
  return symbols.getSymbol(type);
}

void
CSymbol2DMgr::
drawSymbol(CSymbolType type, CSymbol2DRenderer *renderer)
{
  return symbols.drawSymbol(type, renderer);
}

void
CSymbol2DRenderer::
drawSymbol(CSymbolType type)
{
  CSymbol2DMgr::drawSymbol(type, this);
}

#include <CQSVGFontObj.h>
#include <CSVGFontDef.h>
#include <QFontMetricsF>

CQSVGFontObj::
CQSVGFontObj(const CSVGFontDef &def) :
 CSVGFontObj(def)
{
  qfont_ = QFont(def.getFamily().c_str());

  CFontStyles styles = def.getStyle();

  if (styles & CFONT_STYLE_BOLD)
    qfont_.setBold(true);

  if (styles & CFONT_STYLE_ITALIC)
    qfont_.setItalic(true);

  if (styles & CFONT_STYLE_UNDERLINE)
    qfont_.setUnderline(true);

  if (styles & CFONT_STYLE_OVERLINE)
    qfont_.setOverline(true);

  if (styles & CFONT_STYLE_STRIKEOUT)
    qfont_.setStrikeOut(true);

  initPointSize(def);
}

void
CQSVGFontObj::
textSize(const std::string &text, double *w, double *a, double *d) const
{
  QFontMetricsF fm(qfont_);

  if (w) *w = fm.width(text.c_str());
  if (a) *a = fm.ascent();
  if (d) *d = fm.descent();
}

void
CQSVGFontObj::
initPointSize(const CSVGFontDef &def)
{
  double dpi = CScreenUnitsMgrInst->dpi();

  CScreenUnitsMgrInst->setDpi(100);

  size_ = def.getSize().ptValue();

  scale_ = 1;

  double h1 = def.getSize().pxValue();

  for (int i = 0; i < 8; ++i) {
    qfont_.setPointSizeF(scale_*size_);

    QFontMetricsF fm(qfont_);

    double h2 = fm.height();

    scale_ *= h1/h2;
  }

  qfont_.setPointSizeF(scale_*size_);

  CScreenUnitsMgrInst->setDpi(dpi);
}

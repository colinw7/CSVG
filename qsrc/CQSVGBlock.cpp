#include <CQSVGBlock.h>
#include <CQSVGWindow.h>
#include <CQSVGCanvas.h>
#include <CQSVG.h>
#include <CQPropertyTree.h>

CQSVGBlock::
CQSVGBlock(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGBlock(*svg)
{
}

//---

int
CQSVGBlock::
getScreenWidth() const
{
  auto *canvas = qsvg()->window()->canvas();

  return canvas->width();
}

int
CQSVGBlock::
getScreenHeight() const
{
  auto *canvas = qsvg()->window()->canvas();

  return canvas->height();
}

//---

CQSVGEnum::HAlignType
CQSVGBlock::
halignType() const
{
  return CQSVGEnum::halignConv(getHAlign());
}

void
CQSVGBlock::
setHAlignType(const CQSVGEnum::HAlignType &h)
{
  setHAlign(CQSVGEnum::halignConv(h));
}

CQSVGEnum::VAlignType
CQSVGBlock::
valignType() const
{
  return CQSVGEnum::valignConv(getVAlign());
}

void
CQSVGBlock::
setVAlignType(const CQSVGEnum::VAlignType &v)
{
  setVAlign(CQSVGEnum::valignConv(v));
}

//---

void
CQSVGBlock::
addProperties(CQPropertyTree *propTree, const std::string &name)
{
  CQSVGObject::addProperties(propTree, name);

  QString objName = name.c_str();

  propTree->addProperty(objName, this, "x"     );
  propTree->addProperty(objName, this, "y"     );
  propTree->addProperty(objName, this, "width" );
  propTree->addProperty(objName, this, "height");

  propTree->addProperty(objName, this, "halign");
  propTree->addProperty(objName, this, "valign");
}

void
CQSVGBlock::
drawTerm()
{
  drawSelected();

  CSVGBlock::drawTerm();
}

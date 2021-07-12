#include <CQSVGText.h>
#include <CQSVG.h>
#include <CQPropertyTree.h>

CQSVGText::
CQSVGText(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGText(*svg)
{
}

double
CQSVGText::
getX() const
{
  return CSVGText::getX().px().value();
}

void
CQSVGText::
setX(double x)
{
  CSVGText::setX(x);
}

double
CQSVGText::
getY() const
{
  return CSVGText::getY().px().value();
}

void
CQSVGText::
setY(double y)
{
  CSVGText::setY(y);
}

double
CQSVGText::
getDX() const
{
  return CSVGText::getDX().px().value();
}

void
CQSVGText::
setDX(double x)
{
  CSVGText::setDX(x);
}

double
CQSVGText::
getDY() const
{
  return CSVGText::getDY().px().value();
}

void
CQSVGText::
setDY(double y)
{
  CSVGText::setDY(y);
}

QString
CQSVGText::
getRotate() const
{
  return CStrUtil::valuesToString(CSVGText::getRotate()).c_str();
}

void
CQSVGText::
setRotate(const QString &s)
{
  std::vector<std::string> words;

  CStrUtil::addWords(s.toStdString(), words);

  std::vector<double> reals;

  for (const auto &w : words) {
    double r;

    if (CStrUtil::toReal(w, &r))
      reals.push_back(r);
  }

  CSVGText::setRotate(reals);
}

QString
CQSVGText::
getTextLength() const
{
  return CSVGText::getTextLength().c_str();
}

void
CQSVGText::
setTextLength(const QString &s)
{
  CSVGText::setTextLength(s.toStdString());
}

QString
CQSVGText::
getLengthAdjust() const
{
  return CSVGText::getLengthAdjust().c_str();
}

void
CQSVGText::
setLengthAdjust(const QString &s)
{
  CSVGText::setLengthAdjust(s.toStdString());
}

void
CQSVGText::
addProperties(CQPropertyTree *propTree, const std::string &name)
{
  CQSVGObject::addProperties(propTree, name);

  QString objName = name.c_str();

  propTree->addProperty(objName, this, "x"           );
  propTree->addProperty(objName, this, "y"           );
  propTree->addProperty(objName, this, "dx"          );
  propTree->addProperty(objName, this, "dy"          );
  propTree->addProperty(objName, this, "text"        );
  propTree->addProperty(objName, this, "rotate"      );
  propTree->addProperty(objName, this, "textLength"  );
  propTree->addProperty(objName, this, "lengthAdjust");
}

void
CQSVGText::
drawTerm()
{
  drawSelected();
}

#include <CQSVGAnimate.h>
#include <CQSVG.h>

CQSVGAnimate::
CQSVGAnimate(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGAnimate(*svg)
{
}

QString
CQSVGAnimate::
attributeName() const
{
  return CSVGAnimate::getAttributeName().c_str();
}

void
CQSVGAnimate::
setAttributeName(const QString &s)
{
  CSVGAnimate::setAttributeName(s.toStdString());
}

QString
CQSVGAnimate::
attributeType() const
{
  return CSVGAnimate::getAttributeType().c_str();
}

void
CQSVGAnimate::
setAttributeType(const QString &s)
{
  CSVGAnimate::setAttributeType(s.toStdString());
}

QString
CQSVGAnimate::
from() const
{
  return CSVGAnimate::getFrom().c_str();
}

void
CQSVGAnimate::
setFrom(const QString &s)
{
  CSVGAnimate::setFrom(s.toStdString());
}

QString
CQSVGAnimate::
to() const
{
  return CSVGAnimate::getTo().c_str();
}

void
CQSVGAnimate::
setTo(const QString &s)
{
  CSVGAnimate::setTo(s.toStdString());
}

QString
CQSVGAnimate::
begin() const
{
  std::stringstream ss;

  ss << CSVGAnimate::getBegin();

  return ss.str().c_str();
}

#if 0
void
CQSVGAnimate::
setBegin(const QString &s)
{
  CSVGAnimate::setBegin(s.toStdString());
}
#endif

QString
CQSVGAnimate::
end() const
{
  std::stringstream ss;

  ss << CSVGAnimate::getEnd();

  return ss.str().c_str();
}

QString
CQSVGAnimate::
dur() const
{
  std::stringstream ss;

  ss << CSVGAnimate::getDur();

  return ss.str().c_str();
}

#if 0
void
CQSVGAnimate::
setDur(const QString &s)
{
  CSVGAnimate::setDur(s.toStdString());
}
#endif

QString
CQSVGAnimate::
fill() const
{
  return CSVGAnimate::getFill().c_str();
}

void
CQSVGAnimate::
setFill(const QString &s)
{
  CSVGAnimate::setFill(s.toStdString());
}

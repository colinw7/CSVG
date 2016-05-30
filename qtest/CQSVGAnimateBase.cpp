#include <CQSVGAnimateBase.h>
#include <CQSVG.h>
#include <CSVGAnimateBase.h>

CQSVGAnimateBase::
CQSVGAnimateBase(CQSVG *svg, CSVGAnimateBase *base) :
 CQSVGObject(svg, base), base_(base)
{
}

QString
CQSVGAnimateBase::
attributeName() const
{
  return base_->getAttributeName().c_str();
}

void
CQSVGAnimateBase::
setAttributeName(const QString &s)
{
  base_->setAttributeName(s.toStdString());
}

QString
CQSVGAnimateBase::
attributeType() const
{
  return base_->getAttributeType().c_str();
}

void
CQSVGAnimateBase::
setAttributeType(const QString &s)
{
  base_->setAttributeType(s.toStdString());
}

QString
CQSVGAnimateBase::
from() const
{
  return base_->getFrom().c_str();
}

void
CQSVGAnimateBase::
setFrom(const QString &s)
{
  base_->setFrom(s.toStdString());
}

QString
CQSVGAnimateBase::
to() const
{
  return base_->getTo().c_str();
}

void
CQSVGAnimateBase::
setTo(const QString &s)
{
  base_->setTo(s.toStdString());
}

QString
CQSVGAnimateBase::
begin() const
{
  std::stringstream ss;

  ss << base_->getBegin();

  return ss.str().c_str();
}

#if 0
void
CQSVGAnimateBase::
setBegin(const QString &s)
{
  base_->setBegin(s.toStdString());
}
#endif

QString
CQSVGAnimateBase::
end() const
{
  std::stringstream ss;

  ss << base_->getEnd();

  return ss.str().c_str();
}

QString
CQSVGAnimateBase::
dur() const
{
  std::stringstream ss;

  ss << base_->getDur();

  return ss.str().c_str();
}

#if 0
void
CQSVGAnimateBase::
setDur(const QString &s)
{
  base_->setDur(s.toStdString());
}
#endif

QString
CQSVGAnimateBase::
fill() const
{
  return base_->getFill().c_str();
}

void
CQSVGAnimateBase::
setFill(const QString &s)
{
  base_->setFill(s.toStdString());
}

QString
CQSVGAnimateBase::
repeatCount() const
{
  return base_->getRepeatCount().c_str();
}

void
CQSVGAnimateBase::
setRepeatCount(const QString &s)
{
  base_->setRepeatCount(s.toStdString());
}

QString
CQSVGAnimateBase::
repearDur() const
{
  std::stringstream ss;

  ss << base_->getRepeatDur();

  return ss.str().c_str();
}


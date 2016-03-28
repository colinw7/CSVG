#include <CSVGAnimateTransform.h>
#include <CSVG.h>
#include <CSVGLog.h>
#include <CSVGUtil.h>

/* Attributes:
    <Core>
    <Conditional>
    animation event attributes
    xlink attributes
    animation attribute target attributes
    animation timing attributes
    animation value attributes
    animation addition attributes
    <External>
    ‘type’
*/

CSVGAnimateTransform::
CSVGAnimateTransform(CSVG &svg) :
 CSVGAnimateBase(svg)
{
}

CSVGAnimateTransform::
CSVGAnimateTransform(const CSVGAnimateTransform &animateTransform) :
 CSVGAnimateBase(animateTransform),
 type_    (animateTransform.type_),
 additive_(animateTransform.additive_)
{
}

CSVGAnimateTransform *
CSVGAnimateTransform::
dup() const
{
  return new CSVGAnimateTransform(*this);
}

bool
CSVGAnimateTransform::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  if (CSVGAnimateBase::processOption(opt_name, opt_value))
    return true;

  std::string str;

  if      (svg_.stringOption(opt_name, opt_value, "type"    , str))
    type_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "additive", str))
    additive_ = str;
  else
    return false;

  return true;
}

void
CSVGAnimateTransform::
animate(double t)
{
  if (attributeName_.getValue() == "transform") {
    std::vector<double> fromValues, toValues;

    svg_.stringToReals(from_.getValue(""), fromValues);
    svg_.stringToReals(to_  .getValue(""), toValues  );

    if      (type_.getValue("") == "rotate") {
      if (fromValues.size() == 1) { fromValues.push_back(0); fromValues.push_back(0); }
      if (toValues  .size() == 1) { toValues  .push_back(0); toValues  .push_back(0); }

      if (fromValues.size() < 3 || toValues.size() < 3)
        return;

      double a = CSVGUtil::map(t, 0, 1, fromValues[0], toValues[0]);
      double x = CSVGUtil::map(t, 0, 1, fromValues[1], toValues[1]);
      double y = CSVGUtil::map(t, 0, 1, fromValues[2], toValues[2]);

      //std::cerr << "CSVGAnimateTransform: transform " << currentTime_ << ":" << a << std::endl;
      CMatrix2D m;

      if (additive_.getValue("") == "sum")
        m = getParent()->getTransform();
      else
        m = getParent()->getAnimation().getTransform();

      CMatrix2D m1, m2, m3;

      m1.setTranslation(-x, -y);
      m2.setRotation(CMathGen::DegToRad(a));
      m3.setTranslation( x,  y);

      CMatrix2D mm = m3*m2*m1;

      getParent()->setTransform(mm*m);

      svg_.redraw();
    }
    else if (type_.getValue("") == "scale") {
      if (fromValues.size() == 1) fromValues.push_back(fromValues[0]);
      if (toValues  .size() == 1) toValues  .push_back(toValues  [0]);

      if (fromValues.size() < 2 || toValues.size() < 2)
        return;

      double xs = CSVGUtil::map(t, 0, 1, fromValues[0], toValues[0]);
      double ys = CSVGUtil::map(t, 0, 1, fromValues[1], toValues[1]);

      //std::cerr << "CSVGAnimateTransform: scale " << currentTime_ << ":" <<
      //             xs << " " << ys << std::endl;
      CMatrix2D m;

      if (additive_.getValue("") == "sum")
        m = getParent()->getTransform();
      else
        m = getParent()->getAnimation().getTransform();

      CMatrix2D m1;

      m1.setScale(xs, ys);

      getParent()->setTransform(m1*m);

      svg_.redraw();
    }
  }
}

void
CSVGAnimateTransform::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<animateTransform";

    CSVGAnimateBase::printValues(os);

    printNameValue(os, "type"    , type_    );
    printNameValue(os, "additive", additive_);

    os << "/>" << std::endl;
  }
  else
    os << "animateTransform ";
}

std::ostream &
operator<<(std::ostream &os, const CSVGAnimateTransform &animateTransform)
{
  animateTransform.print(os, false);

  return os;
}

#include <CSVGAnimateMotion.h>
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
    ‘path’
    ‘keyPoints’
    ‘rotate’
    ‘origin’
*/

CSVGAnimateMotion::
CSVGAnimateMotion(CSVG &svg) :
 CSVGAnimateBase(svg)
{
}

CSVGAnimateMotion::
CSVGAnimateMotion(const CSVGAnimateMotion &animateMotion) :
 CSVGAnimateBase(animateMotion),
 path_     (animateMotion.path_),
 keyPoints_(animateMotion.keyPoints_),
 rotate_   (animateMotion.rotate_),
 origin_   (animateMotion.origin_)
{
}

CSVGAnimateMotion *
CSVGAnimateMotion::
dup() const
{
  return new CSVGAnimateMotion(*this);
}

bool
CSVGAnimateMotion::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  if (CSVGAnimateBase::processOption(opt_name, opt_value))
    return true;

  std::string str;
  PartList    parts;

  if      (svg_.pathOption  (opt_name, opt_value, "path"     , parts))
    path_ = parts;
  else if (svg_.stringOption(opt_name, opt_value, "keyPoints", str))
    keyPoints_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "rotate"   , str))
    rotate_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "origin"   , str))
    origin_ = str;
  else
    return false;

  return true;
}

void
CSVGAnimateMotion::
animate(double t)
{
  //std::cerr << "CSVGAnimateMotion: " << t_ << ":" << t << std::endl;
  if (! path_.empty()) {
    double x, y, a;

    svg_.interpParts(t, path_, &x, &y, &a);

    getParent()->moveTo(CPoint2D(x, y));

    if (rotate_.getValue("") == "auto")
      getParent()->rotateTo(a, CPoint2D(x, y));
    else {
      if (CStrUtil::toReal(rotate_.getValue(""), &a))
        getParent()->rotateTo(a, CPoint2D(x, y));
    }

    svg_.redraw();
  }
  else {
    std::vector<double> fromValues, toValues;

    svg_.stringToReals(from_.getValue(""), fromValues);
    svg_.stringToReals(to_  .getValue(""), toValues  );

    if (fromValues.size() != 2 || toValues.size() != 2)
      return;

    CPoint2D p1(fromValues[0], fromValues[1]);
    CPoint2D p2(toValues  [0], toValues  [1]);

    CPoint2D p = p1 + (p2 - p1)*t;

    getParent()->moveTo(p);

    svg_.redraw();
  }
}

void
CSVGAnimateMotion::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<animateMotion";

    CSVGAnimateBase::printValues(os);

    printNameParts(os, "path"     , path_     );
    printNameValue(os, "keyPoints", keyPoints_);
    printNameValue(os, "rotate"   , rotate_   );
    printNameValue(os, "origin"   , origin_   );

    if (hasChildren()) {
      os << ">" << std::endl;

      printChildren(os, hier);

      os << "</animateMotion>" << std::endl;
    }
    else
      os << "/>" << std::endl;
  }
  else
    os << "animateMotion ";
}

std::ostream &
operator<<(std::ostream &os, const CSVGAnimateMotion &animateMotion)
{
  animateMotion.print(os, false);

  return os;
}

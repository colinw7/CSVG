#include <CSVGAnimateMotion.h>
#include <CSVGMPath.h>
#include <CSVGPath.h>
#include <CSVG.h>
#include <CSVGLog.h>
#include <CStrUtil.h>

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

  std::string      str;
  CSVGPathPartList parts;

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
  //std::cerr << "CSVGAnimateMotion: " << t_ << ":" << t << "\n";
  auto *parent = getParent();

  ObjectArray objects;

  if (! path_.empty()) {
    double x, y, a;

    path_.interp(t, &x, &y, &a);

    CPoint2D p(x, y);

    CBBox2D bbox;

    if (parent->getBBox(bbox))
      p -= CPoint2D(bbox.getWidth()/2, bbox.getHeight()/2);

    parent->moveTo(p);

    if (rotate_.value_or("") == "auto")
      parent->rotateAt(a, p);
    else {
      if (CStrUtil::toReal(rotate_.value_or(""), &a))
        parent->rotateAt(a, p);
    }

    svg_.redraw();
  }
  else if (getChildrenOfType(CSVGObjTypeId::MPATH, objects)) {
    auto *mpath = dynamic_cast<CSVGMPath *>(objects[0]);

    const auto *path = (mpath ? &mpath->getPartList() : nullptr);

    if (path_.empty()) {
      auto *obj = mpath->xlink().getObject();

      if (obj && obj->getObjTypeId() == CSVGObjTypeId::PATH)
        path = &dynamic_cast<CSVGPath *>(obj)->getPartList();
    }

    if (path) {
      double x, y, a;

      path->interp(t, &x, &y, &a);

      CPoint2D p(x, y);

      parent->moveTo(p);

      if (rotate_.value_or("") == "auto")
        parent->rotateAt(a, p);
      else {
        if (CStrUtil::toReal(rotate_.value_or(""), &a))
          parent->rotateAt(a, p);
      }

      svg_.redraw();
    }
  }
  else {
    std::vector<double> fromValues, toValues;

    svg_.stringToReals(getFrom(), fromValues);
    svg_.stringToReals(getTo  (), toValues  );

    if (fromValues.size() != 2 || toValues.size() != 2)
      return;

    CPoint2D p1(fromValues[0], fromValues[1]);
    CPoint2D p2(toValues  [0], toValues  [1]);

    auto p = p1 + (p2 - p1)*t;

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
      os << ">\n";

      printChildren(os, hier);

      os << "</animateMotion>\n";
    }
    else
      os << "/>\n";
  }
  else
    os << "animateMotion ";
}

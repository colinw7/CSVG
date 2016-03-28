#include <CSVGUse.h>
#include <CSVG.h>
#include <CSVGLog.h>

CSVGUse::
CSVGUse(CSVG &svg) :
 CSVGObject(svg),
 xlink_    (this)
{
}

CSVGUse::
CSVGUse(const CSVGUse &use) :
 CSVGObject(use),
 xlink_    (use.xlink_),
 x_        (use.x_),
 y_        (use.y_),
 width_    (use.width_),
 height_   (use.height_)
{
}

CSVGUse::
~CSVGUse()
{
}

CSVGUse *
CSVGUse::
dup() const
{
  return new CSVGUse(*this);
}

/* Attributes:
    <Core>
    <Conditional>
    <Style>
    <Presentation>
    <GraphicalEvents>
    <XLinkEmbed>
    <External>
    x <Coordinate>
    y <Coordinate>
    width <Length>
    height <Length>
    transform <TransformList>
*/

bool
CSVGUse::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  std::string     str;
  double          real;
  CSVGLengthValue length;

  if      (svg_.coordOption (opt_name, opt_value, "x", &real))
    x_ = real;
  else if (svg_.coordOption (opt_name, opt_value, "y", &real))
    y_ = real;
  else if (svg_.lengthOption(opt_name, opt_value, "width", length))
    width_ = length.value();
  else if (svg_.lengthOption(opt_name, opt_value, "height", length))
    height_ = length.value();
  else if (svg_.stringOption(opt_name, opt_value, "xlink:href", str))
    xlink_ = CSVGXLink(this, str);
  else
    return CSVGObject::processOption(opt_name, opt_value);

  return true;
}

void
CSVGUse::
termParse()
{
  if (svg_.getUniquify()) {
    CSVGObject *object = getObject();

    if (object) {
      CSVGObject *object1 = object->dup();

      CBBox2D box;

      object1->getBBox(box);

      if (x_.isValid() || y_.isValid()) {
        CVector2D delta(0, 0);

        if (x_.isValid()) delta.setX(x_.getValue());
        if (y_.isValid()) delta.setY(y_.getValue());

        object1->moveBy(delta);
      }

      if (width_.isValid() || height_.isValid()) {
        CSize2D size = box.getSize();

        if (width_ .isValid()) size.width  = width_ .getValue();
        if (height_.isValid()) size.height = height_.getValue();

        object1->resizeTo(size);
      }

      object1->updateFill  (getFill  (), true);
      object1->updateStroke(getStroke(), true);

      addChildObject(object1);

      xlink_.setObject(0);
    }
  }
  else {
    if (x_.isValid() || y_.isValid()) {
      CPoint2D delta(0, 0);

      if (x_.isValid()) delta.setX(x_.getValue());
      if (y_.isValid()) delta.setY(y_.getValue());

      CMatrix2D transform;

      transform.setTranslation(delta.x, delta.y);

      setTransform(getTransform()*transform);
    }

    if (width_.isValid() || height_.isValid()) {
      //CSize2D size = box.getSize();

      //if (width_ .isValid()) size.width  = width_ .getValue();
      //if (height_.isValid()) size.height = height_.getValue();

      // TODO
    }
  }
}

void
CSVGUse::
moveBy(const CVector2D &delta)
{
  CSVGObject *object = getObject();

  if (object)
    object->moveBy(delta);

  for (auto &c : children())
    c->moveBy(delta);
}

void
CSVGUse::
draw()
{
  if (svg_.getDebug())
    CSVGLog() << *this;

  CSVGObject *object = getObject();

  if (object != 0)
    object->drawObject();

  if (xlink_.isImage())
    svg_.drawImage(0, 0, xlink_.getImage());
}

void
CSVGUse::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<use";

    CSVGObject::printValues(os);

    std::string xn = xlink_.str();

    if (xn != "")
      os << " xlink:href=\"" << xn << "\"";

    os << "/>" << std::endl;
  }
  else
    os << "use";
}

std::ostream &
operator<<(std::ostream &os, const CSVGUse &use)
{
  use.print(os, false);

  return os;
}

CSVGObject *
CSVGUse::
getObject() const
{
  CSVGObject *object = 0;

  if (xlink_.isObject())
    object = xlink_.getObject();

  return object;
}

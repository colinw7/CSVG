#include "CSVGI.h"

CSVGUse::
CSVGUse(CSVG &svg) :
 CSVGObject(svg),
 xlink_    (this),
 x_        (0),
 y_        (0),
 width_    (1),
 height_   (1)
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
processOption(const string &opt_name, const string &opt_value)
{
  string str;
  double real;

  if      (svg_.coordOption (opt_name, opt_value, "x", &real))
    x_ = real;
  else if (svg_.coordOption (opt_name, opt_value, "y", &real))
    y_ = real;
  else if (svg_.lengthOption(opt_name, opt_value, "width", &real))
    width_  = real;
  else if (svg_.lengthOption(opt_name, opt_value, "height", &real))
    height_ = real;
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

      xlink_.setObject(NULL);
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

  ObjectList::iterator po1, po2;

  for (po1 = childrenBegin(), po2 = childrenEnd(); po1 != po2; ++po1)
    (*po1)->moveBy(delta);
}

void
CSVGUse::
draw()
{
  if (svg_.getDebug())
    CSVGLog() << *this;

  CSVGObject *object = getObject();

  if (object != NULL)
    object->drawObject();

  if (xlink_.isImage())
    svg_.drawImage(0, 0, xlink_.getImage());
}

void
CSVGUse::
print(ostream &os) const
{
  os << "use";
}

ostream &
operator<<(ostream &os, const CSVGUse &use)
{
  use.print(os);

  return os;
}

CSVGObject *
CSVGUse::
getObject() const
{
  CSVGObject *object = NULL;

  if (xlink_.isObject())
    object = xlink_.getObject();

  return object;
}

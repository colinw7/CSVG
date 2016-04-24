#include <CSVGUse.h>
#include <CSVG.h>
#include <CSVGBuffer.h>
#include <CSVGSymbol.h>
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

CSVGUse *
CSVGUse::
dup() const
{
  return new CSVGUse(*this);
}

void
CSVGUse::
setLinkName(const std::string &str)
{
  xlink_ = CSVGXLink(this, str);
}

CSVGObject *
CSVGUse::
getLinkObject() const
{
  CSVGObject *object = 0;

  if (xlink_.isValid()) {
    if (xlink_.getValue().isObject())
      object = xlink_.getValue().getObject();
  }

  return object;
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
    width_ = length;
  else if (svg_.lengthOption(opt_name, opt_value, "height", length))
    height_ = length;
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
    CSVGObject *object = getLinkObject();

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

        if (width_ .isValid())
          size.width  = getWidth();
        if (height_.isValid())
          size.height = getHeight();

        object1->resizeTo(size);
      }

      object1->updateFill  (getFill  (), true);
      object1->updateStroke(getStroke(), true);

      addChildObject(object1);

      if (xlink_.isValid())
        xlink_.getValue().setObject(0);
    }
  }
  else {
    if (x_.isValid() || y_.isValid()) {
      CPoint2D delta(0, 0);

      if (x_.isValid()) delta.setX(x_.getValue());
      if (y_.isValid()) delta.setY(y_.getValue());

      CMatrixStack2D transform;

      transform.append(getTransform());

      transform.translate(delta.x, delta.y);

      setTransform(transform);
    }

    if (width_.isValid() || height_.isValid()) {
      //CSize2D size = box.getSize();

      //if (width_ .isValid())
      //  size.width  = getWidth();
      //if (height_.isValid())
      //  size.height = getHeight();

      // TODO
    }
  }
}

bool
CSVGUse::
getBBox(CBBox2D &bbox) const
{
  if (! viewBox_.isValid()) {
    CSVGObject *object = getLinkObject();

    if (! object)
      return false;

    if (! object->getBBox(bbox))
      return false;

    CSVGSymbol *symbol = dynamic_cast<CSVGSymbol *>(object);

    if (symbol) {
      double w = getWidth(), h = getHeight();

      bbox.setSize(CSize2D(w, h));
    }
  }
  else
    bbox = getViewBox();

  return true;
}

void
CSVGUse::
moveBy(const CVector2D &delta)
{
  CSVGObject *object = getLinkObject();

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

  CSVGObject *object = getLinkObject();

  if      (object) {
    bool drawn = false;

    //------

    // get current buffer
    CSVGBuffer *oldBuffer = svg_.getBuffer();

    //------

    // save object image to temporary buffer if debug or symbol
    bool saveImage = false;

    if (svg_.getDebugUse())
      saveImage = true;

    CSVGSymbol *symbol = dynamic_cast<CSVGSymbol *>(object);

    if (symbol)
      saveImage = true;

    CSVGBuffer *saveBuffer = 0;

    //---

    CBBox2D symbolBBox;
    double  xs = 1, ys = 1;

    if (symbol) {
      symbol->getBBox(symbolBBox);

      xs = getWidth ()/symbolBBox.getWidth ();
      ys = getHeight()/symbolBBox.getHeight();

      CSVGPreserveAspect pa = symbol->preserveAspect();

      if (pa.getScale() == CSVGScale::FIXED_MEET || pa.getScale() == CSVGScale::FIXED_SLICE) {
        double s = (pa.getScale() == CSVGScale::FIXED_MEET ? std::min(xs, ys) : std::max(xs, ys));

        ys = s;
        xs = s;
      }
    }

    //---

    // set buffer to temporary buffer
    if (saveImage) {
      saveBuffer = svg_.getBuffer("_" + getUniqueName());

      svg_.setBuffer(saveBuffer);

      saveBuffer->clear();

      svg_.beginDrawBuffer(saveBuffer, svg_.scale()*xs, svg_.scale()*ys);
    }

    //------

#if 0
    // set current transform
    CMatrixStack2D transform = oldBuffer->transform();

    CMatrixStack2D transform1(transform);

    transform1.append(getTransform());

    svg_.setTransform(transform1);
#endif

    //------

    if (object->drawSubObject())
      drawn = true;

    //------

#if 0
    // restore transform
    svg_.setTransform(transform);
#endif

    //------

    // if object image saved to temporary buffer add to old buffer
    if (saveImage) {
      svg_.endDrawBuffer(saveBuffer);

      //---

      if (drawn) {
        CImagePtr image = saveBuffer->getImage();

        if (symbol) {
          double x = getX    (), y = getY     ();
          double w = getWidth(), h = getHeight();

          double sx = xs*symbolBBox.getWidth ();
          double sy = ys*symbolBBox.getHeight();

          double dx = (sx - w)/2;
          double dy = (sy - h)/2;

          //saveBuffer->setOrigin(CPoint2D(x - dx, y - dy));

          double px1 = (dx    )*svg_.scale();
          double py1 = (dy    )*svg_.scale();
          double px2 = (dx + w)*svg_.scale();
          double py2 = (dy + h)*svg_.scale();

          image->clipOutside(px1, py1, px2, py2);

          double x1 = x*svg_.scale();
          double y1 = y*svg_.scale();

          oldBuffer->addImage(x1 - px1, y1 - py1, image);
        }
        else
          oldBuffer->addImage(image);
      }

      //---

      svg_.setBuffer(oldBuffer);
    }
  }
  else if (xlink_.isValid()) {
    if (xlink_.getValue().isImage())
      svg_.drawImage(0, 0, xlink_.getValue().getImage());
  }
}

void
CSVGUse::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<use";

    printNameXLink(os, "xlink:href", xlink_);

    CSVGObject::printValues(os);

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

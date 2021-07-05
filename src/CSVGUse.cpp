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
  CSVGObject *object = nullptr;

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
  std::string  str;
  double       real;
  CScreenUnits length;

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
#if 0
  if (svg_.getUniquify()) {
    auto *object = getLinkObject();

    if (object) {
      auto *object1 = object->dup();

      CBBox2D box;

      object1->getBBox(box);

      if (x_.isValid() || y_.isValid()) {
        CVector2D delta(0, 0);

        if (x_.isValid()) delta.setX(x_.getValue());
        if (y_.isValid()) delta.setY(y_.getValue());

        object1->moveBy(delta);
      }

      if (width_.isValid() || height_.isValid()) {
        auto size = box.getSize();

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

    return;
  }
#endif

#if 0
  if (x_.isValid() || y_.isValid()) {
    auto transform = adjustedTransform() const

    setTransform(transform);
  }

  if (width_.isValid() || height_.isValid()) {
    //auto size = box.getSize();

    //if (width_ .isValid())
    //  size.width  = getWidth();
    //if (height_.isValid())
    //  size.height = getHeight();

    // TODO
  }
#endif
}

bool
CSVGUse::
getBBox(CBBox2D &bbox) const
{
  if (! hasViewBox()) {
    auto *object = getLinkObject();

    if (! object)
      return false;

    if (! object->getBBox(bbox))
      return false;

    auto *symbol = dynamic_cast<CSVGSymbol *>(object);

    if (symbol) {
      double w = getWidth(), h = getHeight();

      bbox.setSize(CSize2D(w, h));
    }
  }
  else
    bbox = getViewBox();

  return true;
}

CMatrixStack2D
CSVGUse::
adjustedTransform() const
{
  return adjustedTransform(getTransform());
}

CMatrixStack2D
CSVGUse::
adjustedTransform(const CMatrixStack2D &transform) const
{
  if (x_.isValid() && ! y_.isValid())
    return transform;

  //---

  CPoint2D delta(0, 0);

  if (x_.isValid())
    delta.setX(x_.getValue());

  if (y_.isValid())
    delta.setY(y_.getValue());

  CMatrixStack2D transform1;

  transform1.append(transform);

  transform1.translate(delta.x, delta.y);

  return transform1;
}

void
CSVGUse::
moveBy(const CVector2D &delta)
{
  auto *object = getLinkObject();

  if (object)
    object->moveBy(delta);

  for (auto &c : children())
    c->moveBy(delta);
}

bool
CSVGUse::
draw()
{
  if (svg_.getDebug())
    CSVGLog() << *this;

  auto *object = getLinkObject();

  if      (object) {
    if (object->getDisplay() == "none")
      return false;

    bool drawn = false;

    //------

    // get current buffer
    auto *oldBuffer     = svg_.getCurrentBuffer();
    auto *currentBuffer = oldBuffer;

    //------

    // save object image to temporary buffer if debug or symbol
    bool saveImage = false;

    if (svg_.getDebugUse())
      saveImage = true;

    if (object->getOpacityValid())
      saveImage = true;

    auto *symbol = dynamic_cast<CSVGSymbol *>(object);

    if (symbol)
      saveImage = true;

    // if currently drawing to clip then can't use image
    if (oldBuffer->isClip())
      saveImage = false;

    CSVGBuffer *saveBuffer = nullptr;

    //---

    CBBox2D symbolBBox;
    double  xs = 1, ys = 1;

    if (symbol) {
      symbol->getBBox(symbolBBox);

      xs = getWidth ()/symbolBBox.getWidth ();
      ys = getHeight()/symbolBBox.getHeight();

      auto pa = symbol->preserveAspect();

      if (pa.getScale() == CSVGScale::FIXED_MEET || pa.getScale() == CSVGScale::FIXED_SLICE) {
        double s = (pa.getScale() == CSVGScale::FIXED_MEET ? std::min(xs, ys) : std::max(xs, ys));

        ys = s;
        xs = s;
      }
    }

    //---

    // set buffer to temporary buffer
    if (saveImage) {
      saveBuffer = svg_.pushBuffer("_" + object->getUniqueName() + "_" + getUniqueName());

      saveBuffer->clear();

      svg_.beginDrawBuffer(saveBuffer, svg_.offset(), xs, ys);

      if (oldBuffer->hasClipPath())
        saveBuffer->addClipPath(oldBuffer);

      currentBuffer = saveBuffer;
    }

    //------

    // set current transform
    auto transform = oldBuffer->transform();

    auto transform1 = adjustedTransform(transform);

    transform1.append(object->getTransform());

    //if (! saveImage)
    if (! symbol)
      currentBuffer->setTransform(transform1);

    if (object->getOpacityValid())
      currentBuffer->setOpacity(object->getOpacity());

    //------

    // draw object
    if (object->drawSubObject())
      drawn = true;

    //------

    // restore transform
    //if (! saveImage) {
    if (! symbol)
      currentBuffer->setTransform(transform);

    //------

    // if object image saved to temporary buffer add to old buffer
    if (saveImage) {
      svg_.endDrawBuffer(saveBuffer);

      //---

      if (drawn) {
        if (symbol) {
          double x = getX    (), y = getY     ();
          double w = getWidth(), h = getHeight();

          double sx = xs*symbolBBox.getWidth ();
          double sy = ys*symbolBBox.getHeight();

          double dx = (sx - w)/2;
          double dy = (sy - h)/2;

          //saveBuffer->setOrigin(CPoint2D(x - dx, y - dy));

          double xs1 = svg_.flatXScale();
          double ys1 = svg_.flatYScale();

          double px1 = (dx    )*xs1;
          double py1 = (dy    )*ys1;
          double px2 = (dx + w)*xs1;
          double py2 = (dy + h)*ys1;

          double x1 = x*xs1;
          double y1 = y*ys1;

          bool oldDrawing = oldBuffer->isDrawing();

          if (oldDrawing)
            oldBuffer->stopDraw();

          oldBuffer->addClippedBuffer(saveBuffer, x1, y1, px1, py1, px2, py2);

          if (oldDrawing)
            oldBuffer->startDraw();
        }
        else {
#if 0
          double x = 0, y = 0;

          transform.multiplyPoint(0, 0, &x, &y);

          double px, py;

          svg_.lengthToPixel(x, y, &px, &py);
#endif

          bool oldDrawing = oldBuffer->isDrawing();

          if (oldDrawing)
            oldBuffer->stopDraw();

          //oldBuffer->addBuffer(saveBuffer, px, py);
          oldBuffer->addBuffer(saveBuffer, 0, 0);

          if (oldDrawing)
            oldBuffer->startDraw();
        }
      }

      //---

      svg_.popBuffer();
    }
  }
  else if (xlink_.isValid()) {
    if (xlink_.getValue().isImage()) {
      auto *buffer = svg_.getCurrentBuffer();

      buffer->drawImage(0, 0, xlink_.getValue().getImageBuffer());
    }
  }

  return true;
}

void
CSVGUse::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<use";

    printValues(os);

    os << "/>" << std::endl;
  }
  else
    os << "use";
}

void
CSVGUse::
printValues(std::ostream &os, bool flat) const
{
  printNameValue(os, "x"     , x_);
  printNameValue(os, "y"     , y_);
  printNameValue(os, "width" , width_);
  printNameValue(os, "height", height_);

  if (! xlink_.getValue().isNull())
    printNameXLink(os, "xlink:href", xlink_);

  CSVGObject::printValues(os, flat);
}

std::ostream &
operator<<(std::ostream &os, const CSVGUse &use)
{
  use.print(os, false);

  return os;
}

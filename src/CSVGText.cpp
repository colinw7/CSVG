#include <CSVGText.h>
#include <CSVG.h>
#include <CSVGLog.h>

/* Attributes:
    <Core>
    <Conditional>
    <Style>
    <Text>
    <TextContent>
    <Font>
    <Paint>
    <Color>
    <Opacity>
    <Graphics>
    <Clip>
    <Mask>
    <Filter>
    <GraphicalEvents>
    <Cursor>
    <External>
    x <Coordinates>
    y <Coordinates>
    dx <Lengths>
    dy <Lengths>
    rotate <Numbers>
    textLength <Length>
    lengthAdjust ( spacing | spacingAndGlyphs ) #IMPLIED
    transform <TransformList>
*/

CSVGText::
CSVGText(CSVG &svg) :
 CSVGObject(svg)
{
}

CSVGText::
CSVGText(const CSVGText &text) :
 CSVGObject(text),
 x_           (text.x_),
 y_           (text.y_),
 dx_          (text.dx_),
 dy_          (text.dy_),
 rotate_      (text.rotate_),
 textLength_  (text.textLength_),
 lengthAdjust_(text.lengthAdjust_)
{
}

CSVGText *
CSVGText::
dup() const
{
  return new CSVGText(*this);
}

bool
CSVGText::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  if (processCoreOption           (opt_name, opt_value)) return true;
  if (processConditionalOption    (opt_name, opt_value)) return true;
  if (processStyleOption          (opt_name, opt_value)) return true;
  if (processTextOption           (opt_name, opt_value)) return true;
  if (processTextContentOption    (opt_name, opt_value)) return true;
  if (processFontOption           (opt_name, opt_value)) return true;
  if (processPaintOption          (opt_name, opt_value)) return true;
  if (processColorOption          (opt_name, opt_value)) return true;
  if (processOpacityOption        (opt_name, opt_value)) return true;
  if (processGraphicsOption       (opt_name, opt_value)) return true;
  if (processClipOption           (opt_name, opt_value)) return true;
  if (processMaskOption           (opt_name, opt_value)) return true;
  if (processFilterOption         (opt_name, opt_value)) return true;
  if (processGraphicalEventsOption(opt_name, opt_value)) return true;
  if (processCursorOption         (opt_name, opt_value)) return true;
  if (processExternalOption       (opt_name, opt_value)) return true;

  std::string     str;
  double          real;
  CSVGLengthValue length;
  CMatrix2D       transform;

  if      (svg_.coordOption (opt_name, opt_value, "x", &real))
    x_ = real;
  else if (svg_.coordOption (opt_name, opt_value, "y", &real))
    y_ = real;
  else if (svg_.lengthOption(opt_name, opt_value, "dx", length))
    dx_ = length;
  else if (svg_.lengthOption(opt_name, opt_value, "dy", length))
    dy_ = length;
  else if (svg_.stringOption(opt_name, opt_value, "rotate", str))
    rotate_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "textLength", str))
    textLength_ = str;
  else if (svg_.lengthOption(opt_name, opt_value, "lengthAdjust", length))
    lengthAdjust_ = length;
  else if (svg_.transformOption(opt_name, opt_value, "transform", transform))
    setTransform(transform);
  else
    return false;

  return true;
}

bool
CSVGText::
getBBox(CBBox2D &bbox) const
{
  if (! viewBox_.isValid()) {
    double x = getX();
    double y = getY();
    double w = 8;
    double a = 10;
    double d = 2;

    svg_.textSize(getText(), getFont(), &w, &a, &d);

    bbox = CBBox2D(x, y + d, x + w, y - a);

    CBBox2D bbox1;

    if (getChildrenBBox(bbox1))
      bbox += bbox1;
  }
  else
    bbox = getViewBox();

  return true;
}

void
CSVGText::
moveTo(const CPoint2D &p)
{
  x_ = p.x;
  y_ = p.y;
}

void
CSVGText::
moveBy(const CVector2D &delta)
{
  x_ = getX() + delta.x();
  y_ = getY() + delta.y();
}

void
CSVGText::
draw()
{
  if (svg_.getDebug())
    CSVGLog() << *this;

  double x = getX();
  double y = getY();

  if (svg_.isFilled() || svg_.isStroked()) {
    if (svg_.isFilled())
      svg_.fillText(x, y, getText(), getFont(), getTextAnchor());

    if (svg_.isStroked())
      svg_.drawText(x, y, getText(), getFont(), getTextAnchor());
  }
  else
    svg_.fillText(x, y, getText(), getFont(), getTextAnchor());

  double w, a, d;

  svg_.textSize(getText(), getFont(), &w, &a, &d);

  setLastPos(CPoint2D(x + w, y));
}

void
CSVGText::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<text";

    CSVGObject::printValues(os);

    printNameValue (os, "x" , x_ );
    printNameValue (os, "y" , y_ );
    printNameLength(os, "dx", dx_);
    printNameLength(os, "dy", dy_);

    printNameValue (os, "rotate", rotate_);

    printNameValue (os, "textLength"  , textLength_  );
    printNameLength(os, "lengthAdjust", lengthAdjust_);

    os << ">";

    if (hasChildren()) {
      os << std::endl;

      if (hasText())
        os << getText() << std::endl;

      printChildren(os, hier);
    }
    else {
      os << getText();
    }

    os << "</text>" << std::endl;
  }
  else
    os << "text " << getPosition()  << " " << CStrUtil::single_quote(getText());
}

std::ostream &
operator<<(std::ostream &os, const CSVGText &text)
{
  text.print(os, false);

  return os;
}

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
 x_        (text.x_),
 y_        (text.y_),
 text_     (text.text_)
{
}

CSVGText *
CSVGText::
dup() const
{
  return new CSVGText(*this);
}

void
CSVGText::
setText(const std::string &text)
{
  text_ = CStrUtil::stripSpaces(text);
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

  if      (svg_.coordOption (opt_name, opt_value, "x", &real))
    x_ = real;
  else if (svg_.coordOption (opt_name, opt_value, "y", &real))
    y_ = real;
  else if (svg_.lengthOption(opt_name, opt_value, "dx", length))
    notHandled(opt_name, opt_value);
  else if (svg_.lengthOption(opt_name, opt_value, "dy", length))
    notHandled(opt_name, opt_value);
  else if (svg_.stringOption(opt_name, opt_value, "rotate", str))
    notHandled(opt_name, opt_value);
  else if (svg_.stringOption(opt_name, opt_value, "textLength", str))
    notHandled(opt_name, opt_value);
  else if (svg_.lengthOption(opt_name, opt_value, "lengthAdjust", length))
    notHandled(opt_name, opt_value);
  else if (svg_.stringOption(opt_name, opt_value, "transform", str)) {
    CMatrix2D transform;

    if (! svg_.decodeTransform(str, transform))
      return false;

    setTransform(transform);
  }
  else
    return false;

  return true;
}

bool
CSVGText::
getBBox(CBBox2D &bbox) const
{
  if (! viewBox_.isSet()) {
    double w = 10;
    double h = 10;

    bbox = CBBox2D(getX(), getY(), getX() + w, getY() + h);
  }
  else
    bbox = viewBox_;

  return true;
}

void
CSVGText::
draw()
{
  if (svg_.getDebug())
    CSVGLog() << *this;

  if (svg_.isFilled())
    svg_.fillText(getX(), getY(), getText(), getFont(), getTextAnchor());

  if (svg_.isStroked())
    svg_.drawText(getX(), getY(), getText(), getFont(), getTextAnchor());
}

void
CSVGText::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<text";

    printNameValue(os, "id", id_);
    printNameValue(os, "x" , x_ );
    printNameValue(os, "y" , y_ );

    printFilter(os);

    printStyle(os);

    os << ">";

    os << getText();

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

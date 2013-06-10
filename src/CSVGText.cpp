#include "CSVGI.h"

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
 CSVGObject(svg),
 position_ (0,0),
 text_     ()
{
}

CSVGText::
CSVGText(const CSVGText &text) :
 CSVGObject(text),
 position_ (text.position_),
 text_     (text.text_)
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
processOption(const string &opt_name, const string &opt_value)
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

  string str;
  double real;

  if      (svg_.coordOption (opt_name, opt_value, "x", &real))
    position_.x = real;
  else if (svg_.coordOption (opt_name, opt_value, "y", &real))
    position_.y = real;
  else if (svg_.lengthOption(opt_name, opt_value, "dx", &real))
    notHandled(opt_name, opt_value);
  else if (svg_.lengthOption(opt_name, opt_value, "dy", &real))
    notHandled(opt_name, opt_value);
  else if (svg_.stringOption(opt_name, opt_value, "rotate", str))
    notHandled(opt_name, opt_value);
  else if (svg_.stringOption(opt_name, opt_value, "textLength", str))
    notHandled(opt_name, opt_value);
  else if (svg_.lengthOption(opt_name, opt_value, "lengthAdjust", &real))
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
  if (! view_box_.isSet()) {
    double w = 10;
    double h = 10;

    bbox = CBBox2D(position_.x, position_.y, position_.x + w, position_.y + h);
  }
  else
    bbox = view_box_;

  return true;
}

void
CSVGText::
draw()
{
  if (svg_.getDebug())
    CSVGLog() << *this;

  if (svg_.isFilled())
    svg_.fillText(position_.x, position_.y, text_, getFont(), getTextAnchor());

  if (svg_.isStroked())
    svg_.drawText(position_.x, position_.y, text_, getFont(), getTextAnchor());
}

void
CSVGText::
print(ostream &os) const
{
  os << "text " << position_ << " " << CStrUtil::single_quote(text_);
}

ostream &
operator<<(ostream &os, const CSVGText &text)
{
  text.print(os);

  return os;
}

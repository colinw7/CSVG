#include <CSVGStroke.h>
#include <CSVG.h>
#include <CSVGLog.h>

CRGBA
CSVGStroke::
getAlphaColor() const
{
  CRGBA rgba;

  if (color_.isValid()) {
    rgba = color_.getValue();

    if (getOpacityValid())
      rgba.setAlpha(getOpacity());
  }
  else
    rgba = CRGBA(0,0,0,0);

  return rgba;
}

void
CSVGStroke::
setColor(const std::string &color_def)
{
  if (color_def == "none") {
    noColor_ = true;

    resetColor();

    resetOpacity();
  }
  else {
    noColor_ = false;

    CRGBA rgba;

    if (svg_.decodeColorString(color_def, rgba)) {
      setColor(rgba);

      //resetOpacity();
    }
  }
}

double
CSVGStroke::
getOpacity() const
{
  if (opacity_.isValid())
    return opacity_.getValue();
  else
    return 1.0;
}

void
CSVGStroke::
setOpacity(const std::string &opacity_def)
{
  double opacity = svg_.decodeOpacityString(opacity_def);

  setOpacity(opacity);
}

double
CSVGStroke::
getWidth() const
{
  if (width_.isValid())
    return width_.getValue();
  else
    return 1.0;
}

void
CSVGStroke::
setWidth(const std::string &width_def)
{
  double width = svg_.decodeWidthString(width_def);

  setWidth(width);
}

const CLineDash &
CSVGStroke::
getDash() const
{
  if (dash_.isValid())
    return dash_.getValue();
  else {
    static CLineDash dash;

    return dash;
  }
}

void
CSVGStroke::
setDash(const std::string &dash_str)
{
  CLineDash dash;

  svg_.decodeDashString(dash_str, dash);

  setDash(dash);
}

void
CSVGStroke::
setDashOffset(const std::string &offset_str)
{
  CSVGLengthValue lvalue;

  if (! svg_.decodeLengthValue(offset_str, lvalue))
    return;

  setDashOffset(lvalue.value());
}

void
CSVGStroke::
setDashOffset(double offset)
{
  if (getDashValid())
    dash_.getValue().setOffset(offset);
}

CLineCapType
CSVGStroke::
getLineCap() const
{
  if (cap_.isValid())
    return cap_.getValue();
  else
    return LINE_CAP_TYPE_BUTT;
}

void
CSVGStroke::
setLineCap(const std::string &cap_str)
{
  CLineCapType cap = LINE_CAP_TYPE_NONE;

  if      (cap_str == "butt")
    cap = LINE_CAP_TYPE_BUTT;
  else if (cap_str == "round")
    cap = LINE_CAP_TYPE_ROUND;
  else if (cap_str == "square")
    cap = LINE_CAP_TYPE_SQUARE;
  else {
    CSVGLog() << "Illegal line_cap " << cap_str;
    return;
  }

  setLineCap(cap);
}

CLineJoinType
CSVGStroke::
getLineJoin() const
{
  if (join_.isValid())
    return join_.getValue();
  else
    return LINE_JOIN_TYPE_MITRE;
}

void
CSVGStroke::
setLineJoin(const std::string &join_str)
{
  CLineJoinType join = LINE_JOIN_TYPE_NONE;

  if      (join_str == "mitre" || join_str == "miter")
    join = LINE_JOIN_TYPE_MITRE;
  else if (join_str == "round")
    join = LINE_JOIN_TYPE_ROUND;
  else if (join_str == "bevel")
    join = LINE_JOIN_TYPE_BEVEL;
  else {
    CSVGLog() << "Illegal line_join " << join_str;
    return;
  }

  setLineJoin(join);
}

double
CSVGStroke::
getMitreLimit() const
{
  if (mlimit_.isValid())
    return mlimit_.getValue();
  else
    return 0.0;
}

void
CSVGStroke::
setMitreLimit(const std::string &limit_str)
{
  CSVGLengthValue lvalue;

  if (! svg_.decodeLengthValue(limit_str, lvalue))
    return;

  setMitreLimit(lvalue.value());
}

void
CSVGStroke::
reset()
{
  color_  .setInvalid();
  width_  .setInvalid();
  opacity_.setInvalid();
  dash_   .setInvalid();
  cap_    .setInvalid();
  join_   .setInvalid();
  mlimit_ .setInvalid();
}

void
CSVGStroke::
update(const CSVGStroke &stroke)
{
  if (stroke.getColorValid())
    setColor(stroke.getColor());

  if (stroke.getOpacityValid())
    setOpacity(stroke.getOpacity());

  if (stroke.getWidthValid())
    setWidth(stroke.getWidth());

  if (stroke.getDashValid())
    setDash(stroke.getDash());

  if (stroke.getLineCapValid())
    setLineCap(stroke.getLineCap());

  if (stroke.getLineJoinValid())
    setLineJoin(stroke.getLineJoin());

  if (stroke.getMitreLimitValid())
    setMitreLimit(stroke.getMitreLimit());
}

void
CSVGStroke::
print(std::ostream &os) const
{
  if (noColor_)
    os << "stroke: none;";

  if (color_.isValid())
    os << "stroke: " << color_.getValue().getRGB().stringEncode() << ";";

  if (width_.isValid())
    os << " stroke-width: " << width_.getValue() << ";";

  if (dash_.isValid()) {
    os << " stroke-dasharray: " << dash_.getValue().toString() << ";"; // TODO: no offset, format

    if (dash_.getValue().getOffset() != 0)
      os << " stroke-dashoffset: " << dash_.getValue().getOffset() << ";";
  }

  if (cap_.isValid()) {
    os << " stroke-linecap: ";

    if      (cap_.getValue() == LINE_CAP_TYPE_BUTT  ) os << "butt";
    else if (cap_.getValue() == LINE_CAP_TYPE_ROUND ) os << "round";
    else if (cap_.getValue() == LINE_CAP_TYPE_SQUARE) os << "square";
    else os << int(cap_.getValue());

    os << ";";
  }

  if (join_.isValid()) {
    os << " stroke-linejoin: ";

    if      (join_.getValue() == LINE_JOIN_TYPE_MITRE) os << "miter";
    else if (join_.getValue() == LINE_JOIN_TYPE_ROUND) os << "round";
    else if (join_.getValue() == LINE_JOIN_TYPE_BEVEL) os << "bevel";
    else os << int(join_.getValue());

    os << ";";
  }

  if (mlimit_.isValid())
    os << " stroke-miterlimit: " << mlimit_.getValue() << ";";
}

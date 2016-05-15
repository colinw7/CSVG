#include <CSVGStroke.h>
#include <CSVG.h>
#include <CSVGLog.h>
#include <CSVGUtil.h>

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
  noColor_.setInvalid();

  resetColor();

  if (color_def == "none") {
    noColor_ = true;

    resetOpacity();
  }
  else {
    CRGBA rgba;

    if (svg_.decodeColorString(color_def, rgba)) {
      setColor(rgba);
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
  CScreenUnits lvalue;

  if (! svg_.decodeLengthValue(offset_str, lvalue))
    return;

  setDashOffset(lvalue.px().value());
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
  CScreenUnits lvalue;

  if (! svg_.decodeLengthValue(limit_str, lvalue))
    return;

  setMitreLimit(lvalue.px().value());
}

void
CSVGStroke::
reset()
{
  noColor_.setInvalid();
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
  if (stroke.noColor_.isValid())
    noColor_ = stroke.noColor_;
  else {
    if (svg_.styleObject()) {
      bool b;

      if (svg_.getStyleStrokeNoColor(svg_.styleObject(), b))
        noColor_ = b;
    }
  }

  if (stroke.getColorValid()) {
    color_ = stroke.color_;

    noColor_.setInvalid();
  }
  else {
    if (svg_.styleObject()) {
      CRGBA c;

      if (svg_.getStyleStrokeColor(svg_.styleObject(), c)) {
        color_ = c;

        noColor_.setInvalid();
      }
    }
  }

  if (stroke.getOpacityValid())
    opacity_ = stroke.opacity_;
  else {
    if (svg_.styleObject()) {
      double a;

      if (svg_.getStyleStrokeOpacity(svg_.styleObject(), a))
        setOpacity(a);
    }
  }

  if (stroke.getWidthValid())
    setWidth(stroke.getWidth());
  else {
    if (svg_.styleObject()) {
      double w;

      if (svg_.getStyleStrokeWidth(svg_.styleObject(), w))
        setWidth(w);
    }
  }

  if (stroke.getDashValid())
    setDash(stroke.getDash());
  else {
    if (svg_.styleObject()) {
      CLineDash dash;

      if (svg_.getStyleStrokeDash(svg_.styleObject(), dash))
        setDash(dash);
    }
  }

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
  if (noColor_.isValid())
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
    else                                              os << CSVGUtil::round(cap_.getValue());

    os << ";";
  }

  if (join_.isValid()) {
    os << " stroke-linejoin: ";

    if      (join_.getValue() == LINE_JOIN_TYPE_MITRE) os << "miter";
    else if (join_.getValue() == LINE_JOIN_TYPE_ROUND) os << "round";
    else if (join_.getValue() == LINE_JOIN_TYPE_BEVEL) os << "bevel";
    else                                               os << CSVGUtil::round(join_.getValue());

    os << ";";
  }

  if (mlimit_.isValid())
    os << " stroke-miterlimit: " << mlimit_.getValue() << ";";
}

#include <CSVGI.h>

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
setColor(const string &color_def)
{
  if (color_def == "none") {
    setColor(CRGBA(0,0,0,0));

    setOpacity(0.0);
  }
  else {
    CRGBA rgba;

    if (svg_.decodeColorString(color_def, rgba)) {
      setColor(rgba);

      if (! getOpacityValid())
        setOpacity(1.0);
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
setOpacity(const string &opacity_def)
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
setWidth(const string &width_def)
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
setDash(const string &dash_str)
{
  CLineDash dash;

  svg_.decodeDashString(dash_str, dash);

  setDash(dash);
}

void
CSVGStroke::
setDashOffset(const string &offset_str)
{
  double offset;

  if (svg_.lengthToReal(offset_str, &offset))
    setDashOffset(offset);
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
setLineCap(const string &cap_str)
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
setLineJoin(const string &join_str)
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
setMitreLimit(const string &limit_str)
{
  double limit;

  if (svg_.lengthToReal(limit_str, &limit))
    setMitreLimit(limit);
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

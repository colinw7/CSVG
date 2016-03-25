#include <CSVGBlock.h>
#include <CSVG.h>
#include <CSVGLog.h>
#include <CRegExp.h>

CSVGBlock::
CSVGBlock(CSVG &svg) :
 CSVGObject(svg)
{
}

CSVGBlock::
CSVGBlock(const CSVGBlock &block) :
 CSVGObject(block),
 x_        (block.x_),
 y_        (block.y_),
 width_    (block.width_),
 height_   (block.height_),
 halign_   (block.halign_),
 valign_   (block.valign_),
 scale_    (block.scale_)
{
}

CSVGBlock *
CSVGBlock::
dup() const
{
  return new CSVGBlock(*this);
}

/* Attributes:
    <xmlns>
    <Core>
    <Conditional>
    <Style>
    <Presentation>
    <DocumentEvents>
    <GraphicalEvents>
    <External>
    x <Coordinate>
    y <Coordinate>
    width <Length>
    height <Length>
    viewBox <ViewBoxSpec>
    preserveAspectRatio <PreserveAspectRatioSpec> 'xMidYMid meet'
    zoomAndPan ( disable | magnify ) 'magnify'
    version <Number>
    baseProfile <Text>
    contentScriptType <ContentType>
    contentStyleType <ContentType>
*/
bool
CSVGBlock::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  std::string     str;
  double          real;
  CBBox2D         bbox;
  CSVGLengthValue length;

  if (processCoreOption           (opt_name, opt_value)) return true;
  if (processConditionalOption    (opt_name, opt_value)) return true;
  if (processStyleOption          (opt_name, opt_value)) return true;
  if (processPresentationOption   (opt_name, opt_value)) return true;
  if (processDocumentEventsOption (opt_name, opt_value)) return true;
  if (processGraphicalEventsOption(opt_name, opt_value)) return true;
  if (processExternalOption       (opt_name, opt_value)) return true;

  // TODO: xmlns:<prefix>
  if      (svg_.stringOption(opt_name, opt_value, "xmlns", str))
    ;
  else if (svg_.coordOption (opt_name, opt_value, "x", &real))
    x_.setValue(real);
  else if (svg_.coordOption (opt_name, opt_value, "y", &real))
    y_.setValue(real);
  else if (svg_.lengthOption(opt_name, opt_value, "width", length))
    width_.setValue(length);
  else if (svg_.lengthOption(opt_name, opt_value, "height", length))
    height_.setValue(length);
  else if (svg_.bboxOption  (opt_name, opt_value, "viewBox", &bbox))
    viewBox_ = bbox;
  else if (svg_.stringOption(opt_name, opt_value, "preserveAspectRatio", str)) {
    CHAlignType halign;
    CVAlignType valign;
    CSVGScale   scale;

    if (! svg_.decodePreserveAspectRatio(opt_value, &halign, &valign, &scale))
      return false;

    halign_ = halign;
    valign_ = valign;
    scale_  = scale;
  }
  else if (svg_.stringOption(opt_name, opt_value, "zoomAndPan", str))
    ;
  else if (svg_.stringOption(opt_name, opt_value, "version", str))
    ;
  else if (svg_.stringOption(opt_name, opt_value, "baseProfile", str))
    ;
  else if (svg_.stringOption(opt_name, opt_value, "contentScriptType", str))
    ;
  else if (svg_.stringOption(opt_name, opt_value, "contentStyleType", str))
    ;
  else if (CRegExpUtil::parse(opt_name, "xmlns:.*")) {
    //notHandled(opt_name, opt_value);
  }
  else if (CRegExpUtil::parse(opt_name, "sodipodi:.*")) {
    //notHandled(opt_name, opt_value);
  }
  else if (CRegExpUtil::parse(opt_name, "inkscape:.*")) {
    //notHandled(opt_name, opt_value);
  }
  else
    return false;

  return true;
}

double
CSVGBlock::
getX() const
{
  return (x_.isValid() ? x_.getValue() : 0.0);
}

double
CSVGBlock::
getY() const
{
  return (y_.isValid() ? y_.getValue() : 0.0);
}

double
CSVGBlock::
getWidth() const
{
  if      (width_.isValid())
    return width_.getValue().value();
  else if (height_.isValid())
    return height_.getValue().value();
  else
    return 400;
}

double
CSVGBlock::
getHeight() const
{
  if      (height_.isValid())
    return height_.getValue().value();
  else if (width_.isValid())
    return width_.getValue().value();
  else
    return 400;
}

void
CSVGBlock::
setSize(const CSize2D &size)
{
  width_ .setValue(CSVGLengthValue(size.getWidth ()));
  height_.setValue(CSVGLengthValue(size.getHeight()));
}

void
CSVGBlock::
draw()
{
  if (svg_.getDebug())
    CSVGLog() << *this;
}

bool
CSVGBlock::
getBBox(CBBox2D &bbox) const
{
  if (! viewBox_.isSet())
    bbox = CBBox2D(getX(), getY(), getWidth(), getHeight());
  else
    bbox = viewBox_;

  return true;
}

void
CSVGBlock::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<svg";

    printNameValue(os, "id", id_);

    printNameLength(os, "width" , width_ );
    printNameLength(os, "height", height_);

    if (viewBox_.isSet())
      os << " viewBox=\"" << viewBox_.getXMin() << " " << viewBox_.getYMin() <<
            " " << viewBox_.getXMax() << " " << viewBox_.getYMax() << "\"";

    if (halign_.isValid()) {
      os << " preserveAspectRatio=\"";

      if      (getHAlign() == CHALIGN_TYPE_LEFT  ) os << "xMin";
      else if (getHAlign() == CHALIGN_TYPE_CENTER) os << "xMid";
      else if (getHAlign() == CHALIGN_TYPE_RIGHT ) os << "xMax";

      if      (getVAlign() == CVALIGN_TYPE_BOTTOM) os << "YMin";
      else if (getVAlign() == CVALIGN_TYPE_CENTER) os << "YMid";
      else if (getVAlign() == CVALIGN_TYPE_TOP   ) os << "YMax";

      os << " ";

      if      (getScale() == CSVGScale::FIXED_MEET ) os << "meet";
      else if (getScale() == CSVGScale::FIXED_SLICE) os << "slice";
      else if (getScale() == CSVGScale::FREE       ) os << "none";

      os << "\"";
    }

    os << ">" << std::endl;

    for (auto &o : objects_)
      o->print(os, hier);

    os << "</svg>" << std::endl;
  }
  else
    os << "svg " << getWidth() << " " << getHeight();
}

std::ostream &
operator<<(std::ostream &os, const CSVGBlock &block)
{
  block.print(os, false);

  return os;
}

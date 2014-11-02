#include <CSVGI.h>
#include <CRegExp.h>

CSVGBlock::
CSVGBlock(CSVG &svg) :
 CSVGObject(svg),
 x_        (),
 y_        (),
 width_    (),
 height_   (),
 halign_   (CHALIGN_TYPE_CENTER),
 valign_   (CVALIGN_TYPE_CENTER),
 scale_    (CSVG_SCALE_FREE)
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
processOption(const string &opt_name, const string &opt_value)
{
  string  str;
  double  real;
  CBBox2D bbox;

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
  else if (svg_.lengthOption(opt_name, opt_value, "width", &real))
    width_.setValue(real);
  else if (svg_.lengthOption(opt_name, opt_value, "height", &real))
    height_.setValue(real);
  else if (svg_.bboxOption  (opt_name, opt_value, "viewBox", &bbox))
    view_box_ = bbox;
  else if (svg_.stringOption(opt_name, opt_value, "preserveAspectRatio", str)) {
    if (! svg_.decodePreserveAspectRatio(opt_value, &halign_, &valign_,
                                         &scale_))
      return false;
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
    return width_.getValue();
  else if (height_.isValid())
    return height_.getValue();
  else
    return 400;
}

double
CSVGBlock::
getHeight() const
{
  if      (height_.isValid())
    return height_.getValue();
  else if (width_.isValid())
    return width_.getValue();
  else
    return 400;
}

void
CSVGBlock::
setSize(const CSize2D &size)
{
  width_ .setValue(size.getWidth ());
  height_.setValue(size.getHeight());
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
  if (! view_box_.isSet())
    bbox = CBBox2D(getX(), getY(), getWidth(), getHeight());
  else
    bbox = view_box_;

  return true;
}

void
CSVGBlock::
print(ostream &os) const
{
  os << "svg " << getWidth() << " " << getHeight();
}

ostream &
operator<<(ostream &os, const CSVGBlock &block)
{
  block.print(os);

  return os;
}

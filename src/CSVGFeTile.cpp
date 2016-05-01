#include <CSVGFeTile.h>
#include <CSVGFilter.h>
#include <CSVGBuffer.h>
#include <CSVGUtil.h>
#include <CSVG.h>

CSVGFeTile::
CSVGFeTile(CSVG &svg) :
 CSVGFilterBase(svg)
{
}

CSVGFeTile::
CSVGFeTile(const CSVGFeTile &fe) :
 CSVGFilterBase(fe),
 filterIn_ (fe.filterIn_),
 filterOut_(fe.filterOut_)
{
}

CSVGFeTile *
CSVGFeTile::
dup() const
{
  return new CSVGFeTile(*this);
}

bool
CSVGFeTile::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  std::string str;

  if      (svg_.stringOption(opt_name, opt_value, "in", str))
    filterIn_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "result", str))
    filterOut_ = str;
  else
    return CSVGFilterBase::processOption(opt_name, opt_value);

  return true;
}

void
CSVGFeTile::
draw()
{
  CSVGBuffer *inBuffer  = svg_.getBuffer(getFilterIn ());
  CSVGBuffer *outBuffer = svg_.getBuffer(getFilterOut());

  if (svg_.getDebugFilter()) {
    std::string objectBufferName = "_" + getUniqueName();

    CSVGBuffer *buffer = svg_.getBuffer(objectBufferName + "_in");

    buffer->setImage(inBuffer->getImage());
    buffer->setBBox (inBuffer->bbox());
  }

  filterImage(inBuffer, outBuffer);

  if (svg_.getDebugFilter()) {
    std::string objectBufferName = "_" + getUniqueName();

    CSVGBuffer *buffer = svg_.getBuffer(objectBufferName + "_out");

    buffer->setImage(outBuffer->getImage());
    buffer->setBBox (outBuffer->bbox());
  }
}

void
CSVGFeTile::
filterImage(CSVGBuffer *inBuffer, CSVGBuffer *outBuffer)
{
  CImagePtr inImage = inBuffer->getImage();

  CBBox2D inBBox = inBuffer->bbox();

  if (inBBox.isSet()) {
    double pw, ph;

    svg_.lengthToPixel(inBBox.getWidth(), inBBox.getHeight(), &pw, &ph);

    inImage = inImage->subImage(0, 0, pw, ph);
  }

  // get filtered object size
  int w = inImage->getWidth ();
  int h = inImage->getHeight();

  CBBox2D bbox;

  if (getParentBBox(bbox)) {
    double pw, ph;

    svg_.lengthToPixel(bbox.getWidth(), bbox.getHeight(), &pw, &ph);

    w = CSVGUtil::round(pw);
    h = CSVGUtil::round(ph);
  }

  // tile
  CImagePtr outImage = inImage->tile(w, h);

  outBuffer->setImage(outImage);
  outBuffer->setBBox (bbox);
}

void
CSVGFeTile::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<feTile";

    CSVGObject::printValues(os);

    printNameValue(os, "in"    , filterIn_ );
    printNameValue(os, "result", filterOut_);

    os << "/>" << std::endl;
  }
  else
    os << "feTile ";
}

std::ostream &
operator<<(std::ostream &os, const CSVGFeTile &fe)
{
  fe.print(os, false);

  return os;
}

#include <CSVGFeDisplacementMap.h>
#include <CSVGBuffer.h>
#include <CSVG.h>

CSVGFeDisplacementMap::
CSVGFeDisplacementMap(CSVG &svg) :
 CSVGFilterBase(svg)
{
}

CSVGFeDisplacementMap::
CSVGFeDisplacementMap(const CSVGFeDisplacementMap &fe) :
 CSVGFilterBase(fe)
{
}

CSVGFeDisplacementMap *
CSVGFeDisplacementMap::
dup() const
{
  return new CSVGFeDisplacementMap(*this);
}

bool
CSVGFeDisplacementMap::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  std::string str;
  double      r;

  if      (svg_.stringOption(opt_name, opt_value, "class", str))
    class_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "style", str))
    style_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "in", str))
    filterIn1_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "in2", str))
    filterIn2_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "result", str))
    filterOut_ = str;
  else if (svg_.realOption(opt_name, opt_value, "scale", &r))
    scale_ = r;
  else if (svg_.stringOption(opt_name, opt_value, "xChannelSelector", str))
    xChannelSelector_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "yChannelSelector", str))
    yChannelSelector_ = str;
  else
    return CSVGFilterBase::processOption(opt_name, opt_value);

  return true;
}

void
CSVGFeDisplacementMap::
draw()
{
  CSVGBuffer *inBuffer1 = svg_.getBuffer(getFilterIn1());
  CSVGBuffer *inBuffer2 = svg_.getBuffer(getFilterIn2());
  CSVGBuffer *outBuffer = svg_.getBuffer(getFilterOut());

  if (svg_.getDebugFilter()) {
    std::string objectBufferName = "_" + getUniqueName();

    CSVGBuffer *buffer1 = svg_.getBuffer(objectBufferName + "_in1");
    CSVGBuffer *buffer2 = svg_.getBuffer(objectBufferName + "_in2");

    buffer1->setImage(inBuffer1->getImage());
    buffer2->setImage(inBuffer2->getImage());
  }

  filterImage(inBuffer1, inBuffer2, outBuffer);

  if (svg_.getDebugFilter()) {
    std::string objectBufferName = "_" + getUniqueName();

    CSVGBuffer *buffer = svg_.getBuffer(objectBufferName + "_out");

    buffer->setImage(outBuffer->getImage());
  }
}

void
CSVGFeDisplacementMap::
filterImage(CSVGBuffer *inBuffer1, CSVGBuffer *inBuffer2, CSVGBuffer *outBuffer)
{
  CImagePtr src_image1 = inBuffer1->getImage();
  CImagePtr src_image2 = inBuffer2->getImage();

  CColorComponent xcolor = CCOLOR_COMPONENT_RED;
  CColorComponent ycolor = CCOLOR_COMPONENT_RED;

  std::string xchannel = getXChannel();
  std::string ychannel = getYChannel();

  if      (xchannel == "R") xcolor = CCOLOR_COMPONENT_RED;
  else if (xchannel == "G") xcolor = CCOLOR_COMPONENT_GREEN;
  else if (xchannel == "B") xcolor = CCOLOR_COMPONENT_BLUE;
  else if (xchannel == "A") xcolor = CCOLOR_COMPONENT_ALPHA;

  if      (ychannel == "R") ycolor = CCOLOR_COMPONENT_RED;
  else if (ychannel == "G") ycolor = CCOLOR_COMPONENT_GREEN;
  else if (ychannel == "B") ycolor = CCOLOR_COMPONENT_BLUE;
  else if (ychannel == "A") ycolor = CCOLOR_COMPONENT_ALPHA;

  double s = getScale();

  CImagePtr dst_image = src_image1->displacementMap(src_image2, xcolor, ycolor, s);

#if 0
  CImagePtr dst_image = src_image1->dup();

  int w = src_image1->getWidth ();
  int h = src_image1->getHeight();

  for (int y = 0; y < h; ++y) {
    for (int x = 0; x < w; ++x) {
      CRGBA rgba1;

      src_image1->getRGBAPixel(x, y, rgba1);

      if (! src_image2->validPixel(x, y))
        continue;

      CRGBA rgba2;

      src_image2->getRGBAPixel(x, y, rgba2);

      double rx = rgba2.getComponent(xcolor);
      double ry = rgba2.getComponent(ycolor);

      double x1 = x + rx*s;
      double y1 = y + ry*s;

      if (dst_image->validPixel(x1, y1))
        dst_image->setRGBAPixel(x1, y1, rgba1);
    }
  }
#endif

  outBuffer->setImage(dst_image);
}

void
CSVGFeDisplacementMap::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<feDisplacementMap";

    printNameValue(os, "class"           , class_);
    printNameValue(os, "style"           , style_);
    printNameValue(os, "in"              , filterIn1_);
    printNameValue(os, "in2"             , filterIn2_);
    printNameValue(os, "scale"           , scale_);
    printNameValue(os, "xChannelSelector", xChannelSelector_);
    printNameValue(os, "yChannelSelector", yChannelSelector_);

    CSVGObject::printValues(os);

    os << "/>" << std::endl;
  }
  else
    os << "feDisplacementMap ";
}

std::ostream &
operator<<(std::ostream &os, const CSVGFeDisplacementMap &filter)
{
  filter.print(os, false);

  return os;
}

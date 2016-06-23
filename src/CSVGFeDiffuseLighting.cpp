#include <CSVGFeDiffuseLighting.h>
#include <CSVGFeDistantLight.h>
#include <CSVGFePointLight.h>
#include <CSVGFeSpotLight.h>
#include <CSVGBuffer.h>
#include <CSVG.h>

CSVGFeDiffuseLighting::
CSVGFeDiffuseLighting(CSVG &svg) :
 CSVGFeLighting(svg)
{
}

CSVGFeDiffuseLighting::
CSVGFeDiffuseLighting(const CSVGFeDiffuseLighting &fe) :
 CSVGFeLighting(fe)
{
}

CSVGFeDiffuseLighting *
CSVGFeDiffuseLighting::
dup() const
{
  return new CSVGFeDiffuseLighting(*this);
}

bool
CSVGFeDiffuseLighting::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  double      r;
  std::string str;

  if      (svg_.stringOption(opt_name, opt_value, "in", str))
    filterIn_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "result", str))
    filterOut_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "lighting-color", str))
    lightingColor_ = svg_.nameToColor(str);
  else if (svg_.realOption(opt_name, opt_value, "diffuseConstant", &r))
    diffuseConstant_ = r;
  else if (svg_.realOption(opt_name, opt_value, "surfaceScale", &r))
    surfaceScale_ = r;
  else
    return CSVGFilterBase::processOption(opt_name, opt_value);

  return true;
}

bool
CSVGFeDiffuseLighting::
drawElement()
{
  CSVGBuffer *inBuffer  = svg_.getBuffer(getFilterIn ());
  CSVGBuffer *outBuffer = svg_.getBuffer(getFilterOut());

  bool inDrawing = inBuffer->isDrawing();

  if (inDrawing) inBuffer->stopDraw();

  //---

  // get filtered object coords
  CBBox2D inBBox;

  getBufferSubRegion(inBuffer, inBBox);

  //---

  if (svg_.getDebugFilter()) {
    std::string objectBufferName = "_" + getUniqueName();

    CSVGBuffer *buffer = svg_.getBuffer(objectBufferName + "_in");

    buffer->setImageBuffer(inBuffer);
    buffer->setBBox       (inBBox);
  }

  //---

  filterImage(inBuffer, inBBox, outBuffer);

  outBuffer->setBBox(inBBox);

  //---

  if (svg_.getDebugFilter()) {
    std::string objectBufferName = "_" + getUniqueName();

    CSVGBuffer *buffer = svg_.getBuffer(objectBufferName + "_out");

    buffer->setImageBuffer(outBuffer);
    buffer->setBBox       (inBBox);
  }

  //---

  if (inDrawing) inBuffer->startDraw();

  return true;
}

void
CSVGFeDiffuseLighting::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<feDiffuseLighting";

    CSVGObject::printValues(os);

    printNameValue(os, "in"             , filterIn_ );
    printNameValue(os, "result"         , filterOut_);
    printNameValue(os, "lighting-color" , lightingColor_);
    printNameValue(os, "diffuseConstant", diffuseConstant_);
    printNameValue(os, "surfaceScale"   , surfaceScale_);

    if (hasChildren()) {
      os << ">" << std::endl;

      printChildren(os, hier);

      os << "</feDiffuseLighting>" << std::endl;
    }
    else
      os << "/>" << std::endl;
  }
  else
    os << "feDiffuseLighting ";
}

std::ostream &
operator<<(std::ostream &os, const CSVGFeDiffuseLighting &fe)
{
  fe.print(os, false);

  return os;
}

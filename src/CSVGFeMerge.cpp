#include <CSVGFeMerge.h>
#include <CSVGFeMergeNode.h>
#include <CSVG.h>
#include <CSVGBuffer.h>

CSVGFeMerge::
CSVGFeMerge(CSVG &svg) :
 CSVGFilterBase(svg)
{
}

CSVGFeMerge::
CSVGFeMerge(const CSVGFeMerge &merge) :
 CSVGFilterBase(merge),
 filterIn_ (merge.filterIn_),
 filterOut_(merge.filterOut_)
{
}

CSVGFeMerge *
CSVGFeMerge::
dup() const
{
  return new CSVGFeMerge(*this);
}

bool
CSVGFeMerge::
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
CSVGFeMerge::
draw()
{
  CSVGBuffer *outBuffer = svg_.getBuffer(getFilterOut());

  filterImage(outBuffer);

  if (svg_.getDebugFilter()) {
    std::string objectBufferName = "_" + getUniqueName();

    CSVGBuffer *buffer = svg_.getBuffer(objectBufferName + "_out");

    buffer->setImage(outBuffer->getImage());
  }
}

void
CSVGFeMerge::
filterImage(CSVGBuffer *outBuffer)
{
  std::vector<CSVGObject *> objects;

  getChildrenOfType(CSVGObjTypeId::FE_MERGE_NODE, objects);

  // get max image size
  int w = 0, h = 0;

  for (const auto &o : objects) {
    CSVGFeMergeNode *node = dynamic_cast<CSVGFeMergeNode *>(o);

    std::string filterIn = node->getFilterIn();
    CSVGBuffer* bufferIn = svg_.getBuffer(filterIn);

    CISize2D size = bufferIn->getImageSize();

    w = std::max(w, size.getWidth ());
    h = std::max(h, size.getHeight());
  }

  //---

  // create image to merge into
  CImageNoSrc src;

  CImagePtr dst_image = CImageMgrInst->createImage(src);

  dst_image->setDataSize(w, h);

  dst_image->setRGBAData(CRGBA(0,0,0,0));

  //---

  // add merge node images
  int i = 1;

  for (const auto &o : objects) {
    CSVGFeMergeNode *node = dynamic_cast<CSVGFeMergeNode *>(o);

    std::string filterIn = node->getFilterIn();
    CSVGBuffer* bufferIn = svg_.getBuffer(filterIn);

    if (svg_.getDebugFilter()) {
      std::string objectBufferName = "_" + getUniqueName();

      CSVGBuffer *buffer = svg_.getBuffer(objectBufferName + "_node_" + std::to_string(i) + "_in");

      buffer->setImage(bufferIn->getImage());
    }

    //---

    CImagePtr imageIn = bufferIn->getImage();

    dst_image->combine(imageIn);

    //---

    if (svg_.getDebugFilter()) {
      std::string objectBufferName = "_" + getUniqueName();

      CSVGBuffer *buffer = svg_.getBuffer(objectBufferName + "_node_" + std::to_string(i) + "_out");

      buffer->setImage(dst_image);
    }

    ++i;
  }

  outBuffer->setImage(dst_image);
}

void
CSVGFeMerge::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<feMerge";

    CSVGObject::printValues(os);

    printNameValue(os, "in"    , filterIn_ );
    printNameValue(os, "result", filterOut_);

    os << ">" << std::endl;

    printChildren(os, hier);

    os << "</feMerge>" << std::endl;
  }
  else
    os << "feMerge";
}

std::ostream &
operator<<(std::ostream &os, const CSVGFeMerge &filter)
{
  filter.print(os, false);

  return os;
}

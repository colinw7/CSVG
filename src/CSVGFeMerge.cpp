#include <CSVGFeMerge.h>
#include <CSVGFeMergeNode.h>
#include <CSVGFilter.h>
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

std::string
CSVGFeMerge::
getFilterIn() const
{
  return calcFilterIn(filterIn_);
}

std::string
CSVGFeMerge::
getFilterOut() const
{
  return calcFilterOut(filterOut_);
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

bool
CSVGFeMerge::
drawElement()
{
  CSVGBuffer *outBuffer = svg_.getBuffer(getFilterOut());

  //---

  // get merge nodes
  std::vector<CSVGObject *> objects;

  getChildrenOfType(CSVGObjTypeId::FE_MERGE_NODE, objects);

  std::vector<CSVGFeMergeNode *> nodes;

  for (const auto &o : objects) {
    auto *node = dynamic_cast<CSVGFeMergeNode *>(o);

    nodes.push_back(node);
  }

  //---

  // get max image size
  int w = 0, h = 0;

  for (const auto &node : nodes) {
    std::string filterIn = node->getFilterIn();
    CSVGBuffer* bufferIn = svg_.getBuffer(filterIn);

    CISize2D size = bufferIn->getImageSize();

    w = std::max(w, size.getWidth ());
    h = std::max(h, size.getHeight());
  }

  //---

  // merge buffers
  CSVGBuffer::mergeBuffers(nodes, w, h, outBuffer);

  //---

  if (svg_.getDebugFilter()) {
    std::string objectBufferName = "_" + getUniqueName();

    CSVGBuffer *buffer = svg_.getBuffer(objectBufferName + "_out");

    buffer->setImageBuffer(outBuffer);
  }

  return true;
}

void
CSVGFeMerge::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<feMerge";

    printValues(os);

    os << ">" << std::endl;

    printChildren(os, hier);

    os << "</feMerge>" << std::endl;
  }
  else
    os << "feMerge";
}

void
CSVGFeMerge::
printValues(std::ostream &os, bool flat) const
{
  CSVGObject::printValues(os, flat);

  CSVGFilterBase::printValues(os, flat);

  printNameValue(os, "in"    , filterIn_ );
  printNameValue(os, "result", filterOut_);
}

std::ostream &
operator<<(std::ostream &os, const CSVGFeMerge &filter)
{
  filter.print(os, false);

  return os;
}

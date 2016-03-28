#include <CSVGFeMerge.h>
#include <CSVGFeMergeNode.h>
#include <CSVG.h>

CSVGFeMerge::
CSVGFeMerge(CSVG &svg) :
 CSVGFilter(svg)
{
}

CSVGFeMerge::
CSVGFeMerge(const CSVGFeMerge &merge) :
 CSVGFilter (merge),
 filter_in_ (merge.filter_in_),
 filter_out_(merge.filter_out_)
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
    filter_in_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "result", str))
    filter_out_ = str;
  else
    return CSVGFilter::processOption(opt_name, opt_value);

  return true;
}

void
CSVGFeMerge::
draw()
{
  CImagePtr src_image;

  CImagePtr dst_image = filterImage(src_image);

  svg_.setBufferImage(filter_out_.getValue("SourceGraphic"), dst_image);
}

CImagePtr
CSVGFeMerge::
filterImage(CImagePtr)
{
  std::vector<CSVGObject *> objects;

  getChildrenOfType(CSVGObjTypeId::FE_MERGE_NODE, objects);

  uint w = 0, h = 0;

  for (const auto &o : objects) {
    CSVGFeMergeNode *node = dynamic_cast<CSVGFeMergeNode *>(o);

    const std::string &filter_in = node->getFilterIn();

    CImagePtr image_in = svg_.getBufferImage(filter_in);

    w = std::max(w, image_in->getWidth ());
    h = std::max(h, image_in->getHeight());
  }

  CImageNoSrc src;

  CImagePtr dst_image = CImageMgrInst->createImage(src);

  dst_image->setDataSize(w, h);

  dst_image->setRGBAData(CRGBA(1,1,1,1));

  for (const auto &o : objects) {
    CSVGFeMergeNode *node = dynamic_cast<CSVGFeMergeNode *>(o);

    const std::string &filter_in = node->getFilterIn();

    CImagePtr image_in = svg_.getBufferImage(filter_in);

    dst_image->combine(image_in);
  }

  return dst_image;
}

void
CSVGFeMerge::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<feMerge";

    CSVGObject::printValues(os);

    if (filter_in_.isValid())
      os << " in=\"" << filter_in_.getValue() << "\"";

    if (filter_out_.isValid())
      os << " result=\"" << filter_out_.getValue() << "\"";

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

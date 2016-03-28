#include <CSVGFeMergeNode.h>
#include <CSVG.h>

CSVGFeMergeNode::
CSVGFeMergeNode(CSVG &svg) :
 CSVGObject(svg)
{
}

CSVGFeMergeNode::
CSVGFeMergeNode(const CSVGFeMergeNode &merge) :
 CSVGObject (merge),
 filter_in_ (merge.filter_in_),
 filter_out_(merge.filter_out_)
{
}

CSVGFeMergeNode *
CSVGFeMergeNode::
dup() const
{
  return new CSVGFeMergeNode(*this);
}

bool
CSVGFeMergeNode::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  std::string str;

  if      (svg_.stringOption(opt_name, opt_value, "in", str))
    filter_in_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "result", str))
    filter_out_ = str;
  else
    return CSVGObject::processOption(opt_name, opt_value);

  return true;
}

void
CSVGFeMergeNode::
draw()
{
  CImagePtr src_image = svg_.getBufferImage(filter_in_ .getValue("SourceGraphic"));
  CImagePtr dst_image = svg_.getBufferImage(filter_out_.getValue("SourceGraphic"));

  CImagePtr dst_image1 = dst_image;

  if (src_image == dst_image)
    dst_image1 = dst_image->dup();

  if (src_image == dst_image) {
    dst_image1->copyFrom(src_image);

    dst_image->copyFrom(dst_image1);
  }
  else
    dst_image->copyFrom(src_image);
}

void
CSVGFeMergeNode::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<feMergeNode";

    CSVGObject::printValues(os);

    if (filter_in_.isValid())
      os << " in=\"" << filter_in_.getValue() << "\"";

    if (filter_out_.isValid())
      os << " result=\"" << filter_out_.getValue() << "\"";

    os << "/>" << std::endl;
  }
  else
    os << "feMergeNode";
}

std::ostream &
operator<<(std::ostream &os, const CSVGFeMergeNode &filter)
{
  filter.print(os, false);

  return os;
}

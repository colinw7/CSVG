#include "CSVGI.h"

CSVGFeMergeNode::
CSVGFeMergeNode(CSVG &svg) :
 CSVGObject (svg),
 filter_in_ ("SourceGraphic"),
 filter_out_("SourceGraphic")
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
processOption(const string &opt_name, const string &opt_value)
{
  string str;

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
  CImagePtr src_image = svg_.getBufferImage(filter_in_);
  CImagePtr dst_image = svg_.getBufferImage(filter_out_);

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
print(ostream &os) const
{
  os << "feMergeNode";
}

ostream &
operator<<(ostream &os, const CSVGFeMergeNode &filter)
{
  filter.print(os);

  return os;
}

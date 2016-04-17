#include <CSVGFeMergeNode.h>
#include <CSVG.h>

CSVGFeMergeNode::
CSVGFeMergeNode(CSVG &svg) :
 CSVGObject(svg)
{
}

CSVGFeMergeNode::
CSVGFeMergeNode(const CSVGFeMergeNode &merge) :
 CSVGObject(merge),
 filterIn_ (merge.filterIn_),
 filterOut_(merge.filterOut_)
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
    filterIn_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "result", str))
    filterOut_ = str;
  else
    return CSVGObject::processOption(opt_name, opt_value);

  return true;
}

void
CSVGFeMergeNode::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<feMergeNode";

    CSVGObject::printValues(os);

    if (filterIn_.isValid())
      os << " in=\"" << filterIn_.getValue() << "\"";

    if (filterOut_.isValid())
      os << " result=\"" << filterOut_.getValue() << "\"";

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

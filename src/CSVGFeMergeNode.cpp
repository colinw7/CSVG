#include <CSVGFeMergeNode.h>
#include <CSVGFeMerge.h>
#include <CSVGFilter.h>
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

std::string
CSVGFeMergeNode::
getFilterIn() const
{
  if (filterIn_.isValid())
    return filterIn_.getValue();

  auto *filter = getParentFilter();

  return (filter ? filter->getLastFilterName() : "FilterGraphic");
}

std::string
CSVGFeMergeNode::
getFilterOut() const
{
  auto *filter = getParentFilter();

  std::string name = "FilterGraphic";

  if (filterOut_.isValid())
    name = filterOut_.getValue();

  if (filter)
    filter->setLastFilterName(name);

  return name;
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

CSVGFilter *
CSVGFeMergeNode::
getParentFilter() const
{
  auto *merge = dynamic_cast<CSVGFeMerge *>(getParent());

  return (merge ? merge->getParentFilter() : nullptr);
}

void
CSVGFeMergeNode::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<feMergeNode";

    printValues(os);

    os << "/>" << std::endl;
  }
  else
    os << "feMergeNode";
}

void
CSVGFeMergeNode::
printValues(std::ostream &os, bool flat) const
{
  CSVGObject::printValues(os, flat);

  if (filterIn_.isValid())
    os << " in=\"" << filterIn_.getValue() << "\"";

  if (filterOut_.isValid())
    os << " result=\"" << filterOut_.getValue() << "\"";
}

#include <CQSVGFeMergeNode.h>
#include <CQSVG.h>

CQSVGFeMergeNode::
CQSVGFeMergeNode(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGFeMergeNode(*svg)
{
}

void
CQSVGFeMergeNode::
addProperties(CQPropertyTree *propTree, const std::string &name)
{
  CQSVGObject::addProperties(propTree, name);
}

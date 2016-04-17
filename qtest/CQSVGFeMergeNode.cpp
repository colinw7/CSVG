#include <CQSVGFeMergeNode.h>
#include <CQSVG.h>

CQSVGFeMergeNode::
CQSVGFeMergeNode(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGFeMergeNode(*svg)
{
}

#include <CQSVGFeImage.h>
#include <CQSVG.h>

CQSVGFeImage::
CQSVGFeImage(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGFeImage(*svg)
{
}

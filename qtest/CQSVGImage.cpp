#include <CQSVGImage.h>
#include <CQSVG.h>

CQSVGImage::
CQSVGImage(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGImage(*svg)
{
}

void
CQSVGImage::
drawTerm()
{
  drawSelected();
}

#include <CQSVGPathPart.h>
#include <CQSVG.h>

CQSVGPathPart::
CQSVGPathPart()
{
}

CQSVGPathMoveTo::
CQSVGPathMoveTo(CQSVG &svg, double x, double y) :
 CSVGPathMoveTo(svg, x, y)
{
}

CQSVGPathRMoveTo::
CQSVGPathRMoveTo(CQSVG &svg, double x, double y) :
 CSVGPathRMoveTo(svg, x, y)
{
}

CQSVGPathLineTo::
CQSVGPathLineTo(CQSVG &svg, double x, double y) :
 CSVGPathLineTo(svg, x, y)
{
}

CQSVGPathRLineTo::
CQSVGPathRLineTo(CQSVG &svg, double x, double y) :
 CSVGPathRLineTo(svg, x, y)
{
}

CQSVGPathHLineTo::
CQSVGPathHLineTo(CQSVG &svg, double x) :
 CSVGPathHLineTo(svg, x)
{
}

CQSVGPathRHLineTo::
CQSVGPathRHLineTo(CQSVG &svg, double dx) :
 CSVGPathRHLineTo(svg, dx)
{
}

CQSVGPathVLineTo::
CQSVGPathVLineTo(CQSVG &svg, double y) :
 CSVGPathVLineTo(svg, y)
{
}

CQSVGPathRVLineTo::
CQSVGPathRVLineTo(CQSVG &svg, double dy) :
 CSVGPathRVLineTo(svg, dy)
{
}

CQSVGPathArcTo::
CQSVGPathArcTo(CQSVG &svg, double rx, double ry, double xa, int fa, int fs, double x2, double y2) :
 CSVGPathArcTo(svg, rx, ry, xa, fa, fs, x2, y2)
{
}

CQSVGPathRArcTo::
CQSVGPathRArcTo(CQSVG &svg, double rx, double ry, double xa, int fa, int fs, double x2, double y2) :
 CSVGPathRArcTo(svg, rx, ry, xa, fa, fs, x2, y2)
{
}

CQSVGPathBezier2To::
CQSVGPathBezier2To(CQSVG &svg, double x1, double y1, double x2, double y2) :
 CSVGPathBezier2To(svg, x1, y1, x2, y2)
{
}

CQSVGPathMBezier2To::
CQSVGPathMBezier2To(CQSVG &svg, double x2, double y2) :
 CSVGPathMBezier2To(svg, x2, y2)
{
}

CQSVGPathRBezier2To::
CQSVGPathRBezier2To(CQSVG &svg, double x1, double y1, double x2, double y2) :
 CSVGPathRBezier2To(svg, x1, y1, x2, y2)
{
}

CQSVGPathMRBezier2To::
CQSVGPathMRBezier2To(CQSVG &svg, double x2, double y2) :
 CSVGPathMRBezier2To(svg, x2, y2)
{
}

CQSVGPathBezier3To::
CQSVGPathBezier3To(CQSVG &svg, double x1, double y1, double x2, double y2,
                   double x3, double y3) :
 CSVGPathBezier3To(svg, x1, y1, x2, y2, x3, y3)
{
}

CQSVGPathMBezier3To::
CQSVGPathMBezier3To(CQSVG &svg, double x2, double y2, double x3, double y3) :
 CSVGPathMBezier3To(svg, x2, y2, x3, y3)
{
}

CQSVGPathRBezier3To::
CQSVGPathRBezier3To(CQSVG &svg, double x1, double y1, double x2, double y2,
                    double x3, double y3) :
 CSVGPathRBezier3To(svg, x1, y1, x2, y2, x3, y3)
{
}

CQSVGPathMRBezier3To::
CQSVGPathMRBezier3To(CQSVG &svg, double x2, double y2, double x3, double y3) :
 CSVGPathMRBezier3To(svg, x2, y2, x3, y3)
{
}

CQSVGPathClosePath::
CQSVGPathClosePath(CQSVG &svg, bool relative) :
 CSVGPathClosePath(svg, relative)
{
}

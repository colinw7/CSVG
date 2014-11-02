#include <CSVGI.h>

CSVGSymbol::
CSVGSymbol(CSVG &svg) :
 CSVGObject(svg)
{
}

CSVGSymbol::
CSVGSymbol(const CSVGSymbol &symbol) :
 CSVGObject(symbol)
{
}

CSVGSymbol *
CSVGSymbol::
dup() const
{
  return new CSVGSymbol(*this);
}

/* Attributes:
    <Core>
    <Style>
    <Presentation>
    <GraphicalEvents>
    <External>
    viewBox <ViewBoxSpec>
    preserveAspectRatio <PreserveAspectRatioSpec>
*/
bool
CSVGSymbol::
processOption(const string &opt_name, const string &opt_value)
{
  return CSVGObject::processOption(opt_name, opt_value);
}

void
CSVGSymbol::
moveBy(const CVector2D &delta)
{
  ObjectList::iterator po1, po2;

  for (po1 = childrenBegin(), po2 = childrenEnd(); po1 != po2; ++po1)
    (*po1)->moveBy(delta);
}

void
CSVGSymbol::
resizeTo(const CSize2D &size)
{
  ObjectList::iterator po1, po2;

  for (po1 = childrenBegin(), po2 = childrenEnd(); po1 != po2; ++po1)
    (*po1)->resizeTo(size);
}

void
CSVGSymbol::
rotateBy(double da, const CPoint2D &c)
{
  ObjectList::iterator po1, po2;

  for (po1 = childrenBegin(), po2 = childrenEnd(); po1 != po2; ++po1)
    (*po1)->rotateBy(da, c);
}

void
CSVGSymbol::
draw()
{
}

void
CSVGSymbol::
print(ostream &os) const
{
  os << "symbol";
}

ostream &
operator<<(ostream &os, const CSVGSymbol &group)
{
  group.print(os);

  return os;
}

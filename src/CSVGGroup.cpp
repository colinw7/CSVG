#include "CSVGI.h"

CSVGGroup::
CSVGGroup(CSVG &svg) :
 CSVGObject(svg)
{
}

CSVGGroup::
CSVGGroup(const CSVGGroup &group) :
 CSVGObject(group)
{
}

CSVGGroup *
CSVGGroup::
dup() const
{
  return new CSVGGroup(*this);
}

/* Attributes:
    <Core>
    <Conditional>
    <Style>
    <Presentation>
    <GraphicalEvents>
    <External>
    transform <TransformList>
*/
bool
CSVGGroup::
processOption(const string &opt_name, const string &opt_value)
{
  return CSVGObject::processOption(opt_name, opt_value);
}

void
CSVGGroup::
moveBy(const CVector2D &delta)
{
  ObjectList::iterator po1, po2;

  for (po1 = childrenBegin(), po2 = childrenEnd(); po1 != po2; ++po1)
    (*po1)->moveBy(delta);
}

void
CSVGGroup::
resizeTo(const CSize2D &size)
{
  ObjectList::iterator po1, po2;

  for (po1 = childrenBegin(), po2 = childrenEnd(); po1 != po2; ++po1)
    (*po1)->resizeTo(size);
}

void
CSVGGroup::
rotateBy(double da, const CPoint2D &c)
{
  ObjectList::iterator po1, po2;

  for (po1 = childrenBegin(), po2 = childrenEnd(); po1 != po2; ++po1)
    (*po1)->rotateBy(da, c);
}

void
CSVGGroup::
draw()
{
}

void
CSVGGroup::
print(ostream &os) const
{
  os << "group";
}

ostream &
operator<<(ostream &os, const CSVGGroup &group)
{
  group.print(os);

  return os;
}

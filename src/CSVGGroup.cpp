#include <CSVGGroup.h>
#include <CSVGLog.h>
#include <CSVG.h>

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
processOption(const std::string &opt_name, const std::string &opt_value)
{
  if (processPresentationOption(opt_name, opt_value))
    return true;
  else
    return CSVGObject::processOption(opt_name, opt_value);
}

void
CSVGGroup::
draw()
{
  if (svg_.getDebug())
    CSVGLog() << *this;
}

void
CSVGGroup::
moveBy(const CVector2D &delta)
{
  for (auto &c : children())
    c->moveBy(delta);
}

void
CSVGGroup::
resizeTo(const CSize2D &size)
{
  for (auto &c : children())
    c->resizeTo(size);
}

void
CSVGGroup::
rotateBy(double da, const CPoint2D &origin)
{
  for (auto &c : children())
    c->rotateBy(da, origin);
}

void
CSVGGroup::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<g";

    CSVGObject::printValues(os);

    os << ">" << std::endl;

    printChildren(os, hier);

    os << "</g>" << std::endl;
  }
  else
    os << "group";
}

std::ostream &
operator<<(std::ostream &os, const CSVGGroup &group)
{
  group.print(os, false);

  return os;
}

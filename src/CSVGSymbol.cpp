#include <CSVGSymbol.h>
#include <CSVG.h>

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
processOption(const std::string &opt_name, const std::string &opt_value)
{
  return CSVGObject::processOption(opt_name, opt_value);
}

void
CSVGSymbol::
moveBy(const CVector2D &delta)
{
  for (auto &c : children())
    c->moveBy(delta);
}

void
CSVGSymbol::
resizeTo(const CSize2D &size)
{
  for (auto &c : children())
    c->resizeTo(size);
}

void
CSVGSymbol::
rotateBy(double da, const CPoint2D &origin)
{
  for (auto &c : children())
    c->rotateBy(da, origin);
}

void
CSVGSymbol::
draw()
{
}

void
CSVGSymbol::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    if (! objects_.empty()) {
      os << "<symbol";

      std::string id = getId();

      if (id != "")
        os << " id=\"" << id << "\"";

      os << ">" << std::endl;

      for (const auto &o : objects_)
        o->print(os, hier);

      os << "</symbol>" << std::endl;
    }
  }
  else
    os << "symbol";
}

std::ostream &
operator<<(std::ostream &os, const CSVGSymbol &group)
{
  group.print(os, false);

  return os;
}

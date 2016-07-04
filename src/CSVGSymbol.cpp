#include <CSVGSymbol.h>
#include <CSVG.h>

CSVGSymbol::
CSVGSymbol(CSVG &svg) :
 CSVGObject(svg)
{
}

CSVGSymbol::
CSVGSymbol(const CSVGSymbol &symbol) :
 CSVGObject(symbol),
 preserveAspect_(symbol.preserveAspect_)
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
  CSVGPreserveAspect preserveAspect;

  if (svg_.preserveAspectOption(opt_name, opt_value, "preserveAspectRatio", preserveAspect))
    preserveAspect_ = preserveAspect;
  else
    return CSVGObject::processOption(opt_name, opt_value);

  return true;
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
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<symbol";

    printValues(os);

    if (hasChildren()) {
      os << ">" << std::endl;

      printChildren(os, hier);

      os << "</symbol>" << std::endl;
    }
    else
      os << "/>" << std::endl;
  }
  else
    os << "symbol";
}

void
CSVGSymbol::
printValues(std::ostream &os, bool flat) const
{
  CSVGObject::printValues(os, flat);

  printNamePreserveAspect(os, "preserveAspectRatio", preserveAspect_);
}

std::ostream &
operator<<(std::ostream &os, const CSVGSymbol &group)
{
  group.print(os, false);

  return os;
}

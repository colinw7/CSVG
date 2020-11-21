#include <CSVGAnimate.h>
#include <CSVG.h>
#include <CSVGLog.h>
#include <CStrUtil.h>

/* Attributes:
    <Core>
    <Conditional>
    animation event attributes
    xlink attributes
    animation attribute target attributes
    animation timing attributes
    animation value attributes
    animation addition attributes
    <External>
*/

CSVGAnimate::
CSVGAnimate(CSVG &svg) :
 CSVGAnimateBase(svg)
{
}

CSVGAnimate::
CSVGAnimate(const CSVGAnimate &animate) :
 CSVGAnimateBase(animate)
{
}

CSVGAnimate *
CSVGAnimate::
dup() const
{
  return new CSVGAnimate(*this);
}

bool
CSVGAnimate::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  if (CSVGAnimateBase::processOption(opt_name, opt_value))
    return true;

  return false;
}

void
CSVGAnimate::
animate(double t)
{
  //std::cerr << "CSVGAnimate: " << currentTime_ << ":" << t << std::endl;
  if (! attributeName_.isValid())
    return;

  if      (from_.isValid() && to_.isValid()) {
    std::string ystr;

    if (getParent()->interpValue(getAttributeName(), getFrom(), getTo(), t, ystr)) {
      getParent()->processOption(getAttributeName(), ystr);

      svg_.redraw();
    }
  }
  else if (values_.isValid()) {
    std::vector<std::string> words;

    CStrUtil::addFields(values_.getValue(), words, ";");

    if (words.size() >= 2) {
      std::string ystr;

      std::string from = CStrUtil::stripSpaces(words[0]);
      std::string to   = CStrUtil::stripSpaces(words[1]);

      if (getParent()->interpValue(getAttributeName(), from, to, t, ystr)) {
        getParent()->processOption(getAttributeName(), ystr);

        svg_.redraw();
      }
    }
  }
}

void
CSVGAnimate::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<animate";

    CSVGAnimate::printValues(os);

    os << "/>" << std::endl;
  }
  else
    os << "animate ";
}

std::ostream &
operator<<(std::ostream &os, const CSVGAnimate &animate)
{
  animate.print(os, false);

  return os;
}

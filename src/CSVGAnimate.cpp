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
  //std::cerr << "CSVGAnimate: " << currentTime_ << ":" << t << "\n";
  if (! attributeName_)
    return;

  if      (from_ && to_) {
    std::string ystr;

    if (getParent()->interpValue(getAttributeName(), getFrom(), getTo(), t, ystr)) {
      getParent()->processOption(getAttributeName(), ystr);

      svg_.redraw();
    }
  }
  else if (values_) {
    std::vector<std::string> words;

    CStrUtil::addFields(values_.value(), words, ";");

    if (words.size() >= 2) {
      std::string ystr;

      auto from = CStrUtil::stripSpaces(words[0]);
      auto to   = CStrUtil::stripSpaces(words[1]);

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

    os << "/>\n";
  }
  else
    os << "animate ";
}

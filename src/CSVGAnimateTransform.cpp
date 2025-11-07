#include <CSVGAnimateTransform.h>
#include <CSVG.h>
#include <CSVGLog.h>

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
    ‘type’
*/

CSVGAnimateTransform::
CSVGAnimateTransform(CSVG &svg) :
 CSVGAnimateBase(svg)
{
}

CSVGAnimateTransform::
CSVGAnimateTransform(const CSVGAnimateTransform &animateTransform) :
 CSVGAnimateBase(animateTransform),
 type_    (animateTransform.type_),
 additive_(animateTransform.additive_)
{
}

CSVGAnimateTransform *
CSVGAnimateTransform::
dup() const
{
  return new CSVGAnimateTransform(*this);
}

bool
CSVGAnimateTransform::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  if (CSVGAnimateBase::processOption(opt_name, opt_value))
    return true;

  std::string str;

  if      (svg_.stringOption(opt_name, opt_value, "type", str))
    type_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "additive", str))
    additive_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "accumulate", str))
    accumulate_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "calcMode", str))
    calcMode_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "keySplines", str))
    keySplines_ = str;
  else
    return false;

  return true;
}

void
CSVGAnimateTransform::
animate(double t)
{
  if (getAttributeName() == "transform") {
    std::vector<double> fromValues, byValues, toValues;

    if      (from_ && to_) {
      svg_.stringToReals(getFrom(), fromValues);
      svg_.stringToReals(getTo  (), toValues  );

      auto n = std::min(fromValues.size(), toValues.size());

      byValues.resize(n);

      if (by_)
        svg_.stringToReals(getBy(), byValues);
    }
    else if (values_) {
      std::vector<std::string> fields;

      CStrUtil::addFields(getValues(), fields, ";");

      if (fields.size() < 1) {
        std::cerr << "Invalid values data\n";
        return;
      }

      auto parseWords = [&](int ind, std::vector<double> &values) {
        std::vector<std::string> words;
        CStrUtil::addWords(fields[size_t(ind)], words);

        for (const auto &word : words) {
          double r;

          if (! CStrUtil::toReal(word, &r))
            r = 0.0;

          values.push_back(r);
        }
      };

      parseWords(0, fromValues); // from

      int ind = 1;

      if (fields.size() == 3) { // by
        parseWords(0, byValues);

        ++ind;
      }

      parseWords(ind, toValues); // to
    }
    else {
      std::cerr << "CSVGAnimateTransform: no from/to or values specified\n";
    }

    if      (getType() == "translate") {
      if (fromValues.size() == 1) fromValues.push_back(fromValues[0]);
      if (toValues  .size() == 1) toValues  .push_back(toValues  [0]);

      if (fromValues.size() < 2 || toValues.size() < 2)
        return;

      double xs = CMathUtil::map(t, 0, 1, fromValues[0], toValues[0]);
      double ys = CMathUtil::map(t, 0, 1, fromValues[1], toValues[1]);

      //std::cerr << "CSVGAnimateTransform: translate " << currentTime_ << ":" <<
      //             xs << " " << ys << "\n";
      CMatrixStack2D m;

      if (getAdditive() == "sum")
        m = getParent()->getTransform();
      else
        m = getParent()->getAnimation().getTransform();

      m.addTranslation(xs, ys);

      getParent()->setTransform(m);

      svg_.redraw();
    }
    else if (getType() == "rotate") {
      if (fromValues.size() == 1) { fromValues.push_back(0); fromValues.push_back(0); }
      if (toValues  .size() == 1) { toValues  .push_back(0); toValues  .push_back(0); }

      if (fromValues.size() < 3 || toValues.size() < 3)
        return;

      double a = CMathUtil::map(t, 0, 1, fromValues[0], toValues[0]);
      double x = CMathUtil::map(t, 0, 1, fromValues[1], toValues[1]);
      double y = CMathUtil::map(t, 0, 1, fromValues[2], toValues[2]);

      //std::cerr << "CSVGAnimateTransform: transform " << currentTime_ << ":" << a << "\n";
      CMatrixStack2D m;

      if (getAdditive() == "sum")
        m = getParent()->getTransform();
      else
        m = getParent()->getAnimation().getTransform();

      m.addRotation(CMathGen::DegToRad(a), CPoint2D(x, y));

      getParent()->setTransform(m);

      svg_.redraw();
    }
    else if (getType() == "scale") {
      if (fromValues.size() == 1) fromValues.push_back(fromValues[0]);
      if (toValues  .size() == 1) toValues  .push_back(toValues  [0]);

      if (fromValues.size() < 2 || toValues.size() < 2)
        return;

      double xs = CMathUtil::map(t, 0, 1, fromValues[0], toValues[0]);
      double ys = CMathUtil::map(t, 0, 1, fromValues[1], toValues[1]);

      //std::cerr << "CSVGAnimateTransform: scale " << currentTime_ << ":" <<
      //             xs << " " << ys << "\n";
      CMatrixStack2D m;

      if (getAdditive() == "sum")
        m = getParent()->getTransform();
      else
        m = getParent()->getAnimation().getTransform();

      m.addScale(xs, ys);

      getParent()->setTransform(m);

      svg_.redraw();
    }
    else {
      CSVGLog() << "Unhandled animateTransform type " << getType();
    }
  }
}

void
CSVGAnimateTransform::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<animateTransform";

    CSVGAnimateBase::printValues(os);

    printNameValue(os, "type"    , type_    );
    printNameValue(os, "additive", additive_);

    os << "/>\n";
  }
  else
    os << "animateTransform ";
}

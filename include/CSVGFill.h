#ifndef CSVG_FILL_H
#define CSVG_FILL_H

#include <CSVGColor.h>
#include <CSVGInheritVal.h>
#include <CFillType.h>
#include <optional>

class CSVG;
class CSVGObject;

class CSVGFill {
 public:
  using Color    = CSVGInheritValT<CSVGColor>;
  using Opacity  = CSVGInheritValT<double>;
  using FillType = CSVGInheritValT<CFillType>;

 public:
  CSVGFill(CSVG &svg) :
   svg_(svg) {
  }

  CSVGFill(const CSVGFill &fill) :
   svg_       (fill.svg_),
   color_     (fill.color_),
   opacity_   (fill.opacity_),
   rule_      (fill.rule_),
   url_       (fill.url_),
   fillObject_(fill.fillObject_) {
  }

  CSVGFill &operator=(const CSVGFill &fill) {
    color_      = fill.color_;
    opacity_    = fill.opacity_;
    rule_       = fill.rule_;
    url_        = fill.url_;
    fillObject_ = fill.fillObject_;

    return *this;
  }

  bool isSet() const {
    return color_ || opacity_ || rule_ || url_ || fillObject_;
  }

  CSVG& svg() const { return svg_; }

  // color
  bool getColorValid() const { return !!color_; }
  Color getColor() const { return color_.value_or(Color()); }

  void setColor(const std::string &colorStr);
  void setColor(const Color &c) { color_ = c; }

  void resetColor() { color_.reset(); }

  // opacity
  bool getOpacityValid() const { return !!opacity_; }
  Opacity getOpacity() const { return opacity_.value_or(Opacity(1.0)); }

  void setOpacity(const std::string &opacity_str);
  void setOpacity(const Opacity &opacity) { opacity_ = opacity; }

  void resetOpacity() { opacity_.reset(); }

  // rule
  bool getRuleValid() const { return !!rule_; }
  FillType getRule() const { return rule_.value_or(FillType(FILL_TYPE_EVEN_ODD)); }

  void setRule(const std::string &rule_str);
  void setRule(const FillType &rule) { rule_ = rule; }

  void resetRule() { rule_.reset(); }

  // url
  bool getUrlValid() const { return !!url_; }
  std::string getUrl() const { return url_.value_or(""); }

  void setUrl(const std::string &url) { url_ = url; }

  void resetUrl() { url_.reset(); }

  // fill object
  bool getFillObjectValid() const { return !!fillObject_; }
  CSVGObject *getFillObject() const { return fillObject_.value_or(nullptr); }

  void setFillObject(CSVGObject *fillObject) { fillObject_ = fillObject; }

  void resetFillObject() { fillObject_.reset(); }

  //---

  CSVGObject *calcFillObject() const;

  bool isFilled() const;

  // reset
  void reset();

  // update
  void update(const CSVGFill &fill);

  // print
  void print(std::ostream &os) const;

 private:
  CSVG &                      svg_;
  std::optional<Color>        color_;
  std::optional<Opacity>      opacity_;
  std::optional<FillType>     rule_;
  std::optional<std::string>  url_;
  std::optional<CSVGObject *> fillObject_;
};

#endif

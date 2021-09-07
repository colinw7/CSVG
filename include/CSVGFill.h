#ifndef CSVG_FILL_H
#define CSVG_FILL_H

#include <CSVGColor.h>
#include <CSVGInheritVal.h>
#include <CFillType.h>
#include <COptVal.h>

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
    return color_     .isValid() ||
           opacity_   .isValid() ||
           rule_      .isValid() ||
           url_       .isValid() ||
           fillObject_.isValid();
  }

  CSVG& svg() const { return svg_; }

  // color
  bool getColorValid() const { return color_.isValid(); }
  Color getColor() const { return color_.getValue(Color()); }

  void setColor(const std::string &colorStr);
  void setColor(const Color &c) { color_ = c; }

  void resetColor() { color_.setInvalid(); }

  // opacity
  bool getOpacityValid() const { return opacity_.isValid(); }
  Opacity getOpacity() const { return opacity_.getValue(Opacity(1.0)); }

  void setOpacity(const std::string &opacity_str);
  void setOpacity(const Opacity &opacity) { opacity_ = opacity; }

  void resetOpacity() { opacity_.setInvalid(); }

  // rule
  bool getRuleValid() const { return rule_.isValid(); }
  FillType getRule() const { return rule_.getValue(FillType(FILL_TYPE_EVEN_ODD)); }

  void setRule(const std::string &rule_str);
  void setRule(const FillType &rule) { rule_ = rule; }

  void resetRule() { rule_.setInvalid(); }

  // url
  bool getUrlValid() const { return url_.isValid(); }
  std::string getUrl() const { return url_.getValue(""); }

  void setUrl(const std::string &url) { url_ = url; }

  void resetUrl() { url_.setInvalid(); }

  // fill object
  bool getFillObjectValid() const { return fillObject_.isValid(); }
  CSVGObject *getFillObject() const { return fillObject_.getValue(nullptr); }

  void setFillObject(CSVGObject *fillObject) { fillObject_ = fillObject; }

  void resetFillObject() { fillObject_.setInvalid(); }

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
  CSVG &                 svg_;
  COptValT<Color>        color_;
  COptValT<Opacity>      opacity_;
  COptValT<FillType>     rule_;
  COptString             url_;
  COptValT<CSVGObject *> fillObject_;
};

#endif

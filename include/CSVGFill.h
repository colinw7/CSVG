#ifndef CSVG_FILL_H
#define CSVG_FILL_H

#include <CRGBA.h>
#include <CFillType.h>
#include <COptVal.h>

class CSVG;
class CSVGObject;

class CSVGFill {
 public:
  CSVGFill(CSVG &svg) :
   svg_(svg) {
  }

  CSVGFill(const CSVGFill &fill) :
   svg_       (fill.svg_),
   noColor_   (fill.noColor_),
   color_     (fill.color_),
   opacity_   (fill.opacity_),
   rule_      (fill.rule_),
   url_       (fill.url_),
   fillObject_(fill.fillObject_) {
  }

  const CSVGFill &operator=(const CSVGFill &fill) {
    noColor_    = fill.noColor_;
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

  // color
  CRGBA getAlphaColor() const;

  bool  getColorValid() const { return color_.isValid(); }
  CRGBA getColor() const { return color_.getValue(CRGBA(0,0,0,0)); }

  void setColor(const std::string &color_str);
  void setColor(const CRGBA &rgba) { color_ = rgba; }

  void resetColor() { color_.setInvalid(); }

  bool  getDefColorValid() const { return defColor_.isValid(); }
  CRGBA getDefColor() const { return defColor_.getValue(CRGBA(0,0,0,0)); }

  void setDefColor(const CRGBA &rgba) { defColor_ = rgba; }

  // opacity
  bool   getOpacityValid() const { return opacity_.isValid(); }
  double getOpacity() const { return opacity_.getValue(1.0); }

  void setOpacity(const std::string &opacity_str);
  void setOpacity(double opacity) { opacity_ = opacity; }

  void resetOpacity() { opacity_.setInvalid(); }

  // rule
  bool      getRuleValid() const { return rule_.isValid(); }
  CFillType getRule() const { return rule_.getValue(FILL_TYPE_EVEN_ODD); }

  void setRule(const std::string &rule_str);
  void setRule(CFillType rule) { rule_ = rule; }

  // url
  void setUrl(const std::string &url) { url_ = url; }

  // fill object
  bool        getFillObjectValid() const;
  CSVGObject *getFillObject     () const;

  void setFillObject(CSVGObject *fillObject) { fillObject_ = fillObject; }

  // reset
  void reset();

  // update
  void update(const CSVGFill &fill);

  void print(std::ostream &os) const;

 private:
  CSVG &                 svg_;
  bool                   noColor_ { true };
  COptValT<CRGBA>        color_;  // inherited
  COptValT<CRGBA>        defColor_;  // inherited
  COptValT<double>       opacity_;  // inherited
  COptValT<CFillType>    rule_;  // inherited
  COptValT<std::string>  url_;
  COptValT<CSVGObject *> fillObject_;
};

#endif

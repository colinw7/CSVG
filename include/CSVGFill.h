#ifndef CSVG_FILL_H
#define CSVG_FILL_H

class CSVG;
class CSVGObject;

class CSVGFill {
 public:
  CSVGFill(CSVG &svg) :
   svg_        (svg),
   color_      (),
   opacity_    (),
   rule_       (),
   url_        (),
   fill_object_() {
  }

  CSVGFill(const CSVGFill &fill) :
   svg_(fill.svg_), color_(fill.color_), opacity_(fill.opacity_), rule_(fill.rule_),
   url_(fill.url_), fill_object_(fill.fill_object_) {
  }

  const CSVGFill &operator=(const CSVGFill &fill) {
    color_       = fill.color_;
    opacity_     = fill.opacity_;
    rule_        = fill.rule_;
    url_         = fill.url_;
    fill_object_ = fill.fill_object_;

    return *this;
  }

  // color
  CRGBA getAlphaColor() const;

  bool  getColorValid() const { return color_.isValid(); }
  CRGBA getColor() const { return color_.getValue(); }

  void setColor(const std::string &color_str);
  void setColor(const CRGBA &rgba) { color_.setValue(rgba); }

  bool  getDefColorValid() const { return defColor_.isValid(); }
  CRGBA getDefColor() const { return defColor_.getValue(); }

  void setDefColor(const CRGBA &rgba) { defColor_.setValue(rgba); }

  // opacity
  bool   getOpacityValid() const { return opacity_.isValid(); }
  double getOpacity     () const;

  void setOpacity(const std::string &opacity_str);
  void setOpacity(double opacity) { opacity_.setValue(opacity); }

  // rule
  bool      getRuleValid() const { return rule_.isValid(); }
  CFillType getRule     () const;

  void setRule(const std::string &rule_str);
  void setRule(CFillType rule) { rule_.setValue(rule); }

  // url
  void setUrl(const std::string &url) { url_.setValue(url); }

  // fill object
  bool        getFillObjectValid() const;
  CSVGObject *getFillObject     () const;

  void setFillObject(CSVGObject *fill_object) { fill_object_.setValue(fill_object); }

  // reset
  void reset();

  // update
  void update(const CSVGFill &fill);

 private:
  CSVG &                 svg_;
  COptValT<CRGBA>        color_;  // inherited
  COptValT<CRGBA>        defColor_;  // inherited
  COptValT<double>       opacity_;  // inherited
  COptValT<CFillType>    rule_;  // inherited
  COptValT<std::string>  url_;
  COptValT<CSVGObject *> fill_object_;
};

#endif

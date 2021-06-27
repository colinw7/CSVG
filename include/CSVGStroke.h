#ifndef CSVG_STROKE_H
#define CSVG_STROKE_H

#include <CSVGFill.h>
#include <CSVGClip.h>
#include <CSVGStrokeDash.h>
#include <CLineCapType.h>
#include <CLineJoinType.h>
#include <CScreenUnits.h>
#include <COptVal.h>

class CSVG;

class CSVGStroke {
 public:
  CSVGStroke(CSVG &svg) :
   svg_(svg) {
  }

  CSVGStroke(const CSVGStroke &stroke) :
   svg_       (stroke.svg_),
   color_     (stroke.color_),
   opacity_   (stroke.opacity_),
   rule_      (stroke.rule_),
   url_       (stroke.url_),
   fillObject_(stroke.fillObject_),
   width_     (stroke.width_),
   dash_      (stroke.dash_),
   cap_       (stroke.cap_),
   join_      (stroke.join_),
   mlimit_    (stroke.mlimit_) {
  }

  CSVGStroke &operator=(const CSVGStroke &stroke) {
    color_      = stroke.color_;
    opacity_    = stroke.opacity_;
    rule_       = stroke.rule_;
    url_        = stroke.url_;
    fillObject_ = stroke.fillObject_;
    width_      = stroke.width_;
    dash_       = stroke.dash_;
    cap_        = stroke.cap_;
    join_       = stroke.join_;
    mlimit_     = stroke.mlimit_;

    return *this;
  }

  bool isSet() const {
    return color_     .isValid() ||
           opacity_   .isValid() ||
           rule_      .isValid() ||
           url_       .isValid() ||
           fillObject_.isValid() ||
           width_     .isValid() ||
           dash_      .isValid() ||
           cap_       .isValid() ||
           join_      .isValid() ||
           mlimit_    .isValid();
  }

  CSVG& svg() const { return svg_; }

  // color
  bool getColorValid() const { return color_.isValid(); }
  CSVGColor getColor() const { return color_.getValue(CSVGColor()); }

  void setColor(const std::string &color_str);
  void setColor(const CSVGColor &c) { color_ = c; }

  void resetColor() { color_.setInvalid(); }

  // opacity
  bool getOpacityValid() const { return opacity_.isValid(); }
  double getOpacity() const { return opacity_.getValue(1.0); }

  void setOpacity(const std::string &opacity_str);
  void setOpacity(double opacity) { opacity_ = opacity; }

  void resetOpacity() { opacity_.setInvalid(); }

  // rule
  bool getRuleValid() const { return rule_.isValid(); }
  CFillType getRule() const { return rule_.getValue(FILL_TYPE_EVEN_ODD); }

  void setRule(const std::string &rule_str);
  void setRule(CFillType rule) { rule_ = rule; }

  void resetRule() { rule_.setInvalid(); }

  // url
  bool getUrlValid() const { return url_.isValid(); }
  std::string getUrl() const { return url_.getValue(""); }

  void setUrl(const std::string &url) { url_ = url; }

  void resetUrl() { url_.setInvalid(); }

  // fill object
  bool getFillObjectValid() const { return fillObject_.isValid(); }
  CSVGObject *getFillObject() const { return fillObject_.getValue(0); }

  void setFillObject(CSVGObject *fillObject) { fillObject_ = fillObject; }

  void resetFillObject() { fillObject_.setInvalid(); }

  // width
  bool getWidthValid() const { return width_.isValid(); }
  double getWidth() const { return width_.getValue(1.0); }

  void setWidth(const std::string &width_str);
  void setWidth(double width) { width_ = width; }

  void resetWidth() { width_.setInvalid(); }

  // dash
  bool getDashValid() const { return dash_.isValid(); }
  CSVGStrokeDash getDash() const { return dash_.getValue(CSVGStrokeDash()); }

  void setDash(const CSVGStrokeDash &dash) { dash_ = dash; }

  void resetDash() { dash_.setInvalid(); }

  void setDashArray(const std::string &dash_str);
  void setDashArray(const std::vector<CScreenUnits> &array);

  void setDashOffset(const std::string &offset_str);
  void setDashOffset(const CScreenUnits &offset);

  // line cap
  bool getLineCapValid() const { return cap_.isValid(); }
  CLineCapType getLineCap() const { return cap_.getValue(LINE_CAP_TYPE_BUTT); }

  void setLineCap(const std::string &cap_str);
  void setLineCap(const CLineCapType &cap) { cap_ = cap; }

  void resetLineCap() { cap_.setInvalid(); }

  // line join
  bool getLineJoinValid() const { return join_.isValid(); }
  CLineJoinType getLineJoin() const { return join_.getValue(LINE_JOIN_TYPE_MITRE); }

  void setLineJoin(const std::string &join_str);
  void setLineJoin(const CLineJoinType &join) { join_ = join; }

  void resetLineJoin() { join_.setInvalid(); }

  // mitre limit
  bool getMitreLimitValid() const { return mlimit_.isValid(); }
  double getMitreLimit() const { return mlimit_.getValue(0.0); }

  void setMitreLimit(const std::string &limit_str);
  void setMitreLimit(double mlimit) { mlimit_ = mlimit; }

  void resetMitreLimit() { mlimit_.setInvalid(); }

  //---

  CSVGObject *calcFillObject() const;

  bool isStroked() const;

  // reset
  void reset();

  // update
  void update(const CSVGStroke &stroke);

  // print
  void print(std::ostream &os) const;

 private:
  CSVG&                    svg_;
  COptValT<CSVGColor>      color_;
  COptReal                 opacity_;
  COptValT<CFillType>      rule_;
  COptString               url_;
  COptValT<CSVGObject *>   fillObject_;
  COptReal                 width_;
  COptValT<CSVGStrokeDash> dash_;
  COptValT<CLineCapType>   cap_;
  COptValT<CLineJoinType>  join_;
  COptReal                 mlimit_;
};

#endif

#ifndef CSVG_STROKE_H
#define CSVG_STROKE_H

#include <CSVGFill.h>
#include <CSVGClip.h>
#include <CRGBA.h>
#include <CLineDash.h>
#include <CLineCapType.h>
#include <CLineJoinType.h>
#include <COptVal.h>

class CSVG;

class CSVGStroke {
 public:
  CSVGStroke(CSVG &svg) :
   svg_(svg) {
  }

  CSVGStroke(const CSVGStroke &stroke) :
   svg_         (stroke.svg_),
   noColor_     (stroke.noColor_),
   currentColor_(stroke.currentColor_),
   color_       (stroke.color_),
   opacity_     (stroke.opacity_),
   rule_        (stroke.rule_),
   url_         (stroke.url_),
   fillObject_  (stroke.fillObject_),
   width_       (stroke.width_),
   dash_        (stroke.dash_),
   cap_         (stroke.cap_),
   join_        (stroke.join_),
   mlimit_      (stroke.mlimit_) {
  }

  const CSVGStroke &operator=(const CSVGStroke &stroke) {
    noColor_      = stroke.noColor_;
    currentColor_ = stroke.currentColor_;
    color_        = stroke.color_;
    opacity_      = stroke.opacity_;
    rule_         = stroke.rule_;
    url_          = stroke.url_;
    fillObject_   = stroke.fillObject_;
    width_        = stroke.width_;
    dash_         = stroke.dash_;
    cap_          = stroke.cap_;
    join_         = stroke.join_;
    mlimit_       = stroke.mlimit_;

    return *this;
  }

  bool isSet() const {
    return noColor_     .isValid() ||
           currentColor_.isValid() ||
           color_       .isValid() ||
           opacity_     .isValid() ||
           rule_        .isValid() ||
           url_         .isValid() ||
           fillObject_  .isValid() ||
           width_       .isValid() ||
           dash_        .isValid() ||
           cap_         .isValid() ||
           join_        .isValid() ||
           mlimit_      .isValid();
  }

  // no color
  bool getNoColorValid() const { return noColor_.isValid(); }
  bool getNoColor() const { return noColor_.getValue(true); }
  void setNoColor(bool b) { noColor_ = b; }

  // current color
  bool isCurrentColor() const { return currentColor_.getValue(false); }
  void setIsCurrentColor(bool b) { currentColor_ = b; }

  // color
  CRGBA getAlphaColor() const;

  bool getColorValid() const { return color_.isValid(); }
  CRGBA getColor() const { return color_.getValue(CRGBA(0,0,0,0)); }

  void setColor(const std::string &color_str);
  void setColor(const CRGBA &rgba) { color_ = rgba; }

  void resetColor() { color_.setInvalid(); }

  bool getDefColorValid() const { return defColor_.isValid(); }
  CRGBA getDefColor() const { return defColor_.getValue(CRGBA(0,0,0,0)); }

  void setDefColor(const CRGBA &rgba) { defColor_ = rgba; }

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

  // url
  bool getUrlValid() const { return url_.isValid(); }
  std::string getUrl() const { return url_.getValue(""); }
  void setUrl(const std::string &url) { url_ = url; }

  void resetUrl() { url_.setInvalid(); }

  // fill object
  bool getFillObjectValid() const;
  CSVGObject *getFillObject() const;

  void setFillObject(CSVGObject *fillObject) { fillObject_ = fillObject; }

  void resetFillObject() { fillObject_.setInvalid(); }

  // width
  bool getWidthValid() const { return width_.isValid(); }
  double getWidth() const { return width_.getValue(1.0); }

  void setWidth(const std::string &width_str);
  void setWidth(double width) { width_ = width; }

  // dash
  bool getDashValid() const { return dash_.isValid(); }
  CLineDash getDash() const { return dash_.getValue(CLineDash()); }

  void setDash(const std::string &dash_str);
  void setDash(const CLineDash &dash) { dash_ = dash; }

  void setDashOffset(const std::string &offset_str);
  void setDashOffset(double offset);

  // line cap
  bool getLineCapValid() const { return cap_.isValid(); }
  CLineCapType getLineCap() const { return cap_.getValue(LINE_CAP_TYPE_BUTT); }

  void setLineCap(const std::string &cap_str);
  void setLineCap(const CLineCapType &cap) { cap_ = cap; }

  // line join
  bool getLineJoinValid() const { return join_.isValid(); }
  CLineJoinType getLineJoin() const { return join_.getValue(LINE_JOIN_TYPE_MITRE); }

  void setLineJoin(const std::string &join_str);
  void setLineJoin(const CLineJoinType &join) { join_ = join; }

  // mitre limit
  bool getMitreLimitValid() const { return mlimit_.isValid(); }
  double getMitreLimit() const { return mlimit_.getValue(0.0); }

  void setMitreLimit(const std::string &limit_str);
  void setMitreLimit(double mlimit) { mlimit_ = mlimit; }

  // reset
  void reset();

  // update
  void update(const CSVGStroke &stroke);

  // print
  void print(std::ostream &os) const;

 private:
  CSVG&                    svg_;
  COptValT<bool>           noColor_;
  COptValT<bool>           currentColor_;
  COptValT<CRGBA>          color_;
  COptValT<CRGBA>          defColor_;
  COptValT<double>         opacity_;
  COptValT<CFillType>      rule_;
  COptValT<std::string>    url_;
  COptValT<CSVGObject *>   fillObject_;
  COptValT<double>         width_;
  COptValT<CLineDash>      dash_;
  COptValT<CLineCapType>   cap_;
  COptValT<CLineJoinType>  join_;
  COptValT<double>         mlimit_;
};

#endif

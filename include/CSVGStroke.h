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
  using Color      = CSVGInheritValT<CSVGColor>;
  using Opacity    = CSVGInheritValT<double>;
  using FillType   = CSVGInheritValT<CFillType>;
  using Width      = CSVGInheritValT<double>;
  using LineDash   = CSVGInheritValT<CSVGStrokeDash>;
  using LineCap    = CSVGInheritValT<CLineCapType>;
  using LineJoin   = CSVGInheritValT<CLineJoinType>;
  using MiterLimit = CSVGInheritValT<double>;

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
  Color getColor() const { return color_.getValue(Color()); }

  void setColor(const std::string &colorStr);
  void setColor(const Color &c) { color_ = c; }

  void resetColor() { color_.setInvalid(); }

  // opacity
  bool getOpacityValid() const { return opacity_.isValid(); }
  Opacity getOpacity() const { return opacity_.getValue(Opacity(1.0)); }

  void setOpacity(const std::string &opacityStr);
  void setOpacity(const Opacity &opacity) { opacity_ = opacity; }

  void resetOpacity() { opacity_.setInvalid(); }

  // rule
  bool getRuleValid() const { return rule_.isValid(); }
  FillType getRule() const { return rule_.getValue(FillType(FILL_TYPE_EVEN_ODD)); }

  void setRule(const std::string &ruleStr);
  void setRule(const FillType &rule) { rule_ = rule; }

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
  Width getWidth() const { return width_.getValue(Width(1.0)); }

  void setWidth(const std::string &widthStr);
  void setWidth(const Width &width) { width_ = width; }

  void resetWidth() { width_.setInvalid(); }

  // dash
  bool getDashValid() const { return dash_.isValid(); }
  LineDash getDash() const { return dash_.getValue(LineDash(CSVGStrokeDash())); }

  void setDash(const LineDash &dash) { dash_ = dash; }

  void resetDash() { dash_.setInvalid(); }

  void setDashArray(const std::string &dashStr);
  void setDashArray(const std::vector<CScreenUnits> &array);

  void setDashOffset(const std::string &offsetStr);
  void setDashOffset(const CScreenUnits &offset);

  // line cap
  bool getLineCapValid() const { return cap_.isValid(); }
  LineCap getLineCap() const { return cap_.getValue(LineCap(LINE_CAP_TYPE_BUTT)); }

  void setLineCap(const std::string &capStr);
  void setLineCap(const LineCap &cap) { cap_ = cap; }

  void resetLineCap() { cap_.setInvalid(); }

  // line join
  bool getLineJoinValid() const { return join_.isValid(); }
  LineJoin getLineJoin() const { return join_.getValue(LineJoin(LINE_JOIN_TYPE_MITRE)); }

  void setLineJoin(const std::string &joinStr);
  void setLineJoin(const LineJoin &join) { join_ = join; }

  void resetLineJoin() { join_.setInvalid(); }

  // miter limit
  bool getMiterLimitValid() const { return mlimit_.isValid(); }
  MiterLimit getMiterLimit() const { return mlimit_.getValue(MiterLimit(0.0)); }

  void setMiterLimit(const std::string &limitStr);
  void setMiterLimit(const MiterLimit &mlimit) { mlimit_ = mlimit; }

  void resetMiterLimit() { mlimit_.setInvalid(); }

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
  void printLineCap(std::ostream &ss, const LineCap &lineCap) const;
  void printLineCapType(std::ostream &ss, const CLineCapType &lineCap) const;

  void printLineJoin(std::ostream &ss, const LineJoin &lineJoin) const;
  void printLineJoinType(std::ostream &ss, const CLineJoinType &lineJoin) const;

 private:
  CSVG&                    svg_;
  COptValT<Color>          color_;
  COptValT<Opacity>        opacity_;
  COptValT<FillType>       rule_;
  COptString               url_;
  COptValT<CSVGObject *>   fillObject_;
  COptValT<Width>          width_;
  COptValT<LineDash>       dash_;
  COptValT<LineCap>        cap_;
  COptValT<LineJoin>       join_;
  COptValT<MiterLimit>     mlimit_;
};

#endif

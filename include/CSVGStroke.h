#ifndef CSVG_STROKE_H
#define CSVG_STROKE_H

#include <CSVGFill.h>
#include <CSVGClip.h>
#include <CSVGLineDash.h>
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
  using DashArray  = CSVGInheritValT<CSVGLineDash>;
  using DashOffset = CSVGInheritValT<CScreenUnits>;
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
   lineDash_  (stroke.lineDash_),
   lineOffset_(stroke.lineOffset_),
   lineCap_   (stroke.lineCap_),
   lineJoin_  (stroke.lineJoin_),
   mlimit_    (stroke.mlimit_) {
  }

  CSVGStroke &operator=(const CSVGStroke &stroke) {
    color_      = stroke.color_;
    opacity_    = stroke.opacity_;
    rule_       = stroke.rule_;
    url_        = stroke.url_;
    fillObject_ = stroke.fillObject_;
    width_      = stroke.width_;
    lineDash_   = stroke.lineDash_;
    lineOffset_ = stroke.lineOffset_;
    lineCap_    = stroke.lineCap_;
    lineJoin_   = stroke.lineJoin_;
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
           lineDash_  .isValid() ||
           lineOffset_.isValid() ||
           lineCap_   .isValid() ||
           lineJoin_  .isValid() ||
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
  CSVGObject *getFillObject() const { return fillObject_.getValue(nullptr); }

  void setFillObject(CSVGObject *fillObject) { fillObject_ = fillObject; }

  void resetFillObject() { fillObject_.setInvalid(); }

  // width
  bool getWidthValid() const { return width_.isValid(); }
  Width getWidth() const { return width_.getValue(Width(1.0)); }

  void setWidth(const std::string &widthStr);
  void setWidth(const Width &width) { width_ = width; }

  void resetWidth() { width_.setInvalid(); }

  // line dash
  bool getDashArrayValid() const { return lineDash_.isValid(); }
  DashArray getDashArray() const { return lineDash_.getValue(DashArray()); }

  void setDashArray(const std::string &dashStr);
  void setDashArray(const DashArray &dash) { lineDash_ = dash; }

  void resetDashArray() { lineDash_.setInvalid(); }

  // line dash offset
  bool getDashOffsetValid() const { return lineOffset_.isValid(); }
  DashOffset getDashOffset() const { return lineOffset_.getValue(DashOffset(CScreenUnits(0))); }

  void setDashOffset(const std::string &offsetStr);
  void setDashOffset(const DashOffset &offset) { lineOffset_ = offset; }

  void resetDashOffset() { lineOffset_.setInvalid(); }

  // line cap
  bool getLineCapValid() const { return lineCap_.isValid(); }
  LineCap getLineCap() const { return lineCap_.getValue(LineCap(LINE_CAP_TYPE_BUTT)); }

  void setLineCap(const std::string &capStr);
  void setLineCap(const LineCap &lineCap) { lineCap_ = lineCap; }

  void resetLineCap() { lineCap_.setInvalid(); }

  // line join
  bool getLineJoinValid() const { return lineJoin_.isValid(); }
  LineJoin getLineJoin() const { return lineJoin_.getValue(LineJoin(LINE_JOIN_TYPE_MITRE)); }

  void setLineJoin(const std::string &joinStr);
  void setLineJoin(const LineJoin &lineJoin) { lineJoin_ = lineJoin; }

  void resetLineJoin() { lineJoin_.setInvalid(); }

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
  CSVG&                  svg_;
  COptValT<Color>        color_;
  COptValT<Opacity>      opacity_;
  COptValT<FillType>     rule_;
  COptString             url_;
  COptValT<CSVGObject *> fillObject_;
  COptValT<Width>        width_;
  COptValT<DashArray>    lineDash_;
  COptValT<DashOffset>   lineOffset_;
  COptValT<LineCap>      lineCap_;
  COptValT<LineJoin>     lineJoin_;
  COptValT<MiterLimit>   mlimit_;
};

#endif

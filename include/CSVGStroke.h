#ifndef CSVG_STROKE_H
#define CSVG_STROKE_H

#include <CSVGFill.h>
#include <CSVGClip.h>
#include <CSVGLineDash.h>
#include <CLineCapType.h>
#include <CLineJoinType.h>
#include <CScreenUnits.h>

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
    return color_    || opacity_    || rule_    || url_      || fillObject_ ||
           lineDash_ || lineOffset_ || lineCap_ || lineJoin_ || mlimit_;
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

  void setOpacity(const std::string &opacityStr);
  void setOpacity(const Opacity &opacity) { opacity_ = opacity; }

  void resetOpacity() { opacity_.reset(); }

  // rule
  bool getRuleValid() const { return !!rule_; }
  FillType getRule() const { return rule_.value_or(FillType(FILL_TYPE_EVEN_ODD)); }

  void setRule(const std::string &ruleStr);
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

  // width
  bool getWidthValid() const { return !!width_; }
  Width getWidth() const { return width_.value_or(Width(1.0)); }

  void setWidth(const std::string &widthStr);
  void setWidth(const Width &width) { width_ = width; }

  void resetWidth() { width_.reset(); }

  // line dash
  bool getDashArrayValid() const { return !!lineDash_; }
  DashArray getDashArray() const { return lineDash_.value_or(DashArray()); }

  void setDashArray(const std::string &dashStr);
  void setDashArray(const DashArray &dash) { lineDash_ = dash; }

  void resetDashArray() { lineDash_.reset(); }

  // line dash offset
  bool getDashOffsetValid() const { return !!lineOffset_; }
  DashOffset getDashOffset() const { return lineOffset_.value_or(DashOffset(CScreenUnits(0))); }

  void setDashOffset(const std::string &offsetStr);
  void setDashOffset(const DashOffset &offset) { lineOffset_ = offset; }

  void resetDashOffset() { lineOffset_.reset(); }

  // line cap
  bool getLineCapValid() const { return !!lineCap_; }
  LineCap getLineCap() const { return lineCap_.value_or(LineCap(LINE_CAP_TYPE_BUTT)); }

  void setLineCap(const std::string &capStr);
  void setLineCap(const LineCap &lineCap) { lineCap_ = lineCap; }

  void resetLineCap() { lineCap_.reset(); }

  // line join
  bool getLineJoinValid() const { return !!lineJoin_; }
  LineJoin getLineJoin() const { return lineJoin_.value_or(LineJoin(LINE_JOIN_TYPE_MITRE)); }

  void setLineJoin(const std::string &joinStr);
  void setLineJoin(const LineJoin &lineJoin) { lineJoin_ = lineJoin; }

  void resetLineJoin() { lineJoin_.reset(); }

  // miter limit
  bool getMiterLimitValid() const { return !!mlimit_; }
  MiterLimit getMiterLimit() const { return mlimit_.value_or(MiterLimit(0.0)); }

  void setMiterLimit(const std::string &limitStr);
  void setMiterLimit(const MiterLimit &mlimit) { mlimit_ = mlimit; }

  void resetMiterLimit() { mlimit_.reset(); }

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
  CSVG&                       svg_;
  std::optional<Color>        color_;
  std::optional<Opacity>      opacity_;
  std::optional<FillType>     rule_;
  std::optional<std::string>  url_;
  std::optional<CSVGObject *> fillObject_;
  std::optional<Width>        width_;
  std::optional<DashArray>    lineDash_;
  std::optional<DashOffset>   lineOffset_;
  std::optional<LineCap>      lineCap_;
  std::optional<LineJoin>     lineJoin_;
  std::optional<MiterLimit>   mlimit_;
};

#endif

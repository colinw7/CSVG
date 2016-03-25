#ifndef CSVG_STROKE_H
#define CSVG_STROKE_H

#include <CSVGFill.h>
#include <CSVGClip.h>
#include <CSVGFontDef.h>
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
   svg_    (stroke.svg_),
   noColor_(stroke.noColor_),
   color_  (stroke.color_),
   opacity_(stroke.opacity_),
   width_  (stroke.width_),
   dash_   (stroke.dash_),
   cap_    (stroke.cap_),
   join_   (stroke.join_),
   mlimit_ (stroke.mlimit_) {
  }

  const CSVGStroke &operator=(const CSVGStroke &stroke) {
    color_   = stroke.color_;
    noColor_ = stroke.noColor_;
    opacity_ = stroke.opacity_;
    width_   = stroke.width_;
    dash_    = stroke.dash_;
    cap_     = stroke.cap_;
    join_    = stroke.join_;
    mlimit_  = stroke.mlimit_;

    return *this;
  }

  bool isSet() const {
    return color_  .isValid() ||
           opacity_.isValid() ||
           width_  .isValid() ||
           dash_   .isValid() ||
           cap_    .isValid() ||
           join_   .isValid() ||
           mlimit_ .isValid();
  }

  // color
  CRGBA getAlphaColor() const;

  bool  getColorValid() const { return color_.isValid(); }
  CRGBA getColor() const { return color_.getValue(CRGBA(0,0,0,0)); }

  void  setColor(const std::string &color_str);
  void  setColor(const CRGBA &rgba) { color_ = rgba; }
  void  resetColor() { color_.setInvalid(); }

  bool  getDefColorValid() const { return defColor_.isValid(); }
  CRGBA getDefColor() const { return defColor_.getValue(); }

  void  setDefColor(const CRGBA &rgba) { defColor_ = rgba; }

  // opacity
  bool   getOpacityValid() const { return opacity_.isValid(); }
  double getOpacity     () const;

  void setOpacity(const std::string &opacity_str);
  void setOpacity(double opacity) { opacity_ = opacity; }
  void resetOpacity() { opacity_.setInvalid(); }

  // width
  bool   getWidthValid() const { return width_.isValid(); }
  double getWidth     () const;

  void setWidth(const std::string &width_str);
  void setWidth(double width) { width_ = width; }

  // dash
  bool             getDashValid() const { return dash_.isValid(); }
  const CLineDash &getDash     () const;

  void setDash      (const std::string &dash_str);
  void setDashOffset(const std::string &offset_str);
  void setDashOffset(double offset);
  void setDash      (const CLineDash &dash) { dash_ = dash; }

  // line cap
  bool         getLineCapValid() const { return cap_.isValid(); }
  CLineCapType getLineCap     () const;

  void setLineCap(const std::string &cap_str);
  void setLineCap(const CLineCapType &cap) { cap_ = cap; }

  // line join
  bool          getLineJoinValid() const { return join_.isValid(); }
  CLineJoinType getLineJoin     () const;

  void setLineJoin(const std::string &join_str);
  void setLineJoin(const CLineJoinType &join) { join_ = join; }

  // mitre limit
  bool   getMitreLimitValid() const { return mlimit_.isValid(); }
  double getMitreLimit     () const;

  void setMitreLimit(const std::string &limit_str);
  void setMitreLimit(double mlimit) { mlimit_ = mlimit; }

  // reset
  void reset();

  // update
  void update(const CSVGStroke &stroke);

  void print(std::ostream &os) const;

 private:
  CSVG&                    svg_;
  bool                     noColor_ { true };
  COptValT<CRGBA>          color_;
  COptValT<CRGBA>          defColor_;
  COptValT<double>         opacity_;
  COptValT<double>         width_;
  COptValT<CLineDash>      dash_;
  COptValT<CLineCapType>   cap_;
  COptValT<CLineJoinType>  join_;
  COptValT<double>         mlimit_;
};

#endif

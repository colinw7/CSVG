#ifndef CSVG_STROKE_H
#define CSVG_STROKE_H

class CSVG;

class CSVGStroke {
 public:
  CSVGStroke(CSVG &svg) :
   svg_    (svg),
   color_  (),
   opacity_(),
   width_  (),
   dash_   (),
   cap_    (),
   join_   (),
   mlimit_ () {
  }

  CSVGStroke(const CSVGStroke &stroke) :
   svg_(stroke.svg_), color_(stroke.color_), opacity_(stroke.opacity_), width_(stroke.width_),
   dash_(stroke.dash_), cap_(stroke.cap_), join_(stroke.join_), mlimit_(stroke.mlimit_) {
  }

  const CSVGStroke &operator=(const CSVGStroke &stroke) {
    color_   = stroke.color_;
    opacity_ = stroke.opacity_;
    width_   = stroke.width_;
    dash_    = stroke.dash_;
    cap_     = stroke.cap_;
    join_    = stroke.join_;
    mlimit_  = stroke.mlimit_;

    return *this;
  }

  // color
  CRGBA getAlphaColor() const;

  bool  getColorValid() const { return color_.isValid(); }
  CRGBA getColor() const { return color_.getValue(); }

  void  setColor(const std::string &color_str);
  void  setColor(const CRGBA &rgba) { color_.setValue(rgba); }

  bool  getDefColorValid() const { return defColor_.isValid(); }
  CRGBA getDefColor() const { return defColor_.getValue(); }

  void  setDefColor(const CRGBA &rgba) { defColor_.setValue(rgba); }

  // opacity
  bool   getOpacityValid() const { return opacity_.isValid(); }
  double getOpacity     () const;

  void setOpacity(const std::string &opacity_str);
  void setOpacity(double opacity) { opacity_.setValue(opacity); }

  // width
  bool   getWidthValid() const { return width_.isValid(); }
  double getWidth     () const;

  void setWidth(const std::string &width_str);
  void setWidth(double width) { width_.setValue(width); }

  // dash
  bool             getDashValid() const { return dash_.isValid(); }
  const CLineDash &getDash     () const;

  void setDash      (const std::string &dash_str);
  void setDashOffset(const std::string &offset_str);
  void setDashOffset(double offset);
  void setDash      (const CLineDash &dash) { dash_.setValue(dash); }

  // line cap
  bool         getLineCapValid() const { return cap_.isValid(); }
  CLineCapType getLineCap     () const;

  void setLineCap(const std::string &cap_str);
  void setLineCap(const CLineCapType &cap) { cap_.setValue(cap); }

  // line join
  bool          getLineJoinValid() const { return join_.isValid(); }
  CLineJoinType getLineJoin     () const;

  void setLineJoin(const std::string &join_str);
  void setLineJoin(const CLineJoinType &join) { join_.setValue(join); }

  // mitre limit
  bool   getMitreLimitValid() const { return mlimit_.isValid(); }
  double getMitreLimit     () const;

  void setMitreLimit(const std::string &limit_str);
  void setMitreLimit(double mlimit) { mlimit_.setValue(mlimit); }

  // reset
  void reset();

  // update
  void update(const CSVGStroke &stroke);

 private:
  CSVG                    &svg_;
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

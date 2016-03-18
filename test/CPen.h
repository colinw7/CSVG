#ifndef CPEN_H
#define CPEN_H

#include <CRGBA.h>
#include <CLineDash.h>
#include <CILineDash.h>
#include <CLineCapType.h>
#include <CLineJoinType.h>

class CPen {
 private:
  CRGBA         color_;
  double        width_;
  CLineDash     dash_;
  CLineCapType  cap_;
  CLineJoinType join_;
  double        mitre_limit_;
  double        flatness_;

 public:
  CPen() :
   color_      (0,0,0,1),
   width_      (1.0),
   dash_       (),
   cap_        (LINE_CAP_TYPE_BUTT),
   join_       (LINE_JOIN_TYPE_MITRE),
   mitre_limit_(0.0),
   flatness_   (0.0) {
    init();
  }

  CPen(const CPen &pen) :
   color_      (0,0,0,1),
   width_      (1.0),
   dash_       (),
   cap_        (LINE_CAP_TYPE_BUTT),
   join_       (LINE_JOIN_TYPE_MITRE),
   mitre_limit_(0.0),
   flatness_   (0.0) {
    init();

    copy(pen);
  }

  CPen &operator=(const CPen &pen) {
    return copy(pen);
  }

  void init() {
    color_        = CRGBA(0,0,0,1);
    width_        = 1.0;
    cap_          = LINE_CAP_TYPE_BUTT;
    join_         = LINE_JOIN_TYPE_MITRE;
    mitre_limit_  = 0.0;
    flatness_     = 0.0;

    dash_.init();
  }

  CPen &copy(const CPen &pen) {
    if (&pen == this)
      return *this;

    color_        = pen.color_;
    width_        = pen.width_;
    dash_         = pen.dash_;
    cap_          = pen.cap_;
    join_         = pen.join_;
    mitre_limit_  = pen.mitre_limit_;
    flatness_     = pen.flatness_;

    return *this;
  }

  const CRGBA     &getColor     () const { return color_      ; }
  double           getWidth     () const { return width_      ; }
  const CLineDash &getLineDash  () const { return dash_       ; }
  CLineCapType     getLineCap   () const { return cap_        ; }
  CLineJoinType    getLineJoin  () const { return join_       ; }
  double           getMitreLimit() const { return mitre_limit_; }
  double           getFlatness  () const { return flatness_   ; }

  void setColor     (const CRGBA     &color   ) { color_       = color; }
  void setWidth     (double           width   ) { width_       = width; }
  void setLineDash  (const CLineDash &dash    ) { dash_        = dash; }
  void setLineCap   (CLineCapType     cap     ) { cap_         = cap; }
  void setLineJoin  (CLineJoinType    join    ) { join_        = join; }
  void setMitreLimit(double           mitre   ) { mitre_limit_ = mitre; }
  void setFlatness  (double           flatness) { flatness_    = flatness; }
};

class CIPen {
 public:
  CRGBA         color_;
  uint          width_;
  CILineDash    dash_;
  CLineCapType  cap_;
  CLineJoinType join_;
  double        mitre_limit_;
  double        flatness_;

 public:
  CIPen() :
   color_      (0,0,0,1),
   width_      (1),
   dash_       (),
   cap_        (LINE_CAP_TYPE_BUTT),
   join_       (LINE_JOIN_TYPE_MITRE),
   mitre_limit_(0.0),
   flatness_   (0.0) {
    init();
  }

  CIPen(const CIPen &pen) :
   color_      (0,0,0,1),
   width_      (1),
   dash_       (),
   cap_        (LINE_CAP_TYPE_BUTT),
   join_       (LINE_JOIN_TYPE_MITRE),
   mitre_limit_(0.0),
   flatness_   (0.0) {
    init();

    copy(pen);
  }

  CIPen &operator=(const CIPen &pen) {
    return copy(pen);
  }

  void init() {
    color_        = CRGBA(0,0,0,1);
    width_        = 1;
    cap_          = LINE_CAP_TYPE_BUTT;
    join_         = LINE_JOIN_TYPE_MITRE;
    mitre_limit_  = 0.0;
    flatness_     = 0.0;

    dash_.init();
  }

  CIPen &copy(const CIPen &pen) {
    if (&pen == this)
      return *this;

    color_        = pen.color_;
    width_        = pen.width_;
    dash_         = pen.dash_;
    cap_          = pen.cap_;
    join_         = pen.join_;
    mitre_limit_  = pen.mitre_limit_;
    flatness_     = pen.flatness_;

    return *this;
  }
};

#endif

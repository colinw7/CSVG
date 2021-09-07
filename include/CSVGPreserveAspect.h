#ifndef CSVGPreserveAspect_H
#define CSVGPreserveAspect_H

#include <CSVGTypes.h>
#include <CSVGTmpl.h>

#include <CAlignType.h>
#include <sstream>

class CSVGPreserveAspect : public CSVGPrintBase<CSVGPreserveAspect> {
 public:
  CSVGPreserveAspect(const CHAlignType &halign=CHALIGN_TYPE_CENTER,
                     const CVAlignType &valign=CVALIGN_TYPE_CENTER,
                     const CSVGScale &scale=CSVGScale::FIXED_MEET) :
   halign_(halign), valign_(valign), scale_(scale) {
  }

  const CHAlignType &getHAlign() const { return halign_; }
  void setHAlign(const CHAlignType &a) { halign_ = a; }

  const CVAlignType &getVAlign() const { return valign_; }
  void setVAlign(const CVAlignType &a) { valign_ = a; }

  const CSVGScale &getScale() const { return scale_ ; }
  void setScale(const CSVGScale &s) { scale_ = s; }

  int alignValue() const {
    if (isNone())
      return int(CSVGPreserveAspectAlign::NONE);

    if (getHAlign() == CHALIGN_TYPE_LEFT   && getVAlign() == CVALIGN_TYPE_BOTTOM)
      return int(CSVGPreserveAspectAlign::XMINYMIN);
    if (getHAlign() == CHALIGN_TYPE_CENTER && getVAlign() == CVALIGN_TYPE_BOTTOM)
      return int(CSVGPreserveAspectAlign::XMIDYMIN);
    if (getHAlign() == CHALIGN_TYPE_RIGHT  && getVAlign() == CVALIGN_TYPE_BOTTOM)
      return int(CSVGPreserveAspectAlign::XMAXYMIN);
    if (getHAlign() == CHALIGN_TYPE_LEFT   && getVAlign() == CVALIGN_TYPE_CENTER)
      return int(CSVGPreserveAspectAlign::XMINYMID);
    if (getHAlign() == CHALIGN_TYPE_CENTER && getVAlign() == CVALIGN_TYPE_CENTER)
      return int(CSVGPreserveAspectAlign::XMIDYMID);
    if (getHAlign() == CHALIGN_TYPE_RIGHT  && getVAlign() == CVALIGN_TYPE_CENTER)
      return int(CSVGPreserveAspectAlign::XMAXYMID);
    if (getHAlign() == CHALIGN_TYPE_LEFT   && getVAlign() == CVALIGN_TYPE_TOP)
      return int(CSVGPreserveAspectAlign::XMINYMAX);
    if (getHAlign() == CHALIGN_TYPE_CENTER && getVAlign() == CVALIGN_TYPE_TOP)
      return int(CSVGPreserveAspectAlign::XMIDYMAX);
    if (getHAlign() == CHALIGN_TYPE_RIGHT  && getVAlign() == CVALIGN_TYPE_TOP)
      return int(CSVGPreserveAspectAlign::XMAXYMAX);

    return int(CSVGPreserveAspectAlign::NONE);
  }

  int meetOrSliceValue() const {
    if (getScale() == CSVGScale::FIXED_MEET)
      return int(CSVGPreserveAspectMeetOrSlice::MEET);
    if (getScale() == CSVGScale::FIXED_SLICE)
      return int(CSVGPreserveAspectMeetOrSlice::SLICE);

    return int(CSVGPreserveAspectMeetOrSlice::UNKNOWN);
  }

  bool isNone() const {
    return (getScale() == CSVGScale::FREE);
  }

  std::string toString() const {
    std::stringstream ss;

    print(ss);

    return ss.str();
  }

  void print(std::ostream &os) const {
    if (isNone()) { os << "none"; return; }

    if      (getHAlign() == CHALIGN_TYPE_LEFT  ) os << "xMin";
    else if (getHAlign() == CHALIGN_TYPE_CENTER) os << "xMid";
    else if (getHAlign() == CHALIGN_TYPE_RIGHT ) os << "xMax";

    if      (getVAlign() == CVALIGN_TYPE_BOTTOM) os << "YMin";
    else if (getVAlign() == CVALIGN_TYPE_CENTER) os << "YMid";
    else if (getVAlign() == CVALIGN_TYPE_TOP   ) os << "YMax";

    os << " ";

    if      (getScale() == CSVGScale::FIXED_MEET ) os << "meet";
    else if (getScale() == CSVGScale::FIXED_SLICE) os << "slice";
  }

 public:
  CHAlignType halign_ { CHALIGN_TYPE_CENTER };
  CVAlignType valign_ { CVALIGN_TYPE_CENTER };
  CSVGScale   scale_  { CSVGScale::FREE };
};

#endif

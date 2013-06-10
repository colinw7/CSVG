#ifndef CSVG_H
#define CSVG_H

#include <COptVal.h>
#include <CRGBA.h>
#include <CLineDash.h>
#include <CLineCapType.h>
#include <CLineJoinType.h>
#include <CAlignType.h>
#include <CFillType.h>
#include <CLog.h>
#include <CFontStyle.h>
#include <CPoint2D.h>
#include <CMatrix2D.h>
#include <CSize2D.h>
#include <CBBox2D.h>
#include <CLinearGradient.h>
#include <CRadialGradient.h>
#include <CConfig.h>

enum CSVGScale {
  CSVG_SCALE_FREE,
  CSVG_SCALE_FIXED_MEET,
  CSVG_SCALE_FIXED_SLICE
};

enum CSVGCoordUnits {
  CSVG_COORD_UNITS_OBJECT_BBOX,
  CSVG_COORD_UNITS_USER_SPACE
};

class CSVG;

#include <CSVGStroke.h>
#include <CSVGFill.h>
#include <CSVGClip.h>
#include <CSVGFontDef.h>

#include <CSVGBuffer.h>
#include <CSVGObject.h>

class CSVGXLink {
 private:
  CSVGObject  *parent_;
  bool         resolved_;
  std::string  str_;
  CSVGObject  *object_;
  CImagePtr    image_;

 public:
  CSVGXLink(CSVGObject *parent, const std::string &str="") :
   parent_(parent), resolved_(false), str_(str), object_(NULL), image_() {
  }

  CSVGXLink(CSVGObject *parent, CSVGObject *object) :
   parent_(parent), resolved_(true), str_(), object_(object), image_() {
  }

  CSVGXLink(CSVGObject *parent, CImagePtr image) :
   parent_(parent), resolved_(true), str_(), object_(NULL), image_(image) {
  }

  CSVGXLink(const CSVGXLink &xlink);

  CSVGXLink &operator=(const CSVGXLink &xlink);

  bool isResolved() const { return resolved_; }

  bool isObject() const {
    resolve();

    return (object_ != NULL);
  }

  CSVGObject *getObject() const {
    return object_;
  }

  void setObject(CSVGObject *object) {
    object_ = object;
  }

  bool isImage() const {
    resolve();

    return (image_.isValid());
  }

  CImagePtr getImage() const {
    return image_;
  }

  void setImage(CImagePtr image) {
    image_ = image;
  }

 private:
  void resolve() const;
};

#include <CSVGBlock.h>
#include <CSVGCircle.h>
#include <CSVGClipPath.h>
#include <CSVGDefs.h>
#include <CSVGDesc.h>
#include <CSVGEllipse.h>
#include <CSVGFeBlend.h>
#include <CSVGFeColorMatrix.h>
#include <CSVGFeComponentTransfer.h>
#include <CSVGFeComposite.h>
#include <CSVGFeDiffuseLighting.h>
#include <CSVGFeDistantLight.h>
#include <CSVGFeFlood.h>
#include <CSVGFeFunc.h>
#include <CSVGFeGaussianBlur.h>
#include <CSVGFeImage.h>
#include <CSVGFeMerge.h>
#include <CSVGFeMergeNode.h>
#include <CSVGFeOffset.h>
#include <CSVGFePointLight.h>
#include <CSVGFeSpecularLighting.h>
#include <CSVGFeTile.h>
#include <CSVGFeTurbulence.h>
#include <CSVGFilter.h>
#include <CSVGFont.h>
#include <CSVGFontFace.h>
#include <CSVGGlyph.h>
#include <CSVGGroup.h>
#include <CSVGHKern.h>
#include <CSVGImage.h>
#include <CSVGLine.h>
#include <CSVGLinearGradient.h>
#include <CSVGMarker.h>
#include <CSVGMask.h>
#include <CSVGMissingGlyph.h>
#include <CSVGPath.h>
#include <CSVGPattern.h>
#include <CSVGPolygon.h>
#include <CSVGPolyLine.h>
#include <CSVGRadialGradient.h>
#include <CSVGRect.h>
#include <CSVGStop.h>
#include <CSVGStyle.h>
#include <CSVGSymbol.h>
#include <CSVGText.h>
#include <CSVGTitle.h>
#include <CSVGTSpan.h>
#include <CSVGUse.h>

class CXML;
class CXMLTag;
class CXMLToken;
class CSVGRenderer;

class CSVGPathPart;

class CSVGStyleData {
 private:
  std::string id_;
  CSVGStroke  stroke_;
  CSVGFill    fill_;

 public:
  CSVGStyleData(CSVG &svg, const std::string &id) :
   id_(id), stroke_(svg), fill_(svg) {
  }

  void setValue(const std::string &name, const std::string &value);

  bool      getStrokeColorValid  () const { return stroke_.getColorValid(); }
  CRGBA     getStrokeColor       () const { return stroke_.getColor(); }
  bool      getStrokeOpacityValid() const { return stroke_.getOpacityValid(); }
  double    getStrokeOpacity     () const { return stroke_.getOpacity(); }
  bool      getStrokeWidthValid  () const { return stroke_.getWidthValid(); }
  double    getStrokeWidth       () const { return stroke_.getWidth(); }
  bool      getStrokeDashValid   () const { return stroke_.getDashValid(); }
  CLineDash getStrokeDash        () const { return stroke_.getDash(); }
  bool      getFillColorValid    () const { return fill_  .getColorValid(); }
  CRGBA     getFillColor         () const { return fill_  .getColor(); }
};

class CSVGLog : public CLog { };

class CSVG {
 private:
  typedef std::vector<CSVGObject *>            ObjectList;
  typedef std::vector<CSVGFont *>              FontList;
  typedef std::map<std::string, CSVGObject *>  NameObjectMap;
  typedef std::vector<CSVGPathPart *>          PartList;
  typedef std::map<std::string, CSVGStyleData> StyleDataMap;

  CSVGRenderer            *renderer_;
  CAutoPtr<CSVGBufferMgr>  buffer_mgr_;
  CSVGBuffer              *buffer_;
  CMatrix2D                view_matrix_;
  CMatrix2D                transform_;
  CAutoPtr<CSVGBlock>      block_;
  CAutoPtr<CXML>           xml_;
  CXMLTag                 *xml_tag_;
  CSVGStroke               stroke_;
  CSVGFill                 fill_;
  CSVGClip                 clip_;
  CSVGFontDef              font_def_;
  FontList                 font_list_;
  NameObjectMap            id_object_map_;
  StyleDataMap             styleData_;
  bool                     uniquify_;
  bool                     autoName_;
  bool                     debug_;

 public:
  CSVG(CSVGRenderer *renderer=NULL);

 ~CSVG();

  void          setRenderer(CSVGRenderer *renderer);
  CSVGRenderer *getRenderer() const { return renderer_; }

  CSVGRenderer *createRenderer();

  CSVGBlock *getBlock() const { return block_; }

  CImagePtr getBufferImage();
  CImagePtr getBufferImage(const std::string &name);

  void setBufferImage(CImagePtr image);
  void setBufferImage(const std::string &name, CImagePtr image);

  CSVGBuffer  *getBuffer    () const;
  std::string  getBufferName() const;

  void        setBuffer(const std::string &name);
  CSVGBuffer *getBuffer(const std::string &name);

  void beginDrawBuffer();
  void endDrawBuffer();

  void setAntiAlias(bool flag);

  void setUniquify(bool uniquify) { uniquify_ = uniquify; }
  bool getUniquify() const { return uniquify_; }

  void setAutoName(bool autoName);
  bool getAutoName() const { return autoName_; }

  void setDebug(bool debug);
  bool getDebug() const { return debug_; }

  void init();

  bool read(const std::string &filename);
  bool read(const std::string &filename, CSVGObject *object);

  CSVGObject *createObjectByName(const std::string &name);

  CSVGCircle         *createCircle();
  CSVGEllipse        *createEllipse();
  CSVGImage          *createImage();
  CSVGLine           *createLine();
  CSVGLinearGradient *createLinearGradient();
  CSVGPolygon        *createPolygon();
  CSVGPolyLine       *createPolyLine();
  CSVGRadialGradient *createRadialGradient();
  CSVGRect           *createRect();
  CSVGStop           *createStop();

  double getXMin() const { return block_->getXMin(); }
  double getYMin() const { return block_->getYMin(); }

  double getWidth () const { return block_->getWidth (); }
  double getHeight() const { return block_->getHeight(); }

  int getIWidth () const { return int(getWidth ()); }
  int getIHeight() const { return int(getHeight()); }

  const CSVGStroke  &getStroke () const { return stroke_  ; }
  const CSVGFill    &getFill   () const { return fill_    ; }
  const CSVGClip    &getClip   () const { return clip_    ; }
  const CSVGFontDef &getFontDef() const { return font_def_; }

  void setStroke (CSVGStroke  &stroke  ) { stroke_   = stroke  ; }
  void setFill   (CSVGFill    &fill    ) { fill_     = fill    ; }
  void setClip   (CSVGClip    &clip    ) { clip_     = clip    ; }
  void setFontDef(CSVGFontDef &font_def) { font_def_ = font_def; }

  bool processOption(const std::string &opt_name, const std::string &opt_value);

  void addFont(CSVGFont *font);

  CSVGFont *getFont() const;

  CSVGGlyph *getCharGlyph(char c) const;
  CSVGGlyph *getUnicodeGlyph(const std::string &unicode) const;

  CImagePtr drawToImage(int w, int h);

  void draw();
  void draw(const CMatrix2D &matrix);

  void drawObject(CSVGObject *object);
  void drawObject(CSVGObject *object, const CMatrix2D &matrix);

  void resetStroke();
  void updateStroke(const CSVGStroke &stroke);
  void setSelectedStroke();
  bool isStroked() const;
  void setStroke();

  void resetFill();
  void updateFill(const CSVGFill &fill);
  bool isFilled() const;
  void setFill();

  void resetClip();
  void updateClip(const CSVGClip &clip);
  bool isClipped() const;
  void setClip();

  void resetFontDef();
  void updateFontDef(const CSVGFontDef &font_def);
  void setFontDef();

  void setTransform(const CMatrix2D &matrix);
  void getTransform(CMatrix2D &matrix);
  void unsetTransform();

  void drawImage(double x, double y, CImagePtr image);
  void drawImage(const CBBox2D &bbox, CImagePtr image);

  void drawLine(double x1, double y1, double x2, double y2);

  void drawRoundedRectangle(const CBBox2D &bbox, double rx, double ry);
  void fillRoundedRectangle(const CBBox2D &bbox, double rx, double ry);

  void drawRectangle(const CBBox2D &bbox);
  void fillRectangle(const CBBox2D &bbox);

  void drawCircle(double x, double y, double r);
  void drawEllipse(double x, double y, double rx, double ry);

  void drawArc(double xc, double yc, double xr, double yr, double angle1, double angle2);
  void fillArc(double xc, double yc, double xr, double yr, double angle1, double angle2);

  void drawPolygon(const std::vector<CPoint2D> &points);
  void fillPolygon(const std::vector<CPoint2D> &points);

  void drawText(double x, double y, const std::string &text, CFontPtr font, CHAlignType align);
  void fillText(double x, double y, const std::string &text, CFontPtr font, CHAlignType align);

  void pathInit();
  void pathTerm();
  void pathMoveTo(double x, double y);
  void pathLineTo(double x, double y);
  void pathRLineTo(double dx, double dy);
  void pathArcTo(double cx, double cy, double rx, double ry, double theta1, double theta2);
  void pathArcSegment(double xc, double yc, double th0, double th1,
                      double rx, double ry, double xAxisRotation);
  void pathBezier2To(double x1, double y1, double x2, double y2);
  void pathRBezier2To(double x1, double y1, double x2, double y2);
  void pathBezier3To(double x1, double y1, double x2, double y2, double x3, double y3);
  void pathRBezier3To(double x1, double y1, double x2, double y2, double x3, double y3);
  void pathClose();
  bool pathGetCurrentPoint(double *x, double *y);
  void pathStroke();
  void pathFill();
  void pathClip();
  void pathEoClip();
  void pathBBox(CBBox2D &bbox);
  void initClip();

  void addWidthToPoint(double x, double y, double angle, double width,
                       double *x1, double *y1, double *x2, double *y2) const;

  bool pathStringToParts(const std::string &data, PartList &parts);
  void drawParts(const PartList &parts, CSVGObjectMarker *marker=NULL);

  bool getPartsBBox(const PartList &parts, CBBox2D &bbox) const;
  void printParts(std::ostream &os, const PartList &parts) const;

  bool coordOption(const std::string &opt_name, const std::string &opt_value,
                   const std::string &name, double *real);
  bool lengthOption(const std::string &opt_name, const std::string &opt_value,
                    const std::string &name, double *real);
  bool lengthToReal(const std::string &str, double *real);
  bool angleOption(const std::string &opt_name, const std::string &opt_value,
                   const std::string &name, double *real);
  bool realOption(const std::string &opt_name, const std::string &opt_value,
                  const std::string &name, double *real);
  bool integerOption(const std::string &opt_name, const std::string &opt_value,
                     const std::string &name, long *integer);
  bool stringOption(const std::string &opt_name, const std::string &opt_value,
                    const std::string &name, std::string &str);
  bool bboxOption(const std::string &opt_name, const std::string &opt_value,
                  const std::string &name, CBBox2D *bbox);
  bool pointListOption(const std::string &opt_name, const std::string &opt_value,
                       const std::string &name, std::vector<CPoint2D> &points);
  bool realListOption(const std::string &opt_name, const std::string &opt_value,
                      const std::string &name, std::vector<double> &reals);

  bool decodeTransform(const std::string &str, CMatrix2D &matrix);

  bool decodePreserveAspectRatio(const std::string &str, CHAlignType *halign,
                                 CVAlignType *valign, CSVGScale *scale);

  double     decodeWidthString(const std::string &width_str);
  double     decodeOpacityString(const std::string &opacity_str);
  CFillType  decodeFillRuleString(const std::string &rule_str);
  bool       decodeDashString(const std::string &dash_str, CLineDash &dash);
  bool       decodeColorString(const std::string &color_str, CRGBA &rgba);
  CFontStyle decodeFontWeightString(const std::string &weight_str);
  CFontStyle decodeFontStyleString(const std::string &style_str);
  bool       decodePercentString(const std::string &str, double *real);
  bool       decodeUnitsString(const std::string &str, CSVGCoordUnits *units);
  bool       decodeGradientSpread(const std::string &str, CGradientSpreadType *spread);
  bool       decodeUrlObject(const std::string &str, CSVGObject **object);

  bool mmToPixel(double mm, double *pixel);

  void skipCommaSpace(CStrParse &parse);

  bool getTitle(std::string &str);

  void lengthToPixel(double xi, double yi, int *xo, int *yo);
  void windowToPixel(double xi, double yi, int *xo, int *yo);

  void setObjectById(const std::string &id, CSVGObject *object);

  CSVGObject *lookupObjectById(const std::string &id) const;

  bool readCSSFile(const std::string &filename);

  CSVGStyleData &getStyleData(const std::string &id);

  bool getStyleStrokeColor  (const std::string &id, CRGBA &rgba);
  bool getStyleStrokeOpacity(const std::string &id, double &opacity);
  bool getStyleStrokeWidth  (const std::string &id, double &width);
  bool getStyleStrokeDash   (const std::string &id, CLineDash &dash);
  bool getStyleFillColor    (const std::string &id, CRGBA &rgba);

 private:
  CConfig *getConfig();

  CSVGObject *tokenToObject(const CXMLToken *token);

 private:
  CSVG(const CSVG &rhs);
  CSVG &operator=(const CSVG &rhs);
};

namespace CSVGUtil {
  void convertArcCoords(double x1, double y1, double x2, double y2, double phi,
                        double rx, double ry, int fa, int fs, bool unit_circle,
                        double *cx, double *cy, double *xr, double *yr,
                        double *theta, double *delta);
}

#include <CSVGPathPart.h>

#endif

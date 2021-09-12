#ifndef CSVG_H
#define CSVG_H

#include <CSVGStroke.h>
#include <CSVGFill.h>
#include <CSVGClip.h>
#include <CSVGFontDef.h>
#include <CSVGBlock.h>
#include <CSVGEventValue.h>
#include <CSVGTimeValue.h>
#include <CSVGPreserveAspect.h>
#include <CSVGColor.h>
#include <CSVGOrient.h>

#include <CCSS.h>

#include <CScreenUnits.h>
#include <CMatrixStack2D.h>
#include <CBBox2D.h>
#include <CFillType.h>
#include <CAutoPtr.h>
#include <CGenGradient.h>
#include <CFontStyle.h>
#include <CFile.h>
#include <CStrUtil.h>

class CSVGPathPart;
class CSVGPathPartList;
class CSVGRenderer;
class CSVGBuffer;
class CSVGObject;

class CSVGAnchor;
class CSVGAnimate;
class CSVGAnimateColor;
class CSVGAnimateMotion;
class CSVGAnimateTransform;
class CSVGCircle;
class CSVGColorProfile;
class CSVGDefs;
class CSVGDesc;
class CSVGEllipse;
class CSVGFeBlend;
class CSVGFeColorMatrix;
class CSVGFeConvolveMatrix;
class CSVGFeComponentTransfer;
class CSVGFeComposite;
class CSVGFeDiffuseLighting;
class CSVGFeDisplacementMap;
class CSVGFeDistantLight;
class CSVGFeFlood;
class CSVGFont;
class CSVGFontFace;
class CSVGFontFaceSrc;
class CSVGFontFaceUri;
class CSVGFeFunc;
class CSVGFeGaussianBlur;
class CSVGFeImage;
class CSVGFeMerge;
class CSVGFeMergeNode;
class CSVGFeMorphology;
class CSVGFeOffset;
class CSVGFePointLight;
class CSVGFeSpecularLighting;
class CSVGFeSpotLight;
class CSVGFeTile;
class CSVGFeTurbulence;
class CSVGFilter;
class CSVGGroup;
class CSVGHKern;
class CSVGImage;
class CSVGLine;
class CSVGLinearGradient;
class CSVGMarker;
class CSVGMask;
class CSVGMetaData;
class CSVGMissingGlyph;
class CSVGMPath;
class CSVGPath;
class CSVGPattern;
class CSVGPolygon;
class CSVGPolyLine;
class CSVGRadialGradient;
class CSVGRect;
class CSVGScript;
class CSVGSet;
class CSVGStop;
class CSVGStyle;
class CSVGSymbol;
class CSVGSwitch;
class CSVGTBreak;
class CSVGText;
class CSVGTextPath;
class CSVGTitle;
class CSVGTSpan;
class CSVGUse;

class CSVGGlyph;
class CSVGObjectMarker;
class CSVGBufferMgr;
class CSVGImageData;

class CSVGJavaScript;

class CStrParse;
class CConfig;
class CXML;
class CXMLTag;
class CXMLToken;

struct CSVGXmlStyleSheet {
  COptString ref;
  COptBool   is_css;
};

class CSVG {
 public:
  using ObjectList = std::vector<CSVGObject *>;
  using NameToRGBA = std::map<std::string, CRGBA>;
  using Color      = CSVGInheritValT<CSVGColor>;
  using Opacity    = CSVGInheritValT<double>;
  using FillType   = CSVGInheritValT<CFillType>;
  using Width      = CSVGInheritValT<double>;

 public:
  struct DebugData {
    bool debug         { false };
    bool debugObjImage { false };
    bool debugImage    { false };
    bool debugFilter   { false };
    bool debugMask     { false };
    bool debugUse      { false };
  };

 public:
  CSVG(CSVGRenderer *renderer=nullptr);

  virtual ~CSVG();

  virtual CSVG *dup() const;

  //---

  void setRenderer(CSVGRenderer *renderer);
  CSVGRenderer *getRenderer() const { return renderer_; }

  //---

  const CSVGBlockData &rootBlockData() { return rootBlockData_; }

  const CPoint2D &rootBlockOffset() const { return rootBlockData_.offset(); }
  void setRootBlockOffset(const CPoint2D &o) { rootBlockData_.setOffset(o); }

  double rootBlockXScale() const { return rootBlockData_.xscale(); }
  void setRootBlockXScale(double s) { rootBlockData_.setXScale(s); }

  double rootBlockYScale() const { return rootBlockData_.yscale(); }
  void setRootBlockYScale(double s) { rootBlockData_.setYScale(s); }

  const CSVGPreserveAspect &blockPreserveAspect() const { return rootBlockData_.preserveAspect(); }
  void setBlockPreserveAspect(const CSVGPreserveAspect &a) { rootBlockData_.setPreserveAspect(a); }

  //---

  const CSVGBlockData &blockData() { return blockData_; }

  const CPoint2D &offset() const { return blockData_.offset(); }
//void setOffset(const CPoint2D &o) { blockData_.setOffset(o); }

  double xscale() const { return blockData_.xscale(); }
//void setXScale(double s) { blockData_.setXScale(s); }

  double yscale() const { return blockData_.yscale(); }
//void setYScale(double s) { blockData_.setYScale(s); }

  const CSVGPreserveAspect &preserveAspect() const { return blockData_.preserveAspect(); }
//void setPreserveAspect(const CSVGPreserveAspect &a) { blockData_.setPreserveAspect(a); }

  //---

  CPoint2D flatOffset() const;

  double flatXScale() const;
  double flatYScale() const;

  //---

  virtual CSVGRenderer *createRenderer();

  CSVGBlock *getRoot() const;

  CSVGBlock *getRootBlock() const;

  //---

  virtual void updateBusy() { }

  //---

  CSVGBuffer *pushBuffer(const std::string &name);
  CSVGBuffer *popBuffer();

  void printBufferStack(const std::string &desc) const;

  CSVGBuffer *getCurrentBuffer() const;
  void setCurrentBuffer(CSVGBuffer *buffer);

  CSVGBuffer *getBuffer(const std::string &name);

  void getBufferNames(std::vector<std::string> &names, bool includeAlpha=true) const;

  //---

  void beginDrawBuffer (CSVGBuffer *buffer, const CPoint2D &offset, double xs, double ys);
  void beginDrawBuffer (CSVGBuffer *buffer, const CBBox2D &bbox);
  void beginDrawBuffer (CSVGBuffer *buffer, const CBBox2D &pixelBBox, const CBBox2D &viewBBox,
                        const CPoint2D &offset, double xs, double ys,
                        const CSVGPreserveAspect &preserveAspect);
  void endDrawBuffer   (CSVGBuffer *buffer);
  void updateDrawBuffer(CSVGBuffer *buffer);

  //---

  void setPaintBox(const CBBox2D &bbox);

  void setPaintStyle(const CSVGFill &fill, const CSVGStroke &stroke);

  //---

  void setAntiAlias(bool flag);

  CSVGObject *currentDrawObject() const;
  void pushDrawObject(CSVGObject *o);
  void popDrawObject();

  bool getUniquify() const { return uniquify_; }
  void setUniquify(bool uniquify) { uniquify_ = uniquify; }

  bool getAutoName() const { return autoName_; }
  void setAutoName(bool autoName);

  bool getIgnoreFilter() const { return ignoreFilter_; }
  void setIgnoreFilter(bool b) { ignoreFilter_ = b; }

  //---

  CSVGObject *getAltRoot() const { return altRoot_; }
  void setAltRoot(CSVGObject *o);

  const CSize2D &getAltSize() const { return altSize_; }
  void setAltSize(const CSize2D &s) { altSize_ = s; }

  //---

  // get/set debug
  bool getDebug() const { return debugData_.debug; }
  void setDebug(bool b);

  bool getDebugObjImage() const { return debugData_.debugObjImage; }
  void setDebugObjImage(bool b) { debugData_.debugObjImage = b; }

  bool getDebugImage() const { return debugData_.debugImage; }
  void setDebugImage(bool b) { debugData_.debugImage = b; }

  bool getDebugFilter() const { return debugData_.debugFilter; }
  void setDebugFilter(bool b) { debugData_.debugFilter = b; }

  bool getDebugMask() const { return debugData_.debugMask; }
  void setDebugMask(bool b) { debugData_.debugMask = b; }

  bool getDebugUse() const { return debugData_.debugUse; }
  void setDebugUse(bool b) { debugData_.debugUse = b; }

  //---

  // get/set quiet
  bool isQuiet() const { return quiet_; }
  void setQuiet(bool b) { quiet_ = b; }

  //---

  // get/set check view box
  bool isCheckViewBox() const { return checkViewBox_; }
  void setCheckViewBox(bool b) { checkViewBox_ = b; }

  //---

  void init();

  void clear();

  bool read(const std::string &filename);
  bool read(const std::string &filename, CSVGObject *object);

  //---

  virtual CSVGObject *createObjectByName(const std::string &name);

#ifdef CSVG_JAVASCRIPT
  virtual CSVGJavaScript          *createJavaScript();
#endif
  virtual CSVGBlock               *createBlock();
  virtual CSVGAnchor              *createAnchor();
  virtual CSVGAnimate             *createAnimate();
  virtual CSVGAnimateColor        *createAnimateColor();
  virtual CSVGAnimateMotion       *createAnimateMotion();
  virtual CSVGAnimateTransform    *createAnimateTransform();
  virtual CSVGAudio               *createAudio();
  virtual CSVGCircle              *createCircle();
  virtual CSVGClipPath            *createClipPath();
  virtual CSVGColorProfile        *createColorProfile();
  virtual CSVGDefs                *createDefs();
  virtual CSVGDesc                *createDesc();
  virtual CSVGEllipse             *createEllipse();
  virtual CSVGFeBlend             *createFeBlend();
  virtual CSVGFeColorMatrix       *createFeColorMatrix();
  virtual CSVGFeConvolveMatrix    *createFeConvolveMatrix();
  virtual CSVGFeComponentTransfer *createFeComponentTransfer();
  virtual CSVGFeComposite         *createFeComposite();
  virtual CSVGFeDiffuseLighting   *createFeDiffuseLighting();
  virtual CSVGFeDisplacementMap   *createFeDisplacementMap();
  virtual CSVGFeDistantLight      *createFeDistantLight();
  virtual CSVGFeFlood             *createFeFlood();
  virtual CSVGFeFunc              *createFeFunc(CRGBAComponent component);
  virtual CSVGFeGaussianBlur      *createFeGaussianBlur();
  virtual CSVGFeImage             *createFeImage();
  virtual CSVGFeMerge             *createFeMerge();
  virtual CSVGFeMergeNode         *createFeMergeNode();
  virtual CSVGFeMorphology        *createFeMorphology();
  virtual CSVGFeOffset            *createFeOffset();
  virtual CSVGFePointLight        *createFePointLight();
  virtual CSVGFeSpecularLighting  *createFeSpecularLighting();
  virtual CSVGFeSpotLight         *createFeSpotLight();
  virtual CSVGFeTile              *createFeTile();
  virtual CSVGFeTurbulence        *createFeTurbulence();
  virtual CSVGFilter              *createFilter();
  virtual CSVGFont                *createFont();
  virtual CSVGFontFace            *createFontFace();
  virtual CSVGFontFaceSrc         *createFontFaceSrc();
  virtual CSVGFontFaceUri         *createFontFaceUri();
  virtual CSVGGlyph               *createGlyph();
  virtual CSVGGroup               *createGroup();
  virtual CSVGHKern               *createHKern();
  virtual CSVGImage               *createImage();
  virtual CSVGLine                *createLine();
  virtual CSVGLinearGradient      *createLinearGradient();
  virtual CSVGMarker              *createMarker();
  virtual CSVGMask                *createMask();
  virtual CSVGMetaData            *createMetaData();
  virtual CSVGMissingGlyph        *createMissingGlyph();
  virtual CSVGMPath               *createMPath();
  virtual CSVGPath                *createPath();
  virtual CSVGPattern             *createPattern();
  virtual CSVGPolygon             *createPolygon();
  virtual CSVGPolyLine            *createPolyLine();
  virtual CSVGRadialGradient      *createRadialGradient();
  virtual CSVGRect                *createRect();
  virtual CSVGScript              *createScript();
  virtual CSVGSet                 *createSet();
  virtual CSVGStop                *createStop();
  virtual CSVGSymbol              *createSymbol();
  virtual CSVGStyle               *createStyle();
  virtual CSVGSwitch              *createSwitch();
  virtual CSVGTBreak              *createTBreak();
  virtual CSVGText                *createText();
  virtual CSVGTextPath            *createTextPath();
  virtual CSVGTitle               *createTitle();
  virtual CSVGTSpan               *createTSpan();
  virtual CSVGUse                 *createUse();

  virtual CSVGBuffer *createBuffer(const std::string &name);

  virtual CSVGImageData *createImageData();

  virtual CSVGFontObj *createFontObj(const CSVGFontDef &def);

  virtual CSVGPathMoveTo   *createPathMoveTo  (double x, double y);
  virtual CSVGPathRMoveTo  *createPathRMoveTo (double x, double y);
  virtual CSVGPathLineTo   *createPathLineTo  (double x, double y);
  virtual CSVGPathRLineTo  *createPathRLineTo (double x, double y);
  virtual CSVGPathHLineTo  *createPathHLineTo (double x);
  virtual CSVGPathRHLineTo *createPathRHLineTo(double x);
  virtual CSVGPathVLineTo  *createPathVLineTo (double y);
  virtual CSVGPathRVLineTo *createPathRVLineTo(double y);

  virtual CSVGPathArcTo  *createPathArcTo (double rx, double ry, double xa, double fa,
                                           double fs, double x2, double y2);
  virtual CSVGPathRArcTo *createPathRArcTo(double rx, double ry, double xa, double fa,
                                           double fs, double x2, double y2);

  virtual CSVGPathBezier2To   *createPathBezier2To  (double x1, double y1, double x2, double y2);
  virtual CSVGPathMBezier2To  *createPathMBezier2To (double x2, double y2);
  virtual CSVGPathRBezier2To  *createPathRBezier2To (double x1, double y1, double x2, double y2);
  virtual CSVGPathMRBezier2To *createPathMRBezier2To(double x2, double y2);
  virtual CSVGPathBezier3To   *createPathBezier3To  (double x1, double y1, double x2, double y2,
                                                     double x3, double y3);
  virtual CSVGPathMBezier3To  *createPathMBezier3To (double x2, double y2, double x3, double y3);
  virtual CSVGPathRBezier3To  *createPathRBezier3To (double x1, double y1, double x2, double y2,
                                                     double x3, double y3);
  virtual CSVGPathMRBezier3To *createPathMRBezier3To(double x2, double y2, double x3, double y3);

  virtual CSVGPathClosePath *createPathClosePath(bool relative=false);

  //---

  virtual void redraw() { }

  //---

  double getXMin() const;
  double getYMin() const;

  double getWidth () const;
  double getHeight() const;

  int getIWidth () const { return int(getWidth ()); }
  int getIHeight() const { return int(getHeight()); }

  //---

  const CRGBA &background() const { return background_; }
  void setBackground(const CRGBA &c) { background_ = c; }

  //---

  const CSVGStroke &getStroke() const { return styleData_.stroke; }
  void setStroke(const CSVGStroke &stroke) { styleData_.stroke = stroke; }

  const CSVGFill &getFill() const { return styleData_.fill; }
  void setFill(const CSVGFill &fill) { styleData_.fill = fill; }

  const CSVGClip &getClip() const { return styleData_.clip; }
  void setClip(const CSVGClip &clip) { styleData_.clip = clip; }

  const CSVGFontDef &getFontDef() const { return styleData_.fontDef; }
  void setFontDef(const CSVGFontDef &fontDef) { styleData_.fontDef = fontDef; }

  CSVGObject *styleObject() const { return styleData_.object; }
  void setStyleObject(CSVGObject *o) { styleData_.object = o; }

  CRGBA colorToRGBA(const Color &color) const;

  //---

  static const NameToRGBA &getColors() { return colors_; }

  //---

#ifdef CSVG_JAVASCRIPT
  CSVGJavaScript *js() { return js_; }
#endif

  CSVGObject *eventObject() { return eventObject_; }
  void setEventObject(CSVGObject *p) { eventObject_ = p; }

  void execJsEvent(CSVGObject *obj, const std::string &str);

  //---

  //bool processOption(const std::string &opt_name, const std::string &opt_value);

  //---

  double fontDpi() const { return fontDpi_; }
  void setFontDpi(double r) { fontDpi_ = r; }

  //---

  void addFont(CSVGFont *font);

  CSVGFont *getFont() const;

  CSVGGlyph *getCharGlyph(char c) const;
  CSVGGlyph *getUnicodeGlyph(const std::string &unicode) const;

  //---

  void drawToBuffer(CSVGBuffer *buffer, int w, int h, const CPoint2D &offset=CPoint2D(0, 0),
                    double xscale=1, double yscale=1);

  void drawToRenderer(CSVGRenderer *renderer, int w, int h, const CPoint2D &offset=CPoint2D(0, 0),
                      double xscale=1, double yscale=1);

  bool hasAnimation() const;

  void draw();

  void draw(const CPoint2D &offset, double xscale, double yscale);

  void drawRoot(CSVGBlock *block, const CPoint2D &offset=CPoint2D(0, 0),
                double xscale=1, double yscale=1,
                const CSVGPreserveAspect &preserveAspect=CSVGPreserveAspect());

  virtual void drawBackground() { }
  virtual void drawForeground() { }

  //---

  void pushStyle(CSVGObject *object);
  void popStyle();

  void resetStroke();
  void updateStroke(const CSVGStroke &stroke);
  bool isStroked() const;

  void updatedSelectedStroke();

  bool hasSelectedStroke() const { return selectedStroke_; }
  void setSelectedStroke(bool b) { selectedStroke_ = b; }

  void resetFill();
  void updateFill(const CSVGFill &fill);
  bool isFilled() const;

  void setStrokeBuffer(CSVGBuffer *buffer);
  void setFillBuffer  (CSVGBuffer *buffer);

  void resetClip();
  void updateClip(const CSVGClip &clip);
  bool isClipped() const;
  void setClip();

  void resetFontDef();
  void updateFontDef(const CSVGFontDef &fontDef);

  //---

  void setTransform(const CMatrixStack2D &matrix);

  void drawLine(double x1, double y1, double x2, double y2);

  void drawCircle (double x, double y, double r);
  void drawEllipse(double x, double y, double rx, double ry);

  void drawArc(double xc, double yc, double xr, double yr, double angle1, double angle2);
  void fillArc(double xc, double yc, double xr, double yr, double angle1, double angle2);

  void drawPolygon(const std::vector<CPoint2D> &points);
  void fillPolygon(const std::vector<CPoint2D> &points);

  void fillDrawText(double x, double y, const std::string &text, const CSVGFontDef &fontDef,
                    CHAlignType align, bool isFilled, bool isStroked);

  void drawText(double x, double y, const std::string &text, const CSVGFontDef &fontDef,
                CHAlignType align);
  void fillText(double x, double y, const std::string &text, const CSVGFontDef &fontDef,
                CHAlignType align);

  bool pathOption(const std::string &opt_name, const std::string &opt_value,
                  const std::string &name, CSVGPathPartList &parts);
  bool pathStringToParts(const std::string &data, CSVGPathPartList &parts);

  void drawParts(const CSVGPathPartList &parts);

  void drawMarkers(const std::vector<CPoint2D> &points, const std::vector<double> &angles);

  //---

  // coord args
  bool coordListOption(const std::string &opt_name, const std::string &opt_value,
                       const std::string &name, std::vector<CScreenUnits> &lengths);

  bool coordOption(const std::string &opt_name, const std::string &opt_value,
                   const std::string &name, CScreenUnits &length);
  bool coordOption(const std::string &opt_name, const std::string &opt_value,
                   const std::string &name, double *real);
  static bool decodeCoordValue(const std::string &str, CScreenUnits &length);

  //---

  // font family option
  bool fontFamilyOption(const std::string &optName, const std::string &optValue,
                        const std::string &name, std::string &family, bool &inherit);

  // font size option
  bool fontSizeOption(const std::string &opt_name, const std::string &opt_value,
                      const std::string &name, CScreenUnits &length, bool &inherit);

  // letter spacing option
  bool letterSpacingOption(const std::string &opt_name, const std::string &opt_value,
                           const std::string &name, CScreenUnits &length);

  // word spacing option
  bool wordSpacingOption(const std::string &opt_name, const std::string &opt_value,
                         const std::string &name, CScreenUnits &length);

  //---

  // length args
  bool lengthOption(const std::string &opt_name, const std::string &opt_value,
                    const std::string &name, CScreenUnits &length);
  static COptValT<CScreenUnits> decodeLengthValue(const std::string &str);

  bool lengthListOption(const std::string &opt_name, const std::string &opt_value,
                        const std::string &name, std::vector<CScreenUnits> &lengths);
  static bool decodeLengthListValue(const std::string &str, std::vector<CScreenUnits> &lengths);

  //---

  bool realOption(const std::string &opt_name, const std::string &opt_value,
                  const std::string &name, double *real);
  bool integerOption(const std::string &opt_name, const std::string &opt_value,
                     const std::string &name, long *integer);
  bool stringOption(const std::string &opt_name, const std::string &opt_value,
                    const std::string &name, std::string &str);

  //---

  // orient arg
  bool orientOption(const std::string &opt_name, const std::string &opt_value,
                    const std::string &name, CSVGOrient &orient);
  static COptValT<CSVGOrient> decodeOrientString(const std::string &str);
  static std::string encodeOrientString(const CSVGOrient &orient);

  static void printNameOrient(std::ostream &os, const std::string &name,
                              const COptValT<CSVGOrient> &o) {
    if (o.isValid())
      os << " " << name << "=\"" + encodeOrientString(o.getValue()) + "\"";
  }

  //---

  // angle arg
  bool angleOption(const std::string &opt_name, const std::string &opt_value,
                   const std::string &name, CAngle &angle);
  static COptValT<CAngle> decodeAngleString(const std::string &str);
  static std::string encodeAngleString(const CAngle &angle);

  static void printNameAngle(std::ostream &os, const std::string &name,
                             const COptValT<CAngle> &a) {
    if (a.isValid())
      os << " " << name << "=\"" + encodeAngleString(a.getValue()) + "\"";
  }

  //---

  bool percentOption(const std::string &opt_name, const std::string &opt_value,
                     const std::string &name, CScreenUnits &length);
  static bool decodePercentString(const std::string &str, CScreenUnits &length);

  //---

  bool coordUnitsOption(const std::string &opt_name, const std::string &opt_value,
                        const std::string &name, CSVGCoordUnits &units);
  static bool decodeUnitsString(const std::string &str, CSVGCoordUnits &units);
  static std::string encodeUnitsString(const CSVGCoordUnits &units);

  //---

  // bbox
  bool bboxOption(const std::string &opt_name, const std::string &opt_value,
                  const std::string &name, CBBox2D &bbox);
  static bool decodeBBoxString(const std::string &name, CBBox2D &bbox);

  //---

  // preserve aspect
  bool preserveAspectOption(const std::string &opt_name, const std::string &opt_value,
                            const std::string &name, CSVGPreserveAspect &preserveAspect);
  static bool decodePreserveAspectRatio(const std::string &str,
                                        CSVGPreserveAspect &preserveAspect);

  // point list
  bool pointListOption(const std::string &opt_name, const std::string &opt_value,
                       const std::string &name, std::vector<CPoint2D> &points);

  // real list
  bool realListOption(const std::string &opt_name, const std::string &opt_value,
                      const std::string &name, std::vector<double> &reals);
  static bool stringToReals(const std::string &str, std::vector<double> &reals);

  // event value
  bool eventValueOption(const std::string &opt_name, const std::string &opt_value,
                        const std::string &name, CSVGEventValue &event);

  // time value
  bool timeValueOption(const std::string &opt_name, const std::string &opt_value,
                       const std::string &name, CSVGTimeValue &time);
  bool stringToTime(const std::string &str, CSVGTimeValue &time) const;

  //---

  // transform
  bool transformOption(const std::string &opt_name, const std::string &opt_value,
                       const std::string &name, CMatrixStack2D &matrix);
  static bool decodeTransform(const std::string &str, CMatrixStack2D &matrix);

  //---

  // url
  bool urlOption(const std::string &opt_name, const std::string &opt_value,
                 const std::string &name, CSVGObject **obj);
  bool decodeUrlObject(const std::string &str, std::string &id, CSVGObject **object) const;

  //---

  // boolean
  bool booleanOption(const std::string &opt_name, const std::string &opt_value,
                     const std::string &name, bool &b);
  static bool decodeBoolean(const std::string &str, bool &b);

  //---

  static bool decodeWidthString(const std::string &width_str, double &width, bool &inherit);

  bool opacityOption(const std::string &optName, const std::string &optValue,
                     const std::string &name, double &opacity, bool &inherit);
  static bool decodeOpacityString(const std::string &opacity_str,
                                  double &opacity, bool &inherit);

  static FillType decodeFillRuleString(const std::string &rule_str);
  static std::string encodeFillRuleString(const FillType &rule);

  static bool decodeDashString(const std::string &dash_str,
                               std::vector<CScreenUnits> &lengths, bool &solid);

  bool colorOption(const std::string &opt_name, const std::string &opt_value,
                   const std::string &name, CSVGColor &color, bool &inherit);
  static bool decodeColorString(const std::string &color_str, CSVGColor &color, bool &inherit);

  static bool decodeRGBAString(const std::string &color_str, CRGBA &rgba);

  static CSVGColor nameToColor(const std::string &name);

  static CRGBA nameToRGBA(const std::string &name);

  static CFontStyle decodeFontWeightString(const std::string &weightStr, bool &inherit);
  static CFontStyle decodeFontStyleString(const std::string &styleStr, bool &inherit);

  //---

  static bool decodeGradientSpread(const std::string &str, CGradientSpreadType &spread);
  static std::string encodeGradientSpread(const CGradientSpreadType &spread);

  //---

  //bool mmToPixel(double mm, double *pixel);

  bool getTitle(std::string &str);

  CBBox2D transformBBox(const CMatrixStack2D &m, const CBBox2D &bbox) const;
  CBBox2D untransformBBox(const CMatrixStack2D &m, const CBBox2D &bbox) const;

  void lengthToPixel(double xi, double yi, double *xo, double *yo);
  void windowToPixel(double xi, double yi, double *xo, double *yo);

  void setObjectById(const std::string &id, CSVGObject *object);

  CSVGObject *lookupObjectById(const std::string &id) const;

  bool readCSSFile  (const std::string &filename);
  bool readCSSString(const std::string &str);

  bool processCSSIds();

  void setScript(const std::string &str);
  void setScriptFile(const std::string &filename);

  //---

  bool applyStyle(CSVGObject *obj);

  bool visitStyleData(const CCSS &css, const CCSSTagDataP &obj);

  //---

  void getAllChildren(ObjectList &objects) const;

  void getObjectsAtPoint(const CPoint2D &p, ObjectList &objects) const;

  void getChildrenOfId(const std::string &id, ObjectList &objects) const;

  void getSelectedObjects(ObjectList &objects) const;

  //--

  void sendEvent(CSVGEventType type, const std::string &id="", const std::string &data="");

  void print(std::ostream &os, bool hier=false) const;

  void printFlat(std::ostream &os, bool force=false) const;

  void syntaxError(const std::string &msg);

 private:
  CConfig *getConfig();

  CSVGObject *tokenToObject(CSVGObject *parent, const CXMLToken *token);

 private:
  CSVG(const CSVG &rhs);
  CSVG &operator=(const CSVG &rhs);

 private:
  struct StyleData {
    StyleData(CSVG &svg) :
     stroke(svg), fill(svg), clip(svg), fontDef(svg) {
    }

    void reset() {
      stroke .reset();
      fill   .reset();
      clip   .reset();
      fontDef.reset();

      object = 0;
    }

    CSVGStroke  stroke;
    CSVGFill    fill;
    CSVGClip    clip;
    CSVGFontDef fontDef;
    CSVGObject* object { nullptr };
  };

  struct AltData {
    CSVGBlock  *block  { nullptr };
    CSVGObject *object { nullptr };
    CSVGObject *parent { nullptr };

    bool isSet() const { return block; }

    void reset() {
      block  = nullptr;
      object = nullptr;
      parent = nullptr;
    }
  };

  using BufferStack      = std::vector<CSVGBuffer *>;
  using FontList         = std::vector<CSVGFont *>;
  using NameObjectMap    = std::map<std::string, CSVGObject *>;
  using StyleDataStack   = std::vector<StyleData>;
  using BlockDataStack   = std::vector<CSVGBlockData>;
  using BufferMgrP       = CAutoPtr<CSVGBufferMgr>;
  using BlockP           = CAutoPtr<CSVGBlock>;
  using XMLP             = CAutoPtr<CXML>;
  using OptXmlStyleSheet = COptValT<CSVGXmlStyleSheet>;
  using CSSList          = std::vector<CCSS>;

  static NameToRGBA colors_;

  CSVGRenderer*    renderer_       { nullptr };
  BufferMgrP       bufferMgr_;
  CSVGBuffer*      buffer_         { nullptr };
  BufferStack      bufferStack_;
  CSVGBlockData    rootBlockData_;
  CSVGBlockData    blockData_;
  BlockDataStack   blockDataStack_;
  BlockP           block_;
  XMLP             xml_;
  CXMLTag*         xmlTag_         { nullptr };
  CRGBA            background_     { 1, 1, 1};
  StyleData        styleData_;
  StyleDataStack   styleDataStack_;
  bool             selectedStroke_ { true };
  double           fontDpi_        { 100.0 };
  FontList         fontList_;
  NameObjectMap    idObjectMap_;
  CSSList          cssList_;
#ifdef CSVG_JAVASCRIPT
  CSVGJavaScript*  js_             { nullptr };
#endif
  CSVGObject*      eventObject_    { nullptr };
  ObjectList       drawObjects_;
  OptXmlStyleSheet xmlStyleSheet_;
  bool             uniquify_       { false };
  bool             autoName_       { false };
  bool             ignoreFilter_   { false };
  CSize2D          altSize_;
  CSVGObject*      altRoot_        { nullptr };
  AltData          altData_;
  DebugData        debugData_;
  bool             quiet_          { false };
  bool             checkViewBox_   { false };
};

#endif

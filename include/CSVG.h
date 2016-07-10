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
#include <CSVGCSSData.h>
#include <CSVGColor.h>

#include <CCSS.h>
#include <CJavaScript.h>

#include <CFont.h>
#include <CScreenUnits.h>
#include <CMatrixStack2D.h>
#include <CBBox2D.h>
#include <CFillType.h>
#include <CAutoPtr.h>
#include <CGenGradient.h>
#include <CFont.h>
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
  typedef std::vector<CSVGObject *>    ObjectList;
  typedef std::map<std::string, CRGBA> Colors;

 public:
  CSVG(CSVGRenderer *renderer=0);

  virtual ~CSVG();

  virtual CSVG *dup() const;

  void          setRenderer(CSVGRenderer *renderer);
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

  bool getDebug() const { return debug_; }
  void setDebug(bool b);

  bool getDebugObjImage() const { return debugObjImage_; }
  void setDebugObjImage(bool b);

  bool getDebugImage() const { return debugImage_; }
  void setDebugImage(bool b);

  bool getDebugFilter() const { return debugFilter_; }
  void setDebugFilter(bool b);

  bool getDebugMask() const { return debugMask_; }
  void setDebugMask(bool b);

  bool getDebugUse() const { return debugUse_; }
  void setDebugUse(bool b);

  void init();

  void clear();

  bool read(const std::string &filename);
  bool read(const std::string &filename, CSVGObject *object);

  virtual CSVGObject *createObjectByName(const std::string &name);

  virtual CSVGJavaScript          *createJavaScript();
  virtual CSVGBlock               *createBlock();
  virtual CSVGAnchor              *createAnchor();
  virtual CSVGAnimate             *createAnimate();
  virtual CSVGAnimateColor        *createAnimateColor();
  virtual CSVGAnimateMotion       *createAnimateMotion();
  virtual CSVGAnimateTransform    *createAnimateTransform();
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
  virtual CSVGFeFunc              *createFeFunc(CColorComponent component);
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

  virtual CSVGPathClosePath *createPathClosePath(bool relative);

  virtual void redraw() { }

  double getXMin() const;
  double getYMin() const;

  double getWidth () const;
  double getHeight() const;

  int getIWidth () const { return int(getWidth ()); }
  int getIHeight() const { return int(getHeight()); }

  const CRGBA &background() const { return background_; }
  void setBackground(const CRGBA &v) { background_ = v; }

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

  CRGBA colorToRGBA(const CSVGColor &color) const;

  //---

  const CSVGCSSData &getCSSData() const { return cssData_; }

  //---

  const Colors &getColors() const { return colors_; }

  //---

  CSVGJavaScript *js() { return js_; }

  CJObjectTypeP jsDocumentType      () const { return jsDocumentType_; }
  CJObjectTypeP jsObjectType        () const { return jsObjectType_; }
  CJObjectTypeP jsEventType         () const { return jsEventType_; }
  CJObjectTypeP jsTransformStackType() const { return jsTransformStackType_; }
  CJObjectTypeP jsTransformType     () const { return jsTransformType_; }
  CJObjectTypeP jsMatrixType        () const { return jsMatrixType_; }

  CJValueP jsDocument() const { return jsDocument_; }
  CJValueP jsEvent   () const { return jsEvent_; }

  CSVGObject *eventObject() { return eventObject_; }
  void setEventObject(CSVGObject *p) { eventObject_ = p; }

  void execJsEvent(CSVGObject *obj, const std::string &str);

  //---

  //bool processOption(const std::string &opt_name, const std::string &opt_value);

  void addFont(CSVGFont *font);

  CSVGFont *getFont() const;

  CSVGGlyph *getCharGlyph(char c) const;
  CSVGGlyph *getUnicodeGlyph(const std::string &unicode) const;

  void drawToBuffer(CSVGBuffer *buffer, int w, int h, const CPoint2D &offset=CPoint2D(0,0),
                    double xscale=1, double yscale=1);

  void drawToRenderer(CSVGRenderer *renderer, int w, int h, const CPoint2D &offset=CPoint2D(0,0),
                      double xscale=1, double yscale=1);

  bool hasAnimation() const;

  void draw();

  void draw(const CPoint2D &offset, double xscale, double yscale);

  void drawRoot(CSVGBlock *block, const CPoint2D &offset=CPoint2D(0,0),
                double xscale=1, double yscale=1,
                const CSVGPreserveAspect &preserveAspect=CSVGPreserveAspect());

  //---

  void pushStyle(CSVGObject *object);
  void popStyle();

  void resetStroke();
  void updateStroke(const CSVGStroke &stroke);
  void setSelectedStroke();
  bool isStroked() const;

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

  bool coordOption(const std::string &opt_name, const std::string &opt_value,
                   const std::string &name, CScreenUnits &length);
  bool coordOption(const std::string &opt_name, const std::string &opt_value,
                   const std::string &name, double *real);
  bool lengthOption(const std::string &opt_name, const std::string &opt_value,
                    const std::string &name, CScreenUnits &length);
  bool angleOption(const std::string &opt_name, const std::string &opt_value,
                   const std::string &name, double *real);
  bool realOption(const std::string &opt_name, const std::string &opt_value,
                  const std::string &name, double *real);
  bool integerOption(const std::string &opt_name, const std::string &opt_value,
                     const std::string &name, long *integer);
  bool stringOption(const std::string &opt_name, const std::string &opt_value,
                    const std::string &name, std::string &str);
  bool percentOption(const std::string &opt_name, const std::string &opt_value,
                     const std::string &name, CScreenUnits &length);
  bool coordUnitsOption(const std::string &opt_name, const std::string &opt_value,
                        const std::string &name, CSVGCoordUnits &units);
  bool bboxOption(const std::string &opt_name, const std::string &opt_value,
                  const std::string &name, CBBox2D *bbox);

  bool preserveAspectOption(const std::string &opt_name, const std::string &opt_value,
                            const std::string &name, CSVGPreserveAspect &preserveAspect);

  bool pointListOption(const std::string &opt_name, const std::string &opt_value,
                       const std::string &name, std::vector<CPoint2D> &points);

  bool realListOption(const std::string &opt_name, const std::string &opt_value,
                      const std::string &name, std::vector<double> &reals);
  bool stringToReals(const std::string &str, std::vector<double> &reals);

  bool eventValueOption(const std::string &opt_name, const std::string &opt_value,
                        const std::string &name, CSVGEventValue &event);
  bool timeValueOption(const std::string &opt_name, const std::string &opt_value,
                       const std::string &name, CSVGTimeValue &time);
  bool transformOption(const std::string &opt_name, const std::string &opt_value,
                       const std::string &name, CMatrixStack2D &matrix);

  bool stringToTime(const std::string &str, CSVGTimeValue &time) const;

  bool decodeLengthValue(const std::string &str, CScreenUnits &lvalue);

  bool urlOption(const std::string &opt_name, const std::string &opt_value,
                 const std::string &name, CSVGObject **obj);

  bool decodeTransform(const std::string &str, CMatrixStack2D &matrix);

  bool decodePreserveAspectRatio(const std::string &str, CHAlignType *halign,
                                 CVAlignType *valign, CSVGScale *scale);

  double decodeWidthString(const std::string &width_str);
  double decodeOpacityString(const std::string &opacity_str);

  CFillType   decodeFillRuleString(const std::string &rule_str) const;
  std::string encodeFillRuleString(CFillType rule) const;

  bool       decodeDashString(const std::string &dash_str,
                              std::vector<CScreenUnits> &lengths, bool &solid);
  bool       decodeColorString(const std::string &color_str, CSVGColor &color);
  bool       decodeRGBAString(const std::string &color_str, CRGBA &rgba);
  CSVGColor  nameToColor(const std::string &name) const;
  CRGBA      nameToRGBA(const std::string &name) const;
  CFontStyle decodeFontWeightString(const std::string &weight_str);
  CFontStyle decodeFontStyleString(const std::string &style_str);
  bool       decodePercentString(const std::string &str, CScreenUnits &length);

  static bool decodeUnitsString(const std::string &str, CSVGCoordUnits &units);
  static std::string encodeUnitsString(const CSVGCoordUnits &units);

  static bool decodeGradientSpread(const std::string &str, CGradientSpreadType &spread);
  static std::string encodeGradientSpread(const CGradientSpreadType &spread);

  bool decodeUrlObject(const std::string &str, std::string &id, CSVGObject **object);

  //bool mmToPixel(double mm, double *pixel);

  void skipCommaSpace(CStrParse &parse);

  bool getTitle(std::string &str);

  CBBox2D transformBBox(const CMatrixStack2D &m, const CBBox2D &bbox) const;

  void lengthToPixel(double xi, double yi, double *xo, double *yo);
  void windowToPixel(double xi, double yi, double *xo, double *yo);

  void setObjectById(const std::string &id, CSVGObject *object);

  CSVGObject *lookupObjectById(const std::string &id) const;

  bool readCSSFile  (const std::string &filename);
  bool readCSSString(const std::string &str);

  bool processCSSIds();

  void addStyleValues(CSVGStyleData &svgStyleData, const CCSS::StyleData &cssStyleData);

  void setScript(const std::string &str);

  CSVGStyleData &getGlobalStyleData   ();
  CSVGStyleData &getNameStyleData     (const std::string &objName);
  CSVGStyleData &getTypeStyleData     (const std::string &objType);
  CSVGStyleData &getClassStyleData    (const std::string &objClass);
  CSVGStyleData &getTypeClassStyleData(const std::string &objType, const std::string &objClass);

  bool getStyleStrokeColor     (const CSVGObject *obj, CSVGColor &color, CSVGCSSType &type);
  bool getStyleStrokeOpacity   (const CSVGObject *obj, double &opacity, CSVGCSSType &type);
  bool getStyleStrokeRule      (const CSVGObject *obj, CFillType &rule, CSVGCSSType &type);
  bool getStyleStrokeUrl       (const CSVGObject *obj, std::string &url, CSVGCSSType &type);
  bool getStyleStrokeFillObject(const CSVGObject *obj, CSVGObject* &object, CSVGCSSType &type);
  bool getStyleStrokeWidth     (const CSVGObject *obj, double &width, CSVGCSSType &type);
  bool getStyleStrokeDash      (const CSVGObject *obj, CSVGStrokeDash &dash, CSVGCSSType &type);
  bool getStyleStrokeCap       (const CSVGObject *obj, CLineCapType &cap, CSVGCSSType &type);
  bool getStyleStrokeJoin      (const CSVGObject *obj, CLineJoinType &join, CSVGCSSType &type);
  bool getStyleStrokeMitreLimit(const CSVGObject *obj, double &limit, CSVGCSSType &type);

  bool getStyleFillColor     (const CSVGObject *obj, CSVGColor &color, CSVGCSSType &type);
  bool getStyleFillOpacity   (const CSVGObject *obj, double &opacity, CSVGCSSType &type);
  bool getStyleFillRule      (const CSVGObject *obj, CFillType &rule, CSVGCSSType &type);
  bool getStyleFillUrl       (const CSVGObject *obj, std::string &url, CSVGCSSType &type);
  bool getStyleFillFillObject(const CSVGObject *obj, CSVGObject* &object, CSVGCSSType &type);

  bool getStyleMarkerStart(const CSVGObject *obj, CSVGObject* &marker, CSVGCSSType &type);
  bool getStyleMarkerMid  (const CSVGObject *obj, CSVGObject* &marker, CSVGCSSType &type);
  bool getStyleMarkerEnd  (const CSVGObject *obj, CSVGObject* &marker, CSVGCSSType &type);

  void getAllChildren(ObjectList &objects) const;

  void getObjectsAtPoint(const CPoint2D &p, ObjectList &objects) const;

  void sendEvent(CSVGEventType type, const std::string &id="", const std::string &data="");

  void print(std::ostream &os, bool hier=false) const;

  void printFlat(std::ostream &os) const;

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
    CSVGObject* object { 0 };
  };

  typedef std::vector<CSVGBuffer *>           BufferStack;
  typedef std::vector<CSVGFont *>             FontList;
  typedef std::map<std::string, CSVGObject *> NameObjectMap;
  typedef std::vector<StyleData>              StyleDataStack;
  typedef std::vector<CSVGBlockData>          BlockDataStack;
  typedef COptValT<CSVGXmlStyleSheet>         OptXmlStyleSheet;

  CSVGRenderer*           renderer_      { 0 };
  CAutoPtr<CSVGBufferMgr> bufferMgr_;
  CSVGBuffer*             buffer_        { 0 };
  BufferStack             bufferStack_;
  CSVGBlockData           rootBlockData_;
  CSVGBlockData           blockData_;
  BlockDataStack          blockDataStack_;
  CAutoPtr<CSVGBlock>     block_;
  CAutoPtr<CXML>          xml_;
  CXMLTag*                xmlTag_        { 0 };
  CRGBA                   background_    { 1, 1, 1};
  StyleData               styleData_;
  StyleDataStack          styleDataStack_;
  FontList                fontList_;
  NameObjectMap           idObjectMap_;
  CSVGCSSData             cssData_;
  CCSS                    css_;
  CSVGJavaScript*         js_;
  CJObjectTypeP           jsDocumentType_;
  CJObjectTypeP           jsObjectType_;
  CJObjectTypeP           jsEventType_;
  CJObjectTypeP           jsTransformStackType_;
  CJObjectTypeP           jsTransformType_;
  CJObjectTypeP           jsMatrixType_;
  CJValueP                jsDocument_;
  CJValueP                jsEvent_;
  CSVGObject*             eventObject_ { 0 };
  ObjectList              drawObjects_;
  Colors                  colors_;
  OptXmlStyleSheet        xmlStyleSheet_;
  bool                    uniquify_      { false };
  bool                    autoName_      { false };
  bool                    ignoreFilter_  { false };
  bool                    debug_         { false };
  bool                    debugObjImage_ { false };
  bool                    debugImage_    { false };
  bool                    debugFilter_   { false };
  bool                    debugMask_     { false };
  bool                    debugUse_      { false };
};

#endif

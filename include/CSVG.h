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

#include <CCSS.h>
#include <CImageLib.h>
#include <CFont.h>
#include <CScreenUnits.h>
#include <CMatrixStack2D.h>
#include <CBBox2D.h>
#include <CFillType.h>
#include <CLineDash.h>
#include <CAutoPtr.h>
#include <CGenGradient.h>

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
class CSVGStyleData;
class CSVGBufferMgr;

class CStrParse;
class CConfig;
class CXML;
class CXMLTag;
class CXMLToken;

class CSVG {
 public:
  typedef std::vector<CSVGObject *> ObjectList;

 public:
  CSVG(CSVGRenderer *renderer=0);

  virtual ~CSVG();

  void          setRenderer(CSVGRenderer *renderer);
  CSVGRenderer *getRenderer() const { return renderer_; }

  const CMatrixStack2D &viewMatrix() const { return viewMatrix_; }
  void setViewMatrix(const CMatrixStack2D &v) { viewMatrix_ = v; }

  const CPoint2D &offset() const { return offset_; }
  void setOffset(const CPoint2D &o) { offset_ = o; }

  double xscale() const { return xscale_; }
  void setXScale(double s) { xscale_ = s; }

  double yscale() const { return yscale_; }
  void setYScale(double s) { yscale_ = s; }

  virtual CSVGRenderer *createRenderer();

  CSVGBlock *getBlock() const;

  CSVGBuffer *getBuffer() const;
  void setBuffer(CSVGBuffer *buffer);

  CSVGBuffer *getBuffer(const std::string &name);

  void getBufferNames(std::vector<std::string> &names) const;

  void beginDrawBuffer(CSVGBuffer *buffer);
  void beginDrawBuffer(CSVGBuffer *buffer, const CPoint2D &offset, double xs, double ys);
  void beginDrawBuffer(CSVGBuffer *buffer, const CBBox2D &bbox);
  void beginDrawBuffer(CSVGBuffer *buffer, const CBBox2D &bbox,
                       const CPoint2D &offset, double xs, double ys);
  void endDrawBuffer  (CSVGBuffer *buffer);

  void setAntiAlias(bool flag);

  CSVGObject *styleObject() const { return styleObject_; }
  void setStyleObject(CSVGObject *o) { styleObject_ = o; }

  CSVGObject *drawObject() const { return drawObject_; }
  void setDrawObject(CSVGObject *o) { drawObject_ = o; }

  void setUniquify(bool uniquify) { uniquify_ = uniquify; }
  bool getUniquify() const { return uniquify_; }

  void setAutoName(bool autoName);
  bool getAutoName() const { return autoName_; }

  void setDebug(bool b);
  bool getDebug() const { return debug_; }

  void setDebugObjImage(bool b);
  bool getDebugObjImage() const { return debugObjImage_; }

  void setDebugImage(bool b);
  bool getDebugImage() const { return debugImage_; }

  void setDebugFilter(bool b);
  bool getDebugFilter() const { return debugFilter_; }

  void setDebugMask(bool b);
  bool getDebugMask() const { return debugMask_; }

  void setDebugUse(bool b);
  bool getDebugUse() const { return debugUse_; }

  void init();

  void clear();

  bool read(const std::string &filename);
  bool read(const std::string &filename, CSVGObject *object);

  virtual CSVGObject *createObjectByName(const std::string &name);

  virtual CSVGBlock               *createBlock();
  virtual CSVGAnchor              *createAnchor();
  virtual CSVGAnimate             *createAnimate();
  virtual CSVGAnimateColor        *createAnimateColor();
  virtual CSVGAnimateMotion       *createAnimateMotion();
  virtual CSVGAnimateTransform    *createAnimateTransform();
  virtual CSVGCircle              *createCircle();
  virtual CSVGClipPath            *createClipPath();
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

  virtual CSVGPathMoveTo      *createPathMoveTo (double x, double y);
  virtual CSVGPathRMoveTo     *createPathRMoveTo(double x, double y);
  virtual CSVGPathLineTo      *createPathLineTo (double x, double y);
  virtual CSVGPathRLineTo     *createPathRLineTo(double x, double y);
  virtual CSVGPathHLineTo     *createPathHLineTo (double x);
  virtual CSVGPathRHLineTo    *createPathRHLineTo(double x);
  virtual CSVGPathVLineTo     *createPathVLineTo (double y);
  virtual CSVGPathRVLineTo    *createPathRVLineTo(double y);
  virtual CSVGPathArcTo       *createPathArcTo (double rx, double ry, double xa, double fa,
                                                double fs, double x2, double y2);
  virtual CSVGPathRArcTo      *createPathRArcTo(double rx, double ry, double xa, double fa,
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
  virtual CSVGPathClosePath   *createPathClosePath(bool relative);

  virtual void redraw() { }

  double getXMin() const;
  double getYMin() const;

  double getWidth () const;
  double getHeight() const;

  int getIWidth () const { return int(getWidth ()); }
  int getIHeight() const { return int(getHeight()); }

  const CRGBA &background() const { return background_; }
  void setBackground(const CRGBA &v) { background_ = v; }

  const CSVGStroke  &getStroke () const { return stroke_ ; }
  const CSVGFill    &getFill   () const { return fill_   ; }
  const CSVGClip    &getClip   () const { return clip_   ; }
  const CSVGFontDef &getFontDef() const { return fontDef_; }

  void setStroke (const CSVGStroke  &stroke ) { stroke_  = stroke ; }
  void setFill   (const CSVGFill    &fill   ) { fill_    = fill   ; }
  void setClip   (const CSVGClip    &clip   ) { clip_    = clip   ; }
  void setFontDef(const CSVGFontDef &fontDef) { fontDef_ = fontDef; }

  bool processOption(const std::string &opt_name, const std::string &opt_value);

  void addFont(CSVGFont *font);

  CSVGFont *getFont() const;

  CSVGGlyph *getCharGlyph(char c) const;
  CSVGGlyph *getUnicodeGlyph(const std::string &unicode) const;

  CImagePtr drawToImage(int w, int h, const CPoint2D &offset=CPoint2D(0,0),
                        double xscale=1, double yscale=1);

  bool hasAnimation() const;

  void draw();

  void draw(const CMatrixStack2D &matrix, const CPoint2D &offset=CPoint2D(0,0),
            double xscale=1, double yscale=1);

  void drawBlock(CSVGBlock *block);
  void drawBlock(CSVGBlock *block, const CMatrixStack2D &matrix,
                 const CPoint2D &offset=CPoint2D(0,0), double xscale=1, double yscale=1);

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
  void setFontDef();

  void setTransform(const CMatrixStack2D &matrix);

  void drawLine(double x1, double y1, double x2, double y2);

  void drawCircle (double x, double y, double r);
  void drawEllipse(double x, double y, double rx, double ry);

  void drawArc(double xc, double yc, double xr, double yr, double angle1, double angle2);
  void fillArc(double xc, double yc, double xr, double yr, double angle1, double angle2);

  void drawPolygon(const std::vector<CPoint2D> &points);
  void fillPolygon(const std::vector<CPoint2D> &points);

  void fillDrawText(double x, double y, const std::string &text, CFontPtr font, CHAlignType align,
                    bool isFilled, bool isStroked);

  void drawText(double x, double y, const std::string &text, CFontPtr font, CHAlignType align);
  void fillText(double x, double y, const std::string &text, CFontPtr font, CHAlignType align);

  void textSize(const std::string &text, CFontPtr font, double *w, double *a, double *d) const;

  bool pathOption(const std::string &opt_name, const std::string &opt_value,
                  const std::string &name, CSVGPathPartList &parts);
  bool pathStringToParts(const std::string &data, CSVGPathPartList &parts);

  void drawParts(const CSVGPathPartList &parts, CSVGObjectMarker *marker=0);

  double partsLength(const CSVGPathPartList &parts) const;

  bool interpParts(double s, const CSVGPathPartList &parts, double *xi, double *yi, double *a);

  bool getPartsBBox(const CSVGPathPartList &parts, CBBox2D &bbox) const;
  void printParts(std::ostream &os, const CSVGPathPartList &parts) const;

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

  double     decodeWidthString(const std::string &width_str);
  double     decodeOpacityString(const std::string &opacity_str);
  CFillType  decodeFillRuleString(const std::string &rule_str);
  bool       decodeDashString(const std::string &dash_str, CLineDash &dash);
  bool       decodeColorString(const std::string &color_str, CRGBA &rgba);
  CFontStyle decodeFontWeightString(const std::string &weight_str);
  CFontStyle decodeFontStyleString(const std::string &style_str);
  bool       decodePercentString(const std::string &str, CScreenUnits &length);

  static bool decodeUnitsString(const std::string &str, CSVGCoordUnits &units);
  static std::string encodeUnitsString(const CSVGCoordUnits &units);

  static bool decodeGradientSpread(const std::string &str, CGradientSpreadType &spread);
  static std::string encodeGradientSpread(const CGradientSpreadType &spread);

  bool decodeUrlObject(const std::string &str, CSVGObject **object);

  //bool mmToPixel(double mm, double *pixel);

  void skipCommaSpace(CStrParse &parse);

  bool getTitle(std::string &str);

  void lengthToPixel(double xi, double yi, double *xo, double *yo);
  void windowToPixel(double xi, double yi, double *xo, double *yo);

  void setObjectById(const std::string &id, CSVGObject *object);

  CSVGObject *lookupObjectById(const std::string &id) const;

  bool readCSSFile  (const std::string &filename);
  bool readCSSString(const std::string &str);

  bool processCSSIds();

  void addStyleValues(CSVGStyleData &svgStyleData, const CCSS::StyleData &cssStyleData);

  CSVGStyleData &getGlobalStyleData   ();
  CSVGStyleData &getTypeStyleData     (const std::string &objType);
  CSVGStyleData &getClassStyleData    (const std::string &objClass);
  CSVGStyleData &getTypeClassStyleData(const std::string &objType, const std::string &objClass);

  bool getStyleStrokeNoColor(const CSVGObject *obj, bool &noColor, CSVGCSSType &type);
  bool getStyleStrokeColor  (const CSVGObject *obj, CRGBA &rgba, CSVGCSSType &type);
  bool getStyleStrokeOpacity(const CSVGObject *obj, double &opacity);
  bool getStyleStrokeWidth  (const CSVGObject *obj, double &width);
  bool getStyleStrokeDash   (const CSVGObject *obj, CLineDash &dash);

  bool getStyleFillNoColor(const CSVGObject *obj, bool &noColor, CSVGCSSType &type);
  bool getStyleFillColor  (const CSVGObject *obj, CRGBA &rgba, CSVGCSSType &type);
  bool getStyleFillOpacity(const CSVGObject *obj, double &opacity);

  void getObjectsAtPoint(const CPoint2D &p, ObjectList &objects) const;

  void sendEvent(CSVGEventType type, const std::string &id="", const std::string &data="");

  void print(std::ostream &os, bool hier=false) const;

 private:
  CConfig *getConfig();

  CSVGObject *tokenToObject(CSVGObject *parent, const CXMLToken *token);

 private:
  CSVG(const CSVG &rhs);
  CSVG &operator=(const CSVG &rhs);

 private:
  typedef std::vector<CSVGFont *>              FontList;
  typedef std::map<std::string, CSVGObject *>  NameObjectMap;
  typedef std::map<std::string, CSVGStyleData> StyleDataMap;
  typedef std::map<std::string, StyleDataMap>  TypeStyleDataMap;

  CSVGRenderer*           renderer_      { 0 };
  CAutoPtr<CSVGBufferMgr> bufferMgr_;
  CSVGBuffer*             buffer_        { 0 };
  CMatrixStack2D          viewMatrix_;
  CPoint2D                offset_        { 0, 0 };
  double                  xscale_        { 1 };
  double                  yscale_        { 1 };
  CAutoPtr<CSVGBlock>     block_;
  CAutoPtr<CXML>          xml_;
  CXMLTag*                xmlTag_        { 0 };
  CRGBA                   background_    { 1, 1, 1};
  CSVGStroke              stroke_;
  CSVGFill                fill_;
  CSVGClip                clip_;
  CSVGFontDef             fontDef_;
  FontList                fontList_;
  NameObjectMap           idObjectMap_;
  StyleDataMap            globalStyleData_;
  StyleDataMap            typeStyleData_;
  StyleDataMap            classStyleData_;
  TypeStyleDataMap        typeClassStyleData_;
  CCSS                    css_;
  CSVGObject*             styleObject_   { 0 };
  CSVGObject*             drawObject_    { 0 };
  bool                    uniquify_      { false };
  bool                    autoName_      { false };
  bool                    debug_         { false };
  bool                    debugObjImage_ { false };
  bool                    debugImage_    { false };
  bool                    debugFilter_   { false };
  bool                    debugMask_     { false };
  bool                    debugUse_      { false };
};

#endif

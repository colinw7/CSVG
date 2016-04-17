#ifndef CSVG_H
#define CSVG_H

#include <CSVGStroke.h>
#include <CSVGFill.h>
#include <CSVGClip.h>
#include <CSVGFontDef.h>
#include <CSVGBlock.h>
#include <CSVGLengthValue.h>
#include <CSVGEventValue.h>
#include <CSVGTimeValue.h>
#include <CSVGPreserveAspect.h>

#include <CImageLib.h>
#include <CFont.h>
#include <CMatrix2D.h>
#include <CBBox2D.h>
#include <CFillType.h>
#include <CLineDash.h>
#include <CAutoPtr.h>
#include <CGenGradient.h>

class CSVGPathPart;
class CSVGRenderer;
class CSVGBuffer;
class CSVGObject;

class CSVGAnchor;
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
class CSVGImage;
class CSVGLine;
class CSVGLinearGradient;
class CSVGMarker;
class CSVGMask;
class CSVGMPath;
class CSVGPath;
class CSVGPattern;
class CSVGPolygon;
class CSVGPolyLine;
class CSVGRadialGradient;
class CSVGRect;
class CSVGSet;
class CSVGStop;
class CSVGSymbol;
class CSVGText;
class CSVGTextPath;
class CSVGTitle;
class CSVGTSpan;
class CSVGUse;

class CSVGFont;
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
  typedef std::vector<CSVGObject *>   ObjectList;
  typedef std::vector<CSVGPathPart *> PartList;

 public:
  CSVG(CSVGRenderer *renderer=0);

  virtual ~CSVG();

  void          setRenderer(CSVGRenderer *renderer);
  CSVGRenderer *getRenderer() const { return renderer_; }

  const CMatrix2D &viewMatrix() const { return viewMatrix_; }
  void setViewMatrix(const CMatrix2D &v) { viewMatrix_ = v; }

  CSVGRenderer *createRenderer();

  CSVGBlock *getBlock() const;

  CSVGBuffer  *getBuffer    () const;
  std::string  getBufferName() const;

  //void setBuffer(const std::string &name);
  void setBuffer(CSVGBuffer *buffer);

  CSVGBuffer *getBuffer(const std::string &name);

  void getBufferNames(std::vector<std::string> &names) const;

  void beginDrawBuffer(CSVGBuffer *buffer);
  void beginDrawBuffer(CSVGBuffer *buffer, const CBBox2D &bbox);
  void endDrawBuffer  (CSVGBuffer *buffer);

  void setAntiAlias(bool flag);

  CSVGObject *drawObject() const { return drawObject_; }
  void setDrawObject(CSVGObject *o) { drawObject_ = o; }

  void setUniquify(bool uniquify) { uniquify_ = uniquify; }
  bool getUniquify() const { return uniquify_; }

  void setAutoName(bool autoName);
  bool getAutoName() const { return autoName_; }

  void setDebug(bool b);
  bool getDebug() const { return debug_; }

  void setDebugImage(bool b);
  bool getDebugImage() const { return debugImage_; }

  void setDebugFilter(bool b);
  bool getDebugFilter() const { return debugFilter_; }

  void init();

  bool read(const std::string &filename);
  bool read(const std::string &filename, CSVGObject *object);

  virtual CSVGObject *createObjectByName(const std::string &name);

  virtual CSVGBlock               *createBlock();
  virtual CSVGAnchor              *createAnchor();
  virtual CSVGCircle              *createCircle();
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
  virtual CSVGGroup               *createGroup();
  virtual CSVGImage               *createImage();
  virtual CSVGLine                *createLine();
  virtual CSVGLinearGradient      *createLinearGradient();
  virtual CSVGMarker              *createMarker();
  virtual CSVGMask                *createMask();
  virtual CSVGMPath               *createMPath();
  virtual CSVGPath                *createPath();
  virtual CSVGPattern             *createPattern();
  virtual CSVGPolygon             *createPolygon();
  virtual CSVGPolyLine            *createPolyLine();
  virtual CSVGRadialGradient      *createRadialGradient();
  virtual CSVGRect                *createRect();
  virtual CSVGSet                 *createSet();
  virtual CSVGStop                *createStop();
  virtual CSVGSymbol              *createSymbol();
  virtual CSVGText                *createText();
  virtual CSVGTextPath            *createTextPath();
  virtual CSVGTitle               *createTitle();
  virtual CSVGTSpan               *createTSpan();
  virtual CSVGUse                 *createUse();

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

  CImagePtr drawToImage(int w, int h);

  void draw();
  void draw(const CMatrix2D &matrix, double scale=1);

  void drawBlock(CSVGBlock *block);
  void drawBlock(CSVGBlock *block, const CMatrix2D &matrix, double scale=1);

  void resetStroke();
  void updateStroke(const CSVGStroke &stroke);
  void setSelectedStroke();
  bool isStroked() const;

  void resetFill();
  void updateFill(const CSVGFill &fill);
  bool isFilled() const;

  void setStrokeBuffer();
  void setFillBuffer(CSVGBuffer *butter);

  void resetClip();
  void updateClip(const CSVGClip &clip);
  bool isClipped() const;
  void setClip();

  void resetFontDef();
  void updateFontDef(const CSVGFontDef &fontDef);
  void setFontDef();

  void setTransform(const CMatrix2D &matrix);
  void getTransform(CMatrix2D &matrix);
  void unsetTransform();

  void setBufferTransform(CSVGBuffer *butter, const CMatrix2D &matrix);
  void unsetBufferTransform(CSVGBuffer *butter);

  void drawImage(double x, double y, CImagePtr image);
  void drawImage(const CBBox2D &bbox, CImagePtr image);

  void drawBufferImage(CSVGBuffer *butter, double x, double y, CImagePtr image);
  void drawBufferImage(CSVGBuffer *butter, const CBBox2D &bbox, CImagePtr image);

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

  void textSize(const std::string &text, CFontPtr font, double *w, double *a, double *d) const;

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

  bool pathOption(const std::string &opt_name, const std::string &opt_value,
                  const std::string &name, CSVG::PartList &parts);
  bool pathStringToParts(const std::string &data, PartList &parts);

  void drawParts(const PartList &parts, CSVGObjectMarker *marker=0);
  bool interpParts(double s, const CSVG::PartList &parts, double *xi, double *yi, double *a);

  bool getPartsBBox(const PartList &parts, CBBox2D &bbox) const;
  void printParts(std::ostream &os, const PartList &parts) const;

  bool coordOption(const std::string &opt_name, const std::string &opt_value,
                   const std::string &name, CSVGLengthValue &length);
  bool coordOption(const std::string &opt_name, const std::string &opt_value,
                   const std::string &name, double *real);
  bool lengthOption(const std::string &opt_name, const std::string &opt_value,
                    const std::string &name, CSVGLengthValue &length);
  bool angleOption(const std::string &opt_name, const std::string &opt_value,
                   const std::string &name, double *real);
  bool realOption(const std::string &opt_name, const std::string &opt_value,
                  const std::string &name, double *real);
  bool integerOption(const std::string &opt_name, const std::string &opt_value,
                     const std::string &name, long *integer);
  bool stringOption(const std::string &opt_name, const std::string &opt_value,
                    const std::string &name, std::string &str);
  bool percentOption(const std::string &opt_name, const std::string &opt_value,
                     const std::string &name, CSVGLengthValue &length);
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
                       const std::string &name, CMatrix2D &matrix);

  bool stringToTime(const std::string &str, CSVGTimeValue &time) const;

  bool decodeLengthValue(const std::string &str, CSVGLengthValue &lvalue);

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
  bool       decodePercentString(const std::string &str, CSVGLengthValue &length);

  static bool decodeUnitsString(const std::string &str, CSVGCoordUnits &units);
  static std::string encodeUnitsString(const CSVGCoordUnits &units);

  static bool decodeGradientSpread(const std::string &str, CGradientSpreadType &spread);
  static std::string encodeGradientSpread(const CGradientSpreadType &spread);

  bool decodeUrlObject(const std::string &str, CSVGObject **object);

  bool mmToPixel(double mm, double *pixel);

  void skipCommaSpace(CStrParse &parse);

  bool getTitle(std::string &str);

  void lengthToPixel(double xi, double yi, double *xo, double *yo);
  void windowToPixel(double xi, double yi, double *xo, double *yo);

  void setObjectById(const std::string &id, CSVGObject *object);

  CSVGObject *lookupObjectById(const std::string &id) const;

  bool readCSSFile(const std::string &filename);

  CSVGStyleData &getStyleData(const std::string &id);

  bool getStyleStrokeColor  (const std::string &id, CRGBA &rgba);
  bool getStyleStrokeOpacity(const std::string &id, double &opacity);
  bool getStyleStrokeWidth  (const std::string &id, double &width);
  bool getStyleStrokeDash   (const std::string &id, CLineDash &dash);
  bool getStyleFillColor    (const std::string &id, CRGBA &rgba);

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

  CSVGRenderer*           renderer_      { 0 };
  CAutoPtr<CSVGBufferMgr> buffer_mgr_;
  CSVGBuffer*             buffer_        { 0 };
  CMatrix2D               viewMatrix_;
  double                  scale_         { 1 };
  CAutoPtr<CSVGBlock>     block_;
  CAutoPtr<CXML>          xml_;
  CXMLTag*                xml_tag_       { 0 };
  CRGBA                   background_    { 1, 1, 1};
  CSVGStroke              stroke_;
  CSVGFill                fill_;
  CSVGClip                clip_;
  CSVGFontDef             fontDef_;
  FontList                fontList_;
  NameObjectMap           idObjectMap_;
  StyleDataMap            styleData_;
  CSVGObject*             drawObject_    { 0 };
  bool                    uniquify_      { false };
  bool                    autoName_      { false };
  bool                    debug_         { false };
  bool                    debugImage_    { false };
  bool                    debugFilter_   { false };
};

#endif

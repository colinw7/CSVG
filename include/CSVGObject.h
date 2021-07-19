#ifndef CSVG_OBJECT_H
#define CSVG_OBJECT_H

#include <CSVGTypes.h>
#include <CSVGStroke.h>
#include <CSVGTimeValue.h>
#include <CSVGEventValue.h>
#include <CSVGXLink.h>
#include <CSVGPreserveAspect.h>
#include <CSVGAnimation.h>
#include <CSVGFontDef.h>
#include <CSVGPathPart.h>
#include <CSVGObjectMarker.h>

#include <CScreenUnits.h>
#include <CBBox2D.h>
#include <CMatrixStack2D.h>
#include <COptVal.h>
#include <CAlignType.h>
#include <CObjTypeOld.h>
#include <CAngle.h>

#include <list>

#define CSVG_OBJECT_DEF(name, id) \
const CObjType &getObjType() const override { \
  static CObjType objType(getTypeMgr(), name, uint(id)); \
  return objType; \
}

class CXMLTag;
class CSVG;
class CSVGObject;
class CSVGFilter;
class CSVGMask;
class CSVGClipPath;
class CSVGPathPart;

//---

class CSVGObject {
 public:
  using Classes     = std::vector<std::string>;
  using ObjectList  = std::list<CSVGObject *>;
  using ObjectArray = std::vector<CSVGObject *>;
  using NameValues  = std::map<std::string, std::string>;
  using Color       = CSVGInheritValT<CSVGColor>;
  using Opacity     = CSVGInheritValT<double>;
  using Width       = CSVGInheritValT<double>;
  using Overflow    = CSVGInheritValT<CSVGOverflowType>;
  using FillType    = CSVGInheritValT<CFillType>;
  using LineCap     = CSVGInheritValT<CLineCapType>;
  using LineJoin    = CSVGInheritValT<CLineJoinType>;
  using MiterLimit  = CSVGInheritValT<double>;
  using LineDash    = CSVGInheritValT<CSVGStrokeDash>;
  using FontFamily  = CSVGInheritValT<std::string>;
  using FontSize    = CSVGInheritValT<CScreenUnits>;

 protected:
  CObjTypeMgr &getTypeMgr() const {
    static CObjTypeMgr typeMgr_;

    return typeMgr_;
  }

 public:
  CSVGObject(CSVG &svg);
  CSVGObject(const CSVGObject &obj);

  virtual ~CSVGObject();

  virtual CSVGObject *dup() const = 0;

  CSVG &getSVG() const { return svg_; }

  //---

  //! get/set id
  std::string getId() const { return id_.getValue(""); }
  void setId(const std::string &id);

  //! get/set classes
  Classes getClasses() const { return classes_.getValue(Classes()); }
  void setClasses(const Classes &classes);

  //! get/set parent
  CSVGObject *getParent() const { return parent_; }
  void setParent(CSVGObject *parent);

  //---

  int getDepth() const;

  uint getInd() const { return ind_; }

  void autoName();

  //---

  //! get/set stroke
  bool hasStroke() const { return stroke_.isSet(); }
  const CSVGStroke &getStroke() const { return stroke_; }
  void setStroke(const CSVGStroke &s) { stroke_ = s; strokeChanged(); }

  //! get/set fill
  bool hasFill() const { return fill_.isSet(); }
  const CSVGFill &getFill() const { return fill_; }
  void setFill(const CSVGFill &f) { fill_ = f; fillChanged(); }

  //---

  const CSVGClip &getClip() const { return clip_; }

  //---

  //! get/set font def
  bool hasFontDef() const { return fontDef_.isSet(); }
  const CSVGFontDef &getFontDef() const { return fontDef_; }
  void setFontDef(const CSVGFontDef &f) { fontDef_ = f; }

  //---

  void updateStroke(const CSVGStroke &stroke, bool recurse=false);
  void updateFill  (const CSVGFill   &fill  , bool recurse=false);

  //---

  virtual const CObjType &getObjType() const = 0;

  CSVGObjTypeId getObjTypeId() const { return (CSVGObjTypeId) getObjType().getId(); }

  std::string getObjName() const { return getObjType().getName(); }

  virtual std::string getTagName() const { return getObjName(); }

  virtual std::string getUniqueName() const {
    if (id_.isValid() && id_.getValue().size())
      return id_.getValue();
    else
      return getObjName() + std::to_string(getInd());
  }

  bool isObjType(CSVGObjTypeId id) const { return (getObjType().getId() == uint(id)); }

  bool isObjType(const std::string &name) const { return (getObjName() == name); }

  //---

  virtual bool canFlatten() const { return true; }

  virtual bool propagateFlat() const { return true; }

  //---

  const CSVGAnimation &getAnimation() const { return animation_; }

  //---

  //! get/set filter
  bool hasFilter() const { return filter_.isValid(); }
  CSVGFilter *getFilter() const { return filter_.getValue(nullptr); }
  void setFilter(CSVGFilter *filter) { filter_ = filter; }

  //! get/set filtered
  bool getFiltered() const { return filtered_; }
  void setFiltered(bool b) { filtered_ = b; }

  //---

  //! get/set mask
  CSVGMask *getMask() const { return mask_; }
  void setMask(CSVGMask *m) { mask_ = m; }

  //! get/set masked
  bool getMasked() const { return masked_; }
  void setMasked(bool b) { masked_ = b; }

  //---

  //! get/set clip path
  CSVGClipPath *getClipPath() const { return clipPath_; }
  void setClipPath(CSVGClipPath *c) { clipPath_ = c; }

  //! get/set clipped
  bool getClipped() const { return clipped_; }
  void setClipped(bool b) { clipped_ = b; }

  //---

  //! get/set marker start
  CSVGObject *getMarkerStart() const { return marker_.getStart(); }
  void setMarkerStart(CSVGObject *m) { marker_.setStart(m); }

  //! get/set marker mid
  CSVGObject *getMarkerMid() const { return marker_.getMid(); }
  void setMarkerMid(CSVGObject *m) { marker_.setMid(m); }

  //! get/set marker end
  CSVGObject *getMarkerEnd() const { return marker_.getEnd(); }
  void setMarkerEnd(CSVGObject *m) { marker_.setEnd(m); }

  //---

  //! get/set selected
  bool getSelected() const { return selected_; }
  void setSelected(bool selected, bool children=false);

  //---

  //! get/set inside
  bool getInside() const { return inside_; }
  void setInside(bool inside) { inside_ = inside; }

  //---

  //! get/set xml tag
  CXMLTag *getXMLTag() const { return xmlTag_; }
  void setXMLTag(CXMLTag *tag) { xmlTag_ = tag; }

  //---

  //! get/set view box
  bool hasViewBox() const { return viewBox_.isValid(); }
  CBBox2D getViewBox() const { return viewBox_.getValue(CBBox2D()); }
  void setViewBox(const CBBox2D &box) { viewBox_ = box; }

  //---

  virtual void initParse() { }
  virtual void termParse() { }

  virtual bool processOption(const std::string &name, const std::string &value);

  bool processCoreOption           (const std::string &name, const std::string &value);
  bool processConditionalOption    (const std::string &name, const std::string &value);
  bool processStyleOption          (const std::string &name, const std::string &value);
  bool processPaintOption          (const std::string &name, const std::string &value);
  bool processColorOption          (const std::string &name, const std::string &value);
  bool processColorProfileOption   (const std::string &name, const std::string &value);
  bool processFilterColorOption    (const std::string &name, const std::string &value);
  bool processOpacityOption        (const std::string &name, const std::string &value);
  bool processGraphicsOption       (const std::string &name, const std::string &value);
  bool processMarkerOption         (const std::string &name, const std::string &value);
  bool processClipOption           (const std::string &name, const std::string &value);
  bool processMaskOption           (const std::string &name, const std::string &value);
  bool processFilterOption         (const std::string &name, const std::string &value);
  bool processViewportOption       (const std::string &name, const std::string &value);
  bool processGradientOption       (const std::string &name, const std::string &value);
  bool processContainerOption      (const std::string &name, const std::string &value);
  bool processGraphicalEventsOption(const std::string &name, const std::string &value);
  bool processDocumentEventsOption (const std::string &name, const std::string &value);
  bool processAnimationEventsOption(const std::string &name, const std::string &value);
  bool processCursorOption         (const std::string &name, const std::string &value);
  bool processExternalOption       (const std::string &name, const std::string &value);
  bool processFontOption           (const std::string &name, const std::string &value);
  bool processTextOption           (const std::string &name, const std::string &value);
  bool processTextContentOption    (const std::string &name, const std::string &value);
  bool processPresentationOption   (const std::string &name, const std::string &value);
  bool processCSSOption            (const std::string &name, const std::string &value);

  bool parseFont(const std::string &str);

  void notHandled(const std::string &name, const std::string &value);

  void addChildObject(CSVGObject *object);

  void deleteChildObject(CSVGObject *object);

  bool getAllChildren(ObjectArray &objects);

  bool getAllChildrenOfType(CSVGObjTypeId id, ObjectArray &objects);
  bool getChildrenOfType   (CSVGObjTypeId id, ObjectArray &objects);

  bool getAllChildrenOfId(const std::string &id, ObjectArray &objects);
  bool getChildrenOfId   (const std::string &id, ObjectArray &objects);

  bool hasChildren(bool includeAnimated=true) const;

  bool hasAnimation() const;

  uint numChildren() const { return objects_.size(); }

  const ObjectList &children() const { return objects_; }
  CSVGObject *child(int i) const;

  virtual bool isHierDrawable() const { return true; }

  virtual bool isDrawable() const { return true; }

  virtual bool isFilter() const { return false; }

  virtual bool hasFont() const { return false; }

  virtual bool isAnimated() const { return false; }

  virtual void drawInit() { }

  virtual bool draw() { assert(! isDrawable()); return false; }

  virtual void drawTerm() { }

  bool isHierVisible  (bool defValue=true) const;
  bool anyChildVisible(bool defValue=true) const;

  bool isVisible(bool defValue=true) const;
  void setVisible(bool b);

  virtual bool getBBox(CBBox2D &bbox) const;

  bool getParentViewBox(CBBox2D &bbox) const;

  bool getTransformedBBox(CBBox2D &bbox) const;

  bool getFlatTransformedBBox(CBBox2D &bbox) const;

  CMatrixStack2D getTransformTo(CSVGObject *parent) const;

  CBBox2D getDrawBBox() const;

  virtual bool inside(const CPoint2D &pos) const;

  virtual bool getSize(CSize2D &size) const;

  CSVGBuffer *toBufferImage();
  CSVGBuffer *toNamedBufferImage(const std::string &bufferName);

  virtual void moveTo  (const CPoint2D &point);
  virtual void moveBy  (const CVector2D &delta);
  virtual void resizeTo(const CSize2D &size);
  virtual void rotateBy(double da, const CPoint2D &c);
  virtual void rotateTo(double a, const CPoint2D &c);
  virtual void scaleTo (double xs, double ys);

  double getXMin() const;
  double getYMin() const;
  double getXMax() const;
  double getYMax() const;

  bool getChildrenBBox(CBBox2D &bbox) const;

  bool drawObject();

  bool drawSubObject(bool forceDraw=false);

  //------

  // text
  virtual bool hasText() const { return text_.isValid(); }
  virtual std::string getText() const;
  virtual void setText(const std::string &text);

  //------

  // get/set opacity
  void setOpacity(const Opacity opacity) { opacity_ = opacity; }
  bool getOpacityValid() const { return opacity_.isValid(); }
  Opacity getOpacity() const { return opacity_.getValue(Opacity(1.0)); }

  //------

  // stroke
  void setStrokeColor(const std::string &color) { stroke_.setColor(color); strokeChanged(); }
  void setStrokeColor(const Color &color) { stroke_.setColor(color); strokeChanged(); }
  bool getStrokeColorValid() const { return stroke_.getColorValid(); }
  Color getStrokeColor() const { return stroke_.getColor(); }

  void setStrokeOpacity(const std::string &s) { stroke_.setOpacity(s); strokeChanged(); }
  void setStrokeOpacity(const Opacity &t) { stroke_.setOpacity(t); strokeChanged(); }
  bool getStrokeOpacityValid() const { return stroke_.getOpacityValid(); }
  Opacity getStrokeOpacity() const { return stroke_.getOpacity(); }

  void setStrokeRule(const std::string &s) { stroke_.setRule(s); strokeChanged(); }
  void setStrokeRule(const FillType &rule) { stroke_.setRule(rule); strokeChanged(); }
  bool getStrokeRuleValid() const { return stroke_.getRuleValid(); }

  void setStrokeUrl(const std::string &url) { stroke_.setUrl(url); strokeChanged(); }
  bool getStrokeUrlValid() const { return stroke_.getUrlValid(); }

  void setStrokeFillObject(CSVGObject *o) { stroke_.setFillObject(o); strokeChanged(); }
  bool getStrokeFillObjectValid() const { return stroke_.getFillObjectValid(); }

  void setStrokeWidth(const std::string &s) { stroke_.setWidth(s); strokeChanged(); }
  void setStrokeWidth(const Width &width) { stroke_.setWidth(width); strokeChanged(); }
  bool getStrokeWidthValid() const { return stroke_.getWidthValid(); }

  void setStrokeDashArray(const std::string &s) { stroke_.setDashArray(s); strokeChanged(); }
  void setStrokeDashOffset(const std::string &s) { stroke_.setDashOffset(s); strokeChanged(); }

  void setStrokeDash(const LineDash &dash) { stroke_.setDash(dash); strokeChanged(); }
  LineDash getStrokeDash() const { return stroke_.getDash(); }
  bool getStrokeDashValid() const { return stroke_.getDashValid(); }

  void setStrokeLineCap(const std::string &s) { stroke_.setLineCap(s); strokeChanged(); }
  void setStrokeLineCap(const LineCap &cap) { stroke_.setLineCap(cap); strokeChanged(); }
  LineCap getStrokeLineCap() { return stroke_.getLineCap(); }
  bool getStrokeLineCapValid() const { return stroke_.getLineCapValid(); }

  void setStrokeLineJoin(const std::string &s) { stroke_.setLineJoin(s); strokeChanged(); }
  void setStrokeLineJoin(const LineJoin &join) { stroke_.setLineJoin(join); strokeChanged(); }
  LineJoin getStrokeLineJoin() { return stroke_.getLineJoin(); }
  bool getStrokeLineJoinValid() const { return stroke_.getLineJoinValid(); }

  void setStrokeMiterLimit(const std::string &s) { stroke_.setMiterLimit(s); strokeChanged(); }
  bool getStrokeMiterLimitValid() const { return stroke_.getMiterLimitValid(); }

  virtual void strokeChanged() { }

  //---

  CSVGStroke               getFlatStroke          () const;
  COptValT<Color>          getFlatStrokeColor     () const;
  COptValT<Opacity>        getFlatStrokeOpacity   () const;
  COptValT<FillType>       getFlatStrokeRule      () const;
  COptString               getFlatStrokeUrl       () const;
  COptValT<CSVGObject*>    getFlatStrokeFillObject() const;
  COptValT<Width>          getFlatStrokeWidth     () const;
  COptValT<LineDash>       getFlatStrokeLineDash  () const;
  COptValT<LineCap>        getFlatStrokeLineCap   () const;
  COptValT<LineJoin>       getFlatStrokeLineJoin  () const;
  COptValT<MiterLimit>     getFlatStrokeMiterLimit() const;

  //------

  // fill
  void setFillColor(const std::string &s) { fill_.setColor(s); fillChanged(); }
  void setFillColor(const Color &color) { fill_.setColor(color); fillChanged(); }
  bool getFillColorValid() const { return fill_.getColorValid(); }
  Color getFillColor() const { return fill_.getColor(); }

  void setFillOpacity(const std::string &s) { fill_.setOpacity(s); fillChanged(); }
  void setFillOpacity(const Opacity &r) { fill_.setOpacity(r); fillChanged(); }
  bool getFillOpacityValid() const { return fill_.getOpacityValid(); }
  Opacity getFillOpacity() const { return fill_.getOpacity(); }

  void setFillRule(const std::string &s) { fill_.setRule(s); fillChanged(); }
  FillType getFillRule() const { return fill_.getRule(); }
  bool getFillRuleValid() const { return fill_.getRuleValid(); }

  void setFillUrl(const std::string &s) { fill_.setUrl(s); fillChanged(); }
  std::string getFillUrl() const { return fill_.getUrl(); }
  bool getFillUrlValid() const { return fill_.getUrlValid(); }

  void setFillFillObject(CSVGObject *object) { fill_.setFillObject(object); fillChanged(); }
  CSVGObject *getFillFillObject() const { return fill_.getFillObject(); }
  bool getFillFillObjectValid() const { return fill_.getFillObjectValid(); }

  virtual void fillChanged() { }

  //---

  CSVGFill getFlatFill() const;

  COptValT<Color>        getFlatFillColor     () const;
  COptValT<Opacity>      getFlatFillOpacity   () const;
  COptValT<FillType>     getFlatFillRule      () const;
  COptString             getFlatFillUrl       () const;
  COptValT<CSVGObject *> getFlatFillFillObject() const;

  //------

  // font
  void setFontFamily(const FontFamily &family);
  void setFontSize  (const FontSize &size);
  void setFontWeight(const std::string &weight);
  void setFontStyle (const std::string &style );
  void setFontStyle (CFontStyle s);

  CSVGFontDef getFlatFontDef() const;

  COptValT<FontFamily> getFlatFontFamily() const;
  CFontStyles          getFlatFontStyle() const;
  COptValT<FontSize>   getFlatFontSize() const;

  //------

  //! get/set overflow
  void setOverflow(const std::string &str);

  bool hasOverflow() const { return overflow_.isValid(); }
  CSVGOverflowType getOverflow() const {
    return overflow_.getValue(Overflow(CSVGOverflowType::VISIBLE)).getValue(); }
  void setOverflow(CSVGOverflowType o) { overflow_ = Overflow(o); }

  //! get/set visible
  bool hasVisibility() const { return visibility_.isValid(); }
  std::string getVisibility() const { return visibility_.getValue(""); }
  void setVisibility(const std::string &str) { visibility_ = str; }

  //! get/set display
  std::string getDisplay() const { return display_.getValue(""); }
  void setDisplay(const std::string &str) { display_ = str; }

  //! get/set current color
  bool hasCurrentColor() const { return currentColor_.isValid(); }
  CSVGColor currentColor() const { return currentColor_.getValue(CSVGColor()); }
  void setCurrentColor(const CSVGColor &c) { currentColor_ = c; }

  //---

  //! get/set viewport fill
  bool hasViewportFillColor() const { return viewportFill_.getColorValid(); }
  Color viewportFillColor() const { return viewportFill_.getColor(); }
  void setViewportFillColor(const Color &c) { viewportFill_.setColor(c); }

  void setViewportFillOpacity(const Opacity &r) { viewportFill_.setOpacity(r); }
  Opacity getViewportFillOpacity() const { return viewportFill_.getOpacity(); }
  bool getViewportFillOpacityValid() const { return viewportFill_.getOpacityValid(); }

  //---

  CRGBA colorToRGBA(const Color &color) const;

  CRGBA getFlatCurrentColor() const;

  //---

  // clip
  void setClipRule(const std::string &rule) { clip_.setRule(rule); }

  // transform
  const CMatrixStack2D &getTransform() const { return transform_; }

  void setTransform(const CMatrixStack2D &transform) { transform_ = transform; }

  CMatrixStack2D getFlatTransform() const;

  //---

  void setNameValue(const std::string &name, const std::string &value);

  void setStyleValue(const std::string &name, const std::string &value);

  virtual COptString getNameValue       (const std::string &name) const;
  virtual COptReal   getRealNameValue   (const std::string &name) const;
  virtual COptInt    getIntegerNameValue(const std::string &name) const;

  //---

  virtual bool interpValue(const std::string &name, const std::string &from,
                           const std::string &to, double x, std::string &ystr) const;

  //---

  virtual const CSVGPathPartList &getPartList() const {
    static CSVGPathPartList parts;
    return parts;
  }

  double pathLength() const;

  bool pointAtLength(double l, CPoint2D &p, double &a, int &pi) const;

  //---

  void setStyle(const std::string &style);

  //---

  void setTextBaselineShift(const std::string &str);

  void setTextAnchor(const std::string &str);
  CHAlignType getFlatTextAnchor() const;

  //---

  CSVGObject *getFlatMarkerStart() const;
  CSVGObject *getFlatMarkerMid  () const;
  CSVGObject *getFlatMarkerEnd  () const;

  //---

  CSVGTextDecoration getTextDecoration() const {
    return textDecoration_.getValue(CSVGTextDecoration::NONE);
  }
  void setTextDecoration(const std::string &str);

  void setWritingMode(const std::string &mode) {
    setNameValue("writing-mode", mode);
  }

  std::string getWritingMode() const {
    return getNameValue("writing-mode").getValue("lr-tb");
  }

  void setHGlyphOrient(const std::string &o) {
    setNameValue("glyph-orientation-horizontal", o);
  }

  std::string getHGlyphOrient() const {
    return getNameValue("glyph-orientation-horizontal").getValue("auto");
  }

  void setVGlyphOrient(const std::string &o) {
    setNameValue("glyph-orientation-vertical", o);
  }

  std::string getVGlyphOrient() const {
    return getNameValue("glyph-orientation-vertical").getValue("auto");
  }

  bool hasLetterSpacing() const { return letterSpacing_.isValid(); }
  CScreenUnits getLetterSpacing() const { return letterSpacing_.getValue(CScreenUnits(0)); }

  bool hasWordSpacing() const { return wordSpacing_.isValid(); }
  CScreenUnits getWordSpacing() const { return wordSpacing_.getValue(CScreenUnits(0)); }

  //---

  const NameValues &nameValues() const { return nameValues_; }
  const NameValues &styleValues() const { return styleValues_; }

  //---

  const CSVGEnableBackground &enableBackground() const { return enableBackground_; }
  void setEnableBackground(const CSVGEnableBackground &v) { enableBackground_ = v; }

  const CBBox2D &enableBackgroundRect() const { return enableBackgroundRect_; }
  void setEnableBackgroundRect(const CBBox2D &v) { enableBackgroundRect_ = v; }

  bool isExternalResourcesRequired() const { return externalResourcesRequired_; }
  void setExternalResourcesRequired(bool b) { externalResourcesRequired_ = b; }

  //---

  void setShapeRendering(const std::string &rendering);

  bool decodeXLink(const std::string &str, CSVGObject **object=nullptr,
                   CSVGBuffer **buffer=nullptr);

  bool decodeStringToFile(const std::string &str, const std::string &filename);

  CSVGBuffer *getXLinkBuffer();

  static CSVGObject *lookupById(const std::string &id);

  bool getObjectsAtPoint(const CPoint2D &p, ObjectArray &objects) const;

  //---

  virtual void tick(double dt);

  virtual void setTime(double t);

  virtual void handleEvent(CSVGEventType type, const std::string &id="",
                           const std::string &data="", bool propagate=true);

  virtual void execEvent(CSVGEventType type);

  //---

  // print
  virtual void print(std::ostream &os, bool hier=false) const;

  virtual void printFlat(std::ostream &os, bool force=false, int depth=0) const;

  friend std::ostream &operator<<(std::ostream &os, const CSVGObject &object) {
    object.print(os);

    return os;
  }

  virtual void printValues(std::ostream &os, bool flat=false) const;

  void printChildren(std::ostream &os, bool hier) const;

  void printFilter     (std::ostream &os) const;
  void printStyle      (std::ostream &os, bool flat=false) const;
  void printStroke     (std::ostream &os, bool flat=false) const;
  void printFill       (std::ostream &os, bool flat=false) const;
  void printTextContent(std::ostream &os) const;
  void printFontDef    (std::ostream &os) const;
  void printTransform  (std::ostream &os, bool flat) const;

  void printTransform(std::ostream &os, const CMatrixStack2D &m) const;

  void printNameTransform(std::ostream &os, const std::string &name,
                          const COptValT<CMatrixStack2D> &m) const {
    if (m.isValid()) {
      os << " " << name << "=\""; printTransform(os, m.getValue()); os << "\"";
    }
  }

  template<typename T>
  void printNameValue(std::ostream &os, const std::string &name, const COptValT<T> &value) const {
    if (value.isValid())
      os << " " << name << "=\"" << value.getValue() << "\"";
  }

  template<typename T>
  void printNameValues(std::ostream &os, const std::string &name,
                       const COptValT< std::vector<T> > &values) const {
    if (values.isValid()) {
      os << " " << name << "=\"";

      for (uint i = 0; i < values.getValue().size(); ++i) {
         if (i > 0) os << " ";

         os << values.getValue()[i];
      }

      os << "\"";
    }
  }

  template<typename T>
  std::string valuesToString(const std::vector<T> &values) const {
    std::stringstream ss;

    for (uint i = 0; i < values.size(); ++i) {
      if (i > 0) ss << " ";

      ss << values[i];
    }

    return ss.str();
  }

  void printNameParts(std::ostream &os, const std::string &name,
                      const CSVGPathPartList &parts) const;

  void printNameLength(std::ostream &os, const std::string &name,
                       const COptValT<CScreenUnits> &length) const {
    if (length.isValid()) {
      os << " " << name << "=\"" << length.getValue() << "\"";
    }
  }

  void printNameTime(std::ostream &os, const std::string &name,
                     const COptValT<CSVGTimeValue> &time) const {
    if (time.isValid()) {
      os << " " << name << "=\"" << time.getValue() << "\"";
    }
  }

  void printNameEvent(std::ostream &os, const std::string &name,
                      const COptValT<CSVGEventValue> &event) const {
    if (event.isValid()) {
      os << " " << name << "=\"" << event.getValue() << "\"";
    }
  }

  void printNameXLink(std::ostream &os, const std::string &name,
                      const COptValT<CSVGXLink> &xlink) const {
    if (xlink.isValid()) {
      os << " " << name << "=\"" << xlink.getValue().str() << "\"";
    }
  }

  void printNamePreserveAspect(std::ostream &os, const std::string &name,
                               const COptValT<CSVGPreserveAspect> &a) const {
    if (a.isValid()) {
      os << " " << name << "=\"" << a.getValue() << "\"";
    }
  }

  void printNameCoordUnits(std::ostream &os, const std::string &name,
                           const COptValT<CSVGCoordUnits> &units) const;

  void printNamePercent(std::ostream &os, const std::string &name,
                        const COptValT<CScreenUnits> &units) const;

 private:
  CSVGObject &operator=(const CSVGObject &rhs);

 protected:
  using StringVector = std::vector<std::string>;
  using TextAnchor   = CSVGInheritValT<CHAlignType>;

  CSVG&                        svg_;
  COptString                   id_;
  COptValT<StringVector>       classes_;
  CSVGObject*                  parent_ { nullptr };
  uint                         ind_;
  COptValT<Opacity>            opacity_;
  COptString                   text_;
  CSVGStroke                   stroke_;
  CSVGFill                     fill_;
  COptValT<Overflow>           overflow_;
  COptString                   visibility_;
  COptString                   display_;
  COptValT<CSVGColor>          currentColor_;
  CSVGFill                     viewportFill_;
  CSVGClip                     clip_;
  CSVGFontDef                  fontDef_;
  COptValT<TextAnchor>         textAnchor_;
  COptValT<CSVGTextDecoration> textDecoration_;
  COptValT<CScreenUnits>       letterSpacing_;
  COptValT<CScreenUnits>       wordSpacing_;
  NameValues                   nameValues_;
  NameValues                   styleValues_;
  CMatrixStack2D               transform_;
  CSVGEnableBackground         enableBackground_ { CSVGEnableBackground::ACCUMULATE };
  CBBox2D                      enableBackgroundRect_;
  bool                         externalResourcesRequired_ { false };
  ObjectList                   objects_;
  CSVGAnimation                animation_;
  COptValT<CSVGFilter*>        filter_;
  bool                         filtered_ { true };
  CSVGMask*                    mask_     { nullptr };
  bool                         masked_   { true };
  CSVGClipPath*                clipPath_ { nullptr };
  bool                         clipped_  { true };
  CSVGObjectMarker             marker_;
  COptValT<CBBox2D>            viewBox_;
  mutable COptValT<CBBox2D>    bbox_;
  bool                         selected_ { false };
  bool                         inside_   { false };
  CXMLTag*                     xmlTag_   { nullptr };
  COptString                   outBuffer_;
};

#endif

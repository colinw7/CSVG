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

#include <list>

#define CSVG_OBJECT_DEF(name,id) \
const CObjType &getObjType() const { \
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
  typedef std::list<CSVGObject *>   ObjectList;
  typedef std::vector<CSVGObject *> ObjectArray;

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

  std::string getId() const { return id_.getValue(""); }
  void setId(const std::string &id);

  std::vector<std::string> getClasses() const {
    return classes_.getValue(std::vector<std::string>()); }
  void setClasses(const std::vector<std::string> &classes);

  CSVGObject *getParent() const { return parent_; }
  void setParent(CSVGObject *parent);

  uint getInd() const { return ind_; }

  void autoName();

  bool hasStroke() const { return stroke_.isSet(); }
  const CSVGStroke &getStroke() const { return stroke_; }
  void setStroke(const CSVGStroke &s) { stroke_ = s; }

  bool hasFill() const { return fill_.isSet(); }
  const CSVGFill &getFill() const { return fill_; }
  void setFill(const CSVGFill &f) { fill_ = f; }

  const CSVGClip &getClip() const { return clip_; }

  bool hasFontDef() const { return fontDef_.isSet(); }
  const CSVGFontDef &getFontDef() const { return fontDef_; }
  void setFontDef(const CSVGFontDef &f) { fontDef_ = f; }

  void updateStroke(const CSVGStroke &stroke, bool recurse=false);
  void updateFill  (const CSVGFill   &fill  , bool recurse=false);

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

  virtual bool canFlatten() const { return true; }

  virtual bool propagateFlat() const { return true; }

  //---

  const CSVGAnimation &getAnimation() const { return animation_; }

  //---

  bool hasFilter() const { return filter_.isValid(); }
  CSVGFilter *getFilter() const { return filter_.getValue(0); }
  void setFilter(CSVGFilter *filter) { filter_ = filter; }

  bool getFiltered() const { return filtered_; }
  void setFiltered(bool b) { filtered_ = b; }

  //---

  CSVGMask *getMask() const { return mask_; }
  void setMask(CSVGMask *m) { mask_ = m; }

  bool getMasked() const { return masked_; }
  void setMasked(bool b) { masked_ = b; }

  //---

  CSVGClipPath *getClipPath() const { return clipPath_; }
  void setClipPath(CSVGClipPath *c) { clipPath_ = c; }

  bool getClipped() const { return clipped_; }
  void setClipped(bool b) { clipped_ = b; }

  //---

  CSVGObject *getMarkerStart() const { return marker_.getStart(); }
  void setMarkerStart(CSVGObject *m) { marker_.setStart(m); }

  CSVGObject *getMarkerMid() const { return marker_.getMid(); }
  void setMarkerMid(CSVGObject *m) { marker_.setMid(m); }

  CSVGObject *getMarkerEnd() const { return marker_.getEnd(); }
  void setMarkerEnd(CSVGObject *m) { marker_.setEnd(m); }

  //---

  bool getSelected() const { return selected_; }
  void setSelected(bool selected, bool children=false);

  //---

  bool getInside() const { return inside_; }
  void setInside(bool inside) { inside_ = inside; }

  //---

  CXMLTag *getXMLTag() const { return xmlTag_; }
  void setXMLTag(CXMLTag *tag) { xmlTag_ = tag; }

  //---

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

  // opacity
  void   setOpacity     (const std::string &opacityStr);
  void   setOpacity     (double opacity) { opacity_.setValue(opacity); }
  bool   getOpacityValid() const { return opacity_.isValid(); }
  double getOpacity     () const { return opacity_.getValue(1.0); }

  //------

  // stroke
  void setStrokeColor(const std::string &color) { stroke_.setColor(color); }
  void setStrokeColor(const CSVGColor &color) { stroke_.setColor(color); }
  bool getStrokeColorValid() const { return stroke_.getColorValid(); }

  void setStrokeOpacity(const std::string &opacity) { stroke_.setOpacity(opacity); }
  void setStrokeOpacity(double t) { stroke_.setOpacity(t); }
  bool getStrokeOpacityValid() const { return stroke_.getOpacityValid(); }

  void setStrokeWidth(const std::string &width) { stroke_.setWidth(width); }
  void setStrokeWidth(double width) { stroke_.setWidth(width); }

  void setStrokeDashArray(const std::string &dashStr) { stroke_.setDashArray(dashStr); }
  void setStrokeDashOffset(const std::string &offsetStr) { stroke_.setDashOffset(offsetStr); }

  void setStrokeDash(const CSVGStrokeDash &dash) { stroke_.setDash(dash); }
  CSVGStrokeDash getStrokeDash() const { return stroke_.getDash(); }

  void setStrokeLineCap(const std::string &capStr) { stroke_.setLineCap(capStr); }
  void setStrokeLineCap(const CLineCapType &cap) { stroke_.setLineCap(cap); }
  CLineCapType getStrokeLineCap() { return stroke_.getLineCap(); }

  void setStrokeLineJoin(const std::string &joinStr) { stroke_.setLineJoin(joinStr); }
  void setStrokeLineJoin(const CLineJoinType &join) { stroke_.setLineJoin(join); }
  CLineJoinType getStrokeLineJoin() { return stroke_.getLineJoin(); }

  void setStrokeMitreLimit(const std::string &limitStr) { stroke_.setMitreLimit(limitStr); }

  //---

  CSVGStroke               getFlatStroke          () const;
  COptValT<CSVGColor>      getFlatStrokeColor     () const;
  COptReal                 getFlatStrokeOpacity   () const;
  COptValT<CFillType>      getFlatStrokeRule      () const;
  COptString               getFlatStrokeUrl       () const;
  COptValT<CSVGObject*>    getFlatStrokeFillObject() const;
  COptReal                 getFlatStrokeWidth     () const;
  COptValT<CSVGStrokeDash> getFlatStrokeLineDash  () const;
  COptValT<CLineCapType>   getFlatStrokeLineCap   () const;
  COptValT<CLineJoinType>  getFlatStrokeLineJoin  () const;
  COptReal                 getFlatStrokeMitreLimit() const;

  //------

  // fill
  void setFillColor(const std::string &color) { fill_.setColor(color); }
  void setFillColor(const CSVGColor &color) { fill_.setColor(color); }
  bool getFillColorValid() const { return fill_.getColorValid(); }

  void setFillOpacity(const std::string &opacity) { fill_.setOpacity(opacity); }
  void setFillOpacity(double r) { fill_.setOpacity(r); }
  bool getFillOpacityValid() const { return fill_.getOpacityValid(); }

  void setFillRule(const std::string &rule) { fill_.setRule(rule); }
  CFillType getFillRule() const { return fill_.getRule(); }

  void setFillObject(CSVGObject *object) { fill_.setFillObject(object); }
  CSVGObject *getFillObject() const { return fill_.getFillObject(); }

  //---

  CSVGFill getFlatFill() const;

  COptValT<CSVGColor>    getFlatFillColor     () const;
  COptReal               getFlatFillOpacity   () const;
  COptValT<CFillType>    getFlatFillRule      () const;
  COptString             getFlatFillUrl       () const;
  COptValT<CSVGObject *> getFlatFillFillObject() const;

  //------

  // font
  void setFontFamily(const std::string &family);
  void setFontSize  (double size);
  void setFontSize  (const CScreenUnits &lvalue);
  void setFontWeight(const std::string &weight);
  void setFontStyle (const std::string &style );
  void setFontStyle (CFontStyle s);

  CSVGFontDef getFlatFontDef() const;

  std::string  getFlatFontFamily() const;
  CFontStyles  getFlatFontStyle() const;
  CScreenUnits getFlatFontSize() const;

  //------

  // overflow
  void setOverflow(const std::string &str);

  bool hasOverflow() const { return overflow_.isValid(); }
  CSVGOverflowType getOverflow() const { return overflow_.getValue(CSVGOverflowType::VISIBLE); }
  void setOverflow(CSVGOverflowType o) { overflow_ = o; }

  // visible
  bool hasVisibility() const { return visibility_.isValid(); }
  std::string getVisibility() const { return visibility_.getValue(""); }
  void setVisibility(const std::string &str) { visibility_ = str; }

  // display
  std::string getDisplay() const { return display_.getValue(""); }
  void setDisplay(const std::string &str) { display_ = str; }

  // current color
  bool hasCurrentColor() const { return currentColor_.isValid(); }
  CSVGColor currentColor() const { return currentColor_.getValue(CSVGColor()); }
  void setCurrentColor(const CSVGColor &c) { currentColor_ = c; }

  CRGBA colorToRGBA(const CSVGColor &color) const;

  CRGBA getFlatCurrentColor() const;

  // clip
  void setClipRule(const std::string &rule) { clip_.setRule(rule); }

  // transform
  const CMatrixStack2D &getTransform() const { return transform_; }

  void setTransform(const CMatrixStack2D &transform) { transform_ = transform; }

  CMatrixStack2D getFlatTransform() const;

  //---

  void setNameValue(const std::string &name, const std::string &value) {
    nameValues_[name] = value;
  }

  COptString getNameValue(const std::string &name) const {
    COptString value;

    auto p = nameValues_.find(name);

    if (p != nameValues_.end())
      value = (*p).second;

    return value;
  }

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
  CScreenUnits getLetterSpacing() const { return letterSpacing_.getValue(CScreenUnits()); }

  bool hasWordSpacing() const { return wordSpacing_.isValid(); }
  CScreenUnits getWordSpacing() const { return wordSpacing_.getValue(CScreenUnits()); }

  //---

  const CSVGEnableBackground &enableBackground() const { return enableBackground_; }
  void setEnableBackground(const CSVGEnableBackground &v) { enableBackground_ = v; }

  const CBBox2D &enableBackgroundRect() const { return enableBackgroundRect_; }
  void setEnableBackgroundRect(const CBBox2D &v) { enableBackgroundRect_ = v; }

  //---

  void setShapeRendering(const std::string &rendering);

  bool decodeXLink(const std::string &str, CSVGObject **object=0, CSVGBuffer **buffer=0);

  bool decodeStringToFile(const std::string &str, const std::string &filename);

  CSVGBuffer *getXLinkBuffer();

  static CSVGObject *lookupById(const std::string &id);

  bool getObjectsAtPoint(const CPoint2D &p, ObjectArray &objects) const;

  //---

  virtual void tick(double dt);

  virtual void setTime(double t);

  virtual void handleEvent(CSVGEventType type, const std::string &id="",
                           const std::string &data="", bool propagate=true);

  void execEvent(CSVGEventType type);

  //---

  // print
  virtual void print(std::ostream &os, bool hier=false) const;

  virtual void printFlat(std::ostream &os, int depth=0) const;

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
  typedef std::map<std::string,std::string> NameValues;
  typedef std::vector<std::string>          StringVector;

  CSVG&                        svg_;
  COptString                   id_;
  COptValT<StringVector>       classes_;
  CSVGObject*                  parent_ { 0 };
  uint                         ind_;
  COptReal                     opacity_;
  COptString                   text_;
  CSVGStroke                   stroke_;
  CSVGFill                     fill_;
  COptValT<CSVGOverflowType>   overflow_;
  COptString                   visibility_;
  COptString                   display_;
  COptValT<CSVGColor>          currentColor_;
  CSVGClip                     clip_;
  CSVGFontDef                  fontDef_;
  COptValT<CHAlignType>        textAnchor_;
  COptValT<CSVGTextDecoration> textDecoration_;
  COptValT<CScreenUnits>       letterSpacing_;
  COptValT<CScreenUnits>       wordSpacing_;
  NameValues                   nameValues_;
  CMatrixStack2D               transform_;
  CSVGEnableBackground         enableBackground_ { CSVGEnableBackground::ACCUMULATE };
  CBBox2D                      enableBackgroundRect_;
  ObjectList                   objects_;
  CSVGAnimation                animation_;
  COptValT<CSVGFilter*>        filter_;
  bool                         filtered_ { true };
  CSVGMask*                    mask_     { 0 };
  bool                         masked_   { true };
  CSVGClipPath*                clipPath_ { 0 };
  bool                         clipped_  { true };
  CSVGObjectMarker             marker_;
  COptValT<CBBox2D>            viewBox_;
  mutable COptValT<CBBox2D>    bbox_;
  bool                         selected_ { false };
  bool                         inside_   { false };
  CXMLTag*                     xmlTag_   { 0 };
  COptString                   outBuffer_;
};

#endif

#ifndef CSVG_OBJECT_H
#define CSVG_OBJECT_H

#include <CSVGStroke.h>
#include <CSVGLengthValue.h>
#include <CBBox2D.h>
#include <CMatrix2D.h>
#include <COptVal.h>
#include <CImagePtr.h>
#include <CAlignType.h>
#include <list>

enum class CSVGObjTypeId {
  BLOCK=1,
  CIRCLE,
  CLIP_PATH,
  DEFS,
  DESC,
  ELLIPSE,
  FE_BLEND,
  FE_COLOR_MATRIX,
  FE_COMPONENT_TRANSFER,
  FE_COMPOSITE,
  FE_DIFFUSE_LIGHTING,
  FE_DISTANT_LIGHT,
  FE_FLOOD,
  FE_FUNC,
  FE_GAUSSIAN_BLUR,
  FE_IMAGE,
  FE_MERGE,
  FE_MERGE_NODE,
  FE_OFFSET,
  FE_POINT_LIGHT,
  FE_SPECULAR_LIGHTING,
  FE_TILE,
  FE_TURBULENCE,
  FILTER,
  FONT,
  FONT_FACE,
  GLYPH,
  GROUP,
  HKERN,
  IMAGE,
  LINEAR_GRADIENT,
  LINE,
  MARKER,
  MASK,
  MISSING_GLYPH,
  PATH,
  PATTERN,
  POLYGON,
  POLYLINE,
  RADIAL_GRADIENT,
  RECT,
  STOP,
  STYLE,
  SYMBOL,
  TEXT,
  TITLE,
  TSPAN,
  USE
};

#include <CObjTypeOld.h>

#define CSVG_OBJECT_DEF(name,id) \
const CObjType &getObjType() const { \
  static CObjType objType(getTypeMgr(), name, uint(id)); \
  return objType; \
}

class CXMLTag;
class CSVG;
class CSVGObject;
class CSVGFilter;

struct CSVGObjectMarker {
  CSVGObject *start { 0 };
  CSVGObject *mid   { 0 };
  CSVGObject *end   { 0 };

  CSVGObjectMarker() { }
};

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

  const std::string &getClass() const { return class_; }
  void setClass(const std::string &name);

  CSVGObject *getParent() const { return parent_; }
  void setParent(CSVGObject *parent);

  uint getInd() const { return ind_; }

  void autoName();

  const CSVGStroke &getStroke() const { return stroke_; }
  void setStroke(const CSVGStroke &s) { stroke_ = s; }

  const CSVGFill &getFill() const { return fill_; }
  void setFill(const CSVGFill &f) { fill_ = f; }

  const CSVGClip    &getClip   () const { return clip_   ; }
  const CSVGFontDef &getFontDef() const { return fontDef_; }

  virtual std::string getText() const { return ""; }
  virtual void setText(const std::string &);

  void updateStroke(const CSVGStroke &stroke, bool recurse=false);
  void updateFill  (const CSVGFill   &fill  , bool recurse=false);

  virtual const CObjType &getObjType() const = 0;

  CSVGObjTypeId getObjTypeId() const { return (CSVGObjTypeId) getObjType().getId(); }

  std::string getObjName() const { return getObjType().getName(); }

  bool isObjType(CSVGObjTypeId id) const { return (getObjType().getId() == uint(id)); }

  bool isObjType(const std::string &name) const { return (getObjName() == name); }

  CSVGFilter *getFilter() const { return filter_; }
  void setFilter(CSVGFilter *filter) { filter_ = filter; }

  bool getSelected() const { return selected_; }
  void setSelected(bool selected, bool children=false);

  void setXMLTag(CXMLTag *tag) { xmlTag_ = tag; }

  const CBBox2D &getViewBox() const { return viewBox_; }
  void setViewBox(const CBBox2D &box) { viewBox_ = box; }

  CXMLTag *getXMLTag() const { return xmlTag_; }

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

  void notHandled(const std::string &name, const std::string &value);

  void addChildObject(CSVGObject *object);

  void deleteChildObject(CSVGObject *object);

  void getAllChildrenOfType(CSVGObjTypeId id, ObjectArray &objects);
  void getChildrenOfType   (CSVGObjTypeId id, ObjectArray &objects);

  void getAllChildrenOfId(const std::string &id, ObjectArray &objects);
  void getChildrenOfId   (const std::string &id, ObjectArray &objects);

  bool hasChildren() const { return ! objects_.empty(); }

  const ObjectList &children() const { return objects_; }

  //ObjectList::iterator childrenBegin() { return objects_.begin(); }
  //ObjectList::iterator childrenEnd  () { return objects_.end  (); }

  //ObjectList::const_iterator childrenBegin() const { return objects_.begin(); }
  //ObjectList::const_iterator childrenEnd  () const { return objects_.end  (); }

  virtual bool isDrawable() const { return true; }

  virtual bool hasFont() const { return false; }

  virtual void drawInit() { }

  virtual void draw() = 0;

  virtual void drawTerm() { }

  virtual bool getBBox(CBBox2D &bbox) const;

  virtual bool inside(const CPoint2D &pos) const {
    CBBox2D bbox;

    if (! getBBox(bbox)) return false;

    return bbox.inside(pos);
  }

  virtual bool getSize(CSize2D &size) const;

  void moveTo(const CPoint2D &point);

  virtual CImagePtr toImage();

  virtual void moveBy  (const CVector2D &delta);
  virtual void resizeTo(const CSize2D &size);
  virtual void rotateBy(double da, const CPoint2D &c);

  double getXMin() const;
  double getYMin() const;
  double getXMax() const;
  double getYMax() const;

  bool getChildrenBBox(CBBox2D &bbox) const;

  void drawObjects();

  void drawObject();

  // opacity
  void   setOpacity     (const std::string &opacityStr);
  void   setOpacity     (double opacity) { opacity_.setValue(opacity); }
  bool   getOpacityValid() const { return opacity_.isValid(); }
  double getOpacity     () const;

  // stroke
  bool  getStrokeColorValid() const;
  CRGBA getStrokeColor     () const;

  void setStrokeColor(const std::string &color) { stroke_.setColor(color); }
  void setStrokeColor(const CRGBA &color      ) { stroke_.setColor(color); }

  bool   getStrokeOpacityValid() const;
  double getStrokeOpacity() const;

  void setStrokeOpacity(const std::string &opacity) { stroke_.setOpacity(opacity); }
  void setStrokeOpacity(double t) { stroke_.setOpacity(t); }

  double getStrokeWidth() const;

  void setStrokeWidth(const std::string &width) { stroke_.setWidth(width); }
  void setStrokeWidth(double width            ) { stroke_.setWidth(width); }

  const CLineDash &getStrokeLineDash();

  void setStrokeDash(const std::string &dashStr) { stroke_.setDash(dashStr); }
  void setStrokeDash(const CLineDash &dash) { stroke_.setDash(dash); }

  void setStrokeDashOffset(const std::string &offsetStr) { stroke_.setDashOffset(offsetStr); }

  CLineCapType getStrokeLineCap();

  void setStrokeLineCap(const std::string &capStr) { stroke_.setLineCap(capStr); }
  void setStrokeLineCap(const CLineCapType &cap) { stroke_.setLineCap(cap); }

  CLineJoinType getStrokeLineJoin();

  void setStrokeLineJoin(const std::string &joinStr) { stroke_.setLineJoin(joinStr); }
  void setStrokeLineJoin(const CLineJoinType &join) { stroke_.setLineJoin(join); }

  void setStrokeMitreLimit(const std::string &limitStr) { stroke_.setMitreLimit(limitStr); }

  // fill
  bool getFillColorValid() const;

  CRGBA getFillColor() const;
  void setFillColor(const std::string &color) { fill_.setColor(color); }
  void setFillColor(const CRGBA  &color) { fill_.setColor(color); }

  double getFillOpacity() const;
  bool getFillOpacityValid() const;
  void setFillOpacity(const std::string &opacity) { fill_.setOpacity(opacity); }
  void setFillOpacity(double r) { fill_.setOpacity(r); }

  CFillType getFillRule() const;
  void setFillRule(const std::string &rule) { fill_.setRule(rule); }

  CSVGObject *getFillObject() const;
  void setFillObject(CSVGObject *object) { fill_.setFillObject(object); }

  // font
  std::string getFontFamily() const;
  CFontStyles getFontStyle() const;
  double      getFontSize() const;

  CFontPtr getFont() const;

  void setFontFamily(const std::string &family);
  void setFontSize  (double size);
  void setFontSize  (const CSVGLengthValue &lvalue);
  void setFontWeight(const std::string &weight);
  void setFontStyle (const std::string &style );

  // clip
  void setClipRule(const std::string &rule) { clip_.setRule(rule); }

  // transform
  const CMatrix2D &getTransform() const { return transform_; }

  void setTransform(const CMatrix2D &transform) { transform_ = transform; }

  CMatrix2D getFlatTransform() const;

  //---

  void setStyle(const std::string &style);

  void setTextAnchor(const std::string &anchor);
  CHAlignType getTextAnchor() const;

  void setShapeRendering(const std::string &rendering);

  bool decodeXLink(const std::string &str, CSVGObject **object, CImagePtr &image);

  static CSVGObject *lookupById(const std::string &id);

  void getObjectsAtPoint(const CPoint2D &p, ObjectArray &objects) const;

  virtual void print(std::ostream &os, bool hier=false) const;

  friend std::ostream &operator<<(std::ostream &os, const CSVGObject &object) {
    object.print(os);

    return os;
  }

  void printFilter     (std::ostream &os) const;
  void printStyle      (std::ostream &os) const;
  void printTextContent(std::ostream &os) const;
  void printFontDef    (std::ostream &os) const;
  void printTransform  (std::ostream &os) const;

  void printTransform(std::ostream &os, const CMatrix2D &m) const;

  static void printLength(std::ostream &os, const CSVGLengthValue &l);

  template<typename T>
  void printNameValue(std::ostream &os, const std::string &name, const COptValT<T> &value) const {
    if (value.isValid())
      os << " " << name << "=\"" << value.getValue() << "\"";
  }

  void printNameLength(std::ostream &os, const std::string &name,
                       const COptValT<CSVGLengthValue> &length) const {
    if (length.isValid()) {
      os << " " << name << "=\""; printLength(os, length.getValue()); os << "\"";
    }
  }

 private:
  CSVGObject &operator=(const CSVGObject &rhs);

 protected:
  CSVG&                 svg_;
  COptValT<std::string> id_;
  std::string           class_;
  CSVGObject*           parent_ { 0 };
  uint                  ind_;
  COptValT<double>      opacity_;
  CSVGStroke            stroke_;
  CSVGFill              fill_;
  CSVGClip              clip_;
  CSVGFontDef           fontDef_;
  COptValT<CHAlignType> textAnchor_;
  COptValT<std::string> shapeRendering_;
  CMatrix2D             transform_;
  ObjectList            objects_;
  CSVGFilter*           filter_   { 0 };
  CSVGObject*           mask_     { 0 };
  CSVGObject*           clipPath_ { 0 };
  CSVGObjectMarker      marker_;
  CBBox2D               viewBox_;
  bool                  selected_ { false };
  CXMLTag*              xmlTag_   { 0 };
};

#endif

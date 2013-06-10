#ifndef CSVG_OBJECT_H
#define CSVG_OBJECT_H

#include <CMatrix2D.h>

enum CSVGObjectId {
  CSVG_OBJ_TYPE_BLOCK=1,
  CSVG_OBJ_TYPE_CIRCLE,
  CSVG_OBJ_TYPE_CLIP_PATH,
  CSVG_OBJ_TYPE_DEFS,
  CSVG_OBJ_TYPE_DESC,
  CSVG_OBJ_TYPE_ELLIPSE,
  CSVG_OBJ_TYPE_FE_BLEND,
  CSVG_OBJ_TYPE_FE_COLOR_MATRIX,
  CSVG_OBJ_TYPE_FE_COMPONENT_TRANSFER,
  CSVG_OBJ_TYPE_FE_COMPOSITE,
  CSVG_OBJ_TYPE_FE_DIFFUSE_LIGHTING,
  CSVG_OBJ_TYPE_FE_DISTANT_LIGHT,
  CSVG_OBJ_TYPE_FE_FLOOD,
  CSVG_OBJ_TYPE_FE_FUNC,
  CSVG_OBJ_TYPE_FE_GAUSSIAN_BLUR,
  CSVG_OBJ_TYPE_FE_IMAGE,
  CSVG_OBJ_TYPE_FE_MERGE,
  CSVG_OBJ_TYPE_FE_MERGE_NODE,
  CSVG_OBJ_TYPE_FE_OFFSET,
  CSVG_OBJ_TYPE_FE_POINT_LIGHT,
  CSVG_OBJ_TYPE_FE_SPECULAR_LIGHTING,
  CSVG_OBJ_TYPE_FE_TILE,
  CSVG_OBJ_TYPE_FE_TURBULENCE,
  CSVG_OBJ_TYPE_FILTER,
  CSVG_OBJ_TYPE_FONT,
  CSVG_OBJ_TYPE_FONT_FACE,
  CSVG_OBJ_TYPE_GLYPH,
  CSVG_OBJ_TYPE_GROUP,
  CSVG_OBJ_TYPE_HKERN,
  CSVG_OBJ_TYPE_IMAGE,
  CSVG_OBJ_TYPE_LINEAR_GRADIENT,
  CSVG_OBJ_TYPE_LINE,
  CSVG_OBJ_TYPE_MARKER,
  CSVG_OBJ_TYPE_MASK,
  CSVG_OBJ_TYPE_MISSING_GLYPH,
  CSVG_OBJ_TYPE_PATH,
  CSVG_OBJ_TYPE_PATTERN,
  CSVG_OBJ_TYPE_POLYGON,
  CSVG_OBJ_TYPE_POLYLINE,
  CSVG_OBJ_TYPE_RADIAL_GRADIENT,
  CSVG_OBJ_TYPE_RECT,
  CSVG_OBJ_TYPE_STOP,
  CSVG_OBJ_TYPE_STYLE,
  CSVG_OBJ_TYPE_SYMBOL,
  CSVG_OBJ_TYPE_TEXT,
  CSVG_OBJ_TYPE_TITLE,
  CSVG_OBJ_TYPE_TSPAN,
  CSVG_OBJ_TYPE_USE
};

#include <CObjTypeOld.h>

#define CSVG_OBJECT_DEF(name,id) \
const CObjType &getObjType() const { \
  static CObjType obj_type(getTypeMgr(), name, id); \
  return obj_type; \
}

class CXMLTag;
class CSVG;
class CSVGObject;
class CSVGFilter;

struct CSVGObjectMarker {
  CSVGObject *start;
  CSVGObject *mid;
  CSVGObject *end;

  CSVGObjectMarker() :
   start(NULL),
   mid  (NULL),
   end  (NULL) {
  }
};

class CSVGObject {
 public:
  typedef std::list<CSVGObject *> ObjectList;

 protected:
  CSVG                 &svg_;
  std::string           id_;
  std::string           class_;
  CSVGObject           *parent_;
  COptValT<double>      opacity_;
  CSVGStroke            stroke_;
  CSVGFill              fill_;
  CSVGClip              clip_;
  CSVGFontDef           font_def_;
  COptValT<CHAlignType> text_anchor_;
  std::string           shape_rendering_;
  CMatrix2D             transform_;
  ObjectList            objects_;
  CSVGFilter           *filter_;
  CSVGObject           *mask_;
  CSVGObject           *clip_path_;
  CSVGObjectMarker      marker_;
  CBBox2D               view_box_;
  bool                  selected_;
  CXMLTag              *xml_tag_;

 protected:
  CObjTypeMgr &getTypeMgr() const {
    static CObjTypeMgr type_mgr_;

    return type_mgr_;
  }

 public:
  CSVGObject(CSVG &svg);
  CSVGObject(const CSVGObject &obj);

  virtual ~CSVGObject();

  virtual CSVGObject *dup() const = 0;

  CSVG &getSVG() const { return svg_; }

  const std::string &getId() const { return id_; }

  void setId(const std::string &id);

  const std::string &getClass() const { return class_; }

  void setClass(const std::string &name);

  CSVGObject *getParent() const { return parent_; }

  void setParent(CSVGObject *parent);

  void autoName();

  const CSVGStroke  &getStroke () const { return stroke_  ; }
  const CSVGFill    &getFill   () const { return fill_    ; }
  const CSVGClip    &getClip   () const { return clip_    ; }
  const CSVGFontDef &getFontDef() const { return font_def_; }

  virtual void setText(const std::string &);

  void updateStroke(const CSVGStroke &stroke, bool recurse=false);
  void updateFill  (const CSVGFill   &fill  , bool recurse=false);

  virtual const CObjType &getObjType() const = 0;

  std::string getObjName() const { return getObjType().getName(); }

  bool isObjType(uint id) const { return (getObjType().getId() == id); }

  bool isObjType(const std::string &name) const { return (getObjName() == name); }

  CSVGFilter *getFilter() const { return filter_; }

  void setFilter(CSVGFilter *filter) { filter_ = filter; }

  bool getSelected() const { return selected_; }

  void setSelected(bool selected, bool children=false);

  void setXMLTag(CXMLTag *tag) { xml_tag_ = tag; }

  const CBBox2D &getViewBox() const { return view_box_; }

  void setViewBox(const CBBox2D &box) { view_box_ = box; }

  CXMLTag *getXMLTag() const { return xml_tag_; }

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

  void getAllChildrenOfType(uint id, std::vector<CSVGObject *> &objects);
  void getChildrenOfType   (uint id, std::vector<CSVGObject *> &objects);

  void getAllChildrenOfId(const std::string &id, std::vector<CSVGObject *> &objects);
  void getChildrenOfId   (const std::string &id, std::vector<CSVGObject *> &objects);

  bool hasChildren() const { return ! objects_.empty(); }

  ObjectList::iterator childrenBegin() { return objects_.begin(); }
  ObjectList::iterator childrenEnd  () { return objects_.end  (); }

  ObjectList::const_iterator childrenBegin() const { return objects_.begin(); }
  ObjectList::const_iterator childrenEnd  () const { return objects_.end  (); }

  virtual bool isDrawable() { return true; }

  virtual void draw() = 0;

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
  void   setOpacity     (const std::string &opacity_str);
  void   setOpacity     (double opacity) { opacity_.setValue(opacity); }
  bool   getOpacityValid() const { return opacity_.getValid(); }
  double getOpacity     () const;

  // stroke
  bool  getStrokeColorValid() const;
  CRGBA getStrokeColor     () const;

  void setStrokeColor(const std::string &color) { stroke_.setColor(color); }
  void setStrokeColor(const CRGBA &color      ) { stroke_.setColor(color); }

  bool   getStrokeOpacityValid() const;
  double getStrokeOpacity() const;

  void setStrokeOpacity(const std::string &opacity) { stroke_.setOpacity(opacity); }

  double getStrokeWidth() const;

  void setStrokeWidth(const std::string &width) { stroke_.setWidth(width); }
  void setStrokeWidth(double width            ) { stroke_.setWidth(width); }

  const CLineDash &getStrokeLineDash();

  void setStrokeDash(const std::string &dash_str) { stroke_.setDash(dash_str); }

  void setStrokeDashOffset(const std::string &offset_str) { stroke_.setDashOffset(offset_str); }

  CLineCapType getStrokeLineCap();

  void setStrokeLineCap(const std::string &cap_str) { stroke_.setLineCap(cap_str); }

  CLineJoinType getStrokeLineJoin();

  void setStrokeLineJoin(const std::string &join_str) { stroke_.setLineJoin(join_str); }

  void setStrokeMitreLimit(const std::string &limit_str) { stroke_.setMitreLimit(limit_str); }

  // fill
  bool getFillColorValid() const;

  CRGBA getFillColor() const;

  CFillType getFillRule() const;

  CSVGObject *getFillObject() const;

  void setFillColor(const std::string &color) { fill_.setColor(color); }
  void setFillColor(const CRGBA  &color) { fill_.setColor(color); }

  void setFillOpacity(const std::string &opacity) { fill_.setOpacity(opacity); }

  bool getFillOpacityValid() const;

  double getFillOpacity() const;

  void setFillRule(const std::string &rule) { fill_.setRule   (rule   ); }

  void setFillObject(CSVGObject *object) { fill_.setFillObject(object); }

  // font
  std::string getFontFamily() const;
  CFontStyles getFontStyle() const;
  double      getFontSize() const;

  CFontPtr getFont() const;

  void setFontFamily(const std::string &family);
  void setFontSize  (double size);
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

  virtual void print(std::ostream &os) const;

  friend std::ostream &operator<<(std::ostream &os, const CSVGObject &object) {
    object.print(os);

    return os;
  }

 private:
  CSVGObject &operator=(const CSVGObject &rhs);
};

//------------

class CSVGTempStroke {
 private:
  CSVGObject &object_;
  CSVGStroke  stroke_;
  CSVGFill    fill_;
  CSVGClip    clip_;

 public:
  CSVGTempStroke(CSVGObject &object);
 ~CSVGTempStroke();
};

class CSVGTempFont {
 private:
  CSVGObject  &object_;
  CSVGFontDef  font_def_;

 public:
  CSVGTempFont(CSVGObject &object);
 ~CSVGTempFont();
};

#endif

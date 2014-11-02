#include <CSVGI.h>
#include <CFontMgr.h>
#include <CEncode64.h>
#include <CRegExp.h>

CSVGObject::
CSVGObject(CSVG &svg) :
 svg_            (svg),
 id_             (""),
 class_          (),
 parent_         (NULL),
 opacity_        (),
 stroke_         (svg),
 fill_           (svg),
 clip_           (svg),
 font_def_       (svg),
 text_anchor_    (),
 shape_rendering_(""),
 transform_      (),
 objects_        (),
 filter_         (NULL),
 mask_           (NULL),
 clip_path_      (NULL),
 marker_         (),
 view_box_       (),
 selected_       (false),
 xml_tag_        (NULL)
{
  transform_.setIdentity();
}

CSVGObject::
CSVGObject(const CSVGObject &obj) :
 svg_            (obj.svg_),
 id_             (obj.id_),
 class_          (obj.class_),
 parent_         (obj.parent_),
 opacity_        (obj.opacity_),
 stroke_         (obj.stroke_),
 fill_           (obj.fill_),
 clip_           (obj.clip_),
 font_def_       (obj.font_def_),
 text_anchor_    (obj.text_anchor_),
 shape_rendering_(obj.shape_rendering_),
 transform_      (obj.transform_),
 objects_        (),
 filter_         (obj.filter_),
 mask_           (obj.mask_),
 clip_path_      (obj.clip_path_),
 marker_         (obj.marker_),
 view_box_       (obj.view_box_),
 selected_       (obj.selected_),
 xml_tag_        (obj.xml_tag_)
{
  ObjectList::const_iterator po1, po2;

  for (po1 = obj.objects_.begin(), po2 = obj.objects_.end(); po1 != po2; ++po1) {
    CSVGObject *child = (*po1)->dup();

    addChildObject(child);
  }
}

CSVGObject::
~CSVGObject()
{
  for_each(objects_.begin(), objects_.end(), CDeletePointer());
}

void
CSVGObject::
setParent(CSVGObject *parent)
{
  parent_ = parent;
}

void
CSVGObject::
autoName()
{
  typedef std::map<string,int> IdMap;

  static IdMap idMap;

  string typeName = getObjType().getName();

  IdMap::iterator p = idMap.find(typeName);

  if (p == idMap.end())
    p = idMap.insert(p, IdMap::value_type(typeName, 1));

  setId(CStrUtil::strprintf("%s%d", typeName.c_str(), (*p).second));

  ++(*p).second;
}

void
CSVGObject::
setText(const string &str)
{
  string str1 = CStrUtil::stripSpaces(str);

  if (str1.empty()) return;

  std::cerr << "Option does not support text '" + str + "'" << std::endl;
}

void
CSVGObject::
setOpacity(const string &opacity_def)
{
  double opacity = svg_.decodeOpacityString(opacity_def);

  setOpacity(opacity);
}

void
CSVGObject::
updateStroke(const CSVGStroke &stroke, bool recurse)
{
  stroke_.update(stroke);

  if (recurse) {
    ObjectList::iterator p1, p2;

    for (p1 = objects_.begin(), p2 = objects_.end(); p1 != p2; ++p1)
      (*p1)->updateStroke(stroke, recurse);
  }
}

void
CSVGObject::
updateFill(const CSVGFill &fill, bool recurse)
{
  fill_.update(fill);

  if (recurse) {
    ObjectList::iterator p1, p2;

    for (p1 = objects_.begin(), p2 = objects_.end(); p1 != p2; ++p1)
      (*p1)->updateFill(fill, recurse);
  }
}

double
CSVGObject::
getOpacity() const
{
  // TODO: inherit ?
  if (opacity_.getValid())
    return opacity_.getValue();
  else
    return 1.0;
}

bool
CSVGObject::
getStrokeColorValid() const
{
  return stroke_.getColorValid();
}

CRGBA
CSVGObject::
getStrokeColor() const
{
  COptValT<CRGBA> color;

  if (stroke_.getColorValid())
    color.setValue(stroke_.getColor());
  else {
    CSVGObject *parent = getParent();

    while (parent) {
      if (parent->stroke_.getColorValid()) {
        color.setValue(parent->stroke_.getColor());
        break;
      }

      parent = parent->getParent();
    }
  }

  if (! color.getValid()) {
    if (stroke_.getDefColorValid())
      color.setValue(stroke_.getDefColor());
  }

  if (! color.getValid()) {
    CRGBA rgba(0,0,0);

    if (svg_.getStyleStrokeColor(getObjName(), rgba))
      color.setValue(rgba);
    else
      color.setValue(CRGBA(0,0,0));
  }

  return color.getValue();
}

bool
CSVGObject::
getStrokeOpacityValid() const
{
  return stroke_.getOpacityValid();
}

double
CSVGObject::
getStrokeOpacity() const
{
  COptValT<double> opacity;

  if (stroke_.getOpacityValid())
    opacity.setValue(stroke_.getOpacity());
  else {
    CSVGObject *parent = getParent();

    while (parent) {
      if (parent->stroke_.getOpacityValid()) {
        opacity.setValue(parent->stroke_.getOpacity());
        break;
      }

      parent = parent->getParent();
    }
  }

  if (! opacity.getValid()) {
    double opacity1 = 0.0;

    if (svg_.getStyleStrokeOpacity(getObjName(), opacity1))
      opacity.setValue(opacity1);
    else
      opacity.setValue(0.0);
  }

  return opacity.getValue();
}

double
CSVGObject::
getStrokeWidth() const
{
  if (stroke_.getWidthValid())
    return stroke_.getWidth();

  CSVGObject *parent = getParent();

  while (parent) {
    if (parent->stroke_.getWidthValid())
      return parent->getStrokeWidth();

    parent = parent->getParent();
  }

  double width = 1.0;

  if (svg_.getStyleStrokeWidth(getObjName(), width))
    return width;

  return 1.0;
}

const CLineDash &
CSVGObject::
getStrokeLineDash()
{
  if (stroke_.getDashValid())
    return stroke_.getDash();

  CSVGObject *parent = getParent();

  while (parent) {
    if (parent->stroke_.getDashValid())
      return parent->getStrokeLineDash();

    parent = parent->getParent();
  }

  static CLineDash dash;

  if (svg_.getStyleStrokeDash(getObjName(), dash))
    return dash;

  return dash;
}

CLineCapType
CSVGObject::
getStrokeLineCap()
{
  return stroke_.getLineCap();
}

CLineJoinType
CSVGObject::
getStrokeLineJoin()
{
  return stroke_.getLineJoin();
}

bool
CSVGObject::
getFillColorValid() const
{
  return fill_.getColorValid();
}

CRGBA
CSVGObject::
getFillColor() const
{
  COptValT<CRGBA> color;

  if (fill_.getColorValid())
    color.setValue(fill_.getColor());
  else {
    CSVGObject *parent = getParent();

    while (parent) {
      if (parent->fill_.getColorValid()) {
        color.setValue(parent->fill_.getColor());
        break;
      }

      parent = parent->getParent();
    }
  }

  if (! color.getValid()) {
    if (fill_.getDefColorValid())
      color.setValue(fill_.getDefColor());
  }

  if (! color.getValid()) {
    CRGBA rgba(0,0,0);

    if (svg_.getStyleFillColor(getObjName(), rgba))
      color.setValue(rgba);
    else
      color.setValue(CRGBA(0,0,0));
  }

  return color.getValue();
}

bool
CSVGObject::
getFillOpacityValid() const
{
  return fill_.getOpacityValid();
}

double
CSVGObject::
getFillOpacity() const
{
  COptValT<double> opacity;

  if (fill_.getOpacityValid())
    opacity.setValue(fill_.getOpacity());
  else {
    CSVGObject *parent = getParent();

    while (parent) {
      if (parent->fill_.getOpacityValid()) {
        opacity.setValue(parent->fill_.getOpacity());
        break;
      }

      parent = parent->getParent();
    }
  }

  if (! opacity.getValid())
    opacity.setValue(1.0);

  return opacity.getValue();
}

CFillType
CSVGObject::
getFillRule() const
{
  return fill_.getRule();
}

CSVGObject *
CSVGObject::
getFillObject() const
{
  return fill_.getFillObject();
}

string
CSVGObject::
getFontFamily() const
{
  if (font_def_.hasFamily())
    return font_def_.getFamily();

  CSVGObject *parent = getParent();

  while (parent) {
    if (parent->font_def_.hasFamily())
      return parent->getFontFamily();

    parent = parent->getParent();
  }

  return "serif";
}

CFontStyles
CSVGObject::
getFontStyle() const
{
  if (font_def_.hasStyle())
    return font_def_.getStyle();

  CSVGObject *parent = getParent();

  while (parent) {
    if (parent->font_def_.hasStyle())
      return parent->getFontStyle();

    parent = parent->getParent();
  }

  return CFONT_STYLE_NORMAL;
}

double
CSVGObject::
getFontSize() const
{
  if (font_def_.hasSize())
    return font_def_.getSize();

  CSVGObject *parent = getParent();

  while (parent) {
    if (parent->font_def_.hasSize())
      return parent->getFontSize();

    parent = parent->getParent();
  }

  return 10;
}

CFontPtr
CSVGObject::
getFont() const
{
  return CFontMgrInst->lookupFont(getFontFamily(), getFontStyle().value(), getFontSize());
}

void
CSVGObject::
setFontFamily(const string &family)
{
  font_def_.setFamily(family);
}

void
CSVGObject::
setFontSize(double size)
{
  font_def_.setSize(size);
}

void
CSVGObject::
setFontWeight(const string &weight)
{
  font_def_.setWeight(weight);
}

void
CSVGObject::
setFontStyle(const string &style)
{
  font_def_.setStyle(style);
}

CMatrix2D
CSVGObject::
getFlatTransform() const
{
  if (getParent())
    return getParent()->getFlatTransform()*getTransform();
  else
    return getTransform();
}

void
CSVGObject::
setStyle(const string &style)
{
  vector<string> words;

  CStrUtil::addFields(style, words, ";");

  uint num_words = words.size();

  for (uint i = 0; i < num_words; ++i) {
    vector<string> words1;

    words[i] = CStrUtil::stripSpaces(words[i]);

    if (words[i] == "")
      continue;

    CStrUtil::addFields(words[i], words1, ":");

    uint num_words1 = words1.size();

    if (num_words1 == 2) {
      words1[0] = CStrUtil::stripSpaces(words1[0]);
      words1[1] = CStrUtil::stripSpaces(words1[1]);

      if (! processPaintOption      (words1[0], words1[1]) &&
          ! processColorOption      (words1[0], words1[1]) &&
          ! processFontOption       (words1[0], words1[1]) &&
          ! processTextContentOption(words1[0], words1[1]) &&
          ! processOpacityOption    (words1[0], words1[1]) &&
          ! processMarkerOption     (words1[0], words1[1]) &&
          ! processGradientOption   (words1[0], words1[1]) &&
          ! processGraphicsOption   (words1[0], words1[1]) &&
          ! processFilterOption     (words1[0], words1[1]))
        CSVGLog() << "Invalid style option " << words1[0] << ":" << words1[1] <<
                     " for " << getObjName();
    }
    else
      CSVGLog() << "Invalid style option format " << words1[0] << ":" << words1[1] <<
                   " for " << getObjName();
  }
}

void
CSVGObject::
setSelected(bool selected, bool children)
{
  selected_ = selected;

  if (children) {
    ObjectList::iterator p1 = objects_.begin();
    ObjectList::iterator p2 = objects_.end  ();

    for ( ; p1 != p2; ++p1)
      (*p1)->setSelected(selected, children);
  }
}

bool
CSVGObject::
processOption(const string &opt_name, const string &opt_value)
{
  if (processCoreOption           (opt_name, opt_value)) return true;
  if (processConditionalOption    (opt_name, opt_value)) return true;
  if (processStyleOption          (opt_name, opt_value)) return true;
  if (processPaintOption          (opt_name, opt_value)) return true;
  if (processColorOption          (opt_name, opt_value)) return true;
  if (processOpacityOption        (opt_name, opt_value)) return true;
  if (processGraphicsOption       (opt_name, opt_value)) return true;
  if (processMarkerOption         (opt_name, opt_value)) return true;
  if (processClipOption           (opt_name, opt_value)) return true;
  if (processMaskOption           (opt_name, opt_value)) return true;
  if (processFilterOption         (opt_name, opt_value)) return true;
  if (processGradientOption       (opt_name, opt_value)) return true;
  if (processGraphicalEventsOption(opt_name, opt_value)) return true;
  if (processCursorOption         (opt_name, opt_value)) return true;
  if (processExternalOption       (opt_name, opt_value)) return true;
  if (processFontOption           (opt_name, opt_value)) return true;
  if (processTextContentOption    (opt_name, opt_value)) return true;

  string  str;
  CBBox2D bbox;

  // Other properties
  if      (svg_.stringOption(opt_name, opt_value, "transform", str)) {
    CMatrix2D transform;

    if (! svg_.decodeTransform(str, transform))
      return false;

    setTransform(transform);
  }

  else if (svg_.bboxOption(opt_name, opt_value, "viewBox", &bbox))
    view_box_ = bbox;

  // xmlns
  else if (CRegExpUtil::parse(opt_name, "xmlns:.*")) {
    //notHandled(opt_name, opt_value);
  }
  else if (CRegExpUtil::parse(opt_name, "sodipodi:.*")) {
    //notHandled(opt_name, opt_value);
  }
  else if (CRegExpUtil::parse(opt_name, "inkscape:.*")) {
    //notHandled(opt_name, opt_value);
  }
  else {
    CSVGLog() << "Unhandled option " << opt_name << " for " << getObjName();
    return false;
  }

  return true;
}

bool
CSVGObject::
processCoreOption(const string &opt_name, const string &opt_value)
{
  string str;

  // Core Attributes
  if      (svg_.stringOption(opt_name, opt_value, "id", str))
    setId(str);
  else if (svg_.stringOption(opt_name, opt_value, "class", str))
    setClass(str);
  else if (svg_.stringOption(opt_name, opt_value, "xml:base", str))
    notHandled(opt_name, opt_value);
  else if (svg_.stringOption(opt_name, opt_value, "xml:lang", str))
    notHandled(opt_name, opt_value);
  else if (svg_.stringOption(opt_name, opt_value, "xml:space", str))
    /* default or preserve */;
  else
    return false;

  return true;
}

bool
CSVGObject::
processConditionalOption(const string &opt_name, const string &opt_value)
{
  string str;

  // Conditional Attributes
  if      (svg_.stringOption(opt_name, opt_value, "requiredFeatures", str))
    setId(str);
  else if (svg_.stringOption(opt_name, opt_value, "requiredExtensions", str))
    notHandled(opt_name, opt_value);
  else if (svg_.stringOption(opt_name, opt_value, "systemLanguage", str))
    notHandled(opt_name, opt_value);
  else
    return false;

  return true;
}

bool
CSVGObject::
processStyleOption(const string &opt_name, const string &opt_value)
{
  string str;

  // Style Attributes
  if      (svg_.stringOption(opt_name, opt_value, "style", str))
    setStyle(str);
  else if (svg_.stringOption(opt_name, opt_value, "class", str))
    notHandled(opt_name, opt_value);
  else
    return false;

  return true;
}

bool
CSVGObject::
processPaintOption(const string &opt_name, const string &opt_value)
{
  string str;

  // Paint Attributes
  if      (svg_.stringOption(opt_name, opt_value, "fill", str))
    setFillColor(str);
  else if (svg_.stringOption(opt_name, opt_value, "fill-rule", str))
    setFillRule(str);
  else if (svg_.stringOption(opt_name, opt_value, "stroke", str))
    setStrokeColor(str);
  else if (svg_.stringOption(opt_name, opt_value, "stroke-dasharray", str))
    setStrokeDash(str);
  else if (svg_.stringOption(opt_name, opt_value, "stroke-dashoffset", str))
    setStrokeDashOffset(str);
  else if (svg_.stringOption(opt_name, opt_value, "stroke-linecap", str))
    setStrokeLineCap(str);
  else if (svg_.stringOption(opt_name, opt_value, "stroke-linejoin", str))
    setStrokeLineJoin(str);
  else if (svg_.stringOption(opt_name, opt_value, "stroke-miterlimit", str))
    setStrokeMitreLimit(str);
  else if (svg_.stringOption(opt_name, opt_value, "stroke-width", str))
    setStrokeWidth(str);
  else if (svg_.stringOption(opt_name, opt_value, "overflow", str))
    ;
  else if (svg_.stringOption(opt_name, opt_value, "visibility", str))
    ;
  else if (svg_.stringOption(opt_name, opt_value, "display", str))
    ;
  else
    return false;

  return true;
}

bool
CSVGObject::
processColorOption(const string &opt_name, const string &opt_value)
{
  string str;

  // Color Attributes
  if      (svg_.stringOption(opt_name, opt_value, "color", str))
    notHandled(opt_name, opt_value);
  else if (svg_.stringOption(opt_name, opt_value, "color-interpolation", str))
    notHandled(opt_name, opt_value);
  else if (svg_.stringOption(opt_name, opt_value, "color-rendering", str))
    notHandled(opt_name, opt_value);
  else
    return false;

  return true;
}

bool
CSVGObject::
processColorProfileOption(const string &opt_name, const string &opt_value)
{
  string str;

  // Color Profile
  if (svg_.stringOption(opt_name, opt_value, "color-profile", str))
    notHandled(opt_name, opt_value);
  else
    return false;

  return true;
}

bool
CSVGObject::
processFilterColorOption(const string &opt_name, const string &opt_value)
{
  string str;

  // Filter Color
  if (svg_.stringOption(opt_name, opt_value, "color-interpolation-filters", str))
    notHandled(opt_name, opt_value);
  else
    return false;

  return true;
}

bool
CSVGObject::
processOpacityOption(const string &opt_name, const string &opt_value)
{
  string str;

  // Opacity Attributes
  if      (svg_.stringOption(opt_name, opt_value, "opacity", str))
    setOpacity(str);
  else if (svg_.stringOption(opt_name, opt_value, "fill-opacity", str))
    setFillOpacity(str);
  else if (svg_.stringOption(opt_name, opt_value, "stroke-opacity", str))
    setStrokeOpacity(str);
  else
    return false;

  return true;
}

bool
CSVGObject::
processGraphicsOption(const string &opt_name, const string &opt_value)
{
  string str;

  // Graphics Attributes
  if      (svg_.stringOption(opt_name, opt_value, "display", str))
    // inline | block | list-item | run-in | compact | marker |
    // table | inline-table | table-row-group | table-header-group |
    // table-footer-group | table-row | table-column-group |
    // table-column | table-cell | table-caption | none | inherit
    notHandled(opt_name, opt_value);
  else if (svg_.stringOption(opt_name, opt_value, "image-rendering", str))
    // auto | optimizeSpeed | optimizeQuality | inherit
    notHandled(opt_name, opt_value);
  else if (svg_.stringOption(opt_name, opt_value, "pointer-events", str))
    // visiblePainted | visibleFill | visibleStroke | visible |
    // painted | fill | stroke | all | none | inherit
    notHandled(opt_name, opt_value);
  else if (svg_.stringOption(opt_name, opt_value, "shape-rendering", str))
    // auto | optimizeSpeed | crispEdges | geometricPrecision |
    // inherit
    setShapeRendering(str);
  else if (svg_.stringOption(opt_name, opt_value, "text-renderer", str))
    // auto | optimizeSpeed | optimizeLegibility |
    // geometricPrecision | inherit
    notHandled(opt_name, opt_value);
  else if (svg_.stringOption(opt_name, opt_value, "visibility", str))
    // visible | hidden | inherit
    notHandled(opt_name, opt_value);
  else
    return false;

  return true;
}

bool
CSVGObject::
processMarkerOption(const string &opt_name, const string &opt_value)
{
  string str;

  // Marker
  if      (svg_.stringOption(opt_name, opt_value, "marker", str)) {
    if (str != "none") {
      if (! svg_.decodeUrlObject(opt_value, &marker_.start))
        CSVGLog() << "Unknown marker start " << opt_value << " for " << getObjName();

      marker_.mid = marker_.start;
      marker_.end = marker_.start;
    }
  }
  else if (svg_.stringOption(opt_name, opt_value, "marker-start", str)) {
    if (! svg_.decodeUrlObject(opt_value, &marker_.start))
      CSVGLog() << "Unknown marker start " << opt_value << " for " << getObjName();
  }
  else if (svg_.stringOption(opt_name, opt_value, "marker-mid", str)) {
    if (! svg_.decodeUrlObject(opt_value, &marker_.mid))
      CSVGLog() << "Unknown marker mid " << opt_value << " for " << getObjName();
  }
  else if (svg_.stringOption(opt_name, opt_value, "marker-end", str)) {
    if (! svg_.decodeUrlObject(opt_value, &marker_.end))
      CSVGLog() << "Unknown marker end " << opt_value << " for " << getObjName();
  }
  else
    return false;

  return true;
}

bool
CSVGObject::
processClipOption(const string &opt_name, const string &opt_value)
{
  string str;

  if      (svg_.stringOption(opt_name, opt_value, "clip-path", str)) {
    if (! svg_.decodeUrlObject(opt_value, &clip_path_))
      CSVGLog() << "Unknown clip path " << opt_value << " for " << getObjName();
  }
  else if (svg_.stringOption(opt_name, opt_value, "clip-rule", str)) {
    setClipRule(str);
  }
  else
    return false;

  return true;
}

bool
CSVGObject::
processPresentationOption(const string &opt_name, const string &opt_value)
{
  string str;

  if (processContainerOption   (opt_name, opt_value)) return true;
  if (processViewportOption    (opt_name, opt_value)) return true;
  if (processTextOption        (opt_name, opt_value)) return true;
  if (processTextContentOption (opt_name, opt_value)) return true;
  if (processFontOption        (opt_name, opt_value)) return true;
  if (processPaintOption       (opt_name, opt_value)) return true;
  if (processColorOption       (opt_name, opt_value)) return true;
  if (processOpacityOption     (opt_name, opt_value)) return true;
  if (processGraphicsOption    (opt_name, opt_value)) return true;
  if (processMarkerOption      (opt_name, opt_value)) return true;
  if (processColorProfileOption(opt_name, opt_value)) return true;
  if (processGradientOption    (opt_name, opt_value)) return true;
  if (processClipOption        (opt_name, opt_value)) return true;
  if (processMaskOption        (opt_name, opt_value)) return true;
  if (processFilterOption      (opt_name, opt_value)) return true;
  if (processFilterColorOption (opt_name, opt_value)) return true;
  if (processCursorOption      (opt_name, opt_value)) return true;

  // Presentation
  if      (svg_.stringOption(opt_name, opt_value, "flood-color", str))
    notHandled(opt_name, opt_value);
  else if (svg_.stringOption(opt_name, opt_value, "flood-opacity", str))
    notHandled(opt_name, opt_value);
  else if (svg_.stringOption(opt_name, opt_value, "lighting-color", str))
    notHandled(opt_name, opt_value);
  else
    return false;

  return true;
}

bool
CSVGObject::
processMaskOption(const string &opt_name, const string &opt_value)
{
  string str;

  if (svg_.stringOption(opt_name, opt_value, "mask", str)) {
    if (! svg_.decodeUrlObject(opt_value, &mask_))
      CSVGLog() << "Unknown mask " << opt_value << " for " << getObjName();
  }
  else
    return false;

  return true;
}

bool
CSVGObject::
processFilterOption(const string &opt_name, const string &opt_value)
{
  string str;

  // Filter Attributes
  if (svg_.stringOption(opt_name, opt_value, "filter", str)) {
    CSVGObject *object;

    if (! svg_.decodeUrlObject(opt_value, &object))
      CSVGLog() << "Unknown filter " << opt_value << " for " << getObjName();

    filter_ = dynamic_cast<CSVGFilter *>(object);
  }
  else
    return false;

  return true;
}

bool
CSVGObject::
processViewportOption(const string &opt_name, const string &opt_value)
{
  string str;

  if      (svg_.stringOption(opt_name, opt_value, "clip", str))
    notHandled(opt_name, opt_value);
  else if (svg_.stringOption(opt_name, opt_value, "overflow", str))
    notHandled(opt_name, opt_value);
  else
    return false;

  return true;
}

bool
CSVGObject::
processGradientOption(const string &opt_name, const string &opt_value)
{
  string str;

  // Gradient Properties
  if      (svg_.stringOption(opt_name, opt_value, "stop-color", str)) {
    CSVGStop *stop = dynamic_cast<CSVGStop *>(this);

    if (stop != NULL)
      stop->processOption(opt_name, opt_value);
    else
      notHandled(opt_name, opt_value);
  }
  else if (svg_.stringOption(opt_name, opt_value, "stop-opacity", str)) {
    CSVGStop *stop = dynamic_cast<CSVGStop *>(this);

    if (stop != NULL)
      stop->processOption(opt_name, opt_value);
    else
      notHandled(opt_name, opt_value);
  }
  else
    return false;

  return true;
}

bool
CSVGObject::
processContainerOption(const string &opt_name, const string &opt_value)
{
  string str;

  // Container Properties
  if (svg_.stringOption(opt_name, opt_value, "enable-background", str))
    notHandled(opt_name, opt_value);
  else
    return false;

  return true;
}

bool
CSVGObject::
processGraphicalEventsOption(const string &opt_name, const string &opt_value)
{
  string str;

  if      (svg_.stringOption(opt_name, opt_value, "onfocusin", str))
    notHandled(opt_name, opt_value);
  else if (svg_.stringOption(opt_name, opt_value, "onfocusout", str))
    notHandled(opt_name, opt_value);
  else if (svg_.stringOption(opt_name, opt_value, "onactivate", str))
    notHandled(opt_name, opt_value);
  else if (svg_.stringOption(opt_name, opt_value, "onclick", str))
    notHandled(opt_name, opt_value);
  else if (svg_.stringOption(opt_name, opt_value, "onmousedown", str))
    notHandled(opt_name, opt_value);
  else if (svg_.stringOption(opt_name, opt_value, "onmouseup", str))
    notHandled(opt_name, opt_value);
  else if (svg_.stringOption(opt_name, opt_value, "onmouseover", str))
    notHandled(opt_name, opt_value);
  else if (svg_.stringOption(opt_name, opt_value, "onmousemove", str))
    notHandled(opt_name, opt_value);
  else if (svg_.stringOption(opt_name, opt_value, "onmouseout", str))
    notHandled(opt_name, opt_value);
  else if (svg_.stringOption(opt_name, opt_value, "onload", str))
    notHandled(opt_name, opt_value);
  else
    return false;

  return true;
}

bool
CSVGObject::
processDocumentEventsOption(const string &opt_name, const string &opt_value)
{
  string str;

  if      (svg_.stringOption(opt_name, opt_value, "onunload", str))
    notHandled(opt_name, opt_value);
  else if (svg_.stringOption(opt_name, opt_value, "onabort", str))
    notHandled(opt_name, opt_value);
  else if (svg_.stringOption(opt_name, opt_value, "onerror", str))
    notHandled(opt_name, opt_value);
  else if (svg_.stringOption(opt_name, opt_value, "onresize", str))
    notHandled(opt_name, opt_value);
  else if (svg_.stringOption(opt_name, opt_value, "onscroll", str))
    notHandled(opt_name, opt_value);
  else if (svg_.stringOption(opt_name, opt_value, "onzoom", str))
    notHandled(opt_name, opt_value);
  else
    return false;

  return true;
}

bool
CSVGObject::
processAnimationEventsOption(const string &opt_name, const string &opt_value)
{
  string str;

  if      (svg_.stringOption(opt_name, opt_value, "onbegin", str))
    notHandled(opt_name, opt_value);
  else if (svg_.stringOption(opt_name, opt_value, "onend", str))
    notHandled(opt_name, opt_value);
  else if (svg_.stringOption(opt_name, opt_value, "onrepeat", str))
    notHandled(opt_name, opt_value);
  else if (svg_.stringOption(opt_name, opt_value, "onload", str))
    notHandled(opt_name, opt_value);
  else
    return false;

  return true;
}

bool
CSVGObject::
processCursorOption(const string &opt_name, const string &opt_value)
{
  string str;

  if (svg_.stringOption(opt_name, opt_value, "cursor", str))
    notHandled(opt_name, opt_value);
  else
    return false;

  return true;
}

bool
CSVGObject::
processExternalOption(const string &opt_name, const string &opt_value)
{
  string str;

  if (svg_.stringOption(opt_name, opt_value, "externalResourcesRequired", str))
    notHandled(opt_name, opt_value);
  else
    return false;

  return true;
}

bool
CSVGObject::
processFontOption(const string &opt_name, const string &opt_value)
{
  string str;

  if      (svg_.stringOption(opt_name, opt_value, "font", str))
    notHandled(opt_name, opt_value); //setFont(str)

  // Font properties
  if      (svg_.stringOption(opt_name, opt_value, "font-family", str))
    setFontFamily(str);
  else if (svg_.stringOption(opt_name, opt_value, "font-size", str)) {
    double size;

    if (svg_.lengthToReal(str, &size))
      setFontSize(size);
  }
  else if (svg_.stringOption(opt_name, opt_value, "font-size-adjust", str))
    notHandled(opt_name, opt_value); //setFontSizeAdjust(str)
  else if (svg_.stringOption(opt_name, opt_value, "font-stretch", str))
    notHandled(opt_name, opt_value); //setFontStretch(str)
  else if (svg_.stringOption(opt_name, opt_value, "font-style", str))
    setFontStyle(str);
  else if (svg_.stringOption(opt_name, opt_value, "font-variant", str))
    notHandled(opt_name, opt_value); //setFontVariant(str)
  else if (svg_.stringOption(opt_name, opt_value, "font-weight", str))
    setFontWeight(str);
  else
    return false;

  return true;
}

bool
CSVGObject::
processTextOption(const string &opt_name, const string &opt_value)
{
  string str;

  // Text properties
  if (svg_.stringOption(opt_name, opt_value, "writing-mode", str))
    notHandled(opt_name, opt_value);
  else
    return false;

  return true;
}

bool
CSVGObject::
processTextContentOption(const string &opt_name, const string &opt_value)
{
  string str;

  // Text content propeties
  if      (svg_.stringOption(opt_name, opt_value, "alignment-baseline", str))
    notHandled(opt_name, opt_value);
  else if (svg_.stringOption(opt_name, opt_value, "baseline-shift", str))
    notHandled(opt_name, opt_value);
  else if (svg_.stringOption(opt_name, opt_value, "direction", str))
    notHandled(opt_name, opt_value);
  else if (svg_.stringOption(opt_name, opt_value, "dominant-baseline", str))
    notHandled(opt_name, opt_value);
  else if (svg_.stringOption(opt_name, opt_value, "glyph-orientation-horizontal", str))
    notHandled(opt_name, opt_value);
  else if (svg_.stringOption(opt_name, opt_value, "glyph-orientation-vertical", str))
    notHandled(opt_name, opt_value);
  else if (svg_.stringOption(opt_name, opt_value, "kerning", str))
    notHandled(opt_name, opt_value);
  else if (svg_.stringOption(opt_name, opt_value, "letter-spacing", str))
    notHandled(opt_name, opt_value);
  else if (svg_.stringOption(opt_name, opt_value, "text-anchor", str))
    setTextAnchor(str);
  else if (svg_.stringOption(opt_name, opt_value, "text-decoration", str))
    notHandled(opt_name, opt_value);
  else if (svg_.stringOption(opt_name, opt_value, "unicode-bidi", str))
    notHandled(opt_name, opt_value);
  else if (svg_.stringOption(opt_name, opt_value, "word-spacing", str))
    notHandled(opt_name, opt_value);
  else
    return false;

  return true;
}

void
CSVGObject::
notHandled(const string &opt_name, const string &opt_value)
{
  CSVGLog() << "Option " << opt_name << ":" << opt_value << " not handled " <<
               "for " << getObjName();
}

void
CSVGObject::
setTextAnchor(const string &anchor)
{
  if      (anchor == "start" ) text_anchor_ = CHALIGN_TYPE_LEFT;
  else if (anchor == "middle") text_anchor_ = CHALIGN_TYPE_CENTER;
  else if (anchor == "end"   ) text_anchor_ = CHALIGN_TYPE_RIGHT;
  else                         notHandled("text-anchor", anchor);
}

CHAlignType
CSVGObject::
getTextAnchor() const
{
  if (text_anchor_.isValid())
    return text_anchor_.getValue();

  CSVGObject *parent = getParent();

  while (parent) {
    if (parent->text_anchor_.isValid())
      return parent->getTextAnchor();

    parent = parent->getParent();
  }

  return CHALIGN_TYPE_LEFT;
}

void
CSVGObject::
setShapeRendering(const string &rendering)
{
  shape_rendering_ = rendering;
}

void
CSVGObject::
addChildObject(CSVGObject *object)
{
  object->setParent(this);

  objects_.push_back(object);
}

void
CSVGObject::
deleteChildObject(CSVGObject *object)
{
  object->setParent(NULL);

  objects_.remove(object);
}

void
CSVGObject::
getAllChildrenOfType(uint id, vector<CSVGObject *> &objects)
{
  getChildrenOfType(id, objects);

  ObjectList::iterator p1, p2;

  for (p1 = objects_.begin(), p2 = objects_.end(); p1 != p2; ++p1)
    (*p1)->getAllChildrenOfType(id, objects);
}

void
CSVGObject::
getChildrenOfType(uint id, vector<CSVGObject *> &objects)
{
  ObjectList::iterator p1, p2;

  for (p1 = objects_.begin(), p2 = objects_.end(); p1 != p2; ++p1)
    if ((*p1)->isObjType(id))
      objects.push_back(*p1);
}

void
CSVGObject::
getAllChildrenOfId(const string &id, vector<CSVGObject *> &objects)
{
  getChildrenOfId(id, objects);

  ObjectList::iterator p1, p2;

  for (p1 = objects_.begin(), p2 = objects_.end(); p1 != p2; ++p1)
    (*p1)->getAllChildrenOfId(id, objects);
}

void
CSVGObject::
getChildrenOfId(const string &id, vector<CSVGObject *> &objects)
{
  ObjectList::iterator p1, p2;

  for (p1 = objects_.begin(), p2 = objects_.end(); p1 != p2; ++p1)
    if ((*p1)->getId() == id)
      objects.push_back(*p1);
}

void
CSVGObject::
drawObjects()
{
  CAux::for_each(objects_.begin(), objects_.end(), &CSVGObject::drawObject);
}

void
CSVGObject::
drawObject()
{
  if (! isDrawable()) return;

  //------

  // set current transform and stroke

  CMatrix2D transform;

  svg_.getTransform(transform);

  svg_.setTransform(transform*getTransform());

  CSVGTempStroke temp_stroke(*this);
  CSVGTempFont   temp_font  (*this);

  //------

  // draw clip path if specified

  if (clip_path_ != NULL) {
    svg_.pathInit();

    clip_path_->drawObject();

    if (clip_.getRule() == FILL_TYPE_EVEN_ODD)
      svg_.pathEoClip();
    else
      svg_.pathClip();
  }

  //------

  // draw object (virtual)

  draw();

  //------

  // draw children

  drawObjects();

  //------

  if (filter_ != NULL) {
    filter_->setObject(this);

    filter_->drawObject();
  }

  if (clip_path_ != NULL)
    svg_.initClip();

  if (mask_ != NULL) {
    CSVGMask *mask = dynamic_cast<CSVGMask *>(mask_);

    mask->objectDraw(*this);
  }

  //------

  svg_.setTransform(transform);
}

CImagePtr
CSVGObject::
toImage()
{
  CBBox2D bbox;

  if (! getBBox(bbox))
    return CImagePtr();

  string old_buffer = svg_.getBufferName();

  svg_.setBuffer("SourceGraphic");

  CSVGBuffer *buffer = svg_.getBuffer();

  buffer->setup(bbox);

  drawObject();

  CImagePtr image = svg_.getBufferImage("SourceGraphic");

  svg_.setBuffer(old_buffer);

  return image;
}

void
CSVGObject::
moveTo(const CPoint2D &point)
{
  CBBox2D bbox;

  if (! getBBox(bbox))
    return;

  moveBy(CVector2D(bbox.getMin(), point));
}

void
CSVGObject::
moveBy(const CVector2D &)
{
  if (isDrawable())
    CSVGLog() << "moveBy: not implemented";
}

void
CSVGObject::
resizeTo(const CSize2D &)
{
  if (isDrawable())
    CSVGLog() << "resizeTo: not implemented";
}

void
CSVGObject::
rotateBy(double da, const CPoint2D &)
{
  CMatrix2D m;

  m.setRotation(da);

  setTransform(m*getTransform());
}

bool
CSVGObject::
getBBox(CBBox2D &bbox) const
{
  if (! view_box_.isSet()) {
    CBBox2D bbox1;

    if (! getChildrenBBox(bbox1))
      return false;

    bbox += bbox1;
  }
  else
    bbox = view_box_;

  return bbox.isSet();
}

bool
CSVGObject::
getSize(CSize2D &size) const
{
  double w = getXMax() - getXMin();
  double h = getYMax() - getYMin();

  size = CSize2D(w, h);

  return true;
}

double
CSVGObject::
getXMin() const
{
  CBBox2D bbox;

  if (getBBox(bbox))
    return bbox.getXMin();

  return 0.0;
}

double
CSVGObject::
getYMin() const
{
  CBBox2D bbox;

  if (getBBox(bbox))
    return bbox.getYMin();

  return 0.0;
}

double
CSVGObject::
getXMax() const
{
  CBBox2D bbox;

  if (getBBox(bbox))
    return bbox.getXMax();

  return 1.0;
}

double
CSVGObject::
getYMax() const
{
  CBBox2D bbox;

  if (getBBox(bbox))
    return bbox.getYMax();

  return 1.0;
}

bool
CSVGObject::
getChildrenBBox(CBBox2D &bbox) const
{
  CBBox2D bbox1;

  ObjectList::const_iterator p1, p2;

  for (p1 = childrenBegin(), p2 = childrenEnd(); p1 != p2; ++p1) {
    if ((*p1)->getBBox(bbox1))
      bbox.add(bbox1);
  }

  return bbox.isSet();
}

void
CSVGObject::
setId(const string &id)
{
  id_ = id;

  svg_.setObjectById(id, this);
}

void
CSVGObject::
setClass(const string &c)
{
  class_ = c;
}

bool
CSVGObject::
decodeXLink(const string &str, CSVGObject **object, CImagePtr &image)
{
  if (object != NULL) *object = NULL;

  CSVGObject *object1 = NULL;

  uint len = str.size();

  if (len >= 5 && str.substr(0, 5) == "data:") {
    uint pos = 5;

    string format;

    while (pos < len && str[pos] != ',')
      format += str[pos++];

    if (pos >= len) return false;

    ++pos;

    if (format == "image/png;base64") {
      string str1 = CEncode64Inst->decode(str.substr(pos));

      CFile file(".svg.png");

      file.write(str1);

      file.flush();

      file.close();

      CImageFileSrc src(".svg.png");

      image = CImageMgrInst->createImage(src);

      return true;
    }
    else
      return false;
  }

  CImagePtr image1;

  string::size_type pos = str.find('#');

  if (pos != string::npos) {
    string lhs = str.substr(0, pos);
    string rhs = str.substr(pos + 1);

    if (lhs != "") {
      CSVGBlock *block = new CSVGBlock(svg_);

      svg_.read(lhs, block);

      if (rhs != "")
        object1 = svg_.lookupObjectById(rhs);
      else
        object1 = block;
    }
    else
      object1 = svg_.lookupObjectById(rhs);

    if (object1 == NULL) {
      CSVGLog() << "Object " << rhs << " does not exist";
      return false;
    }
  }
  else {
    CFile file(str);

    if (! file.exists()) {
      CSVGLog() << "File " << str << " does not exist";
      return false;
    }

    if (! file.isRegular()) {
      CSVGLog() << "File " << str << " is not a regular file";
      return false;
    }

    CFileType type = file.getType();

    if      (type & CFILE_TYPE_IMAGE) {
      if (type == CFILE_TYPE_IMAGE_SVG) {
        CImageNameSrc src(file.getPath());

        image1 = CImageMgrInst->createImage(src);

        CSVG svg;

        svg.init();

        svg.read(file.getPath());

        CSize2D size;

        getSize(size);

        svg.getBlock()->setSize(size);

        int w = svg.getIWidth();
        int h = svg.getIHeight();

        image1 = svg.drawToImage(w, h);
      }
      else {
        CImageFileSrc src(file);

        image1 = CImageMgrInst->createImage(src);

        image1->read(str);
      }
    }
    else if (type == CFILE_TYPE_TEXT_HTML) {
      CSVGBlock *block = new CSVGBlock(svg_);

      svg_.read(str, block);

      object1 = block;
    }
    else {
      CSVGLog() << "Unknown type of file for " << str;
      return false;
    }
  }

  if (object != NULL) *object = object1;

  image = image1;

  return true;
}

void
CSVGObject::
print(ostream &os) const
{
  os << getObjName() << ": '" << getId() << "'";
}

//-----------

CSVGTempStroke::
CSVGTempStroke(CSVGObject &object) :
 object_(object),
 stroke_(object_.getSVG()),
 fill_  (object_.getSVG()),
 clip_  (object_.getSVG())
{
  stroke_ = object_.getSVG().getStroke();
  fill_   = object_.getSVG().getFill  ();
  clip_   = object_.getSVG().getClip  ();

  if (! object.getSelected())
    object_.getSVG().updateStroke(object_.getStroke());
  else
    object_.getSVG().setSelectedStroke();

  object_.getSVG().updateFill(object_.getFill());

  object_.getSVG().updateClip(object_.getClip());
}

CSVGTempStroke::
~CSVGTempStroke()
{
  object_.getSVG().resetStroke();
  object_.getSVG().resetFill  ();
  object_.getSVG().resetClip  ();

  object_.getSVG().updateStroke(stroke_);
  object_.getSVG().updateFill  (fill_  );
  object_.getSVG().updateClip  (clip_  );
}

CSVGTempFont::
CSVGTempFont(CSVGObject &object) :
 object_  (object),
 font_def_(object_.getFontDef())
{
  font_def_ = object_.getSVG().getFontDef();

  object_.getSVG().updateFontDef(object_.getFontDef());
}

CSVGTempFont::
~CSVGTempFont()
{
  object_.getSVG().updateFontDef(font_def_);
}

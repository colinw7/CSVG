#include <CSVGObject.h>
#include <CSVGFilter.h>
#include <CSVGStop.h>
#include <CSVGMask.h>
#include <CSVGBuffer.h>
#include <CSVG.h>
#include <CSVGLog.h>
#include <CSVGTempStroke.h>
#include <CSVGTempFont.h>

#include <CFontMgr.h>
#include <CEncode64.h>
#include <CRegExp.h>
//#include <CFuncs.h>

namespace {

static uint s_ObjectInd;

uint nextInd() {
  ++s_ObjectInd;

  return s_ObjectInd;
}

}

CSVGObject::
CSVGObject(CSVG &svg) :
 svg_    (svg),
 ind_    (nextInd()),
 stroke_ (svg),
 fill_   (svg),
 clip_   (svg),
 fontDef_(svg)
{
  transform_.setIdentity();
}

CSVGObject::
CSVGObject(const CSVGObject &obj) :
 svg_           (obj.svg_),
 id_            (obj.id_),
 class_         (obj.class_),
 parent_        (obj.parent_),
 ind_           (nextInd()),
 opacity_       (obj.opacity_),
 stroke_        (obj.stroke_),
 fill_          (obj.fill_),
 clip_          (obj.clip_),
 fontDef_       (obj.fontDef_),
 textAnchor_    (obj.textAnchor_),
 shapeRendering_(obj.shapeRendering_),
 transform_     (obj.transform_),
 objects_       (),
 filter_        (obj.filter_),
 mask_          (obj.mask_),
 clipPath_      (obj.clipPath_),
 marker_        (obj.marker_),
 viewBox_       (obj.viewBox_),
 selected_      (obj.selected_),
 xmlTag_        (obj.xmlTag_)
{
  for (const auto &o : obj.children()) {
    CSVGObject *child = o->dup();

    addChildObject(child);
  }
}

CSVGObject::
~CSVGObject()
{
  for (auto &o : objects_)
    delete o;
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
  typedef std::map<std::string,int> IdMap;

  static IdMap idMap;

  std::string typeName = getObjType().getName();

  auto p = idMap.find(typeName);

  if (p == idMap.end())
    p = idMap.insert(p, IdMap::value_type(typeName, 1));

  setId(CStrUtil::strprintf("%s%d", typeName.c_str(), (*p).second));

  ++(*p).second;
}

void
CSVGObject::
setText(const std::string &str)
{
  std::string str1 = CStrUtil::stripSpaces(str);

  if (str1.empty()) return;

  std::cerr << "Option does not support text '" + str + "'" << std::endl;
}

void
CSVGObject::
setOpacity(const std::string &opacityDef)
{
  double opacity = svg_.decodeOpacityString(opacityDef);

  setOpacity(opacity);
}

void
CSVGObject::
updateStroke(const CSVGStroke &stroke, bool recurse)
{
  stroke_.update(stroke);

  if (recurse) {
    for (auto &c : children())
      c->updateStroke(stroke, recurse);
  }
}

void
CSVGObject::
updateFill(const CSVGFill &fill, bool recurse)
{
  fill_.update(fill);

  if (recurse) {
    for (auto &c : children())
      c->updateFill(fill, recurse);
  }
}

double
CSVGObject::
getOpacity() const
{
  // TODO: inherit ?
  if (opacity_.isValid())
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

  if (! color.isValid()) {
    if (stroke_.getDefColorValid())
      color.setValue(stroke_.getDefColor());
  }

  if (! color.isValid()) {
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

  if (! opacity.isValid()) {
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

  if (! color.isValid()) {
    if (fill_.getDefColorValid())
      color.setValue(fill_.getDefColor());
  }

  if (! color.isValid()) {
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

  if (! opacity.isValid())
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

std::string
CSVGObject::
getFontFamily() const
{
  if (fontDef_.hasFamily())
    return fontDef_.getFamily();

  CSVGObject *parent = getParent();

  while (parent) {
    if (parent->fontDef_.hasFamily())
      return parent->getFontFamily();

    parent = parent->getParent();
  }

  return "serif";
}

CFontStyles
CSVGObject::
getFontStyle() const
{
  if (fontDef_.hasStyle())
    return fontDef_.getStyle();

  CSVGObject *parent = getParent();

  while (parent) {
    if (parent->fontDef_.hasStyle())
      return parent->getFontStyle();

    parent = parent->getParent();
  }

  return CFONT_STYLE_NORMAL;
}

double
CSVGObject::
getFontSize() const
{
  if (fontDef_.hasSize())
    return fontDef_.getSize();

  CSVGObject *parent = getParent();

  while (parent) {
    if (parent->fontDef_.hasSize())
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
setFontFamily(const std::string &family)
{
  fontDef_.setFamily(family);
}

void
CSVGObject::
setFontSize(double size)
{
  fontDef_.setSize(size);
}

void
CSVGObject::
setFontSize(const CSVGLengthValue &size)
{
  fontDef_.setSize(size);
}

void
CSVGObject::
setFontWeight(const std::string &weight)
{
  fontDef_.setWeight(weight);
}

void
CSVGObject::
setFontStyle(const std::string &style)
{
  fontDef_.setStyle(style);
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
setStyle(const std::string &style)
{
  std::vector<std::string> words;

  CStrUtil::addFields(style, words, ";");

  uint num_words = words.size();

  for (uint i = 0; i < num_words; ++i) {
    std::vector<std::string> words1;

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
setSelected(bool selected, bool hier)
{
  selected_ = selected;

  if (hier) {
    for (auto &c : children())
      c->setSelected(selected, hier);
  }
}

bool
CSVGObject::
processOption(const std::string &optName, const std::string &optValue)
{
  if (processCoreOption           (optName, optValue)) return true;
  if (processConditionalOption    (optName, optValue)) return true;
  if (processStyleOption          (optName, optValue)) return true;
  if (processPaintOption          (optName, optValue)) return true;
  if (processColorOption          (optName, optValue)) return true;
  if (processOpacityOption        (optName, optValue)) return true;
  if (processGraphicsOption       (optName, optValue)) return true;
  if (processMarkerOption         (optName, optValue)) return true;
  if (processClipOption           (optName, optValue)) return true;
  if (processMaskOption           (optName, optValue)) return true;
  if (processFilterOption         (optName, optValue)) return true;
  if (processGradientOption       (optName, optValue)) return true;
  if (processGraphicalEventsOption(optName, optValue)) return true;
  if (processCursorOption         (optName, optValue)) return true;
  if (processExternalOption       (optName, optValue)) return true;
  if (processFontOption           (optName, optValue)) return true;
  if (processTextContentOption    (optName, optValue)) return true;

  std::string str;
  CBBox2D     bbox;

  // Other properties
  if      (svg_.stringOption(optName, optValue, "transform", str)) {
    CMatrix2D transform;

    if (! svg_.decodeTransform(str, transform))
      return false;

    setTransform(transform);
  }

  else if (svg_.bboxOption(optName, optValue, "viewBox", &bbox))
    viewBox_ = bbox;

  // xmlns
  else if (CRegExpUtil::parse(optName, "xmlns:.*")) {
    //notHandled(optName, optValue);
  }
  else if (CRegExpUtil::parse(optName, "sodipodi:.*")) {
    //notHandled(optName, optValue);
  }
  else if (CRegExpUtil::parse(optName, "inkscape:.*")) {
    //notHandled(optName, optValue);
  }
  else {
    CSVGLog() << "Unhandled option " << optName << " for " << getObjName();
    return false;
  }

  return true;
}

bool
CSVGObject::
processCoreOption(const std::string &optName, const std::string &optValue)
{
  std::string str;

  // Core Attributes
  if      (svg_.stringOption(optName, optValue, "id", str))
    setId(str);
  else if (svg_.stringOption(optName, optValue, "class", str))
    setClass(str);
  else if (svg_.stringOption(optName, optValue, "xml:base", str))
    notHandled(optName, optValue);
  else if (svg_.stringOption(optName, optValue, "xml:lang", str))
    notHandled(optName, optValue);
  else if (svg_.stringOption(optName, optValue, "xml:space", str))
    /* default or preserve */;
  else
    return false;

  return true;
}

bool
CSVGObject::
processConditionalOption(const std::string &optName, const std::string &optValue)
{
  std::string str;

  // Conditional Attributes
  if      (svg_.stringOption(optName, optValue, "requiredFeatures", str))
    setId(str);
  else if (svg_.stringOption(optName, optValue, "requiredExtensions", str))
    notHandled(optName, optValue);
  else if (svg_.stringOption(optName, optValue, "systemLanguage", str))
    notHandled(optName, optValue);
  else
    return false;

  return true;
}

bool
CSVGObject::
processStyleOption(const std::string &optName, const std::string &optValue)
{
  std::string str;

  // Style Attributes
  if      (svg_.stringOption(optName, optValue, "style", str))
    setStyle(str);
  else if (svg_.stringOption(optName, optValue, "class", str))
    notHandled(optName, optValue);
  else
    return false;

  return true;
}

bool
CSVGObject::
processPaintOption(const std::string &optName, const std::string &optValue)
{
  std::string str;

  // Paint Attributes
  if      (svg_.stringOption(optName, optValue, "fill", str))
    setFillColor(str);
  else if (svg_.stringOption(optName, optValue, "fill-rule", str))
    setFillRule(str);
  else if (svg_.stringOption(optName, optValue, "stroke", str))
    setStrokeColor(str);
  else if (svg_.stringOption(optName, optValue, "stroke-dasharray", str))
    setStrokeDash(str);
  else if (svg_.stringOption(optName, optValue, "stroke-dashoffset", str))
    setStrokeDashOffset(str);
  else if (svg_.stringOption(optName, optValue, "stroke-linecap", str))
    setStrokeLineCap(str);
  else if (svg_.stringOption(optName, optValue, "stroke-linejoin", str))
    setStrokeLineJoin(str);
  else if (svg_.stringOption(optName, optValue, "stroke-miterlimit", str))
    setStrokeMitreLimit(str);
  else if (svg_.stringOption(optName, optValue, "stroke-width", str))
    setStrokeWidth(str);
  else if (svg_.stringOption(optName, optValue, "overflow", str))
    ;
  else if (svg_.stringOption(optName, optValue, "visibility", str))
    ;
  else if (svg_.stringOption(optName, optValue, "display", str))
    ;
  else
    return false;

  return true;
}

bool
CSVGObject::
processColorOption(const std::string &optName, const std::string &optValue)
{
  std::string str;

  // Color Attributes
  if      (svg_.stringOption(optName, optValue, "color", str))
    notHandled(optName, optValue);
  else if (svg_.stringOption(optName, optValue, "color-interpolation", str))
    notHandled(optName, optValue);
  else if (svg_.stringOption(optName, optValue, "color-rendering", str))
    notHandled(optName, optValue);
  else
    return false;

  return true;
}

bool
CSVGObject::
processColorProfileOption(const std::string &optName, const std::string &optValue)
{
  std::string str;

  // Color Profile
  if (svg_.stringOption(optName, optValue, "color-profile", str))
    notHandled(optName, optValue);
  else
    return false;

  return true;
}

bool
CSVGObject::
processFilterColorOption(const std::string &optName, const std::string &optValue)
{
  std::string str;

  // Filter Color
  if (svg_.stringOption(optName, optValue, "color-interpolation-filters", str))
    notHandled(optName, optValue);
  else
    return false;

  return true;
}

bool
CSVGObject::
processOpacityOption(const std::string &optName, const std::string &optValue)
{
  std::string str;

  // Opacity Attributes
  if      (svg_.stringOption(optName, optValue, "opacity", str))
    setOpacity(str);
  else if (svg_.stringOption(optName, optValue, "fill-opacity", str))
    setFillOpacity(str);
  else if (svg_.stringOption(optName, optValue, "stroke-opacity", str))
    setStrokeOpacity(str);
  else
    return false;

  return true;
}

bool
CSVGObject::
processGraphicsOption(const std::string &optName, const std::string &optValue)
{
  std::string str;

  // Graphics Attributes
  if      (svg_.stringOption(optName, optValue, "display", str))
    // inline | block | list-item | run-in | compact | marker |
    // table | inline-table | table-row-group | table-header-group |
    // table-footer-group | table-row | table-column-group |
    // table-column | table-cell | table-caption | none | inherit
    notHandled(optName, optValue);
  else if (svg_.stringOption(optName, optValue, "image-rendering", str))
    // auto | optimizeSpeed | optimizeQuality | inherit
    notHandled(optName, optValue);
  else if (svg_.stringOption(optName, optValue, "pointer-events", str))
    // visiblePainted | visibleFill | visibleStroke | visible |
    // painted | fill | stroke | all | none | inherit
    notHandled(optName, optValue);
  else if (svg_.stringOption(optName, optValue, "shape-rendering", str))
    // auto | optimizeSpeed | crispEdges | geometricPrecision |
    // inherit
    setShapeRendering(str);
  else if (svg_.stringOption(optName, optValue, "text-renderer", str))
    // auto | optimizeSpeed | optimizeLegibility |
    // geometricPrecision | inherit
    notHandled(optName, optValue);
  else if (svg_.stringOption(optName, optValue, "visibility", str))
    // visible | hidden | inherit
    notHandled(optName, optValue);
  else
    return false;

  return true;
}

bool
CSVGObject::
processMarkerOption(const std::string &optName, const std::string &optValue)
{
  std::string str;

  // Marker
  if      (svg_.stringOption(optName, optValue, "marker", str)) {
    if (str != "none") {
      if (! svg_.decodeUrlObject(optValue, &marker_.start))
        CSVGLog() << "Unknown marker start " << optValue << " for " << getObjName();

      marker_.mid = marker_.start;
      marker_.end = marker_.start;
    }
  }
  else if (svg_.stringOption(optName, optValue, "marker-start", str)) {
    if (! svg_.decodeUrlObject(optValue, &marker_.start))
      CSVGLog() << "Unknown marker start " << optValue << " for " << getObjName();
  }
  else if (svg_.stringOption(optName, optValue, "marker-mid", str)) {
    if (! svg_.decodeUrlObject(optValue, &marker_.mid))
      CSVGLog() << "Unknown marker mid " << optValue << " for " << getObjName();
  }
  else if (svg_.stringOption(optName, optValue, "marker-end", str)) {
    if (! svg_.decodeUrlObject(optValue, &marker_.end))
      CSVGLog() << "Unknown marker end " << optValue << " for " << getObjName();
  }
  else
    return false;

  return true;
}

bool
CSVGObject::
processClipOption(const std::string &optName, const std::string &optValue)
{
  std::string str;

  if      (svg_.stringOption(optName, optValue, "clip-path", str)) {
    if (! svg_.decodeUrlObject(optValue, &clipPath_))
      CSVGLog() << "Unknown clip path " << optValue << " for " << getObjName();
  }
  else if (svg_.stringOption(optName, optValue, "clip-rule", str)) {
    setClipRule(str);
  }
  else
    return false;

  return true;
}

bool
CSVGObject::
processPresentationOption(const std::string &optName, const std::string &optValue)
{
  std::string str;

  if (processContainerOption   (optName, optValue)) return true;
  if (processViewportOption    (optName, optValue)) return true;
  if (processTextOption        (optName, optValue)) return true;
  if (processTextContentOption (optName, optValue)) return true;
  if (processFontOption        (optName, optValue)) return true;
  if (processPaintOption       (optName, optValue)) return true;
  if (processColorOption       (optName, optValue)) return true;
  if (processOpacityOption     (optName, optValue)) return true;
  if (processGraphicsOption    (optName, optValue)) return true;
  if (processMarkerOption      (optName, optValue)) return true;
  if (processColorProfileOption(optName, optValue)) return true;
  if (processGradientOption    (optName, optValue)) return true;
  if (processClipOption        (optName, optValue)) return true;
  if (processMaskOption        (optName, optValue)) return true;
  if (processFilterOption      (optName, optValue)) return true;
  if (processFilterColorOption (optName, optValue)) return true;
  if (processCursorOption      (optName, optValue)) return true;

  // Presentation
  if      (svg_.stringOption(optName, optValue, "flood-color", str))
    notHandled(optName, optValue);
  else if (svg_.stringOption(optName, optValue, "flood-opacity", str))
    notHandled(optName, optValue);
  else if (svg_.stringOption(optName, optValue, "lighting-color", str))
    notHandled(optName, optValue);
  else
    return false;

  return true;
}

bool
CSVGObject::
processMaskOption(const std::string &optName, const std::string &optValue)
{
  std::string str;

  if (svg_.stringOption(optName, optValue, "mask", str)) {
    if (! svg_.decodeUrlObject(optValue, &mask_))
      CSVGLog() << "Unknown mask " << optValue << " for " << getObjName();
  }
  else
    return false;

  return true;
}

bool
CSVGObject::
processFilterOption(const std::string &optName, const std::string &optValue)
{
  std::string str;

  // Filter Attributes
  if (svg_.stringOption(optName, optValue, "filter", str)) {
    CSVGObject *object;

    if (! svg_.decodeUrlObject(optValue, &object))
      CSVGLog() << "Unknown filter " << optValue << " for " << getObjName();

    filter_ = dynamic_cast<CSVGFilter *>(object);
  }
  else
    return false;

  return true;
}

bool
CSVGObject::
processViewportOption(const std::string &optName, const std::string &optValue)
{
  std::string str;

  if      (svg_.stringOption(optName, optValue, "clip", str))
    notHandled(optName, optValue);
  else if (svg_.stringOption(optName, optValue, "overflow", str))
    notHandled(optName, optValue);
  else
    return false;

  return true;
}

bool
CSVGObject::
processGradientOption(const std::string &optName, const std::string &optValue)
{
  std::string str;

  // Gradient Properties
  if      (svg_.stringOption(optName, optValue, "stop-color", str)) {
    CSVGStop *stop = dynamic_cast<CSVGStop *>(this);

    if (stop != 0)
      stop->processOption(optName, optValue);
    else
      notHandled(optName, optValue);
  }
  else if (svg_.stringOption(optName, optValue, "stop-opacity", str)) {
    CSVGStop *stop = dynamic_cast<CSVGStop *>(this);

    if (stop != 0)
      stop->processOption(optName, optValue);
    else
      notHandled(optName, optValue);
  }
  else
    return false;

  return true;
}

bool
CSVGObject::
processContainerOption(const std::string &optName, const std::string &optValue)
{
  std::string str;

  // Container Properties
  if (svg_.stringOption(optName, optValue, "enable-background", str))
    notHandled(optName, optValue);
  else
    return false;

  return true;
}

bool
CSVGObject::
processGraphicalEventsOption(const std::string &optName, const std::string &optValue)
{
  std::string str;

  if      (svg_.stringOption(optName, optValue, "onfocusin", str))
    notHandled(optName, optValue);
  else if (svg_.stringOption(optName, optValue, "onfocusout", str))
    notHandled(optName, optValue);
  else if (svg_.stringOption(optName, optValue, "onactivate", str))
    notHandled(optName, optValue);
  else if (svg_.stringOption(optName, optValue, "onclick", str))
    notHandled(optName, optValue);
  else if (svg_.stringOption(optName, optValue, "onmousedown", str))
    notHandled(optName, optValue);
  else if (svg_.stringOption(optName, optValue, "onmouseup", str))
    notHandled(optName, optValue);
  else if (svg_.stringOption(optName, optValue, "onmouseover", str))
    notHandled(optName, optValue);
  else if (svg_.stringOption(optName, optValue, "onmousemove", str))
    notHandled(optName, optValue);
  else if (svg_.stringOption(optName, optValue, "onmouseout", str))
    notHandled(optName, optValue);
  else if (svg_.stringOption(optName, optValue, "onload", str))
    notHandled(optName, optValue);
  else
    return false;

  return true;
}

bool
CSVGObject::
processDocumentEventsOption(const std::string &optName, const std::string &optValue)
{
  std::string str;

  if      (svg_.stringOption(optName, optValue, "onunload", str))
    notHandled(optName, optValue);
  else if (svg_.stringOption(optName, optValue, "onabort", str))
    notHandled(optName, optValue);
  else if (svg_.stringOption(optName, optValue, "onerror", str))
    notHandled(optName, optValue);
  else if (svg_.stringOption(optName, optValue, "onresize", str))
    notHandled(optName, optValue);
  else if (svg_.stringOption(optName, optValue, "onscroll", str))
    notHandled(optName, optValue);
  else if (svg_.stringOption(optName, optValue, "onzoom", str))
    notHandled(optName, optValue);
  else
    return false;

  return true;
}

bool
CSVGObject::
processAnimationEventsOption(const std::string &optName, const std::string &optValue)
{
  std::string str;

  if      (svg_.stringOption(optName, optValue, "onbegin", str))
    notHandled(optName, optValue);
  else if (svg_.stringOption(optName, optValue, "onend", str))
    notHandled(optName, optValue);
  else if (svg_.stringOption(optName, optValue, "onrepeat", str))
    notHandled(optName, optValue);
  else if (svg_.stringOption(optName, optValue, "onload", str))
    notHandled(optName, optValue);
  else
    return false;

  return true;
}

bool
CSVGObject::
processCursorOption(const std::string &optName, const std::string &optValue)
{
  std::string str;

  if (svg_.stringOption(optName, optValue, "cursor", str))
    notHandled(optName, optValue);
  else
    return false;

  return true;
}

bool
CSVGObject::
processExternalOption(const std::string &optName, const std::string &optValue)
{
  std::string str;

  if (svg_.stringOption(optName, optValue, "externalResourcesRequired", str))
    notHandled(optName, optValue);
  else
    return false;

  return true;
}

bool
CSVGObject::
processFontOption(const std::string &optName, const std::string &optValue)
{
  std::string str;

  if      (svg_.stringOption(optName, optValue, "font", str))
    notHandled(optName, optValue); //setFont(str)

  // Font properties
  if      (svg_.stringOption(optName, optValue, "font-family", str))
    setFontFamily(str);
  else if (svg_.stringOption(optName, optValue, "font-size", str)) {
    CSVGLengthValue lvalue;

    if (! svg_.decodeLengthValue(str, lvalue))
      return false;

    setFontSize(lvalue);
  }
  else if (svg_.stringOption(optName, optValue, "font-size-adjust", str))
    notHandled(optName, optValue); //setFontSizeAdjust(str)
  else if (svg_.stringOption(optName, optValue, "font-stretch", str))
    notHandled(optName, optValue); //setFontStretch(str)
  else if (svg_.stringOption(optName, optValue, "font-style", str))
    setFontStyle(str);
  else if (svg_.stringOption(optName, optValue, "font-variant", str))
    notHandled(optName, optValue); //setFontVariant(str)
  else if (svg_.stringOption(optName, optValue, "font-weight", str))
    setFontWeight(str);
  else
    return false;

  return true;
}

bool
CSVGObject::
processTextOption(const std::string &optName, const std::string &optValue)
{
  std::string str;

  // Text properties
  if (svg_.stringOption(optName, optValue, "writing-mode", str))
    notHandled(optName, optValue);
  else
    return false;

  return true;
}

bool
CSVGObject::
processTextContentOption(const std::string &optName, const std::string &optValue)
{
  std::string str;

  // Text content propeties
  if      (svg_.stringOption(optName, optValue, "alignment-baseline", str))
    notHandled(optName, optValue);
  else if (svg_.stringOption(optName, optValue, "baseline-shift", str))
    notHandled(optName, optValue);
  else if (svg_.stringOption(optName, optValue, "direction", str))
    notHandled(optName, optValue);
  else if (svg_.stringOption(optName, optValue, "dominant-baseline", str))
    notHandled(optName, optValue);
  else if (svg_.stringOption(optName, optValue, "glyph-orientation-horizontal", str))
    notHandled(optName, optValue);
  else if (svg_.stringOption(optName, optValue, "glyph-orientation-vertical", str))
    notHandled(optName, optValue);
  else if (svg_.stringOption(optName, optValue, "kerning", str))
    notHandled(optName, optValue);
  else if (svg_.stringOption(optName, optValue, "letter-spacing", str))
    notHandled(optName, optValue);
  else if (svg_.stringOption(optName, optValue, "text-anchor", str))
    setTextAnchor(str);
  else if (svg_.stringOption(optName, optValue, "text-decoration", str))
    notHandled(optName, optValue);
  else if (svg_.stringOption(optName, optValue, "unicode-bidi", str))
    notHandled(optName, optValue);
  else if (svg_.stringOption(optName, optValue, "word-spacing", str))
    notHandled(optName, optValue);
  else
    return false;

  return true;
}

void
CSVGObject::
notHandled(const std::string &optName, const std::string &optValue)
{
  CSVGLog() << "Option " << optName << ":" << optValue << " not handled " <<
               "for " << getObjName();
}

void
CSVGObject::
setTextAnchor(const std::string &anchor)
{
  if      (anchor == "start" ) textAnchor_ = CHALIGN_TYPE_LEFT;
  else if (anchor == "middle") textAnchor_ = CHALIGN_TYPE_CENTER;
  else if (anchor == "end"   ) textAnchor_ = CHALIGN_TYPE_RIGHT;
  else                         notHandled("text-anchor", anchor);
}

CHAlignType
CSVGObject::
getTextAnchor() const
{
  if (textAnchor_.isValid())
    return textAnchor_.getValue();

  CSVGObject *parent = getParent();

  while (parent) {
    if (parent->textAnchor_.isValid())
      return parent->getTextAnchor();

    parent = parent->getParent();
  }

  return CHALIGN_TYPE_LEFT;
}

void
CSVGObject::
setShapeRendering(const std::string &rendering)
{
  shapeRendering_ = rendering;
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
  object->setParent(0);

  objects_.remove(object);
}

void
CSVGObject::
getAllChildrenOfType(CSVGObjTypeId id, std::vector<CSVGObject *> &objects)
{
  getChildrenOfType(id, objects);

  for (const auto &c : children())
    c->getAllChildrenOfType(id, objects);
}

void
CSVGObject::
getChildrenOfType(CSVGObjTypeId id, std::vector<CSVGObject *> &objects)
{
  for (const auto &c : children())
    if (c->isObjType(id))
      objects.push_back(c);
}

void
CSVGObject::
getAllChildrenOfId(const std::string &id, std::vector<CSVGObject *> &objects)
{
  getChildrenOfId(id, objects);

  for (const auto &c : children())
    c->getAllChildrenOfId(id, objects);
}

void
CSVGObject::
getChildrenOfId(const std::string &id, std::vector<CSVGObject *> &objects)
{
  for (const auto &c : children())
    if (c->getId() == id)
      objects.push_back(c);
}

void
CSVGObject::
drawObjects()
{
  for (const auto &o : objects_)
    o->drawObject();
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

  CSVGTempStroke tempStroke(*this);
  CSVGTempFont   tempFont  (*this);

  //------

  drawInit();

  //------

  // draw clip path if specified
  if (clipPath_ != 0) {
    svg_.pathInit();

    clipPath_->drawObject();

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

  if (filter_ != 0) {
    filter_->setObject(this);

    filter_->drawObject();
  }

  if (clipPath_ != 0)
    svg_.initClip();

  if (mask_ != 0) {
    CSVGMask *mask = dynamic_cast<CSVGMask *>(mask_);

    mask->objectDraw(*this);
  }

  //------

  drawTerm();

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

  std::string oldBuffer = svg_.getBufferName();

  svg_.setBuffer("SourceGraphic");

  CSVGBuffer *buffer = svg_.getBuffer();

  buffer->setup(bbox);

  drawObject();

  CImagePtr image = svg_.getBufferImage("SourceGraphic");

  svg_.setBuffer(oldBuffer);

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
  if (! viewBox_.isSet()) {
    CBBox2D bbox1;

    if (! getChildrenBBox(bbox1))
      return false;

    bbox += bbox1;
  }
  else
    bbox = viewBox_;

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

  for (const auto &c  : children()) {
    if (c->getBBox(bbox1))
      bbox.add(bbox1);
  }

  return bbox.isSet();
}

void
CSVGObject::
setId(const std::string &id)
{
  id_ = id;

  svg_.setObjectById(id, this);
}

void
CSVGObject::
setClass(const std::string &c)
{
  class_ = c;
}

bool
CSVGObject::
decodeXLink(const std::string &str, CSVGObject **object, CImagePtr &image)
{
  if (object != 0) *object = 0;

  CSVGObject *object1 = 0;

  uint len = str.size();

  if (len >= 5 && str.substr(0, 5) == "data:") {
    uint pos = 5;

    std::string format;

    while (pos < len && str[pos] != ',')
      format += str[pos++];

    if (pos >= len) return false;

    ++pos;

    if (format == "image/png;base64") {
      std::string str1 = CEncode64Inst->decode(str.substr(pos));

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

  std::string::size_type pos = str.find('#');

  if (pos != std::string::npos) {
    std::string lhs = str.substr(0, pos);
    std::string rhs = str.substr(pos + 1);

    if (lhs != "") {
      CSVGBlock *block = svg_.createBlock();

      svg_.read(lhs, block);

      if (rhs != "")
        object1 = svg_.lookupObjectById(rhs);
      else
        object1 = block;
    }
    else
      object1 = svg_.lookupObjectById(rhs);

    if (object1 == 0) {
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
      CSVGBlock *block = svg_.createBlock();

      svg_.read(str, block);

      object1 = block;
    }
    else {
      CSVGLog() << "Unknown type of file for " << str;
      return false;
    }
  }

  if (object != 0) *object = object1;

  image = image1;

  return true;
}

void
CSVGObject::
getObjectsAtPoint(const CPoint2D &p, ObjectArray &objects) const
{
  if (inside(p))
    objects.push_back(const_cast<CSVGObject *>(this));

  for (const auto &c : children())
    c->getObjectsAtPoint(p, objects);
}

void
CSVGObject::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    std::string s = getText();

    if (! objects_.empty()) {
      os << "<" << getObjName() << ">" << std::endl;

      if (s != "")
        os << s << std::endl;

      for (const auto &o : objects_)
        o->print(os, hier);

      os << "</" << getObjName() << ">" << std::endl;
    }
    else if (s != "") {
      os << "<" << getObjName() << ">" << s;
      os << "</" << getObjName() << ">" << std::endl;
    }
    else
      os << "<" << getObjName() << "/>" << std::endl;
  }
  else
    os << getObjName() << ": '" << getId() << "'";
}

void
CSVGObject::
printFilter(std::ostream &os) const
{
  if (filter_)
    os << " filter=\"url(#" << filter_->getId() << ")\"";
}

void
CSVGObject::
printStyle(std::ostream &os) const
{
  if (! fontDef_.isSet() && ! stroke_.isSet() && ! fill_.isSet())
    return;

  os << " style=\"";

  bool output = false;

  if (fontDef_.isSet()) {
    printFontDef(os);

    output = true;
  }

  if (stroke_.isSet()) {
    if (output)
      os << " ";

    stroke_.print(os);

    output = true;
  }

  if (fill_.isSet()) {
    if (output)
      os << " ";

    fill_.print(os);

    output = true;
  }

  os << "\"";
}

void
CSVGObject::
printTextContent(std::ostream &os) const
{
  if (textAnchor_.isValid()) {
    os << " text-anchor=\"";

    if      (textAnchor_.getValue() == CHALIGN_TYPE_LEFT  ) os << "start";
    else if (textAnchor_.getValue() == CHALIGN_TYPE_CENTER) os << "middle";
    else if (textAnchor_.getValue() == CHALIGN_TYPE_RIGHT ) os << "end";

    os << "\"";
  }
}

void
CSVGObject::
printFontDef(std::ostream &os) const
{
  fontDef_.print(os);
}

void
CSVGObject::
printTransform(std::ostream &os) const
{
  if (! transform_.isIdentity()) {
    os << " transform=\"";

    printTransform(os, transform_);

    os << "\"";
  }
}

void
CSVGObject::
printTransform(std::ostream &os, const CMatrix2D &m) const
{
  if (! m.isIdentity()) {
    bool output = false;

    if (m.isInnerRotation()) {
      double a = m.getAngle();

      os << "rotate(" << a << ")";

      output = true;
    }

    if (! m.isInnerIdentity()) {
      if (output)
        os << " ";

      double sx, sy;

      m.getSize(&sx, &sy);

      os << "scale(" << sx << " " << sy << ")";

      output = true;
    }

    if (! m.isTranslateIdentity()) {
      if (output)
        os << " ";

      double tx, ty;

      m.getTranslate(&tx, &ty);

      os << "translate(" << tx << " " << ty << ")";

      output = true;
    }
  }
}

void
CSVGObject::
printLength(std::ostream &os, const CSVGLengthValue &l)
{
  if      (l.type() == CSVGLengthValue::Type::EM)
    os << l.ivalue() << "em";
  else if (l.type() == CSVGLengthValue::Type::EX)
    os << l.ivalue() << "ex";
  else if (l.type() == CSVGLengthValue::Type::PT)
    os << l.ivalue() << "pt";
  else if (l.type() == CSVGLengthValue::Type::PC)
    os << l.ivalue() << "pc";
  else if (l.type() == CSVGLengthValue::Type::CM)
    os << l.ivalue() << "cm";
  else if (l.type() == CSVGLengthValue::Type::MM)
    os << l.ivalue() << "mm";
  else if (l.type() == CSVGLengthValue::Type::IN)
    os << l.ivalue() << "in";
  else if (l.type() == CSVGLengthValue::Type::PX)
    os << l.ivalue() << "px";
  else if (l.type() == CSVGLengthValue::Type::PERCENT)
    os << 100*l.ivalue() << "%";
  else
    os << l.value();
}

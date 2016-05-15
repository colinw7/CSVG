#include <CSVGObject.h>
#include <CSVGFilter.h>
#include <CSVGStop.h>
#include <CSVGMask.h>
#include <CSVGClipPath.h>
#include <CSVGBuffer.h>
#include <CSVG.h>
#include <CSVGLog.h>
#include <CSVGTempStroke.h>
#include <CSVGTempFont.h>
#include <CSVGUtil.h>

#include <CFontMgr.h>
#include <CEncode64.h>
#include <CRegExp.h>
#include <CFileUtil.h>

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
  animation_.setParent(const_cast<CSVGObject *>(this));

  transform_.reset();
}

CSVGObject::
CSVGObject(const CSVGObject &obj) :
 svg_           (obj.svg_),
 id_            (obj.id_),
 classes_       (obj.classes_),
 parent_        (obj.parent_),
 ind_           (nextInd()),
 opacity_       (obj.opacity_),
 text_          (obj.text_),
 stroke_        (obj.stroke_),
 fill_          (obj.fill_),
 visibility_    (obj.visibility_),
 clip_          (obj.clip_),
 fontDef_       (obj.fontDef_),
 textAnchor_    (obj.textAnchor_),
 textDecoration_(obj.textDecoration_),
 letterSpacing_ (obj.letterSpacing_),
 wordSpacing_   (obj.wordSpacing_),
 nameValues_    (obj.nameValues_),
 transform_     (obj.transform_),
 objects_       (),
 animation_     (),
 filter_        (obj.filter_),
 filtered_      (obj.filtered_),
 mask_          (obj.mask_),
 masked_        (obj.masked_),
 clipPath_      (obj.clipPath_),
 clipped_       (obj.clipped_),
 marker_        (obj.marker_),
 viewBox_       (obj.viewBox_),
 selected_      (obj.selected_),
 inside_        (obj.inside_),
 xmlTag_        (obj.xmlTag_)
{
  animation_.setParent(const_cast<CSVGObject *>(this));

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

std::string
CSVGObject::
getText() const
{
  return text_.getValue("");
}

void
CSVGObject::
setText(const std::string &str)
{
  // if (! hasText())
  //   std::cerr << "Option does not support text '" + str + "'" << std::endl;

  std::string str1 = CStrUtil::stripSpaces(str);

  if (str1.empty()) return;

  text_ = str1;
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

bool
CSVGObject::
getFlatStrokeNoColor() const
{
  // if no color set use it
  if (stroke_.getNoColorValid())
    return stroke_.getNoColor();

  // if has stroke color then no color is false
  if (stroke_.getColorValid())
    return false;

  CSVGObject *parent = getParent();

  if (parent)
    return parent->getFlatStrokeNoColor();
  else
    return true; // default to no color (no stroke)
}

CRGBA
CSVGObject::
getFlatStrokeColor() const
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

    if (svg_.getStyleStrokeColor(this, rgba))
      color.setValue(rgba);
    else
      color.setValue(CRGBA(0,0,0));
  }

  return color.getValue();
}

double
CSVGObject::
getFlatStrokeOpacity() const
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
    double opacity1 = 1.0;

    if (svg_.getStyleStrokeOpacity(this, opacity1))
      opacity.setValue(opacity1);
    else
      opacity.setValue(1.0);
  }

  return opacity.getValue();
}

double
CSVGObject::
getFlatStrokeWidth() const
{
  if (stroke_.getWidthValid())
    return stroke_.getWidth();

  CSVGObject *parent = getParent();

  while (parent) {
    if (parent->stroke_.getWidthValid())
      return parent->getFlatStrokeWidth();

    parent = parent->getParent();
  }

  double width = 1.0;

  if (svg_.getStyleStrokeWidth(this, width))
    return width;

  return 1.0;
}

const CLineDash &
CSVGObject::
getFlatStrokeLineDash() const
{
  if (stroke_.getDashValid())
    return stroke_.getDash();

  CSVGObject *parent = getParent();

  while (parent) {
    if (parent->stroke_.getDashValid())
      return parent->getFlatStrokeLineDash();

    parent = parent->getParent();
  }

  static CLineDash dash;

  if (svg_.getStyleStrokeDash(this, dash))
    return dash;

  return dash;
}

bool
CSVGObject::
getFlatFillNoColor() const
{
  // if no color set use it
  if (fill_.getNoColorValid())
    return fill_.getNoColor();

  // if has fill color then no color is false
  if (fill_.getColorValid())
    return false;

  CSVGObject *parent = getParent();

  if (parent)
    return parent->getFlatFillNoColor();
  else
    return false; // default false (filled)
}

CRGBA
CSVGObject::
getFlatFillColor() const
{
  if (fill_.getColorValid())
    return fill_.getColor();

  CSVGObject *parent = getParent();

  while (parent) {
    if (parent->fill_.getColorValid())
      return parent->fill_.getColor();

    parent = parent->getParent();
  }

  if (fill_.getDefColorValid())
    return fill_.getDefColor();

  CRGBA rgba(0,0,0);

  if (svg_.getStyleFillColor(this, rgba))
    return rgba;

  return CRGBA(0,0,0);
}

double
CSVGObject::
getFlatFillOpacity() const
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

std::string
CSVGObject::
getFlatFontFamily() const
{
  if (fontDef_.hasFamily())
    return fontDef_.getFamily();

  CSVGObject *parent = getParent();

  while (parent) {
    if (parent->fontDef_.hasFamily())
      return parent->getFlatFontFamily();

    parent = parent->getParent();
  }

  return "serif";
}

CFontStyles
CSVGObject::
getFlatFontStyle() const
{
  if (fontDef_.hasStyle())
    return fontDef_.getStyle();

  CSVGObject *parent = getParent();

  while (parent) {
    if (parent->fontDef_.hasStyle())
      return parent->getFlatFontStyle();

    parent = parent->getParent();
  }

  return CFONT_STYLE_NORMAL;
}

CScreenUnits
CSVGObject::
getFlatFontSize() const
{
  if (fontDef_.hasSize())
    return fontDef_.getSize();

  CSVGObject *parent = getParent();

  while (parent) {
    if (parent->fontDef_.hasSize())
      return parent->getFlatFontSize();

    parent = parent->getParent();
  }

  return CScreenUnits(12);
}

CFontPtr
CSVGObject::
getFont() const
{
  std::string fontFamily = getFlatFontFamily();
  CFontStyles fontStyles = getFlatFontStyle();
  CFontStyle  fontStyle  = (fontStyles | CFONT_STYLE_FULL_SIZE).value();
  double      fontSize   = getFlatFontSize().px().value();

  // want full size font
  return CFontMgrInst->lookupFont(fontFamily, fontStyle, fontSize);
}

void
CSVGObject::
setFont(CFontPtr f)
{
  setFontFamily(f->getFamily());
  setFontStyle (f->getStyle());
  setFontSize  (f->getSize());
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
setFontSize(const CScreenUnits &size)
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

void
CSVGObject::
setFontStyle(CFontStyle s)
{
  fontDef_.setStyle(s);
}

CMatrixStack2D
CSVGObject::
getFlatTransform() const
{
  if (getParent()) {
    CMatrixStack2D matrix(getParent()->getFlatTransform());

    matrix.append(getTransform());

    return matrix;
  }
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

    if (num_words1 >= 2) {
      words1[0] = CStrUtil::stripSpaces(words1[num_words1 - 2]);
      words1[1] = CStrUtil::stripSpaces(words1[num_words1 - 1]);

      if (! processPaintOption      (words1[0], words1[1]) &&
          ! processColorOption      (words1[0], words1[1]) &&
          ! processFontOption       (words1[0], words1[1]) &&
          ! processTextContentOption(words1[0], words1[1]) &&
          ! processOpacityOption    (words1[0], words1[1]) &&
          ! processMarkerOption     (words1[0], words1[1]) &&
          ! processGradientOption   (words1[0], words1[1]) &&
          ! processGraphicsOption   (words1[0], words1[1]) &&
          ! processClipOption       (words1[0], words1[1]) &&
          ! processMaskOption       (words1[0], words1[1]) &&
          ! processTextOption       (words1[0], words1[1]) &&
          ! processFilterOption     (words1[0], words1[1]) &&
          ! processCSSOption        (words1[0], words1[1]))
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

  std::string    str;
  CBBox2D        bbox;
  CMatrixStack2D transform;

  // Other properties
  if      (svg_.transformOption(optName, optValue, "transform", transform))
    setTransform(transform);
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
  else if (svg_.stringOption(optName, optValue, "class", str)) {
    std::vector<std::string> classes;

    CStrUtil::addWords(str, classes, " ");

    setClasses(classes);
  }
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
    nameValues_["requiredExtensions"] = str;
  else if (svg_.stringOption(optName, optValue, "systemLanguage", str))
    nameValues_["systemLanguage"] = str;
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
  else if (svg_.stringOption(optName, optValue, "class", str)) { // duplicate ?
    std::vector<std::string> classes;

    CStrUtil::addWords(str, classes, " ");

    setClasses(classes);
  }
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
    nameValues_["overflow"] = str;
  else if (svg_.stringOption(optName, optValue, "visibility", str))
    setVisibility(str); // visible | hidden | inherit
  else if (svg_.stringOption(optName, optValue, "display", str))
    nameValues_["display"] = str;
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
    nameValues_["color"] = str; // fill and stroke color ?
  else if (svg_.stringOption(optName, optValue, "color-interpolation", str))
    nameValues_["color-interpolation"] = str;
  else if (svg_.stringOption(optName, optValue, "color-rendering", str))
    nameValues_["color-rendering"] = str;
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
    nameValues_["color-profile"] = str;
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
    nameValues_["color-interpolation-filters"] = str;
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
    // inline | block | list-item | run-in | compact | marker | table | inline-table |
    // table-row-group | table-header-group | table-footer-group | table-row |
    // table-column-group | table-column | table-cell | table-caption | none | inherit
    notHandled(optName, optValue);
  else if (svg_.stringOption(optName, optValue, "image-rendering", str))
    // auto | optimizeSpeed | optimizeQuality | inherit
    nameValues_["image-rendering"] = str;
  else if (svg_.stringOption(optName, optValue, "pointer-events", str))
    // visiblePainted | visibleFill | visibleStroke | visible | painted | fill |
    // stroke | all | none | inherit
    nameValues_["pointer-events"] = str;
  else if (svg_.stringOption(optName, optValue, "shape-rendering", str))
    // auto | optimizeSpeed | crispEdges | geometricPrecision | inherit
    nameValues_["shape-rendering"] = str;
  else if (svg_.stringOption(optName, optValue, "text-renderer", str))
    // auto | optimizeSpeed | optimizeLegibility | geometricPrecision | inherit
    nameValues_["text-renderer"] = str;
  else if (svg_.stringOption(optName, optValue, "visibility", str))
    setVisibility(str); // visible | hidden | inherit
  else
    return false;

  return true;
}

bool
CSVGObject::
processMarkerOption(const std::string &optName, const std::string &optValue)
{
  std::string str;
  CSVGObject* obj;

  // Marker
  if      (svg_.urlOption(optName, optValue, "marker", &obj)) {
    setMarkerStart(obj);
    setMarkerMid  (obj);
    setMarkerEnd  (obj);
  }
  else if (svg_.urlOption(optName, optValue, "marker-start", &obj))
    setMarkerStart(obj);
  else if (svg_.urlOption(optName, optValue, "marker-mid", &obj))
    setMarkerMid(obj);
  else if (svg_.urlOption(optName, optValue, "marker-end", &obj))
    setMarkerEnd(obj);
  else
    return false;

  return true;
}

bool
CSVGObject::
processClipOption(const std::string &optName, const std::string &optValue)
{
  std::string str;
  CSVGObject* obj;

  if      (svg_.urlOption(optName, optValue, "clip-path", &obj))
    clipPath_ = dynamic_cast<CSVGClipPath *>(obj);
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
    nameValues_["flood-color"] = str;
  else if (svg_.stringOption(optName, optValue, "flood-opacity", str))
    nameValues_["flood-opacity"] = str;
  else if (svg_.stringOption(optName, optValue, "lighting-color", str))
    nameValues_["lighting-color"] = str;
  else
    return false;

  return true;
}

bool
CSVGObject::
processMaskOption(const std::string &optName, const std::string &optValue)
{
  CSVGObject *obj;

  if (svg_.urlOption(optName, optValue, "mask", &obj))
    mask_ = dynamic_cast<CSVGMask *>(obj);
  else
    return false;

  return true;
}

bool
CSVGObject::
processFilterOption(const std::string &optName, const std::string &optValue)
{
  CSVGObject *obj;

  // Filter Attributes
  if (svg_.urlOption(optName, optValue, "filter", &obj))
    filter_ = dynamic_cast<CSVGFilter *>(obj);
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

    if (stop)
      stop->processOption(optName, optValue);
    else
      notHandled(optName, optValue);
  }
  else if (svg_.stringOption(optName, optValue, "stop-opacity", str)) {
    CSVGStop *stop = dynamic_cast<CSVGStop *>(this);

    if (stop)
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
    nameValues_["enable-background"] = str;
  else
    return false;

  return true;
}

bool
CSVGObject::
processGraphicalEventsOption(const std::string &optName, const std::string &optValue)
{
  std::string str;

  if      (svg_.stringOption(optName, optValue, "onfocusin"  , str))
    notHandled(optName, optValue);
  else if (svg_.stringOption(optName, optValue, "onfocusout" , str))
    notHandled(optName, optValue);
  else if (svg_.stringOption(optName, optValue, "onactivate" , str))
    notHandled(optName, optValue);
  else if (svg_.stringOption(optName, optValue, "onclick"    , str))
    notHandled(optName, optValue);
  else if (svg_.stringOption(optName, optValue, "onmousedown", str))
    notHandled(optName, optValue);
  else if (svg_.stringOption(optName, optValue, "onmouseup"  , str))
    notHandled(optName, optValue);
  else if (svg_.stringOption(optName, optValue, "onmouseover", str))
    notHandled(optName, optValue);
  else if (svg_.stringOption(optName, optValue, "onmousemove", str))
    notHandled(optName, optValue);
  else if (svg_.stringOption(optName, optValue, "onmouseout" , str))
    notHandled(optName, optValue);
  else if (svg_.stringOption(optName, optValue, "onload"     , str))
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
  std::string  str;
  CScreenUnits length;

  if      (svg_.stringOption(optName, optValue, "font", str))
    notHandled(optName, optValue); //setFont(str)

  // Font properties
  if      (svg_.stringOption(optName, optValue, "font-family", str))
    setFontFamily(str);
  else if (svg_.lengthOption(optName, optValue, "font-size", length))
    setFontSize(length);
  else if (svg_.stringOption(optName, optValue, "font-size-adjust", str))
    nameValues_["font-size-adjust"] = str;
  else if (svg_.stringOption(optName, optValue, "font-stretch", str))
    nameValues_["font-stretch"] = str;
  else if (svg_.stringOption(optName, optValue, "font-style", str))
    setFontStyle(str);
  else if (svg_.stringOption(optName, optValue, "font-variant", str))
    nameValues_["font-variant"] = str;
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
    setWritingMode(str);
  else
    return false;

  return true;
}

bool
CSVGObject::
processTextContentOption(const std::string &optName, const std::string &optValue)
{
  std::string  str;
  CScreenUnits length;

  // Text content propeties
  if      (svg_.stringOption(optName, optValue, "alignment-baseline", str))
    nameValues_["alignment-baseline"] = str;
  else if (svg_.stringOption(optName, optValue, "baseline-shift", str))
    setTextBaselineShift(str);
  else if (svg_.stringOption(optName, optValue, "dominant-baseline", str))
    nameValues_["dominant-baseline"] = str;
  else if (svg_.stringOption(optName, optValue, "glyph-orientation-horizontal", str))
    setHGlyphOrient(str);
  else if (svg_.stringOption(optName, optValue, "glyph-orientation-vertical", str))
    setVGlyphOrient(str);
  else if (svg_.stringOption(optName, optValue, "kerning", str))
    nameValues_["kerning"] = str;
  else if (svg_.stringOption(optName, optValue, "text-anchor", str))
    setTextAnchor(str);

  else if (svg_.stringOption(optName, optValue, "direction", str))
    notHandled(optName, optValue);
  else if (svg_.lengthOption(optName, optValue, "letter-spacing", length))
    letterSpacing_ = length;
  else if (svg_.stringOption(optName, optValue, "text-decoration", str))
    setTextDecoration(str);
  else if (svg_.stringOption(optName, optValue, "unicode-bidi", str))
    nameValues_["unicode-bidi"] = str;
  else if (svg_.lengthOption(optName, optValue, "word-spacing", length))
    wordSpacing_ = length;
  else
    return false;

  return true;
}

bool
CSVGObject::
processCSSOption(const std::string &optName, const std::string &optValue)
{
  std::string str;

  if (svg_.stringOption(optName, optValue, "background-color", str))
    nameValues_["background-color"] = str;
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

bool
CSVGObject::
interpValue(const std::string &name, const std::string &from, const std::string &to,
            double x, std::string &ystr) const
{
  if (name == "fill") {
    CRGBA fromColor;

    if (! svg_.decodeColorString(from, fromColor))
      return false;

    CRGBA toColor;

    if (! svg_.decodeColorString(to, toColor))
      return false;

    CRGBA c = fromColor*(1 - x) + toColor*x;

    ystr = CStrUtil::strprintf("rgb(%d,%d,%d)", c.getRedI(), c.getGreenI(), c.getBlueI());

    return true;
  }
  else {
    double fromVal = 0;
    uint   fromPos = 0;

    if (! CStrUtil::readReal(from, &fromPos, &fromVal))
      return false;

    double toVal = 0;
    uint   toPos = 0;

    if (! CStrUtil::readReal(to, &toPos, &toVal))
      return false;

    double y = CSVGUtil::map(x, 0, 1, fromVal, toVal);

    ystr = std::to_string(y);

    return true;
  }
}

void
CSVGObject::
setTextBaselineShift(const std::string &str)
{
  nameValues_["baseline-shift"] = str;

  if (str == "super")
    fontDef_.setSuperscript(true);
  else if (str == "sub")
    fontDef_.setSubscript(true);
}

void
CSVGObject::
setTextAnchor(const std::string &str)
{
  if      (str == "start" ) textAnchor_ = CHALIGN_TYPE_LEFT;
  else if (str == "middle") textAnchor_ = CHALIGN_TYPE_CENTER;
  else if (str == "end"   ) textAnchor_ = CHALIGN_TYPE_RIGHT;
  else                      notHandled("text-anchor", str);
}

CHAlignType
CSVGObject::
getFlatTextAnchor() const
{
  if (textAnchor_.isValid())
    return textAnchor_.getValue();

  CSVGObject *parent = getParent();

  while (parent) {
    if (parent->textAnchor_.isValid())
      return parent->getFlatTextAnchor();

    parent = parent->getParent();
  }

  return CHALIGN_TYPE_LEFT;
}

void
CSVGObject::
setTextDecoration(const std::string &str)
{
  if      (str == "underline") {
    textDecoration_ = CSVGTextDecoration::UNDERLINE;

    fontDef_.setUnderline(true);
  }
  else if (str == "overline") {
    textDecoration_ = CSVGTextDecoration::OVERLINE;

    fontDef_.setOverline(true);
  }
  else if (str == "line-through") {
    textDecoration_ = CSVGTextDecoration::LINE_THROUGH;

    fontDef_.setLineThrough(true);
  }
  else
    notHandled("text-decoration", str);
}

void
CSVGObject::
addChildObject(CSVGObject *object)
{
  object->setParent(this);

  if (object->isAnimated())
    animation_.addObject(object);
  else
    objects_.push_back(object);
}

void
CSVGObject::
deleteChildObject(CSVGObject *object)
{
  object->setParent(0);

  if (object->isAnimated())
    animation_.removeObject(object);
  else
    objects_.remove(object);
}

bool
CSVGObject::
getAllChildren(std::vector<CSVGObject *> &objects)
{
  for (const auto &c : children())
    objects.push_back(c);

  for (const auto &c : children())
    c->getAllChildren(objects);

  return ! objects.empty();
}

bool
CSVGObject::
getAllChildrenOfType(CSVGObjTypeId id, std::vector<CSVGObject *> &objects)
{
  getChildrenOfType(id, objects);

  for (const auto &c : children())
    c->getAllChildrenOfType(id, objects);

  return ! objects.empty();
}

bool
CSVGObject::
getChildrenOfType(CSVGObjTypeId id, std::vector<CSVGObject *> &objects)
{
  for (const auto &c : children())
    if (c->isObjType(id))
      objects.push_back(c);

  return ! objects.empty();
}

bool
CSVGObject::
getAllChildrenOfId(const std::string &id, std::vector<CSVGObject *> &objects)
{
  getChildrenOfId(id, objects);

  for (const auto &c : children())
    c->getAllChildrenOfId(id, objects);

  return ! objects.empty();
}

bool
CSVGObject::
getChildrenOfId(const std::string &id, std::vector<CSVGObject *> &objects)
{
  for (const auto &c : children())
    if (c->getId() == id)
      objects.push_back(c);

  return ! objects.empty();
}

bool
CSVGObject::
drawObject()
{
  if (! isHierDrawable())
    return false;

  bool drawn = false;

  //------

  // get current buffer
  CSVGBuffer *oldBuffer     = svg_.getBuffer();
  CSVGBuffer *currentBuffer = oldBuffer;

  //------

  // save object image to temporary buffer if debug or filter
  bool saveImage = false;

  if (svg_.getDebugObjImage()) {
    saveImage = true;

    if (! isDrawable() && ! hasChildren())
      saveImage = false;
  }

  bool isFiltered = (getFilter() && getFiltered());

  if (isFiltered)
    saveImage = true;

  bool isMasked = (getMask() && getMasked());

  if (isMasked)
    saveImage = true;

  CSVGBuffer *saveBuffer = 0;

  //---

  // set buffer to temporary buffer
  if (saveImage) {
    saveBuffer = svg_.getBuffer("_" + getUniqueName());

    svg_.setBuffer(saveBuffer);

    saveBuffer->clear();

    svg_.beginDrawBuffer(saveBuffer);

    currentBuffer = saveBuffer;
  }

  //------

  // set current transform (if no filter)
  CMatrixStack2D transform = oldBuffer->transform();

  CMatrixStack2D transform1(transform);

  transform1.append(getTransform());

  if (! isFiltered)
    currentBuffer->setTransform(transform1);

  //------

  // draw object
  if (drawSubObject())
    drawn = true;

  //------

  // restore transform
  if (! isFiltered)
    currentBuffer->setTransform(transform);

  //------

  // if object image saved to temporary buffer add to old buffer
  if (saveImage) {
    svg_.endDrawBuffer(saveBuffer);

    //---

    if (drawn) {
      CImagePtr image = saveBuffer->getImage();

      if (isFiltered) {
        double x = 0, y = 0;

        transform1.multiplyPoint(0, 0, &x, &y);

        saveBuffer->setOrigin(CPoint2D(x, y));

        double px, py;

        oldBuffer->lengthToPixel(x, y, &px, &py);

        oldBuffer->addImage(px, py, image);
      }
      else
        oldBuffer->addImage(image);
    }

    //---

    svg_.setBuffer(oldBuffer);
  }

  return drawn;
}

bool
CSVGObject::
drawSubObject(bool forceDraw)
{
  if (! isVisible())
    return false;

  bool drawn = false;

  //------

  CSVGBuffer *oldBuffer = svg_.getBuffer();

  // set stroke
  CSVGTempStroke tempStroke(*this);
  CSVGTempFont   tempFont  (*this);

  //------

  drawInit();

  //------

  // draw clip path if specified
  bool isClipped = (getClipPath() && getClipped());

  if (isClipped)
    getClipPath()->drawPath(this);

  //------

  // draw object (virtual)
  if (isDrawable() || isFilter()) {
    svg_.setDrawObject(this);

    draw();

    drawn = true;

    svg_.setDrawObject(0);
  }

  //------

  // draw children
  for (const auto &c : children()) {
    if (! forceDraw && ! c->isDrawable())
      continue;

    if (c->drawObject())
      drawn = true;
  }

  //------

  // apply filter
  bool isFiltered = (getFilter() && getFiltered());

  if (isFiltered) {
    // store current buffer image into SourceGraphic
    CSVGBuffer *srcBuffer = svg_.getBuffer("SourceGraphic");

    srcBuffer->setImage(oldBuffer->getImage());

    //bool srcDrawing = srcBuffer->isDrawing();
    // stop drawing to SourceGraphic if needed
    //if (srcDrawing) srcBuffer->stopDraw();

    bool oldDrawing = oldBuffer->isDrawing();
    if (oldDrawing) oldBuffer->stopDraw();

    if (svg_.getDebugFilter()) {
      // save current image into filter in
      CSVGBuffer *filterInBuffer = svg_.getBuffer(oldBuffer->getName() + "_filter_in");

      filterInBuffer->setImage(srcBuffer->getImage());
    }

    //--

    // apply filter
    filter_->setObject(this);

    if (filter_->drawSubObject(true))
      drawn = true;

    //--

    if (svg_.getDebugFilter()) {
      // save SourceGraphic (output of filter) into filter_out
      CSVGBuffer *filterOutBuffer = svg_.getBuffer(oldBuffer->getName() + "_filter_out");

      filterOutBuffer->setImage(srcBuffer->getImage());
    }

    //---

    oldBuffer->setImage(srcBuffer->getImage());

    // restart drawing to SourceGraphic if needed
    //if (srcDrawing) srcBuffer->startDraw();

    if (oldDrawing) oldBuffer->startDraw();
  }

  //---

  if (isClipped)
    oldBuffer->initClip();

  //---

  // mask if defined
  bool isMasked = (getMask() && getMasked());

  if (isMasked)
    getMask()->drawMask(this);

  //------

  drawTerm();

  return drawn;
}

bool
CSVGObject::
isVisible() const
{
  if (visibility_.isValid()) {
    if (visibility_.getValue() == "hidden")
      return false;
  }

  return true;
}

void
CSVGObject::
setVisible(bool b)
{
  if (! b)
    visibility_ = "hidden";
  else
    visibility_.setInvalid();
}

CImagePtr
CSVGObject::
toImage()
{
  CSVGBuffer *imageBuffer = toBufferImage();

  if (! imageBuffer)
    return CImagePtr();

  return imageBuffer->getImage();
}

CImagePtr
CSVGObject::
toNamedImage(const std::string &bufferName)
{
  CSVGBuffer *buffer = toNamedBufferImage(bufferName);

  if (! buffer)
    return CImagePtr();

  return buffer->getImage();
}

CSVGBuffer *
CSVGObject::
toBufferImage()
{
  // TODO: use toNamedBufferImage, optional filter ?
  CBBox2D bbox;

  if (! getBBox(bbox))
    return 0;

  CSVGBuffer *oldBuffer   = svg_.getBuffer();
  CSVGBuffer *imageBuffer = svg_.getBuffer("_" + getUniqueName() + "_image");

  svg_.setBuffer(imageBuffer);

  imageBuffer->clear();

  // draw object to buffer
  svg_.beginDrawBuffer(imageBuffer, bbox);

  //buffer->setup(bbox);

  (void) drawSubObject(true);

  svg_.endDrawBuffer(imageBuffer);

  svg_.setBuffer(oldBuffer);

  return imageBuffer;
}

CSVGBuffer *
CSVGObject::
toNamedBufferImage(const std::string &bufferName)
{
  CBBox2D bbox;

  if (! getBBox(bbox))
    return 0;

  CSVGFilter *saveFilter { 0 };

  std::swap(saveFilter, filter_);

  CSVGBuffer *oldBuffer   = svg_.getBuffer();
  CSVGBuffer *imageBuffer = svg_.getBuffer(bufferName);

  svg_.setBuffer(imageBuffer);

  imageBuffer->clear();

  svg_.beginDrawBuffer(imageBuffer, bbox);

  (void) drawSubObject(true);

  svg_.endDrawBuffer(imageBuffer);

  svg_.setBuffer(oldBuffer);

  std::swap(saveFilter, filter_);

  return imageBuffer;
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
  CMatrixStack2D m;

  m.rotate(da);

  m.append(getTransform());

  setTransform(m);
}

void
CSVGObject::
rotateTo(double a, const CPoint2D &c)
{
  CMatrixStack2D m;

  m.rotate(a, c);

  m.append(getTransform());

  setTransform(m);
}

void
CSVGObject::
scaleTo(double xs, double ys)
{
  CMatrixStack2D m;

  m.scale(xs, ys);

  setTransform(m);
}

bool
CSVGObject::
getBBox(CBBox2D &bbox) const
{
  if (! viewBox_.isValid()) {
    bbox = CBBox2D();

    CBBox2D bbox1;

    if (getChildrenBBox(bbox1))
      bbox += bbox1;
  }
  else
    bbox = getViewBox();

  return bbox.isSet();
}

bool
CSVGObject::
getTransformedBBox(CBBox2D &bbox) const
{
  CMatrixStack2D m = getTransform();

  if (! getBBox(bbox))
    return false;

  bbox = transformBBox(m, bbox);

  return true;
}

bool
CSVGObject::
getFlatTransformedBBox(CBBox2D &bbox) const
{
  CMatrixStack2D m = getFlatTransform();

  if (! getBBox(bbox))
    return false;

  bbox = transformBBox(m, bbox);

  return true;
}

CBBox2D
CSVGObject::
transformBBox(const CMatrixStack2D &m, const CBBox2D &bbox) const
{
  CPoint2D p1, p2, p3, p4;

  m.multiplyPoint(bbox.getLL(), p1);
  m.multiplyPoint(bbox.getLR(), p2);
  m.multiplyPoint(bbox.getUL(), p3);
  m.multiplyPoint(bbox.getUR(), p4);

  CBBox2D bbox1(p1, p2);

  bbox1 += p3;
  bbox1 += p4;

  return bbox1;
}

bool
CSVGObject::
inside(const CPoint2D &pos) const
{
  CBBox2D bbox;

  if (! getTransformedBBox(bbox))
    return false;

  return bbox.inside(pos);
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

  for (const auto &c : children()) {
    if (c->getTransformedBBox(bbox1))
      bbox.add(bbox1);
  }

  return bbox.isSet();
}

void
CSVGObject::
tick(double dt)
{
  for (const auto &c : children())
    c->tick(dt);

  animation_.tick(dt);
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
setClasses(const std::vector<std::string> &classes)
{
  classes_ = classes;
}

bool
CSVGObject::
decodeXLink(const std::string &str, CSVGObject **object, CImagePtr *image)
{
  if (object)
    *object = 0;

  //---

  // check for inline image data
  uint len = str.size();

  if (len >= 5 && str.substr(0, 5) == "data:") {
    // get format
    uint pos = 5;

    std::string format;

    while (pos < len && str[pos] != ',')
      format += str[pos++];

    if (pos >= len) {
      // error ?
      return false;
    }

    ++pos;

    if (format == "image/png;base64") {
      // image string to buffer
      std::string str1 = CEncode64Inst->decode(str.substr(pos));

      CFile file(".svg.png");

      file.write(str1);

      file.flush();

      file.close();

      CImageFileSrc src(".svg.png");

      CImagePtr image1 = CImageMgrInst->createImage(src);

      CSVGBuffer *imgBuffer = svg_.getBuffer(getUniqueName() + "_xlink");

      imgBuffer->setImage(image1);

      if (image)
        *image = image1;

      return true;
    }
    else {
      std::cerr << "Unhandled images format '" + format + "'" << std::endl;
      return false;
    }
  }

  //---

  // get link name
  std::string::size_type pos = str.find('#');

  if (pos != std::string::npos) {
    CSVGObject *object1 = 0;

    std::string lhs = str.substr(0, pos);
    std::string rhs = str.substr(pos + 1);

    // Handle file: <filename>#<id>
    if (lhs != "") {
      // read file
      CSVGBlock *block = svg_.createBlock();

      svg_.read(lhs, block);

      // get object from id
      if (rhs != "")
        object1 = svg_.lookupObjectById(rhs);
      else
        object1 = block;
    }
    // handle id: <id>
    else {
      object1 = svg_.lookupObjectById(rhs);
    }

    if (! object1) {
      CSVGLog() << "Object " << rhs << " does not exist";
      return false;
    }

    if (object)
      *object = object1;
  }
  // handle file
  else {
    // check valid
    CFile file(str);

    if (! file.exists()) {
      CSVGLog() << "File " << str << " does not exist";
      return false;
    }

    if (! file.isRegular()) {
      CSVGLog() << "File " << str << " is not a regular file";
      return false;
    }

    // get type from contents/suffix
    CFileType type = CFileUtil::getType(&file);

    if (type == CFILE_TYPE_INODE_REG && file.getSuffix() == "svg")
      type = CFILE_TYPE_IMAGE_SVG;

    // handle image file
    if      (type & CFILE_TYPE_IMAGE) {
      CImagePtr image1;

      // svg image
      if (type == CFILE_TYPE_IMAGE_SVG) {
        // draw SVG file to image at current scale
        CImageNameSrc src(file.getPath());

        image1 = CImageMgrInst->createImage(src);

        CSVG svg;

        svg.setRenderer(svg_.getRenderer());

        svg.init();

        svg.read(file.getPath());

        CSize2D size;

        getSize(size);

        svg.getBlock()->setSize(size);

        int w = svg.getIWidth();
        int h = svg.getIHeight();

        image1 = svg.drawToImage(w, h, CPoint2D(0, 0), svg_.xscale(), svg_.yscale());
      }
      // image file (png, jpeg, ...)
      else {
        CImageFileSrc src(file);

        image1 = CImageMgrInst->createImage(src);

        image1->read(str);
      }

      if (image1.isValid()) {
        CSVGBuffer *imgBuffer = svg_.getBuffer(getUniqueName() + "_xlink");

        imgBuffer->setImage(image1);
      }

      if (image)
        *image = image1;
    }
    // handle svg file
    else if (type == CFILE_TYPE_TEXT_HTML) {
      // read svg file
      CSVGBlock *block = svg_.createBlock();

      svg_.read(str, block);

      if (object)
        *object = block;
    }
    else {
      CSVGLog() << "Unknown type of file for " << str;
      return false;
    }
  }

  return true;
}

bool
CSVGObject::
getObjectsAtPoint(const CPoint2D &p, ObjectArray &objects) const
{
  if (inside(p))
    objects.push_back(const_cast<CSVGObject *>(this));

  for (const auto &c : children())
    c->getObjectsAtPoint(p, objects);

  return ! objects.empty();
}

void
CSVGObject::
handleEvent(CSVGEventType type, const std::string &id, const std::string &data)
{
  for (const auto &c : children())
    c->handleEvent(type, id, data);

  if (! animation_.objects().empty())
    animation_.handleEvent(type, id, data);
}

void
CSVGObject::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    std::string s = getText();

    os << "<" << getObjName();

    printValues(os);

    if (hasChildren()) {
      os << ">" << std::endl;

      if (s != "")
        os << s << std::endl;

      printChildren(os, hier);

      os << "</" << getObjName() << ">" << std::endl;
    }
    else if (s != "") {
      os << ">" << s;
      os << "</" << getObjName() << ">" << std::endl;
    }
    else
      os << "/>" << std::endl;
  }
  else
    os << getObjName() << ": '" << getId() << "'";
}

void
CSVGObject::
printValues(std::ostream &os) const
{
  printNameValue (os, "id"   , id_     );
  printNameValues(os, "class", classes_);

  if (viewBox_.isValid()) {
    CBBox2D viewBox = getViewBox();

    os << " viewBox=\"" << viewBox.getXMin() << " " << viewBox.getYMin() <<
                    " " << viewBox.getXMax() << " " << viewBox.getYMax() << "\"";
  }

  printTransform(os);

  printFilter(os);

  printStyle(os);

  printTextContent(os);

  for (const auto &nv : nameValues_)
    os << " " << nv.first << "=\"" << nv.second << "\"";
}

bool
CSVGObject::
hasChildren(bool includeAnimated) const
{
  if (includeAnimated)
    return (! children().empty() || ! animation_.objects().empty());
  else
    return ! children().empty();
}

void
CSVGObject::
printChildren(std::ostream &os, bool hier) const
{
  for (const auto &c : children())
    c->print(os, hier);

  for (const auto &o : animation_.objects())
    o->print(os, hier);
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
  if (! fontDef_.isSet() && ! stroke_.isSet() && ! fill_.isSet() &&
      ! letterSpacing_.isValid() && ! wordSpacing_.isValid())
    return;

  os << " style=\"";

  bool output = false;

  if (fontDef_.isSet()) {
    printFontDef(os);

    output = true;
  }

  if (stroke_.isSet()) {
    if (output) os << " ";

    stroke_.print(os);

    output = true;
  }

  if (fill_.isSet()) {
    if (output) os << " ";

    fill_.print(os);

    output = true;
  }

  if (letterSpacing_.isValid()) {
    if (output) os << " ";

    os << "letter-spacing: "; printLength(os, letterSpacing_.getValue());

    output = true;
  }

  if (wordSpacing_.isValid()) {
    if (output) os << " ";

    os << "word-spacing: "; printLength(os, wordSpacing_.getValue());

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
  if (! transform_.isEmpty()) {
    os << " transform=\"";

    printTransform(os, transform_);

    os << "\"";
  }
}

void
CSVGObject::
printTransform(std::ostream &os, const CMatrixStack2D &m) const
{
  if (m.isEmpty())
    return;

  bool output = false;

  for (const auto &t : m.transformStack()) {
    switch (t.type()) {
      case CMatrixTransformType::TRANSLATE: {
        if (output) os << " ";

        os << "translate(" << t.dx() << " " << t.dy() << ")";

        output = true;

        break;
      }
      case CMatrixTransformType::SCALE1:
      case CMatrixTransformType::SCALE2: {
        if (output) os << " ";

        if (t.type() == CMatrixTransformType::SCALE1)
          os << "scale(" << t.xscale() << ")";
        else
          os << "scale(" << t.xscale() << " " << t.yscale() << ")";

        output = true;

        break;
      }
      case CMatrixTransformType::ROTATE:
      case CMatrixTransformType::ROTATE_ORIGIN: {
        if (output) os << " ";

        if (t.type() == CMatrixTransformType::ROTATE)
          os << "rotate(" << CMathGen::RadToDeg(t.angle()) << ")";
        else
          os << "rotate(" << CMathGen::RadToDeg(t.angle()) << " " <<
                 t.xo() << " " << t.yo() << ")";

        output = true;

        break;
      }
      case CMatrixTransformType::SKEWX: {
        if (output) os << " ";

        os << "skewX(" << CMathGen::RadToDeg(t.angle()) << ")";

        output = true;

        break;
      }
      case CMatrixTransformType::SKEWY: {
        if (output) os << " ";

        os << "skewX(" << CMathGen::RadToDeg(t.angle()) << ")";

        output = true;

        break;
      }
      default: {
        if (output) os << " ";

        os << "matrix(";

        const double *v = t.values();

        for (int i = 0; i < 6; ++i) {
          if (i > 0) os << " ";

          // SVG is ((a c e) (b d f) (0 0 1))
          if (i == 1 || i == 2)
            os << v[3 - i];
          else
            os << v[i];
        }

        output = true;

        break;
      }
    }
  }
}

void
CSVGObject::
printNameParts(std::ostream &os, const std::string &name, const CSVGPathPartList &parts) const
{
  if (! parts.empty()) {
    os << " " << name << "=\"";

    svg_.printParts(os, parts);

    os << "\"";
  }
}

void
CSVGObject::
printLength(std::ostream &os, const CScreenUnits &l)
{
  if      (l.type() == CScreenUnits::Type::EM)
    os << l.value() << "em";
  else if (l.type() == CScreenUnits::Type::EX)
    os << l.value() << "ex";
  else if (l.type() == CScreenUnits::Type::PT)
    os << l.value() << "pt";
  else if (l.type() == CScreenUnits::Type::PC)
    os << l.value() << "pc";
  else if (l.type() == CScreenUnits::Type::CM)
    os << l.value() << "cm";
  else if (l.type() == CScreenUnits::Type::MM)
    os << l.value() << "mm";
  else if (l.type() == CScreenUnits::Type::IN)
    os << l.value() << "in";
  else if (l.type() == CScreenUnits::Type::PX)
    os << l.value() << "px";
  else if (l.type() == CScreenUnits::Type::PERCENT)
    os << l.value() << "%";
  else
    os << l.value();
}

void
CSVGObject::
printTime(std::ostream &os, const CSVGTimeValue &t)
{
  if      (t.type() == CSVGTimeValueType::HOURS)
    os << t.value() << "h";
  else if (t.type() == CSVGTimeValueType::MINUTES)
    os << t.value() << "min";
  else if (t.type() == CSVGTimeValueType::SECONDS)
    os << t.value() << "s";
  else if (t.type() == CSVGTimeValueType::MILLISECONDS)
    os << t.value() << "ms";
  else
    os << t.value();
}

void
CSVGObject::
printEvent(std::ostream &os, const CSVGEventValue &e)
{
  if      (e.type() == CSVGEventType::TIMEOUT)
    printTime(os, e.time());
  else if (e.type() == CSVGEventType::CLICK)
    os << "click";
  else if (e.type() == CSVGEventType::MOUSE_DOWN)
    os << "mousedown";
  else if (e.type() == CSVGEventType::MOUSE_UP)
    os << "mouseup";
  else if (e.type() == CSVGEventType::MOUSE_OVER)
    os << "mouseover";
  else if (e.type() == CSVGEventType::MOUSE_OUT)
    os << "mouseout";
  else if (e.type() == CSVGEventType::ANIMATE_BEGIN)
    os << e.id() << ".begin";
  else if (e.type() == CSVGEventType::ANIMATE_END)
    os << e.id() << ".end";
  else if (e.type() == CSVGEventType::ANIMATE_REPEAT)
    os << e.id() << ".repeat";

  if (e.type() != CSVGEventType::TIMEOUT) {
    if (e.args() != "")
      os << "(" << e.args() << ")";

    if (e.time().type() != CSVGTimeValueType::NONE) {
      os << "+";

      printTime(os, e.time());
    }
  }
}

void
CSVGObject::
printXLink(std::ostream &os, const CSVGXLink &xlink)
{
  os << xlink.str();
}

void
CSVGObject::
printPreserveAspect(std::ostream &os, const CSVGPreserveAspect &a)
{
  if      (a.getHAlign() == CHALIGN_TYPE_LEFT  ) os << "xMin";
  else if (a.getHAlign() == CHALIGN_TYPE_CENTER) os << "xMid";
  else if (a.getHAlign() == CHALIGN_TYPE_RIGHT ) os << "xMax";

  if      (a.getVAlign() == CVALIGN_TYPE_BOTTOM) os << "YMin";
  else if (a.getVAlign() == CVALIGN_TYPE_CENTER) os << "YMid";
  else if (a.getVAlign() == CVALIGN_TYPE_TOP   ) os << "YMax";

  os << " ";

  if      (a.getScale() == CSVGScale::FIXED_MEET ) os << "meet";
  else if (a.getScale() == CSVGScale::FIXED_SLICE) os << "slice";
  else if (a.getScale() == CSVGScale::FREE       ) os << "none";
}

void
CSVGObject::
printNameCoordUnits(std::ostream &os, const std::string &name,
                    const COptValT<CSVGCoordUnits> &units) const
{
  if (units.isValid())
    os << " " << name << "=\"" << CSVG::encodeUnitsString(units.getValue()) << "\"";
}

#include <CSVGObject.h>
#include <CSVGFilter.h>
#include <CSVGGroup.h>
#include <CSVGStop.h>
#include <CSVGMask.h>
#include <CSVGClipPath.h>
#include <CSVGBuffer.h>
#include <CSVG.h>
#include <CSVGLog.h>

#include <CFontMgr.h>
#include <CEncode64.h>
#include <CRegExp.h>
#include <CFileUtil.h>
#include <CStrParse.h>

namespace {

static uint s_ObjectInd;

uint nextInd() {
  ++s_ObjectInd;

  return s_ObjectInd;
}

}

CSVGObject::
CSVGObject(CSVG &svg) :
 svg_         (svg),
 ind_         (nextInd()),
 stroke_      (svg),
 fill_        (svg),
 viewportFill_(svg),
 clip_        (svg),
 fontDef_     (svg)
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
 display_       (obj.display_),
 viewportFill_  (obj.viewportFill_),
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
    auto *child = o->dup();

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

int
CSVGObject::
getDepth() const
{
  return (parent_ ? parent_->getDepth() + 1 : 0);
}

void
CSVGObject::
autoName()
{
  using IdMap = std::map<std::string, int>;

  static IdMap idMap;

  auto typeName = getObjType().getName();

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
  //   std::cerr << "Option does not support text '" + str + "'\n";

  std::string str1 = CStrUtil::stripSpaces(str);

  if (str1.empty()) return;

  text_ = str1;
}

#if 0
void
CSVGObject::
setOpacity(const std::string &opacityDef)
{
  double opacity;
  bool   inherit;

  if (! svg_.decodeOpacityString(opacityDef, opacity, inherit)) {
    CSVGLog() << "Bad opacity value " << opacityDef;
    return;
  }

  setOpacity(opacity);
}
#endif

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

//------

CSVGStroke
CSVGObject::
getFlatStroke() const
{
  CSVGStroke stroke(svg_);

  if (! stroke_.isSet()) {
    auto *parent = getParent();

    if (parent)
      return parent->getFlatStroke();
    else
      return stroke;
  }

  auto color = getFlatStrokeColor();

  if (color.isValid())
    stroke.setColor(color.getValue());
  else
    stroke.resetColor();

  //---

  auto opacity = getFlatStrokeOpacity();

  if (opacity.isValid())
    stroke.setOpacity(opacity.getValue());
  else
    stroke.resetOpacity();

  //---

  auto rule = getFlatStrokeRule();

  if (rule.isValid())
    stroke.setRule(rule.getValue());
  else
    stroke.resetRule();

  //---

  auto url = getFlatStrokeUrl();

  if (url.isValid())
    stroke.setUrl(url.getValue());
  else
    stroke.resetUrl();

  //---

  auto obj = getFlatStrokeFillObject();

  if (obj.isValid())
    stroke.setFillObject(obj.getValue());
  else
    stroke.resetFillObject();

  //---

  auto width = getFlatStrokeWidth();

  if (width.isValid())
    stroke.setWidth(width.getValue());
  else
    stroke.resetWidth();

  //---

  auto dash = getFlatStrokeLineDash();

  if (dash.isValid())
    stroke.setDash(dash.getValue());
  else
    stroke.resetDash();

  //---

  auto lineCap = getFlatStrokeLineCap();

  if (lineCap.isValid())
    stroke.setLineCap(lineCap.getValue());
  else
    stroke.resetLineCap();

  //---

  auto lineJoin = getFlatStrokeLineJoin();

  if (lineJoin.isValid())
    stroke.setLineJoin(lineJoin.getValue());
  else
    stroke.resetLineJoin();

  //---

  auto mlimit = getFlatStrokeMitreLimit();

  if (mlimit.isValid())
    stroke.setMitreLimit(mlimit.getValue());
  else
    stroke.resetMitreLimit();

  return stroke;
}

COptValT<CSVGColor>
CSVGObject::
getFlatStrokeColor() const
{
  // if color set use it
  if (stroke_.getColorValid())
    return COptValT<CSVGColor>(stroke_.getColor());

  auto *parent = getParent();

  while (parent) {
    if (parent->stroke_.getColorValid())
      return COptValT<CSVGColor>(parent->stroke_.getColor());

    parent = parent->getParent();
  }

  return COptValT<CSVGColor>();
}

COptReal
CSVGObject::
getFlatStrokeOpacity() const
{
  // if opacity set use it
  if (stroke_.getOpacityValid())
    return COptReal(stroke_.getOpacity());

  auto *parent = getParent();

  while (parent) {
    if (parent->stroke_.getOpacityValid())
      return COptReal(parent->stroke_.getOpacity());

    parent = parent->getParent();
  }

  return COptReal();
}

COptValT<CFillType>
CSVGObject::
getFlatStrokeRule() const
{
  if (stroke_.getRuleValid())
    return COptValT<CFillType>(stroke_.getRule());

  auto *parent = getParent();

  while (parent) {
    if (parent->stroke_.getRuleValid())
      return COptValT<CFillType>(parent->getFlatStrokeRule());

    parent = parent->getParent();
  }

  return COptValT<CFillType>();
}

COptString
CSVGObject::
getFlatStrokeUrl() const
{
  if (stroke_.getUrlValid())
    return COptString(stroke_.getUrl());

  auto *parent = getParent();

  while (parent) {
    if (parent->stroke_.getUrlValid())
      return parent->getFlatStrokeUrl();

    parent = parent->getParent();
  }

  return COptString();
}

COptValT<CSVGObject*>
CSVGObject::
getFlatStrokeFillObject() const
{
  if (stroke_.getFillObjectValid())
    return COptValT<CSVGObject*>(stroke_.getFillObject());

  auto *parent = getParent();

  while (parent) {
    if (parent->stroke_.getFillObjectValid())
      return COptValT<CSVGObject*>(parent->getFlatStrokeFillObject());

    parent = parent->getParent();
  }

  return COptValT<CSVGObject*>();
}

COptReal
CSVGObject::
getFlatStrokeWidth() const
{
  if (stroke_.getWidthValid())
    return COptReal(stroke_.getWidth());

  auto *parent = getParent();

  while (parent) {
    if (parent->stroke_.getWidthValid())
      return COptReal(parent->getFlatStrokeWidth());

    parent = parent->getParent();
  }

  return COptReal();
}

COptValT<CSVGStrokeDash>
CSVGObject::
getFlatStrokeLineDash() const
{
  if (stroke_.getDashValid())
    return COptValT<CSVGStrokeDash>(stroke_.getDash());

  auto *parent = getParent();

  while (parent) {
    if (parent->stroke_.getDashValid())
      return COptValT<CSVGStrokeDash>(parent->getFlatStrokeLineDash());

    parent = parent->getParent();
  }

  return COptValT<CSVGStrokeDash>();
}

COptValT<CLineCapType>
CSVGObject::
getFlatStrokeLineCap() const
{
  if (stroke_.getLineCapValid())
    return COptValT<CLineCapType>(stroke_.getLineCap());

  auto *parent = getParent();

  while (parent) {
    if (parent->stroke_.getLineCapValid())
      return COptValT<CLineCapType>(parent->getFlatStrokeLineCap());

    parent = parent->getParent();
  }

  return COptValT<CLineCapType>();
}

COptValT<CLineJoinType>
CSVGObject::
getFlatStrokeLineJoin() const
{
  if (stroke_.getLineJoinValid())
    return COptValT<CLineJoinType>(stroke_.getLineJoin());

  auto *parent = getParent();

  while (parent) {
    if (parent->stroke_.getLineJoinValid())
      return COptValT<CLineJoinType>(parent->getFlatStrokeLineJoin());

    parent = parent->getParent();
  }

  return COptValT<CLineJoinType>();
}

COptReal
CSVGObject::
getFlatStrokeMitreLimit() const
{
  if (stroke_.getMitreLimitValid())
    return COptReal(stroke_.getMitreLimit());

  auto *parent = getParent();

  while (parent) {
    if (parent->stroke_.getMitreLimitValid())
      return COptReal(parent->getFlatStrokeMitreLimit());

    parent = parent->getParent();
  }

  return COptReal();
}

//------

CSVGFill
CSVGObject::
getFlatFill() const
{
  CSVGFill fill(svg_);

  auto color = getFlatFillColor();

  if (color.isValid())
    fill.setColor(color.getValue());
  else
    fill.resetColor();

  //---

  auto opacity = getFlatFillOpacity();

  if (opacity.isValid())
    fill.setOpacity(opacity.getValue());
  else
    fill.resetOpacity();

  //---

  auto rule = getFlatFillRule();

  if (rule.isValid())
    fill.setRule(rule.getValue());
  else
    fill.resetRule();

  //---

  auto url = getFlatFillUrl();

  if (url.isValid())
    fill.setUrl(url.getValue());
  else
    fill.resetUrl();

  //---

  auto obj = getFlatFillFillObject();

  if (obj.isValid())
    fill.setFillObject(obj.getValue());
  else
    fill.resetFillObject();

  return fill;
}

COptValT<CSVGColor>
CSVGObject::
getFlatFillColor() const
{
  // if color set use it
  if (fill_.getColorValid())
    return COptValT<CSVGColor>(fill_.getColor());

  auto *parent = getParent();

  while (parent) {
    if (parent->fill_.getColorValid())
      return COptValT<CSVGColor>(parent->fill_.getColor());

    parent = parent->getParent();
  }

  return COptValT<CSVGColor>();
}

COptReal
CSVGObject::
getFlatFillOpacity() const
{
  // if opacity set use it
  if (fill_.getOpacityValid())
    return COptReal(fill_.getOpacity().getValue());

  auto *parent = getParent();

  while (parent) {
    if (parent->fill_.getOpacityValid())
      return COptReal(parent->fill_.getOpacity().getValue());

    parent = parent->getParent();
  }

  return COptReal();
}

COptValT<CFillType>
CSVGObject::
getFlatFillRule() const
{
  // if opacity set use it
  if (fill_.getRuleValid())
    return COptValT<CFillType>(fill_.getRule());

  auto *parent = getParent();

  while (parent) {
    if (parent->fill_.getRuleValid())
      return COptValT<CFillType>(parent->fill_.getRule());

    parent = parent->getParent();
  }

  return COptValT<CFillType>();
}

COptString
CSVGObject::
getFlatFillUrl() const
{
  // if opacity set use it
  if (fill_.getUrlValid())
    return COptString(fill_.getUrl());

  auto *parent = getParent();

  while (parent) {
    if (parent->fill_.getUrlValid())
      return COptString(parent->fill_.getUrl());

    parent = parent->getParent();
  }

  return COptString();
}

COptValT<CSVGObject *>
CSVGObject::
getFlatFillFillObject() const
{
  // if opacity set use it
  if (fill_.getFillObjectValid())
    return COptValT<CSVGObject *>(fill_.getFillObject());

  auto *parent = getParent();

  while (parent) {
    if (parent->fill_.getFillObjectValid())
      return COptValT<CSVGObject *>(parent->fill_.getFillObject());

    parent = parent->getParent();
  }

  return COptValT<CSVGObject *>();
}

CSVGFontDef
CSVGObject::
getFlatFontDef() const
{
  auto fontFamily = getFlatFontFamily();
  auto fontSize   = getFlatFontSize();
  auto fontStyles = getFlatFontStyle();

  CSVGFontDef fontDef(svg_);

  fontDef.setFamily(fontFamily);
  fontDef.setSize  (fontSize);
  fontDef.setStyle (fontStyles);

  return fontDef;
}

std::string
CSVGObject::
getFlatFontFamily() const
{
  if (fontDef_.hasFamily())
    return fontDef_.getFamily();

  auto *parent = getParent();

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

  auto *parent = getParent();

  while (parent) {
    if (parent->fontDef_.hasStyle())
      return parent->getFlatFontStyle();

    parent = parent->getParent();
  }

  return CFontStyles(CFONT_STYLE_NORMAL);
}

CScreenUnits
CSVGObject::
getFlatFontSize() const
{
  if (fontDef_.hasSize())
    return fontDef_.getSize();

  auto *parent = getParent();

  while (parent) {
    if (parent->fontDef_.hasSize())
      return parent->getFlatFontSize();

    parent = parent->getParent();
  }

  return CScreenUnits(12);
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

CRGBA
CSVGObject::
colorToRGBA(const CSVGColor &color) const
{
  if (color.isRGBA())
    return color.rgba();

  if (color.isCurrent())
    return getFlatCurrentColor();

  // inherit, none
  if (color.isInherit()) {
    if (parent_)
      return parent_->colorToRGBA(color);
    else
      return CRGBA(0, 0, 0);
  }

  return CRGBA(0, 0, 0, 0);
}

CRGBA
CSVGObject::
getFlatCurrentColor() const
{
  if (hasCurrentColor()) {
    auto color = currentColor();

    if (color.isRGBA())
      return color.rgba();
  }

  if (parent_)
    return parent_->getFlatCurrentColor();

  return CRGBA(0, 0, 0);
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

CMatrixStack2D
CSVGObject::
getTransformTo(CSVGObject *parent) const
{
  auto *parent1 = getParent();

  if (parent1 && parent != parent1) {
    CMatrixStack2D matrix(parent1->getTransformTo(parent));

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
          ! processCSSOption        (words1[0], words1[1]) &&
          ! processContainerOption  (words1[0], words1[1])) {
        if (! svg_.isQuiet())
          CSVGLog() << "Invalid style option " << words1[0] << ":" << words1[1] <<
                       " for " << getTagName();
      }
    }
    else {
      CSVGLog() << "Invalid style option format " << words1[0] << " for " << getTagName();
    }
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
  if (processContainerOption      (optName, optValue)) return true;
  if (processViewportOption       (optName, optValue)) return true;

  std::string    str;
  CBBox2D        bbox;
  CMatrixStack2D transform;

  // Other properties
  if      (svg_.transformOption(optName, optValue, "transform", transform))
    setTransform(transform);
  else if (svg_.bboxOption(optName, optValue, "viewBox", bbox))
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
    if (! svg_.isQuiet())
      CSVGLog() << "Unhandled option " << optName << " for " << getTagName();
    return true; // don't propagate warning
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
  else if (svg_.stringOption(optName, optValue, "xml:id", str))
    setId(str);
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
  else if (svg_.stringOption(optName, optValue, "requiredFonts", str))
    nameValues_["requiredFonts"] = str;
  else if (svg_.stringOption(optName, optValue, "requiredFormats", str))
    nameValues_["requiredFormats"] = str;
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
    setStrokeDashArray(str);
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
    setOverflow(str);
  else if (svg_.stringOption(optName, optValue, "visibility", str)) // TODO: duplicate
    setVisibility(str); // visible | hidden | collapse | inherit
  else if (svg_.stringOption(optName, optValue, "display", str)) // TODO: duplicate
    setDisplay(str); // inline | block | list-item| run-in | compact | marker
  else
    return false;

  return true;
}

bool
CSVGObject::
processColorOption(const std::string &optName, const std::string &optValue)
{
  std::string str;
  CSVGColor   color;

  // Color Attributes
  if      (svg_.colorOption(optName, optValue, "color", color)) {
    setCurrentColor(color);
  }
  else if (svg_.stringOption(optName, optValue, "color-interpolation", str))
    nameValues_["color-interpolation"] = str;
  else if (svg_.stringOption(optName, optValue, "color-rendering", str)) {
    // auto | optimizeSpeed | optimizeQuality | inherit
    nameValues_["color-rendering"] = str;
  }
  else if (svg_.colorOption(optName, optValue, "solid-color", color)) {
    notHandled(optName, optValue);
  }
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
  double      real;
  bool        inherit;

  // Opacity Attributes
  if      (svg_.opacityOption(optName, optValue, "opacity", real, inherit))
    setOpacity(real);
  else if (svg_.stringOption(optName, optValue, "fill-opacity", str))
    setFillOpacity(str);
  else if (svg_.stringOption(optName, optValue, "stroke-opacity", str))
    setStrokeOpacity(str);
  else if (svg_.opacityOption(optName, optValue, "solid-opacity", real, inherit))
    notHandled(optName, optValue);
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
    setDisplay(str);
  else if (svg_.stringOption(optName, optValue, "image-rendering", str)) {
    // auto | optimizeSpeed | optimizeQuality | inherit
    nameValues_["image-rendering"] = str;
  }
  else if (svg_.stringOption(optName, optValue, "pointer-events", str))
    // visiblePainted | visibleFill | visibleStroke | visible | painted | fill |
    // stroke | all | none | inherit
    nameValues_["pointer-events"] = str;
  else if (svg_.stringOption(optName, optValue, "shape-rendering", str))
    // auto | optimizeSpeed | crispEdges | geometricPrecision | inherit
    nameValues_["shape-rendering"] = str;
  else if (svg_.stringOption(optName, optValue, "text-rendering", str)) // TODO: duplicate
    // auto | optimizeSpeed | optimizeLegibility | geometricPrecision | inherit
    nameValues_["text-rendering"] = str;
  else if (svg_.stringOption(optName, optValue, "buffered-rendering", str))
    // auto | dynamic | static | inherit
    nameValues_["buffered-rendering"] = str;
  else if (svg_.stringOption(optName, optValue, "visibility", str))
    setVisibility(str); // visible | hidden | collapse | inherit
  else if (svg_.stringOption(optName, optValue, "vector-effect", str))
    // non-scaling-stroke | none | inherit
    nameValues_["vector-effect"] = str;
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
    // no shortcut "marker" for group (other grouping svg)
    auto *group = dynamic_cast<CSVGGroup *>(this);

    if (! group) {
      setMarkerStart(obj);
      setMarkerMid  (obj);
      setMarkerEnd  (obj);
    }
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
  CSVGColor   color;
  double      real;
  bool        inherit;

  if      (svg_.stringOption(optName, optValue, "clip", str))
    nameValues_["clip"] = str;
  else if (svg_.stringOption(optName, optValue, "overflow", str))
    setOverflow(str);
  else if (svg_.colorOption(optName, optValue, "viewport-fill", color))
    setViewportFillColor(color);
  else if (svg_.opacityOption(optName, optValue, "viewport-fill-opacity", real, inherit))
    setViewportFillOpacity(real);
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
    auto *stop = dynamic_cast<CSVGStop *>(this);

    if (stop)
      stop->processOption(optName, optValue);
    else
      notHandled(optName, optValue);
  }
  else if (svg_.stringOption(optName, optValue, "stop-opacity", str)) {
    auto *stop = dynamic_cast<CSVGStop *>(this);

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
  if (svg_.stringOption(optName, optValue, "enable-background", str)) {
    CStrParse parse(str);

    parse.skipSpace();

    std::string word;

    parse.readNonSpace(word);

    if      (word == "accumulate")
      enableBackground_ = CSVGEnableBackground::ACCUMULATE;
    else if (word == "new")
      enableBackground_ = CSVGEnableBackground::NEW;

    parse.skipSpace();

    double x1, y1, x2, y2;

    if (parse.readReal(&x1) && parse.readReal(&y1) && parse.readReal(&x2) && parse.readReal(&y2))
      enableBackgroundRect_ = CBBox2D(x1, y1, x2, y2);
  }
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
    nameValues_["onfocusin"] = str;
  else if (svg_.stringOption(optName, optValue, "onfocusout" , str))
    nameValues_["onfocusout"] = str;
  else if (svg_.stringOption(optName, optValue, "onactivate" , str))
    nameValues_["onactivate"] = str;
  else if (svg_.stringOption(optName, optValue, "onclick"    , str))
    nameValues_["onclick"] = str;
  else if (svg_.stringOption(optName, optValue, "onmousedown", str))
    nameValues_["onmousedown"] = str;
  else if (svg_.stringOption(optName, optValue, "onmouseup"  , str))
    nameValues_["onmouseup"] = str;
  else if (svg_.stringOption(optName, optValue, "onmouseover", str))
    nameValues_["onmouseover"] = str;
  else if (svg_.stringOption(optName, optValue, "onmousemove", str))
    nameValues_["onmousemove"] = str;
  else if (svg_.stringOption(optName, optValue, "onmouseout" , str))
    nameValues_["onmouseout"] = str;
  else if (svg_.stringOption(optName, optValue, "onload"     , str))
    nameValues_["onload"] = str;
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
    nameValues_["onbegin"] = str;
  else if (svg_.stringOption(optName, optValue, "onend", str))
    nameValues_["onend"] = str;
  else if (svg_.stringOption(optName, optValue, "onrepeat", str))
    nameValues_["onrepeat"] = str;
  else if (svg_.stringOption(optName, optValue, "onload", str))
    nameValues_["onload"] = str;
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
  bool b;

  if (svg_.booleanOption(optName, optValue, "externalResourcesRequired", b))
    setExternalResourcesRequired(b);
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

  // Font properties
  if      (svg_.stringOption(optName, optValue, "font", str))
    parseFont(str);
  else if (svg_.stringOption(optName, optValue, "font-family", str))
    setFontFamily(str);
  else if (svg_.fontSizeOption(optName, optValue, "font-size", length))
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
  else if (svg_.stringOption(optName, optValue, "-inkscape-font-specification", str))
   nameValues_["-inkscape-font-specification"] = str;
  else
    return false;

  return true;
}

bool
CSVGObject::
parseFont(const std::string &str)
{
  CStrParse parse(str);

  parse.skipSpace();

  if (parse.isDigit()) {
    std::string word;

    parse.readNonSpace(word);

    auto length = svg_.decodeLengthValue(word);

    if (! length.isValid()) {
      CSVGLog() << "Illegal font length value '" << word << "'";
      return false;
    }

    parse.skipSpace();

    parse.readNonSpace(word);

    std::vector<std::string> words;

    CStrUtil::addFields(word, words, ",");

    if (! words.empty())
      setFontFamily(words[0]);
  }
  else {
    CSVGLog() << "Invalid font string '" << str << "'";
    return false;
  }

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
    nameValues_["direction"] = str;
  else if (svg_.letterSpacingOption(optName, optValue, "letter-spacing", length))
    letterSpacing_ = length;
  else if (svg_.stringOption(optName, optValue, "text-decoration", str))
    setTextDecoration(str);
  else if (svg_.stringOption(optName, optValue, "text-rendering", str)) {
    // auto | optimizeSpeed | optimizeLegibility | geometricPrecision | inherit
    nameValues_["text-rendering"] = str;
  }
  else if (svg_.stringOption(optName, optValue, "unicode-bidi", str))
    nameValues_["unicode-bidi"] = str;
  else if (svg_.wordSpacingOption(optName, optValue, "word-spacing", length))
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
  if (! svg_.isQuiet())
    CSVGLog() << "Option " << optName << ":" << optValue << " not handled " <<
                 "for " << getTagName();
}

COptString
CSVGObject::
getNameValue(const std::string &name) const
{
  COptString str;

  if      (name == "className") {
    std::vector<std::string> classes = getClasses();

    if (! classes.empty())
      str = classes.front();
  }
  else if (name == "viewBox") {
    auto bbox = getViewBox();

    str = CStrUtil::toString(bbox.getXMin()) + ", " +
          CStrUtil::toString(bbox.getYMin()) + ", " +
          CStrUtil::toString(bbox.getXMax()) + ", " +
          CStrUtil::toString(bbox.getYMax());
  }
  else if (name == "transform") {
    str = getTransform().toString();
  }
  else if (name == "externalResourcesRequired") {
    str = (isExternalResourcesRequired() ? "true" : "false");
  }
  else {
    auto p = nameValues_.find(name);

    if (p != nameValues_.end())
      str = (*p).second;
  }

  return str;
}

COptReal
CSVGObject::
getRealNameValue(const std::string &name) const
{
  auto str = getNameValue(name);

  if (! str.isValid()) return COptReal();

  double r;

  if (! CStrUtil::toReal(str.getValue(), &r))
    return COptReal();

  return COptReal(r);
}

COptInt
CSVGObject::
getIntegerNameValue(const std::string &name) const
{
  auto str = getNameValue(name);

  if (! str.isValid()) return COptInt();

  long i;

  if (! CStrUtil::toInteger(str.getValue(), &i))
    return COptInt();

  return COptInt(i);
}

void
CSVGObject::
setNameValue(const std::string &name, const std::string &value)
{
  nameValues_[name] = value;
}

void
CSVGObject::
setStyleValue(const std::string &name, const std::string &value)
{
  styleValues_[name] = value;

  std::string lname = CStrUtil::toLower(name);

  if      (lname == "fill")
    fill_.setColor(value);
  else if (lname == "fill-opacity")
    fill_.setOpacity(value);
  else if (lname == "stroke")
    stroke_.setColor(value);
  else if (lname == "stroke-width")
    stroke_.setWidth(value);
  else if (lname == "stroke-dasharray")
    stroke_.setDashArray(value);
  else if (lname == "marker-start") {
    std::string  id;
    CSVGObject  *obj;

    if (svg_.decodeUrlObject(value, id, &obj))
      marker_.setStart(obj);
    else
      CSVGLog() << "Illegal url value '" << id << "' for " << name;
  }
  else if (lname == "marker-mid") {
    std::string  id;
    CSVGObject  *obj;

    if (svg_.decodeUrlObject(value, id, &obj))
      marker_.setMid(obj);
    else
      CSVGLog() << "Illegal url value '" << id << "' for " << name;
  }
  else if (lname == "marker-end") {
    std::string  id;
    CSVGObject  *obj;

    if (svg_.decodeUrlObject(value, id, &obj))
      marker_.setEnd(obj);
    else
      CSVGLog() << "Illegal url value '" << id << "' for " << name;
  }
  else if (lname == "marker") {
    std::string  id;
    CSVGObject  *obj;

    if (svg_.decodeUrlObject(value, id, &obj)) {
      marker_.setStart(obj);
      marker_.setMid  (obj);
      marker_.setEnd  (obj);
    }
    else
      CSVGLog() << "Illegal url value '" << id << "' for " << name;
  }
  else if (lname == "font-family") {
    // TODO
    CSVGLog() << "Unhandled style font-family: " << value;
  }
  else if (lname == "font-weight") {
    // TODO
    CSVGLog() << "Unhandled style font-weight: " << value;
  }
  else if (lname == "font-style") {
    // TODO
    CSVGLog() << "Unhandled style font-style: " << value;
  }
  else if (lname == "src") {
    // TODO
    CSVGLog() << "Unhandled style src: " << value;
  }
  else
    CSVGLog() << "Unhandled style name: " << name << ":" << value;
}

bool
CSVGObject::
interpValue(const std::string &name, const std::string &from, const std::string &to,
            double x, std::string &ystr) const
{
  if (name == "fill") {
    CSVGColor fromColor;

    if (! svg_.decodeColorString(from, fromColor))
      return false;

    CSVGColor toColor;

    if (! svg_.decodeColorString(to, toColor)) {
      CSVGLog() << "Invalid color '" << to << "'";
      return false;
    }

    auto c = fromColor.rgba()*(1 - x) + toColor.rgba()*x;

    ystr = CStrUtil::strprintf("rgb(%d,%d,%d)", c.getRedI(), c.getGreenI(), c.getBlueI());

    return true;
  }
  else if (from == to) {
    ystr = from;

    return true;
  }
  else {
    double fromVal = 0;
    uint   fromPos = 0;

    if (! CStrUtil::readReal(from, &fromPos, &fromVal)) {
      CSVGLog() << "Invalid real '" << from << "'";
      return false;
    }

    double toVal = 0;
    uint   toPos = 0;

    if (! CStrUtil::readReal(to, &toPos, &toVal))
      return false;

    double y = CMathUtil::map(x, 0, 1, fromVal, toVal);

    ystr = std::to_string(y);

    return true;
  }
}

double
CSVGObject::
pathLength() const
{
  return getPartList().getLength();
}

bool
CSVGObject::
pointAtLength(double l, CPoint2D &p, double &a, int &pi) const
{
  double s = l/pathLength();

  double xi, yi;

  if (! getPartList().interp(s, &xi, &yi, &a, &pi))
    return false;

  p = CPoint2D(xi, yi);

  return true;
}

void
CSVGObject::
setOverflow(const std::string &str)
{
  if      (str == "visible") overflow_ = CSVGOverflowType::VISIBLE;
  else if (str == "hidden" ) overflow_ = CSVGOverflowType::HIDDEN;
  else if (str == "scroll" ) overflow_ = CSVGOverflowType::SCROLL;
  else if (str == "auto"   ) overflow_ = CSVGOverflowType::AUTO;
  else if (str == "inherit") overflow_ = CSVGOverflowType::INHERIT;
  else                       notHandled("overflow", str);
}

void
CSVGObject::
setTextBaselineShift(const std::string &str)
{
  nameValues_["baseline-shift"] = str;

  if      (str == "super") fontDef_.setSuperscript(true);
  else if (str == "sub"  ) fontDef_.setSubscript(true);
  else                     notHandled("baseline-shift", str);
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

  auto *parent = getParent();

  while (parent) {
    if (parent->textAnchor_.isValid())
      return parent->getFlatTextAnchor();

    parent = parent->getParent();
  }

  return CHALIGN_TYPE_LEFT;
}

CSVGObject *
CSVGObject::
getFlatMarkerStart() const
{
  auto *marker = marker_.getStart();

  if (marker)
    return marker;

  return nullptr;
}

CSVGObject *
CSVGObject::
getFlatMarkerMid() const
{
  auto *marker = marker_.getMid();

  if (marker)
    return marker;

  if (parent_)
    return parent_->getFlatMarkerMid();

  return nullptr;
}

CSVGObject *
CSVGObject::
getFlatMarkerEnd() const
{
  auto *marker = marker_.getEnd();

  if (marker)
    return marker;

  if (parent_)
    return parent_->getFlatMarkerEnd();

  return nullptr;
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
  object->setParent(nullptr);

  if (object->isAnimated())
    animation_.removeObject(object);
  else
    objects_.remove(object);
}

CSVGObject *
CSVGObject::
child(int i) const
{
  int i1 = 0;

  for (auto &c : children()) {
    if (i1 == i)
      return c;

    ++i1;
  }

  return nullptr;
}

bool
CSVGObject::
getAllChildren(ObjectArray &objects)
{
  for (const auto &c : children())
    objects.push_back(c);

  for (const auto &c : children())
    c->getAllChildren(objects);

  return ! objects.empty();
}

bool
CSVGObject::
getAllChildrenOfType(CSVGObjTypeId id, ObjectArray &objects)
{
  getChildrenOfType(id, objects);

  for (const auto &c : children())
    c->getAllChildrenOfType(id, objects);

  return ! objects.empty();
}

bool
CSVGObject::
getChildrenOfType(CSVGObjTypeId id, ObjectArray &objects)
{
  for (const auto &c : children())
    if (c->isObjType(id))
      objects.push_back(c);

  return ! objects.empty();
}

bool
CSVGObject::
getAllChildrenOfId(const std::string &id, ObjectArray &objects)
{
  getChildrenOfId(id, objects);

  for (const auto &c : children())
    c->getAllChildrenOfId(id, objects);

  return ! objects.empty();
}

bool
CSVGObject::
getChildrenOfId(const std::string &id, ObjectArray &objects)
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
  svg_.updateBusy();

  //------

  if (! isHierDrawable())
    return false;

  if (getDisplay() == "none")
    return false;

  bool drawn = false;

  //------

  // draw to new temp background
  if (enableBackground() == CSVGEnableBackground::NEW) {
    auto *bgBuffer      = svg_.getBuffer("BackgroundImage");
    auto *tempBgBuffer1 = svg_.getBuffer(getUniqueName() + "_BackgroundImage1");

    if (tempBgBuffer1->isDrawing())
      tempBgBuffer1->stopDraw();

    bool oldBgDrawing = bgBuffer->isDrawing();

    if (oldBgDrawing)
      bgBuffer->stopDraw();

    tempBgBuffer1->setImageBuffer(bgBuffer);

    bgBuffer->clear();

    if (oldBgDrawing)
      bgBuffer->startDraw();
  }

  //------

  // get current buffer
  auto *oldBuffer     = svg_.getCurrentBuffer();
  auto *currentBuffer = oldBuffer;

  //------

  // save object image to temporary buffer if debug or filter
  bool saveImage = false;

  if (svg_.getDebugObjImage()) {
    saveImage = true;

    if (! isDrawable() && ! hasChildren())
      saveImage = false;
  }

  if (getOpacityValid())
    saveImage = true;

  bool isFiltered = (hasFilter() && getFiltered());

  if (svg_.getIgnoreFilter())
    isFiltered = false;

  if (isFiltered)
    saveImage = true;

  bool isMasked = (getMask() && getMasked());

  if (isMasked)
    saveImage = true;

  CSVGBuffer *saveBuffer = nullptr;

  //---

  // set buffer to temporary buffer
  if (saveImage) {
    CBBox2D bbox;

    saveBuffer = svg_.pushBuffer("_" + getUniqueName());

    if (isFiltered) {
      auto *filter = getFilter();

      filter->getRegion(this, bbox);

      saveBuffer->updateBBoxSize(bbox);
    }

    saveBuffer->clear();

    auto *block = dynamic_cast<CSVGBlock *>(this);

    if (bbox.isSet())
      saveBuffer->setBBox(bbox);

    if      (block)
      svg_.beginDrawBuffer(saveBuffer, svg_.offset(), svg_.xscale(), svg_.yscale());
    else if (bbox.isSet()) {
      auto *root = svg_.getRoot();

      auto   pixelBox       = root->calcPixelBox();
      auto   viewBox        = root->calcViewBox();
      auto   offset         = CPoint2D(0, 0);
      double xs             = 1;
      double ys             = 1;
      auto   preserveAspect = svg_.blockPreserveAspect();

      viewBox.setXMax(std::max(viewBox.getXMax(), bbox.getXMax()));
      viewBox.setYMax(std::max(viewBox.getYMax(), bbox.getYMax()));

      svg_.beginDrawBuffer(saveBuffer, pixelBox, viewBox, offset, xs, ys, preserveAspect);
    }
    else
      svg_.beginDrawBuffer(saveBuffer, CPoint2D(0, 0), 1, 1);

    currentBuffer = saveBuffer;
  }

  //------

  // set current transform
  auto transform = oldBuffer->transform();

  CMatrixStack2D transform1(transform);

  transform1.append(getTransform());

  //if (! isFiltered)
    currentBuffer->setTransform(transform1);

  if (getOpacityValid())
    currentBuffer->setOpacity(getOpacity());

  //------

  // draw object
  if (drawSubObject())
    drawn = true;

  //------

  // restore transform
  //if (! isFiltered)
  currentBuffer->setTransform(transform);

  //------

  // if object image saved to temporary buffer add to old buffer
  if (saveImage) {
    svg_.endDrawBuffer(saveBuffer);

    //---

    if (drawn) {
      if (isFiltered) {
        // filtered image draw in local coords so need to place
        // image at expected position

#if 1
        double x1 = 0, y1 = 0, x2 = 0, y2 = 0;

        transform .multiplyPoint(0, 0, &x1, &y1);
        transform1.multiplyPoint(0, 0, &x2, &y2);

        double dx = x2 - x1;
        double dy = y2 - y1;

        saveBuffer->setOrigin(CPoint2D(dx, dy));

        double px, py;

        svg_.lengthToPixel(dx, dy, &px, &py);
#else
        double x = 0, y = 0;

        transform1.multiplyPoint(0, 0, &x, &y);

        saveBuffer->setOrigin(CPoint2D(x, y));

        double px, py;

        svg_.lengthToPixel(x, y, &px, &py);
#endif

        bool oldDrawing = oldBuffer->isDrawing();

        if (oldDrawing)
          oldBuffer->stopDraw();

        px = 0; py = 0;

        oldBuffer->addImageBuffer(px, py, saveBuffer);

        if (oldDrawing)
          oldBuffer->startDraw();
      }
      else {
        bool oldDrawing = oldBuffer->isDrawing();

        if (oldDrawing)
          oldBuffer->stopDraw();

        oldBuffer->addImageBuffer(saveBuffer);

        if (oldDrawing)
          oldBuffer->startDraw();
      }
    }

    //---

    svg_.popBuffer();
  }

  //---

  // add new background to original
  if (enableBackground() == CSVGEnableBackground::NEW) {
    auto *bgBuffer      = svg_.getBuffer("BackgroundImage");
    auto *tempBgBuffer1 = svg_.getBuffer(getUniqueName() + "_BackgroundImage1");
    auto *tempBgBuffer2 = svg_.getBuffer(getUniqueName() + "_BackgroundImage2");

    bool oldBgDrawing = bgBuffer->isDrawing();

    if (oldBgDrawing)
      bgBuffer->stopDraw();

    tempBgBuffer2->setImageBuffer(bgBuffer);

    bgBuffer->clear();

    bgBuffer->addImageBuffer(tempBgBuffer1);
    bgBuffer->addImageBuffer(tempBgBuffer2);

    if (oldBgDrawing)
      bgBuffer->startDraw();
  }

  //---

  return drawn;
}

bool
CSVGObject::
drawSubObject(bool forceDraw)
{
  bool visible      = isVisible();
  bool childVisible = (! visible ? anyChildVisible(visible) : true);

  if (! visible && ! childVisible)
    return false;

  //------

  bool drawn = false;

  auto *oldBuffer = svg_.getCurrentBuffer();

  // set stroke
  //CSVGTempStroke tempStroke(*this);
  //CSVGTempFont   tempFont  (*this);
  svg_.pushStyle(this);

  drawInit();

  //------

  // draw clip path if specified
  bool isClipped = (getClipPath() && getClipped());

  if (isClipped)
    getClipPath()->drawPath(this);

  //------

  // draw object (virtual)
  if (visible) {
    if (isDrawable() || isFilter()) {
      svg_.pushDrawObject(this);

      draw();

      drawn = true;

      svg_.popDrawObject();
    }
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
  bool isFiltered = (hasFilter() && getFiltered());

  if (svg_.getIgnoreFilter())
    isFiltered = false;

  if (isFiltered) {
    auto *filter = getFilter();

    // init filter
    if (filter) {
      filter->setObject(this);

      filter->initDraw(oldBuffer);

      CBBox2D filterBBox;

      if (filter->getRegion(this, filterBBox))
        filter->setContentsBBox(filterBBox);

      // draw filter
      if (filter->drawSubObject(true))
        drawn = true;

      //--

      filter->termDraw(oldBuffer);
    }
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

  svg_.popStyle();

  return drawn;
}

bool
CSVGObject::
isHierVisible(bool defValue) const
{
  if (! hasVisibility()) {
    if (parent_)
      return parent_->isHierVisible();

    return defValue;
  }
  else
    return isVisible(defValue);
}

bool
CSVGObject::
anyChildVisible(bool defValue) const
{
  for (const auto &c : children()) {
    if (c->isVisible(defValue))
      return true;
  }

  for (const auto &c : children()) {
    if (c->anyChildVisible(defValue))
      return true;
  }

  return defValue;
}

bool
CSVGObject::
isVisible(bool defValue) const
{
  // visible | hidden | collapse | inherit
  if (hasVisibility()) {
    if      (getVisibility() == "visible")
      return true;
    else if (getVisibility() == "hidden" || getVisibility() == "collapse")
      return false;
  }

  return defValue;
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

CSVGBuffer *
CSVGObject::
toBufferImage()
{
  // TODO: use toNamedBufferImage, optional filter ?
  CBBox2D bbox;

  if (! getBBox(bbox))
    return nullptr;

  auto *imageBuffer = svg_.pushBuffer("_" + getUniqueName() + "_image");

  imageBuffer->clear();

  // draw object to buffer
  svg_.beginDrawBuffer(imageBuffer, bbox);

  //buffer->setup(bbox);

  (void) drawSubObject(true);

  svg_.endDrawBuffer(imageBuffer);

  svg_.popBuffer();

  return imageBuffer;
}

CSVGBuffer *
CSVGObject::
toNamedBufferImage(const std::string &bufferName)
{
  CBBox2D bbox;

  if (! getBBox(bbox))
    return nullptr;

  COptValT<CSVGFilter*> saveFilter;

  std::swap(saveFilter, filter_);

  auto *imageBuffer = svg_.pushBuffer(bufferName);

  imageBuffer->clear();

  svg_.beginDrawBuffer(imageBuffer, bbox);

  (void) drawSubObject(true);

  svg_.endDrawBuffer(imageBuffer);

  svg_.popBuffer();

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
  if (! hasViewBox()) {
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
getParentViewBox(CBBox2D &bbox) const
{
  if (hasViewBox()) {
    bbox = getViewBox();
    return true;
  }

  if (! parent_)
    return false;

  return parent_->getParentViewBox(bbox);
}

bool
CSVGObject::
getTransformedBBox(CBBox2D &bbox) const
{
  auto m = getTransform();

  if (! getBBox(bbox))
    return false;

  bbox = svg_.transformBBox(m, bbox);

  return true;
}

bool
CSVGObject::
getFlatTransformedBBox(CBBox2D &bbox) const
{
  auto m = getFlatTransform();

  if (! getBBox(bbox)) {
    if (! getParent())
      return false;

    return getParent()->getFlatTransformedBBox(bbox);
  }

  bbox = svg_.transformBBox(m, bbox);

  return true;
}

CBBox2D
CSVGObject::
getDrawBBox() const
{
  CBBox2D bbox;

  if (hasViewBox())
    bbox = getViewBox();
  else {
    if (! getParentViewBox(bbox))
      bbox = CBBox2D(0, 0, 100, 100);
  }

  return bbox;
}

bool
CSVGObject::
inside(const CPoint2D &pos) const
{
  CBBox2D bbox;

  if (! getFlatTransformedBBox(bbox))
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
setTime(double t)
{
  for (const auto &c : children())
    c->setTime(t);

  animation_.setTime(t);
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
decodeXLink(const std::string &str, CSVGObject **object, CSVGBuffer **buffer)
{
  if (object) *object = nullptr;
  if (buffer) *buffer = nullptr;

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

    if      (format == "image/png;base64" || format == "image/jpeg;base64") {
      // image string to buffer
      std::string filename;

      if (format == "image/png;base64")
        filename = ".svg.png";
      else
        filename = ".svg.jpeg";

      decodeStringToFile(str.substr(pos), filename);

      auto *imgBuffer = getXLinkBuffer();

      imgBuffer->setImageFile(filename);

      if (buffer)
        *buffer = imgBuffer;

      return true;
    }
    else if (format == "image/svg+xml;base64") {
      // SVG string to buffer
      std::string filename(".svg.svg");

      decodeStringToFile(str.substr(pos), filename);

      auto type = CFileUtil::getTextType(filename);

      if (type == CFILE_TYPE_TEXT_GZIP) {
        std::string gzfilename = filename + ".gz";

        rename(filename.c_str(), gzfilename.c_str());

        std::string cmd = "/bin/gzip -d " + gzfilename;

        system(cmd.c_str());
      }

      // draw SVG file to image at current scale
      auto *svg = svg_.dup();

      svg->setRenderer(svg_.getRenderer());

      svg->init();

      svg->read(filename);

      CSize2D size;

      getSize(size);

      svg->getRoot()->setSize(size);

      auto *imgBuffer = getXLinkBuffer();

      int w = svg->getIWidth();
      int h = svg->getIHeight();

      svg->drawToBuffer(imgBuffer, w, h, svg_.offset(), svg_.xscale(), svg_.yscale());

      if (buffer)
        *buffer = imgBuffer;

      delete svg;

      return true;
    }
    else if (format == "text/javascript;base64") {
      // Javascript string to buffer
      std::string filename(".svg.js");

      decodeStringToFile(str.substr(pos), filename);

      auto type = CFileUtil::getTextType(filename);

      if (type == CFILE_TYPE_TEXT_GZIP) {
        std::string gzfilename = filename + ".gz";

        rename(filename.c_str(), gzfilename.c_str());

        std::string cmd = "/bin/gzip -d " + gzfilename;

        system(cmd.c_str());
      }

      // load javascript
      svg_.setScriptFile(".svg.js");

      return true;
    }
    else {
      CSVGLog() << "Unhandled images format '" + format + "'";
      return true; // don't propagate warning
    }
  }

  //---

  // get link name
  std::string::size_type pos = str.find('#');

  if (pos != std::string::npos) {
    CSVGObject *object1 = nullptr;

    std::string lhs = str.substr(0, pos);
    std::string rhs = str.substr(pos + 1);

    // Handle file: <filename>#<id>
    if (lhs != "") {
      // read file
      auto *block = svg_.createBlock();

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
      CSVGLog() << "Object '" << rhs << "' does not exist";
      return true; // don't propagate warning
    }

    if (object)
      *object = object1;
  }
  // handle file
  else {
    // check valid
    CFile file(str);

    if (! file.exists()) {
      CSVGLog() << "File '" << str << "' does not exist";
      return true; // don't propagate warning
    }

    if (! file.isRegular()) {
      CSVGLog() << "File '" << str << "' is not a regular file";
      return true; // don't propagate warning
    }

    // get type from contents/suffix
    auto type = CFileUtil::getType(&file);

    if (type == CFILE_TYPE_INODE_REG && file.getSuffix() == "svg")
      type = CFILE_TYPE_IMAGE_SVG;

    // handle image file
    if      (type & CFILE_TYPE_IMAGE) {
      // svg image
      if (type == CFILE_TYPE_IMAGE_SVG) {
        // draw SVG file to image at current scale
        auto *svg = svg_.dup();

        svg->setRenderer(svg_.getRenderer());

        svg->init();

        svg->read(file.getPath());

        CSize2D size;

        getSize(size);

        svg->getRoot()->setSize(size);

        auto *imgBuffer = getXLinkBuffer();

        int w = svg->getIWidth();
        int h = svg->getIHeight();

        svg->drawToBuffer(imgBuffer, w, h, svg_.offset(), svg_.xscale(), svg_.yscale());

        if (buffer)
          *buffer = imgBuffer;
      }
      // image file (png, jpeg, ...)
      else {
        auto *imgBuffer = getXLinkBuffer();

        imgBuffer->setImageFile(str);

        if (buffer)
          *buffer = imgBuffer;
      }
    }
    // handle svg file
    else if (type == CFILE_TYPE_TEXT_HTML) {
      // read svg file
      auto *block = svg_.createBlock();

      svg_.read(str, block);

      if (object)
        *object = block;
    }
    else {
      CSVGLog() << "Unknown type of file for '" << str << "'";
      return true; // don't propagate warning
    }
  }

  return true;
}

bool
CSVGObject::
decodeStringToFile(const std::string &str, const std::string &filename)
{
  std::string str1 = CEncode64Inst->decode(str);

  CFile file(filename);

  file.write(str1);

  file.flush();

  file.close();

  return true;
}

CSVGBuffer *
CSVGObject::
getXLinkBuffer()
{
  return svg_.getBuffer(getUniqueName() + "_xlink");
}

bool
CSVGObject::
getObjectsAtPoint(const CPoint2D &p, ObjectArray &objects) const
{
  ObjectArray objects1;

  if (inside(p))
    objects1.push_back(const_cast<CSVGObject *>(this));

  for (const auto &c : children())
    c->getObjectsAtPoint(p, objects1);

  using DepthObjects = std::map<int, ObjectArray>;

  DepthObjects depthObjects;

  for (const auto &obj : objects1)
    depthObjects[-obj->getDepth()].push_back(obj);

  for (const auto &pd : depthObjects)
    for (const auto &obj : pd.second)
      objects.push_back(obj);

  return ! objects.empty();
}

void
CSVGObject::
handleEvent(CSVGEventType type, const std::string &id, const std::string &data, bool propagate)
{
  if (id == "" || id == this->getId())
    execEvent(type);

  if (type == CSVGEventType::MOUSE_OVER || type == CSVGEventType::MOUSE_OUT) {
    if (! isHierDrawable())
      propagate = false;
  }

  if (propagate) {
    for (const auto &c : children())
      c->handleEvent(type, id, data);
  }

  if (! animation_.objects().empty())
    animation_.handleEvent(type, id, data);
}

void
CSVGObject::
execEvent(CSVGEventType type)
{
  if      (type == CSVGEventType::LOAD) {
    auto p = nameValues_.find("onload");

    if (p != nameValues_.end()) {
      svg_.execJsEvent(this, (*p).second);
    }
  }
  else if (type == CSVGEventType::MOUSE_OVER) {
    auto p = nameValues_.find("onmouseover");

    if (p != nameValues_.end()) {
      svg_.execJsEvent(this, (*p).second);
    }
  }
  else if (type == CSVGEventType::MOUSE_OUT) {
    auto p = nameValues_.find("onmouseout");

    if (p != nameValues_.end()) {
      svg_.execJsEvent(this, (*p).second);
    }
  }
  else if (type == CSVGEventType::CLICK) {
    auto p = nameValues_.find("onclick");

    if (p != nameValues_.end()) {
      svg_.execJsEvent(this, (*p).second);
    }
  }
  else if (type == CSVGEventType::BEGIN) {
    auto p = nameValues_.find("onbegin");

    if (p != nameValues_.end()) {
      svg_.execJsEvent(this, (*p).second);
    }
  }
  else if (type == CSVGEventType::END) {
    auto p = nameValues_.find("onend");

    if (p != nameValues_.end()) {
      svg_.execJsEvent(this, (*p).second);
    }
  }
}

void
CSVGObject::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    std::string s = getText();

    os << "<" << getTagName();

    printValues(os);

    if (hasChildren()) {
      os << ">\n";

      if (s != "")
        os << s << "\n";

      printChildren(os, hier);

      os << "</" << getTagName() << ">\n";
    }
    else if (s != "") {
      os << ">" << s;
      os << "</" << getTagName() << ">\n";
    }
    else
      os << "/>\n";
  }
  else
    os << getTagName() << ": '" << getId() << "'";
}

void
CSVGObject::
printFlat(std::ostream &os, bool force, int depth) const
{
  auto id = getObjTypeId();

  if (id == CSVGObjTypeId::BLOCK) {
    os << "<svg";

    printValues(os, /*flat*/ false); // true ?

    os << ">\n";
  }

  if (hasChildren()) {
    bool startTag = false;

    if (id == CSVGObjTypeId::GROUP && ! getId().empty())
      startTag = true;

    if (id == CSVGObjTypeId::DEFS)
      startTag = true;

    if (id == CSVGObjTypeId::TEXT)
      startTag = true;

    if (! canFlatten() && ! getId().empty())
      startTag = true;

    if (startTag) {
      for (int i = 0; i < depth; ++i)
        os << " ";

      os << "<" << getTagName();

      printValues(os, /*flat*/true);

      os << ">\n";
    }

    for (const auto &c : children()) {
      if (force || propagateFlat()) {
        if (startTag)
          c->printFlat(os, force, depth + 1);
        else
          c->printFlat(os, force, depth);
      }
      else
        c->print(os, /*hier*/true);
    }

    if (startTag) {
      for (int i = 0; i < depth; ++i)
        os << " ";

      os << "</" << getTagName() << ">\n";
    }
  }
  else {
    if (id == CSVGObjTypeId::GROUP || id == CSVGObjTypeId::DEFS)
      return;

    for (int i = 0; i < depth; ++i)
      os << " ";

    std::string s = getText();

    os << "<" << getTagName();

    printValues(os, /*flat*/true);

    if (s != "") {
      os << ">" << s;

      os << "</" << getTagName() << ">\n";
    }
    else
      os << "/>\n";
  }

  if (id == CSVGObjTypeId::BLOCK) {
    os << "</svg>\n";
  }
}

void
CSVGObject::
printValues(std::ostream &os, bool flat) const
{
  printNameValue (os, "id"   , id_     );
  printNameValues(os, "class", classes_);

  if (hasViewBox()) {
    auto viewBox = getViewBox();

    os << " viewBox=\"" << viewBox.getXMin() << " " << viewBox.getYMin() <<
                    " " << viewBox.getXMax() << " " << viewBox.getYMax() << "\"";
  }

  printTransform(os, flat);

  printFilter(os);

  printStyle(os, flat);

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

bool
CSVGObject::
hasAnimation() const
{
  if (! animation_.objects().empty())
    return true;

  for (const auto &c : children())
    if (c->hasAnimation())
      return true;

  return false;
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
  if (hasFilter()) {
    auto *filter = getFilter();

    if (filter)
      os << " filter=\"url(#" << filter->getId() << ")\"";
    else
      os << " filter=\"url(none)\"";
  }
}

void
CSVGObject::
printStyle(std::ostream &os, bool flat) const
{
  std::stringstream ss;

  bool output = false;

  if (hasFontDef()) {
    std::stringstream ss1;

    printFontDef(ss1);

    if (ss1.str().size()) {
      ss << ss1.str();

      output = true;
    }
  }

  if (getOpacityValid()) {
    if (output) ss << " ";

    ss << "opacity: " << getOpacity() << ";";

    output = true;
  }

  if (flat || hasStroke()) {
    std::stringstream ss1;

    printStroke(ss1, flat);

    if (ss1.str().size()) {
      if (output) ss << " ";

      ss << ss1.str();

      output = true;
    }
  }

  if (flat || hasFill()) {
    std::stringstream ss1;

    printFill(ss1, flat);

    if (ss1.str().size()) {
      if (output) ss << " ";

      ss << ss1.str();

      output = true;
    }
  }

  if (getMarkerStart()) {
    if (output) ss << " ";

    ss << "marker-start: url(#" << getMarkerStart()->getId() << ");";
  }

  if (getMarkerMid()) {
    if (output) ss << " ";

    ss << "marker-mid: url(#" << getMarkerMid()->getId() << ");";
  }

  if (getMarkerEnd()) {
    if (output) ss << " ";

    ss << "marker-end: url(#" << getMarkerEnd()->getId() << ");";
  }

  if (hasLetterSpacing()) {
    if (output) ss << " ";

    ss << "letter-spacing: " << getLetterSpacing() << ";";

    output = true;
  }

  if (hasWordSpacing()) {
    if (output) ss << " ";

    ss << "word-spacing: " << getWordSpacing() << ";";

    output = true;
  }

  if (getClipPath()) {
    if (output) ss << " ";

    ss << "clip-path: url(#" << getClipPath()->getId() << ");";

    output = true;
  }

  if (getMask()) {
    if (output) ss << " ";

    ss << "mask: url(#" << getMask()->getId() << ");";

    output = true;
  }

  if (hasVisibility()) {
    if (output) ss << " ";

    ss << "visibility: " << getVisibility() << ";";

    output = true;
  }

  if (output) {
    os << " style=\"" << ss.str() << "\"";
  }
}

void
CSVGObject::
printStroke(std::ostream &os, bool flat) const
{
  if (flat) {
    auto stroke = getFlatStroke();

    stroke.print(os);
  }
  else {
    const auto &stroke = getStroke();

    stroke.print(os);
  }
}

void
CSVGObject::
printFill(std::ostream &os, bool flat) const
{
  if (flat) {
    auto fill = getFlatFill();

    fill.print(os);
  }
  else {
    const auto &fill = getFill();

    fill.print(os);
  }
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
printTransform(std::ostream &os, bool flat) const
{
  if (! flat) {
    if (! transform_.isEmpty()) {
      os << " transform=\"";

      printTransform(os, transform_);

      os << "\"";
    }
  }
  else {
    if (! hasChildren()) {
      auto transform = getFlatTransform();

      if (! transform.isEmpty()) {
        os << " transform=\"";

        printTransform(os, transform);

        os << "\"";
      }
    }
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

        os << "skewY(" << CMathGen::RadToDeg(t.angle()) << ")";

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

        os << ")";

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
    os << " " << name << "=\"" << parts << "\"";
  }
}

void
CSVGObject::
printNameCoordUnits(std::ostream &os, const std::string &name,
                    const COptValT<CSVGCoordUnits> &units) const
{
  if (units.isValid())
    os << " " << name << "=\"" << CSVG::encodeUnitsString(units.getValue()) << "\"";
}

void
CSVGObject::
printNamePercent(std::ostream &os, const std::string &name,
                 const COptValT<CScreenUnits> &units) const
{
  if (units.isValid()) {
    if (units.getValue().units() == CScreenUnits::Units::RATIO)
      os << " " << name << "=\"" << units.getValue().ratioValue() << "\"";
    else
      os << " " << name << "=\"" << units.getValue() << "\"";
  }
}

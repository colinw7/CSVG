#include <CSVGWrite.h>

#include <CSVG.h>
#include <CSVGAnchor.h>
#include <CSVGAnimate.h>
#include <CSVGAnimateColor.h>
#include <CSVGAnimateMotion.h>
#include <CSVGAnimateTransform.h>
#include <CSVGAudio.h>
#include <CSVGCircle.h>
#include <CSVGClipPath.h>
#include <CSVGColorProfile.h>
#include <CSVGDefs.h>
#include <CSVGDesc.h>
#include <CSVGEllipse.h>

#include <CSVGFeBlend.h>
#include <CSVGFeColorMatrix.h>
#include <CSVGFeComponentTransfer.h>
#include <CSVGFeComposite.h>
#include <CSVGFeConvolveMatrix.h>
#include <CSVGFeDiffuseLighting.h>
#include <CSVGFeDisplacementMap.h>
#include <CSVGFeDistantLight.h>
#include <CSVGFeFlood.h>
#include <CSVGFeFunc.h>
#include <CSVGFeGaussianBlur.h>
#include <CSVGFeImage.h>
#include <CSVGFeLighting.h>
#include <CSVGFeMerge.h>
#include <CSVGFeMergeNode.h>
#include <CSVGFeMorphology.h>
#include <CSVGFeOffset.h>
#include <CSVGFePointLight.h>
#include <CSVGFeSpecularLighting.h>
#include <CSVGFeSpotLight.h>
#include <CSVGFeTile.h>
#include <CSVGFeTurbulence.h>
#include <CSVGFilter.h>

#include <CSVGFont.h>
#include <CSVGFontFace.h>
#include <CSVGFontFaceSrc.h>
#include <CSVGFontFaceUri.h>
#include <CSVGGlyph.h>
#include <CSVGGroup.h>
#include <CSVGHKern.h>
#include <CSVGImage.h>
#include <CSVGLinearGradient.h>
#include <CSVGLine.h>
#include <CSVGMPath.h>
#include <CSVGMarker.h>
#include <CSVGMask.h>
#include <CSVGMetaData.h>
#include <CSVGPath.h>
#include <CSVGPattern.h>
#include <CSVGPolygon.h>
#include <CSVGPolyLine.h>
#include <CSVGRadialGradient.h>
#include <CSVGRect.h>
#include <CSVGSet.h>
#include <CSVGScript.h>
#include <CSVGStop.h>
#include <CSVGStyle.h>
#include <CSVGSwitch.h>
#include <CSVGSymbol.h>
#include <CSVGTBreak.h>
#include <CSVGText.h>
#include <CSVGTextPath.h>
#include <CSVGTitle.h>
#include <CSVGTSpan.h>
#include <CSVGUse.h>

CSVGWriteVisitor::
CSVGWriteVisitor(std::ostream &os) :
 os_(os)
{
}

void
CSVGWriteVisitor::
exec(CSVGObject *object)
{
  CSVGVisitor::exec(object);
}

// called at start of start tag
void
CSVGWriteVisitor::
preVisit(CSVGObject *object)
{
  for (int i = 0; i < depth_; ++i)
    os_ << "  ";

  ++depth_;

  const auto &name = object->getObjName();

  os_ << "<" << name;

#if 0
  if (object->getId() != "")
    os_ << " id=\"" << object->getId() << "\"";
#endif

  newline_ = false;
}

// called at end of start tag
void
CSVGWriteVisitor::
postVisit(CSVGObject *object)
{
  if (! object->getTransform().isEmpty()) {
    double a, b, c, d, tx, ty;

    object->getTransform().getMatrix().getValues(&a, &b, &c, &d, &tx, &ty);

    os_ << " transform=\"matrix(" <<
          a << "," << b << "," << c << "," << d << "," << tx << "," << ty << ")\"";
  }

  if (object->hasChildren()) {
    os_ << ">\n";

    newline_ = true;
  }
  else
    newline_ = false;
}

// called at end of end tag
void
CSVGWriteVisitor::
postExec(CSVGObject *object)
{
  --depth_;

  if (newline_) {
    for (int i = 0; i < depth_; ++i)
      os_ << "  ";
  }

  const auto &name = object->getObjName();

  if      (object->hasChildren())
    os_ << "</" << name << ">\n";
  else if (object->hasText())
    os_ << ">" << object->getText() << "</" << name << ">\n";
  else
    os_ << "/>\n";

  newline_ = true;
}

void
CSVGWriteVisitor::
visit(CSVGAnchor *anchor)
{
  anchor->printValues(os_);
}

void
CSVGWriteVisitor::
visit(CSVGBlock *block)
{
  block->printValues(os_);
}

void
CSVGWriteVisitor::
visit(CSVGAnimate *animate)
{
  animate->printValues(os_);
}

void
CSVGWriteVisitor::
visit(CSVGAnimateColor *animateColor)
{
  animateColor->printValues(os_);
}

void
CSVGWriteVisitor::
visit(CSVGAnimateMotion *animateMotion)
{
  animateMotion->printValues(os_);
}

void
CSVGWriteVisitor::
visit(CSVGAnimateTransform *animateTransform)
{
  animateTransform->printValues(os_);
}

void
CSVGWriteVisitor::
visit(CSVGAudio *audio)
{
  audio->printValues(os_);
}

void
CSVGWriteVisitor::
visit(CSVGCircle *circle)
{
  circle->printValues(os_);
}

void
CSVGWriteVisitor::
visit(CSVGClipPath *clipPath)
{
  clipPath->printValues(os_);
}

void
CSVGWriteVisitor::
visit(CSVGColorProfile *colorProfile)
{
  colorProfile->printValues(os_);
}

void
CSVGWriteVisitor::
visit(CSVGDefs *defs)
{
  defs->printValues(os_);
}

void
CSVGWriteVisitor::
visit(CSVGDesc *desc)
{
  desc->printValues(os_);
}

void
CSVGWriteVisitor::
visit(CSVGEllipse *ellipse)
{
  ellipse->printValues(os_);
}

void
CSVGWriteVisitor::
visit(CSVGFeBlend *feBlend)
{
  feBlend->printValues(os_);
}

void
CSVGWriteVisitor::
visit(CSVGFeColorMatrix *feColorMatrix)
{
  feColorMatrix->printValues(os_);
}

void
CSVGWriteVisitor::
visit(CSVGFeComponentTransfer *feComponentTransfer)
{
  feComponentTransfer->printValues(os_);
}

void
CSVGWriteVisitor::
visit(CSVGFeComposite *feComposite)
{
  feComposite->printValues(os_);
}

void
CSVGWriteVisitor::
visit(CSVGFeConvolveMatrix *feConvolveMatrix)
{
  feConvolveMatrix->printValues(os_);
}

void
CSVGWriteVisitor::
visit(CSVGFeDiffuseLighting *feDiffuseLighting)
{
  feDiffuseLighting->printValues(os_);
}

void
CSVGWriteVisitor::
visit(CSVGFeDisplacementMap *feDisplacementMap)
{
  feDisplacementMap->printValues(os_);
}

void
CSVGWriteVisitor::
visit(CSVGFeDistantLight *feDistantLight)
{
  feDistantLight->printValues(os_);
}

void
CSVGWriteVisitor::
visit(CSVGFeFlood *feFlood)
{
  feFlood->printValues(os_);
}

void
CSVGWriteVisitor::
visit(CSVGFeFunc *feFunc)
{
  feFunc->printValues(os_);
}

void
CSVGWriteVisitor::
visit(CSVGFeGaussianBlur *feGaussianBlur)
{
  feGaussianBlur->printValues(os_);
}

void
CSVGWriteVisitor::
visit(CSVGFeImage *feImage)
{
  feImage->printValues(os_);
}

void
CSVGWriteVisitor::
visit(CSVGFeLighting *feLighting)
{
  feLighting->printValues(os_);
}

void
CSVGWriteVisitor::
visit(CSVGFeMerge *feMerge)
{
  feMerge->printValues(os_);
}

void
CSVGWriteVisitor::
visit(CSVGFeMergeNode *feMergeNode)
{
  feMergeNode->printValues(os_);
}

void
CSVGWriteVisitor::
visit(CSVGFeMorphology *feMorphology)
{
  feMorphology->printValues(os_);
}

void
CSVGWriteVisitor::
visit(CSVGFeOffset *feOffset)
{
  feOffset->printValues(os_);
}

void
CSVGWriteVisitor::
visit(CSVGFePointLight *fePointLight)
{
  fePointLight->printValues(os_);
}

void
CSVGWriteVisitor::
visit(CSVGFeSpecularLighting *feSpecularLighting)
{
  feSpecularLighting->printValues(os_);
}

void
CSVGWriteVisitor::
visit(CSVGFeSpotLight *feSpotLight)
{
  feSpotLight->printValues(os_);
}

void
CSVGWriteVisitor::
visit(CSVGFeTile *feTile)
{
  feTile->printValues(os_);
}

void
CSVGWriteVisitor::
visit(CSVGFeTurbulence *feTurbulence)
{
  feTurbulence->printValues(os_);
}

void
CSVGWriteVisitor::
visit(CSVGFilter *filter)
{
  filter->printValues(os_);
}

void
CSVGWriteVisitor::
visit(CSVGFont *font)
{
  font->printValues(os_);
}

void
CSVGWriteVisitor::
visit(CSVGFontFace *fontFace)
{
  fontFace->printValues(os_);
}

void
CSVGWriteVisitor::
visit(CSVGFontFaceSrc *fontFaceSrc)
{
  fontFaceSrc->printValues(os_);
}

void
CSVGWriteVisitor::
visit(CSVGFontFaceUri *fontFaceUri)
{
  fontFaceUri->printValues(os_);
}

void
CSVGWriteVisitor::
visit(CSVGGlyph *glyph)
{
  glyph->printValues(os_);
}

void
CSVGWriteVisitor::
visit(CSVGGroup *group)
{
  group->printValues(os_);
}

void
CSVGWriteVisitor::
visit(CSVGHKern *hkern)
{
  hkern->printValues(os_);
}

void
CSVGWriteVisitor::
visit(CSVGImage *image)
{
  image->printValues(os_);
}

void
CSVGWriteVisitor::
visit(CSVGLinearGradient *lg)
{
  lg->printValues(os_);
}

void
CSVGWriteVisitor::
visit(CSVGLine *line)
{
  line->printValues(os_);
}

void
CSVGWriteVisitor::
visit(CSVGMarker *marker)
{
  marker->printValues(os_);
}

void
CSVGWriteVisitor::
visit(CSVGMask *mask)
{
  mask->printValues(os_);
}

void
CSVGWriteVisitor::
visit(CSVGMetaData *metaData)
{
  metaData->printValues(os_);
}

void
CSVGWriteVisitor::
visit(CSVGMPath *mpath)
{
  mpath->printValues(os_);
}

void
CSVGWriteVisitor::
visit(CSVGPath *path)
{
#if 0
  std::ostringstream ss;

  const auto &parts = path->getPartList();

  uint num_parts = parts.size();

  for (uint i = 0; i < num_parts; ++i) {
    const auto *part = parts.part(i);

    auto type = part->getType();

    if      (type == CSVGPathPartType::MOVE_TO) {
      const auto *part1 = dynamic_cast<const CSVGPathMoveTo *>(part);

      const auto &p = part1->getPoint();

      ss << "M " << p.x << " " << p.y << " ";
    }
    else if (type == CSVGPathPartType::LINE_TO) {
      const auto *part1 = dynamic_cast<const CSVGPathLineTo *>(part);

      const auto &p = part1->getPoint();

      ss << "L " << p.x << " " << p.y << " ";
    }
    else if (type == CSVGPathPartType::RLINE_TO) {
      const auto *part1 = dynamic_cast<const CSVGPathRLineTo *>(part);

      const auto &p = part1->getPoint();

      ss << "l " << p.x << " " << p.y << " ";
    }
    else if (type == CSVGPathPartType::HLINE_TO) {
      const auto *part1 = dynamic_cast<const CSVGPathHLineTo *>(part);

      double l = part1->getLength();

      ss << "H " << l << " ";
    }
    else if (type == CSVGPathPartType::VLINE_TO) {
      const auto *part1 = dynamic_cast<const CSVGPathVLineTo *>(part);

      double l = part1->getLength();

      ss << "V " << l << " ";
    }
    else if (type == CSVGPathPartType::ARC_TO) {
      const auto *part1 = dynamic_cast<const CSVGPathArcTo *>(part);

      double rx = part1->getRadiusX();
      double ry = part1->getRadiusY();

      double xa = part1->getXA();

      int fa = part1->getFA();
      int fs = part1->getFS();

      const auto &point2 = part1->getPoint2();

      ss << "A " << rx << " " << ry << " " << xa << " " << fa << " " << fs << " " <<
             point2.x << " " << point2.y << " ";
    }
    else if (type == CSVGPathPartType::RARC_TO) {
      const auto *part1 = dynamic_cast<const CSVGPathRArcTo *>(part);

      double rx = part1->getRadiusX();
      double ry = part1->getRadiusY();

      double xa = part1->getXA();

      int fa = part1->getFA();
      int fs = part1->getFS();

      const auto &point2 = part1->getPoint2();

      ss << "a " << rx << " " << ry << " " << xa << " " << fa << " " << fs << " " <<
             point2.x << " " << point2.y << " ";
    }
    else if (type == CSVGPathPartType::BEZIER2_TO) {
      const auto *part1 = dynamic_cast<const CSVGPathBezier2To *>(part);

      const auto &point1 = part1->getPoint1();
      const auto &point2 = part1->getPoint2();

      ss << "Q " << point1.x << " " << point1.y << " " << point2.x << " " << point2.y << " ";
    }
    else if (type == CSVGPathPartType::RBEZIER2_TO) {
      const auto *part1 = dynamic_cast<const CSVGPathRBezier2To *>(part);

      const auto &point1 = part1->getPoint1();
      const auto &point2 = part1->getPoint2();

      ss << "q " << point1.x << " " << point1.y << " " << point2.x << " " << point2.y << " ";
    }
    else if (type == CSVGPathPartType::BEZIER3_TO) {
      const auto *part1 = dynamic_cast<const CSVGPathBezier3To *>(part);

      const auto &point1 = part1->getPoint1();
      const auto &point2 = part1->getPoint2();
      const auto &point3 = part1->getPoint3();

      ss << "C " << point1.x << " " << point1.y << " " << point2.x << " " << point2.y <<
             " " << point3.x << " " << point3.y << " ";
    }
    else if (type == CSVGPathPartType::RBEZIER3_TO) {
      const auto *part1 = dynamic_cast<const CSVGPathRBezier3To *>(part);

      const auto &point1 = part1->getPoint1();
      const auto &point2 = part1->getPoint2();
      const auto &point3 = part1->getPoint3();

      ss << "c " << point1.x << " " << point1.y << " " << point2.x << " " << point2.y <<
             " " << point3.x << " " << point3.y << " ";
    }
    else if (type == CSVGPathPartType::CLOSE_PATH) {
      ss << "z ";
    }
  }

  auto str = CStrUtil::stripSpaces(ss.str());

  os_ << " d=\"" << str << "\"";
#endif

  path->printValues(os_);
}

void
CSVGWriteVisitor::
visit(CSVGPattern *pattern)
{
  pattern->printValues(os_);
}

void
CSVGWriteVisitor::
visit(CSVGPolygon *polygon)
{
  polygon->printValues(os_);
}

void
CSVGWriteVisitor::
visit(CSVGPolyLine *polyLine)
{
  polyLine->printValues(os_);
}

void
CSVGWriteVisitor::
visit(CSVGRadialGradient *rg)
{
  rg->printValues(os_);
}

void
CSVGWriteVisitor::
visit(CSVGRect *rect)
{
  rect->printValues(os_);
}

void
CSVGWriteVisitor::
visit(CSVGScript *script)
{
  script->printValues(os_);
}

void
CSVGWriteVisitor::
visit(CSVGSet *set)
{
  set->printValues(os_);
}

void
CSVGWriteVisitor::
visit(CSVGStop *stop)
{
  stop->printValues(os_);
}

void
CSVGWriteVisitor::
visit(CSVGStyle *style)
{
  style->printValues(os_);
}

void
CSVGWriteVisitor::
visit(CSVGSwitch *sw)
{
  sw->printValues(os_);
}

void
CSVGWriteVisitor::
visit(CSVGSymbol *symbol)
{
  symbol->printValues(os_);
}

void
CSVGWriteVisitor::
visit(CSVGTBreak *tbreak)
{
  tbreak->printValues(os_);
}

void
CSVGWriteVisitor::
visit(CSVGText *text)
{
  text->printValues(os_);
}

void
CSVGWriteVisitor::
visit(CSVGTextPath *textPath)
{
  textPath->printValues(os_);
}

void
CSVGWriteVisitor::
visit(CSVGTitle *title)
{
  title->printValues(os_);
}

void
CSVGWriteVisitor::
visit(CSVGTSpan *tspan)
{
  tspan->printValues(os_);
}

void
CSVGWriteVisitor::
visit(CSVGUse *use)
{
  use->printValues(os_);
}

//----

// TODO: remove
void
CSVGWriteVisitor::
visit(CSVGObject *object)
{
  std::cerr << "Unhandled object type " << object->getId() << "\n";

  assert(false);
}

// TODO: remove
void
CSVGWriteVisitor::
visit(CSVGAnimateBase *object)
{
  std::cerr << "Unhandled animate type " << object->getId() << "\n";

  assert(false);
}

#if 0
void
CSVGWriteVisitor::
writeStyle(CSVGObject *object) const
{
  auto str = std::string(" style=\"");

  const auto &stroke = object->getStroke();

  auto   scolor   = stroke.getColor().getValue();
  double sopacity = stroke.getOpacity().getValue();
  double width    = stroke.getWidth().getValue();
  auto   dash     = stroke.getDashArray().getValue();
  auto   cap      = stroke.getLineCap().getValue();
  auto   join     = stroke.getLineJoin().getValue();

  if (sopacity > 0.0) {
    str += "stroke: ";

    if      (scolor.isNone())
      str += "none";
    else if (scolor.isCurrent())
      str += "currentColor";
    else
      str += CStrUtil::strprintf("rgb(%d,%d,%d)", scolor.rgba().getRedI(),
                                 scolor.rgba().getGreenI(), scolor.rgba().getBlueI());

    str += ";";

    if (sopacity < 1.0)
      str += CStrUtil::strprintf(" stroke-opacity: %g;", sopacity);
  }
  else
    str += "stroke: none;";

  if (width != 1.0)
    str += CStrUtil::strprintf(" stroke-width: %g;", width);

  if (! dash.isSolid())
    str += CStrUtil::strprintf(" stroke-dasharray: %s;", dash.toString().c_str());

  if      (cap == LINE_CAP_TYPE_ROUND)
    str += CStrUtil::strprintf(" stroke-linecap: round;");
  else if (cap == LINE_CAP_TYPE_SQUARE)
    str += CStrUtil::strprintf(" stroke-linecap: square;");

  if      (join == LINE_JOIN_TYPE_ROUND)
    str += CStrUtil::strprintf(" stroke-linejoin: round;");
  else if (join == LINE_JOIN_TYPE_BEVEL)
    str += CStrUtil::strprintf(" stroke-linejoin: bevel;");

  const auto &fill = object->getFill();

  if (fill.getFillObjectValid()) {
    // TODO
  }
  else {
    double fopacity = 1.0;

    if (fill.getOpacityValid())
      fopacity = fill.getOpacity().getValue();

    if (fill.getColorValid()) {
      auto fcolor = fill.getColor().getValue();

      str += " fill:";

      if (fopacity > 0.0) {
        if      (fcolor.isNone())
          str += "none";
        else if (fcolor.isCurrent())
          str += "currentColor";
        else
          str += CStrUtil::strprintf("rgb(%d,%d,%d)", fcolor.rgba().getRedI(),
                                     fcolor.rgba().getGreenI(), fcolor.rgba().getBlueI());

        str += ";";

        if (fopacity < 1.0)
          str += CStrUtil::strprintf(" fill-opacity: %g;", fopacity);
      }
      else
        str += " fill: none;";
    }
    else
      str += " fill: none;";
  }

  str += "\"";

  os_ << str;
}
#endif

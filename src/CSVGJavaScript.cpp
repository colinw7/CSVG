#include <CSVGJavaScript.h>

#ifdef CSVG_JAVASCRIPT

#include <CSVGJDocument.h>
#include <CSVGJElement.h>
#include <CSVGJEvent.h>
#include <CSVGJTypes.h>

CSVGJavaScript::
CSVGJavaScript(CSVG *svg) :
 CJavaScript(), svg_(svg)
{
}

void
CSVGJavaScript::
init()
{
  objectType_ = addObjectType("SVGElement", CJObjTypeP(new CSVGJElementType(this)));
  eventType_  = addObjectType("SVGEvent"  , CJObjTypeP(new CSVGJEventType(this)));

  document_ = CJValueP(new CSVGJDocument(svg_));
  event_    = CJValueP(new CSVGJEvent(svg_));

  setProperty("document"   , document());
  setProperty("svgDocument", document());

  auto *transformDict = new CJDictionary(this, "");

  setProperty("SVGTransform", CJValueP(transformDict));

  transformDict->setIntegerProperty(this, "SVG_TRANSFORM_TRANSLATE",
                                    long(CMatrixTransformType::TRANSLATE));
  transformDict->setIntegerProperty(this, "SVG_TRANSFORM_SCALE"    ,
                                    long(CMatrixTransformType::SCALE1));
  transformDict->setIntegerProperty(this, "SVG_TRANSFORM_ROTATE"   ,
                                    long(CMatrixTransformType::ROTATE));
  transformDict->setIntegerProperty(this, "SVG_TRANSFORM_MATRIX"   ,
                                    long(CMatrixTransformType::MATRIX));
}

#endif

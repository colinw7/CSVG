#include <CSVGJavaScript.h>
#include <CSVGJDocument.h>
#include <CSVGJObject.h>
#include <CSVGJEvent.h>
#include <CSVGJTransform.h>

CSVGJavaScript::
CSVGJavaScript(CSVG *svg) :
 CJavaScript(), svg_(svg)
{
}

void
CSVGJavaScript::
init()
{
  documentType_       = CJObjectTypeP(new CSVGJDocumentType());
  objectType_         = CJObjectTypeP(new CSVGJObjectType());
  eventType_          = CJObjectTypeP(new CSVGJEventType());
  transformStackType_ = CJObjectTypeP(new CSVGJTransformStackType());
  transformType_      = CJObjectTypeP(new CSVGJTransformType());
  matrixType_         = CJObjectTypeP(new CSVGJMatrixType());

  document_  = CJValueP(new CSVGJDocument(svg_));
  event_     = CJValueP(new CSVGJEvent(svg_));

  setProperty("document"   , document());
  setProperty("svgDocument", document());

  CJDictionary *transformDict = new CJDictionary(this);

  setProperty("SVGTransform", CJValueP(transformDict));

  transformDict->setIntegerProperty(this, "SVG_TRANSFORM_TRANSLATE",
                                    long(CMatrixTransformType::TRANSLATE));
  transformDict->setIntegerProperty(this, "SVG_TRANSFORM_SCALE"    ,
                                    long(CMatrixTransformType::SCALE1));
  transformDict->setIntegerProperty(this, "SVG_TRANSFORM_ROTATE"   ,
                                    long(CMatrixTransformType::ROTATE));
}

#include <CSVGJDocument.h>
#include <CSVGJElement.h>
#include <CSVGJTypes.h>
#include <CSVG.h>
#include <CSVGObject.h>
#include <CSVGJavaScript.h>

CJObjTypeP CSVGJDocumentType::type_;

CJObjTypeP
CSVGJDocumentType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJObjTypeP(new CSVGJDocumentType(js));

    js->addObjectType("document", type_);
  }

  return type_;
}

CSVGJDocumentType::
CSVGJDocumentType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Object, "SVGDocument")
{
}

//------

/*
  interface SVGDocument : Document, DocumentEvent {
    readonly attribute DOMString title;
    readonly attribute DOMString referrer;
    readonly attribute DOMString domain;
    readonly attribute DOMString URL;
    readonly attribute SVGSVGElement rootElement;
  };
*/

/*
  <svg> DOM

  interface SVGSVGElement : SVGElement, SVGTests, SVGLangSpace, SVGExternalResourcesRequired,
                            SVGStylable, SVGLocatable, SVGFitToViewBox, SVGZoomAndPan,
                            DocumentEvent, ViewCSS, DocumentCSS {
    readonly attribute SVGAnimatedLength x;
    readonly attribute SVGAnimatedLength y;
    readonly attribute SVGAnimatedLength width;
    readonly attribute SVGAnimatedLength height;
             attribute DOMString contentScriptType setraises(DOMException);
             attribute DOMString contentStyleType setraises(DOMException);readonly
    readonly attribute SVGRect viewport;
    readonly attribute float pixelUnitToMillimeterX;
    readonly attribute float pixelUnitToMillimeterY;
    readonly attribute float screenPixelToMillimeterX;
    readonly attribute float screenPixelToMillimeterY;
    readonly attribute boolean useCurrentView;
    readonly attribute SVGViewSpec currentView;
                       float currentScale;
    readonly attribute SVGPoint currentTranslate;

    unsigned long suspendRedraw(in unsigned long maxWaitMilliseconds);
    void unsuspendRedraw(in unsigned long suspendHandleID);
    void unsuspendRedrawAll();
    void forceRedraw();
    void pauseAnimations();
    void unpauseAnimations();
    boolean animationsPaused();
    float getCurrentTime();
    void setCurrentTime(in float seconds);
    NodeList getIntersectionList(in SVGRect rect, in SVGElement referenceElement);
    NodeList getEnclosureList(in SVGRect rect, in SVGElement referenceElement);
    boolean checkIntersection(in SVGElement element, in SVGRect rect);
    boolean checkEnclosure(in SVGElement element, in SVGRect rect);
    void deselectAll();
    SVGNumber createSVGNumber();
    SVGLength createSVGLength();
    SVGAngle createSVGAngle();
    SVGPoint createSVGPoint();
    SVGMatrix createSVGMatrix();
    SVGRect createSVGRect();
    SVGTransform createSVGTransform();
    SVGTransform createSVGTransformFromMatrix(in SVGMatrix matrix);
    Element getElementById(in DOMString elementId);
  };
*/
CSVGJDocument::
CSVGJDocument(CSVG *svg) :
 CJObj(svg->js(), CSVGJDocumentType::instance(svg->js())), svg_(svg)
{
  objType_->addObjectFunction(svg->js(), "getElementById");
  objType_->addObjectFunction(svg->js(), "createElementNS");
  objType_->addObjectFunction(svg->js(), "createTextNode");
  objType_->addObjectFunction(svg->js(), "createEvent");

  element_ = CJValueP(new CSVGJDocumentElement(svg));

  setProperty(js_, "documentElement", element_);
  setProperty(js_, "defaultView", CJValueP());
}

CJValueP
CSVGJDocument::
execNameFn(CJavaScript *, const std::string &name, const Values &values)
{
  if      (name == "getElementById") {
    if (values.size() == 2) {
      std::string id = values[1]->toString();

      CSVGObject *obj = svg_->lookupObjectById(id);

      if (obj)
        return CJValueP(new CSVGJElement(obj));
    }

    return CJValueP();
  }
  else if (name == "createElementNS") {
    if (values.size() == 3) {
      std::string name1 = values[2]->toString();

      CSVGObject *obj = svg_->createObjectByName(name1);

      if (obj)
        return CJValueP(new CSVGJElement(obj));
    }

    return CJValueP();
  }
  else if (name == "createTextNode") {
    std::cerr << "CSVGJDocument::createTextNode not implemented" << std::endl;

    return CJValueP();
  }
  else if (name == "createEvent") {
    std::cerr << "CSVGJDocument::createEvent not implemented" << std::endl;

    return CJValueP();
  }
  else
    return CJValueP();
}

//------

CJObjTypeP CSVGJDocumentElementType::type_;

CJObjTypeP
CSVGJDocumentElementType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJObjTypeP(new CSVGJDocumentElementType(js));

    js->addObjectType("documentElement", type_);
  }

  return type_;
}

CSVGJDocumentElementType::
CSVGJDocumentElementType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Object, "SVGDocumentElement")
{
}

CSVGJDocumentElement::
CSVGJDocumentElement(CSVG *svg) :
 CJObj(svg->js(), CSVGJDocumentElementType::instance(svg->js())), svg_(svg)
{
  objType_->addObjectFunction(svg->js(), "createSVGMatrix");
  objType_->addObjectFunction(svg->js(), "createSVGTransformFromMatrix");
  objType_->addObjectFunction(svg->js(), "createSVGPoint");
}

CJValueP
CSVGJDocumentElement::
execNameFn(CJavaScript *js, const std::string &name, const Values &values)
{
  if      (name == "createSVGMatrix") {
    return CJValueP(new CSVGJMatrix(svg_, CMatrix2D()));
  }
  else if (name == "createSVGTransformFromMatrix") {
    CSVGJMatrixP matrix(new CSVGJMatrix(svg_, CMatrix2D()));

    if (values.size() >= 2) {
      CJValueP matrixVal = values[1];

      CSVGJMatrixP matrix1;

      if (matrixVal && matrixVal->type() == CJValue::Type::Object) {
        CJObjP matrixObj = std::static_pointer_cast<CJObj>(matrixVal);

        if (matrixObj->objType()->name() == "SVGMatrix") {
          matrix1 = std::static_pointer_cast<CSVGJMatrix>(matrixObj);
        }
      }

      if (matrix1) {
        matrix->setMatrix(matrix1->matrix());
      }
    }

    return CJValueP(new CSVGJTransform(svg_, matrix));
  }
  else if (name == "createSVGPoint") {
    CJDictionary *dict = new CJDictionary(js, "");

    dict->setRealProperty(js, "x", 0.0);
    dict->setRealProperty(js, "y", 0.0);

    return CJValueP(dict);
  }
  else
    return CJValueP();
}

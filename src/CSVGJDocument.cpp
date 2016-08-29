#include <CSVGJDocument.h>
#include <CSVGJObject.h>
#include <CSVGJTransform.h>
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

CSVGJDocument::
CSVGJDocument(CSVG *svg) :
 CJObj(svg->js(), CSVGJDocumentType::instance(svg->js())), svg_(svg)
{
  type_->addObjectFunction(svg->js(), "getElementById");
  type_->addObjectFunction(svg->js(), "createElementNS");
  type_->addObjectFunction(svg->js(), "createTextNode");
  type_->addObjectFunction(svg->js(), "createEvent");

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
        return CJValueP(new CSVGJObject(obj));
    }

    return CJValueP();
  }
  else if (name == "createElementNS") {
    if (values.size() == 3) {
      std::string name = values[2]->toString();

      CSVGObject *obj = svg_->createObjectByName(name);

      if (obj)
        return CJValueP(new CSVGJObject(obj));
    }

    return CJValueP();
  }
  else if (name == "createTextNode") {
    assert(false);

    return CJValueP();
  }
  else if (name == "createEvent") {
    assert(false);

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
  type_->addObjectFunction(svg->js(), "createSVGMatrix");
  type_->addObjectFunction(svg->js(), "createSVGTransformFromMatrix");
  type_->addObjectFunction(svg->js(), "createSVGPoint");
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

        if (matrixObj->type()->name() == "SVGMatrix") {
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
    CJDictionary *dict = new CJDictionary(js);

    dict->setRealProperty(js, "x", 0.0);
    dict->setRealProperty(js, "y", 0.0);

    return CJValueP(dict);
  }
  else
    return CJValueP();
}

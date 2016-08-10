#include <CSVGJDocument.h>
#include <CSVGJObject.h>
#include <CSVGJTransform.h>
#include <CSVG.h>
#include <CSVGObject.h>
#include <CSVGJavaScript.h>

CJObjectTypeP CSVGJDocumentType::type_;

CJObjectTypeP
CSVGJDocumentType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJObjectTypeP(new CSVGJDocumentType());

    js->addObjectType("document", type_);
  }

  return type_;
}

CSVGJDocumentType::
CSVGJDocumentType() :
 CJObjectType(CJToken::Type::Object, "SVGDocument")
{
}

//------

CSVGJDocument::
CSVGJDocument(CSVG *svg) :
 CJObject(CSVGJDocumentType::instance(svg->js())), svg_(svg)
{
  type_->addFunction(svg->js(), "getElementById");
  type_->addFunction(svg->js(), "createElementNS");
  type_->addFunction(svg->js(), "createTextNode");
  type_->addFunction(svg->js(), "createEvent");

  element_ = CJValueP(new CSVGJDocumentElement(svg));

  setProperty("documentElement", element_);
  setProperty("defaultView", CJValueP());
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

CJObjectTypeP CSVGJDocumentElementType::type_;

CJObjectTypeP
CSVGJDocumentElementType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJObjectTypeP(new CSVGJDocumentElementType());

    js->addObjectType("documentElement", type_);
  }

  return type_;
}

CSVGJDocumentElementType::
CSVGJDocumentElementType() :
 CJObjectType(CJToken::Type::Object, "SVGDocumentElement")
{
}

CSVGJDocumentElement::
CSVGJDocumentElement(CSVG *svg) :
 CJObject(CSVGJDocumentElementType::instance(svg->js())), svg_(svg)
{
  type_->addFunction(svg->js(), "createSVGMatrix");
  type_->addFunction(svg->js(), "createSVGTransformFromMatrix");
}

CJValueP
CSVGJDocumentElement::
execNameFn(CJavaScript *, const std::string &name, const Values &values)
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
        CJObjectP matrixObj = std::static_pointer_cast<CJObject>(matrixVal);

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
  else
    return CJValueP();
}

#include <CSVGJDocument.h>
#include <CSVGJObject.h>
#include <CSVG.h>
#include <CSVGObject.h>

CSVGJDocument::
CSVGJDocument(CSVG *svg) :
 CJObject(svg->jsDocumentType()), svg_(svg)
{
  type_->addFunction("getElementById");
}

CJValueP
CSVGJDocument::
execNameFn(CJavaScript *, const std::string &name, const Values &values)
{
  if (name == "getElementById") {
    if (values.size() == 2) {
      std::string id = values[1]->toString();

      CSVGObject *obj = svg_->lookupObjectById(id);

      CSVGJObject *jobj = new CSVGJObject(obj);

      return CJValueP(jobj);
    }

    return CJValueP();
  }
  else
    return CJValueP();
}

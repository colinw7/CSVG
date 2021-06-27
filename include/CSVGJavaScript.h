#ifndef CSVGJavaScript_H
#define CSVGJavaScript_H

#ifdef CSVG_JAVASCRIPT

#include <CJavaScript.h>

class CSVG;

class CSVGJavaScript : public CJavaScript {
 public:
  CSVGJavaScript(CSVG *svg);

  virtual ~CSVGJavaScript() { }

  virtual void init();

  CJObjTypeP objectType() const { return objectType_; }
  CJObjTypeP eventType () const { return eventType_; }

  CJValueP document() const { return document_; }
  CJValueP event   () const { return event_; }

 protected:
  CSVG*      svg_ { nullptr };
  CJObjTypeP objectType_;
  CJObjTypeP eventType_;
  CJValueP   document_;
  CJValueP   event_;
};

#endif

#endif

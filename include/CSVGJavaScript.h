#ifndef CSVGJavaScript_H
#define CSVGJavaScript_H

#include <CJavaScript.h>

class CSVG;

class CSVGJavaScript : public CJavaScript {
 public:
  CSVGJavaScript(CSVG *svg);

  virtual ~CSVGJavaScript() { }

  virtual void init();

  CJObjectTypeP objectType() const { return objectType_; }
  CJObjectTypeP eventType () const { return eventType_; }

  CJValueP document() const { return document_; }
  CJValueP event   () const { return event_; }

 protected:
  CSVG*         svg_;
  CJObjectTypeP objectType_;
  CJObjectTypeP eventType_;
  CJValueP      document_;
  CJValueP      event_;
};

#endif

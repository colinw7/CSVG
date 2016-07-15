#ifndef CSVGJavaScript_H
#define CSVGJavaScript_H

#include <CJavaScript.h>

class CSVG;

class CSVGJavaScript : public CJavaScript {
 public:
  CSVGJavaScript(CSVG *svg);

  virtual ~CSVGJavaScript() { }

  virtual void init();

  CJObjectTypeP documentType      () const { return documentType_; }
  CJObjectTypeP objectType        () const { return objectType_; }
  CJObjectTypeP eventType         () const { return eventType_; }
  CJObjectTypeP transformStackType() const { return transformStackType_; }
  CJObjectTypeP transformType     () const { return transformType_; }
  CJObjectTypeP matrixType        () const { return matrixType_; }

  CJValueP document() const { return document_; }
  CJValueP event   () const { return event_; }

 protected:
  CSVG*         svg_;
  CJObjectTypeP documentType_;
  CJObjectTypeP objectType_;
  CJObjectTypeP eventType_;
  CJObjectTypeP transformStackType_;
  CJObjectTypeP transformType_;
  CJObjectTypeP matrixType_;
  CJValueP      document_;
  CJValueP      event_;
};

#endif

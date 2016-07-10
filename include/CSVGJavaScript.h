#ifndef CSVGJavaScript_H
#define CSVGJavaScript_H

#include <CJavaScript.h>

class CSVG;

class CSVGJavaScript : public CJavaScript {
 public:
  CSVGJavaScript(CSVG *svg) :
   CJavaScript(), svg_(svg) {
  }

 protected:
  CSVG *svg_;
};

#endif

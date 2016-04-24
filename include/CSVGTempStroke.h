#ifndef CSVGTempStroke_H
#define CSVGTempStroke_H

#include <CSVGStroke.h>
#include <CSVGFill.h>
#include <CSVGClip.h>

class CSVGObject;

class CSVGTempStroke {
 public:
  CSVGTempStroke(CSVGObject &object);
 ~CSVGTempStroke();

 private:
  CSVGObject &object_;
  CSVGStroke  stroke_;
  CSVGFill    fill_;
  CSVGClip    clip_;
};

#endif

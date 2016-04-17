#ifndef CQSVGFeTile_H
#define CQSVGFeTile_H

#include <CQSVGObject.h>
#include <CSVGFeTile.h>

class CQSVG;

class CQSVGFeTile : public CQSVGObject, public CSVGFeTile {
  Q_OBJECT

 public:
  CQSVGFeTile(CQSVG *svg);
};

#endif

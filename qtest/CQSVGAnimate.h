#ifndef CQSVGAnimate_H
#define CQSVGAnimate_H

#include <CQSVGAnimateBase.h>
#include <CSVGAnimate.h>

class CQSVG;

class CQSVGAnimate : public CQSVGAnimateBase, public CSVGAnimate {
  Q_OBJECT

 public:
  CQSVGAnimate(CQSVG *svg);
};

#endif

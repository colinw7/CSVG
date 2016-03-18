#ifndef CRENDERER_2D_FUNC_H
#define CRENDERER_2D_FUNC_H

class CRenderer2DFunc {
 protected:
  double start_, end_;
  uint   nsteps_;

 public:
  CRenderer2DFunc(double start=0.0, double end=1.0, uint nsteps=100) :
   start_(start), end_(end), nsteps_(nsteps) {
  }

  virtual ~CRenderer2DFunc() { }

  double getStart() const { return start_; }
  double getEnd  () const { return end_  ; }

  uint getNSteps() const { return nsteps_; }

  virtual double y(double x) const = 0;
};

#endif

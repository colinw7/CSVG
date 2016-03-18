#ifndef CFreeTypePath2D_H
#define CFreeTypePath2D_H

#include <CFreeType.h>

class CFreeTypePath2D : public CFreeTypePathRenderer {
 public:
  CFreeTypePath2D(CPath2D *path) :
   path_(path) {
  }

  void init() {
    path_->init();
  }

  void moveTo(const CPoint2D &p) {
    path_->moveTo(p);
  }

  void lineTo(const CPoint2D &p) {
    path_->lineTo(p);
  }

  void bezier2To(const CPoint2D &p1, const CPoint2D &p2) {
    path_->bezier2To(p1, p2);
  }

  void bezier3To(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3) {
    path_->bezier3To(p1, p2, p3);
  }

  void close() {
    path_->close();
  }

  void stroke() {
    path_->stroke();
  }

  void fill() {
    path_->setFillType(FILL_TYPE_EVEN_ODD);

    path_->fill();
  }

  CPath2D *getPath() const { return path_; }

 private:
  CPath2D *path_;
};

#endif

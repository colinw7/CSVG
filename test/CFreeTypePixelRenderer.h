#ifndef CFreeTypePixelRenderer_H
#define CFreeTypePixelRenderer_H

#include <CFreeType.h>

class CFreeTypePixelRenderer : public CFreeTypeImageRenderer {
 public:
  CFreeTypePixelRenderer(CPixelRenderer *renderer) :
   renderer_(renderer) {
    renderer_->getForeground(fg_);
  }

 ~CFreeTypePixelRenderer() {
    renderer_->setForeground(fg_);
  }

  void drawPoint(const CIPoint2D &p, double gray) {
    renderer_->setForeground(fg_*gray);

    renderer_->drawPoint(p);
  }

 private:
  CPixelRenderer *renderer_;
  CRGBA           fg_;
};

#endif

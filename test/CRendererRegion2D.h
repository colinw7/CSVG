#ifndef CRENDERER_REGION_2D_H
#define CRENDERER_REGION_2D_H

#include <CCursor.h>
#include <CBBox2D.h>

class CRenderer2D;

class CRendererRegion2D {
 public:
  enum Position {
    POSITION_OUTSIDE,
    POSITION_INSIDE,
    POSITION_TL,
    POSITION_TR,
    POSITION_BL,
    POSITION_BR,
    POSITION_L,
    POSITION_R,
    POSITION_B,
    POSITION_T
  };

  enum Mode {
    MODE_SIZE,
    MODE_ROTATE
  };

 protected:
  CRenderer2D *renderer_;
  bool         active_;
  CBBox2D      bbox_;
  Position     position_;
  Mode         mode_;
  CPoint2D     press_pos_, pos_;

 public:
  CRendererRegion2D(CRenderer2D *renderer);

  virtual ~CRendererRegion2D() { }

  void setActive(bool active) { active_ = active; }

  void setBBox(const CBBox2D &bbox) { bbox_ = bbox; }

  Position getPosition() { return position_; }

  CCursorType getCursor();

  void draw();

  bool update(const CPoint2D &pos);

  void press  (const CPoint2D &pos);
  void move   (const CPoint2D &pos);
  void release(const CPoint2D &pos);

  virtual bool inside(const CPoint2D &pos);

  virtual void move(double dx, double dy);
  virtual void expand(double dx1, double dy1, double dx2, double dy2);
  virtual void rotate(double da, const CPoint2D &c);

 private:
  CRendererRegion2D(const CRendererRegion2D &rhs);
  CRendererRegion2D &operator=(const CRendererRegion2D &rhs);
};

#endif

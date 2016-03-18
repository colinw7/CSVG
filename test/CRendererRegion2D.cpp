#include <CRendererRegion2D.h>
#include <CRenderer2D.h>
#include <CImageMgr.h>

#include <region_images/tlsize1.xpm>
#include <region_images/tlsize2.xpm>
#include <region_images/trsize1.xpm>
#include <region_images/trsize2.xpm>
#include <region_images/blsize1.xpm>
#include <region_images/blsize2.xpm>
#include <region_images/brsize1.xpm>
#include <region_images/brsize2.xpm>
#include <region_images/lsize1.xpm>
#include <region_images/lsize2.xpm>
#include <region_images/rsize1.xpm>
#include <region_images/rsize2.xpm>
#include <region_images/bsize1.xpm>
#include <region_images/bsize2.xpm>
#include <region_images/tsize1.xpm>
#include <region_images/tsize2.xpm>
#include <region_images/tlrotate1.xpm>
#include <region_images/tlrotate2.xpm>
#include <region_images/trrotate1.xpm>
#include <region_images/trrotate2.xpm>
#include <region_images/blrotate1.xpm>
#include <region_images/blrotate2.xpm>
#include <region_images/brrotate1.xpm>
#include <region_images/brrotate2.xpm>

#define XPM_TO_IMAGE(d) \
  CImageMgrInst->lookupImage(CImageXPMSrc(d, sizeof(d)/sizeof(d[0])))

static CImagePtr region_size_tl1;
static CImagePtr region_size_tl2;
static CImagePtr region_size_tr1;
static CImagePtr region_size_tr2;
static CImagePtr region_size_bl1;
static CImagePtr region_size_bl2;
static CImagePtr region_size_br1;
static CImagePtr region_size_br2;
static CImagePtr region_size_l1;
static CImagePtr region_size_l2;
static CImagePtr region_size_r1;
static CImagePtr region_size_r2;
static CImagePtr region_size_b1;
static CImagePtr region_size_b2;
static CImagePtr region_size_t1;
static CImagePtr region_size_t2;
static CImagePtr region_rotate_tl1;
static CImagePtr region_rotate_tl2;
static CImagePtr region_rotate_tr1;
static CImagePtr region_rotate_tr2;
static CImagePtr region_rotate_bl1;
static CImagePtr region_rotate_bl2;
static CImagePtr region_rotate_br1;
static CImagePtr region_rotate_br2;

CRendererRegion2D::
CRendererRegion2D(CRenderer2D *renderer) :
 renderer_(renderer), active_(false), position_(POSITION_OUTSIDE),
 mode_(MODE_ROTATE)
{
  if (! region_size_tl1.isValid()) {
    region_size_tl1 = XPM_TO_IMAGE(tlsize1_data);
    region_size_tl2 = XPM_TO_IMAGE(tlsize2_data);
    region_size_tr1 = XPM_TO_IMAGE(trsize1_data);
    region_size_tr2 = XPM_TO_IMAGE(trsize2_data);
    region_size_bl1 = XPM_TO_IMAGE(blsize1_data);
    region_size_bl2 = XPM_TO_IMAGE(blsize2_data);
    region_size_br1 = XPM_TO_IMAGE(brsize1_data);
    region_size_br2 = XPM_TO_IMAGE(brsize2_data);

    region_size_l1 = XPM_TO_IMAGE(lsize1_data);
    region_size_l2 = XPM_TO_IMAGE(lsize2_data);
    region_size_r1 = XPM_TO_IMAGE(rsize1_data);
    region_size_r2 = XPM_TO_IMAGE(rsize2_data);
    region_size_b1 = XPM_TO_IMAGE(bsize1_data);
    region_size_b2 = XPM_TO_IMAGE(bsize2_data);
    region_size_t1 = XPM_TO_IMAGE(tsize1_data);
    region_size_t2 = XPM_TO_IMAGE(tsize2_data);

    region_rotate_tl1 = XPM_TO_IMAGE(tlrotate1_data);
    region_rotate_tl2 = XPM_TO_IMAGE(tlrotate2_data);
    region_rotate_tr1 = XPM_TO_IMAGE(trrotate1_data);
    region_rotate_tr2 = XPM_TO_IMAGE(trrotate2_data);
    region_rotate_bl1 = XPM_TO_IMAGE(blrotate1_data);
    region_rotate_bl2 = XPM_TO_IMAGE(blrotate2_data);
    region_rotate_br1 = XPM_TO_IMAGE(brrotate1_data);
    region_rotate_br2 = XPM_TO_IMAGE(brrotate2_data);
  }
}

CCursorType
CRendererRegion2D::
getCursor()
{
  switch (position_) {
    case POSITION_TL:     return CURSOR_TYPE_TOP_LEFT_CORNER;
    case POSITION_TR:     return CURSOR_TYPE_TOP_RIGHT_CORNER;
    case POSITION_BL:     return CURSOR_TYPE_BOTTOM_LEFT_CORNER;
    case POSITION_BR:     return CURSOR_TYPE_BOTTOM_RIGHT_CORNER;
    case POSITION_L:      return CURSOR_TYPE_LEFT_SIDE;
    case POSITION_R:      return CURSOR_TYPE_RIGHT_SIDE;
    case POSITION_B:      return CURSOR_TYPE_BOTTOM_SIDE;
    case POSITION_T:      return CURSOR_TYPE_TOP_SIDE;
    case POSITION_INSIDE: return CURSOR_TYPE_FLEUR;
    case POSITION_OUTSIDE:return CURSOR_TYPE_TOP_LEFT_ARROW;
    default:              return CURSOR_TYPE_TOP_LEFT_ARROW;
  }
}

void
CRendererRegion2D::
draw()
{
  if (! active_) return;

  double dashes[] = { 3, 3 };

  renderer_->setForeground(CRGBA(0,0,0));

  renderer_->setLineWidth(0);

  double l;

  renderer_->pixelLengthToWindowLength(12, &l);

  double l2 = l/2;

  double x1 = bbox_.getXMin(), y1 = bbox_.getYMin();
  double x2 = bbox_.getXMax(), y2 = bbox_.getYMax();

  double xm = (x1 + x2)/2;
  double ym = (y1 + y2)/2;

  if (mode_ == MODE_SIZE) {
    renderer_->drawAlphaImage(CPoint2D(x1 - l, y2 + l),
      (position_ == POSITION_TL ? region_size_tl2 : region_size_tl1));
    renderer_->drawAlphaImage(CPoint2D(x2    , y2 + l),
      (position_ == POSITION_TR ? region_size_tr2 : region_size_tr1));
    renderer_->drawAlphaImage(CPoint2D(x1 - l, y1    ),
      (position_ == POSITION_BL ? region_size_bl2 : region_size_bl1));
    renderer_->drawAlphaImage(CPoint2D(x2    , y1    ),
      (position_ == POSITION_BR ? region_size_br2 : region_size_br1));

    renderer_->drawAlphaImage(CPoint2D(x1 -  l, ym + l2),
      (position_ == POSITION_L ? region_size_l2 : region_size_l1));
    renderer_->drawAlphaImage(CPoint2D(x2     , ym + l2),
      (position_ == POSITION_R ? region_size_r2 : region_size_r1));
    renderer_->drawAlphaImage(CPoint2D(xm - l2, y1     ),
      (position_ == POSITION_B ? region_size_b2 : region_size_b1));
    renderer_->drawAlphaImage(CPoint2D(xm - l2, y2 + l ),
      (position_ == POSITION_T ? region_size_t2 : region_size_t1));
  }
  else {
    renderer_->drawAlphaImage(CPoint2D(x1 - l, y2 + l),
      (position_ == POSITION_TL ? region_rotate_tl2 : region_rotate_tl1));
    renderer_->drawAlphaImage(CPoint2D(x2    , y2 + l),
      (position_ == POSITION_TR ? region_rotate_tr2 : region_rotate_tr1));
    renderer_->drawAlphaImage(CPoint2D(x1 - l, y1    ),
      (position_ == POSITION_BL ? region_rotate_bl2 : region_rotate_bl1));
    renderer_->drawAlphaImage(CPoint2D(x2    , y1    ),
      (position_ == POSITION_BR ? region_rotate_br2 : region_rotate_br1));

    renderer_->drawAlphaImage(CPoint2D(x1 -  l, ym + l2),
      (position_ == POSITION_L ? region_size_b2 : region_size_b1));
    renderer_->drawAlphaImage(CPoint2D(x2     , ym + l2),
      (position_ == POSITION_R ? region_size_t2 : region_size_t1));
    renderer_->drawAlphaImage(CPoint2D(xm - l2, y1     ),
      (position_ == POSITION_B ? region_size_l2 : region_size_l1));
    renderer_->drawAlphaImage(CPoint2D(xm - l2, y2 + l ),
      (position_ == POSITION_T ? region_size_r2 : region_size_r1));
  }

  renderer_->setLineDash(CLineDash(dashes, 2));

  renderer_->drawRectangle(bbox_);
}

bool
CRendererRegion2D::
update(const CPoint2D &pos)
{
  double l;

  renderer_->pixelLengthToWindowLength(12, &l);

  double l2 = l/2;

  double x1 = bbox_.getXMin(), y1 = bbox_.getYMin();
  double x2 = bbox_.getXMax(), y2 = bbox_.getYMax();

  double xm = (x1 + x2)/2;
  double ym = (y1 + y2)/2;

  bool changed = false;

  Position position = POSITION_OUTSIDE;

  if      (pos.x >= x1 - l  && pos.x <= x1     ) {
    if      (pos.y >= y2      && pos.y <= y2 + l )
      position = POSITION_TL;
    else if (pos.y >= y1 - l  && pos.y <= y1     )
      position = POSITION_BL;
    else if (pos.y >= ym - l2 && pos.y <= ym + l2)
      position = POSITION_L;
  }
  else if (pos.x >= x2      && pos.x <= x2 + l ) {
    if      (pos.y >= y2      && pos.y <= y2 + l )
      position = POSITION_TR;
    else if (pos.y >= y1 - l  && pos.y <= y1     )
      position = POSITION_BR;
    else if (pos.y >= ym - l2 && pos.y <= ym + l2)
      position = POSITION_R;
  }
  else if (pos.x >= xm - l2 && pos.x <= xm + l2) {
    if      (pos.y >= y2      && pos.y <= y2 + l )
      position = POSITION_T;
    else if (pos.y >= y1 - l  && pos.y <= y1     )
      position = POSITION_B;
  }

  if (position == POSITION_OUTSIDE) {
    if (inside(pos))
      position = POSITION_INSIDE;
  }

  if (position != position_) {
    changed   = true;
    position_ = position;
  }

  return changed;
}

void
CRendererRegion2D::
press(const CPoint2D &pos)
{
  press_pos_ = pos;

  pos_ = pos;

  if (mode_ == MODE_ROTATE) {
    if      (position_ == POSITION_TL)
      pos_ = bbox_.getUR();
    else if (position_ == POSITION_TR)
      pos_ = bbox_.getUR();
    else if (position_ == POSITION_BL)
      pos_ = bbox_.getLL();
    else if (position_ == POSITION_BR)
      pos_ = bbox_.getLR();
  }
}

void
CRendererRegion2D::
move(const CPoint2D &pos)
{
  CPoint2D delta = pos - pos_;

  if (mode_ == MODE_SIZE) {
    if      (position_ == POSITION_L)
      expand(delta.x, 0, 0, 0);
    else if (position_ == POSITION_R)
      expand(0, 0, delta.x, 0);
    else if (position_ == POSITION_B)
      expand(0, delta.y, 0, 0);
    else if (position_ == POSITION_T)
      expand(0, 0, 0, delta.y);
    else if (position_ == POSITION_TL)
      expand(delta.x, 0, 0, delta.y);
    else if (position_ == POSITION_TR)
      expand(0, 0, delta.x, delta.y);
    else if (position_ == POSITION_BL)
      expand(delta.x, delta.y, 0, 0);
    else if (position_ == POSITION_BR)
      expand(0, delta.y, delta.x, 0);
    else if (position_ == POSITION_INSIDE)
      move(delta.x, delta.y);
  }
  else {
    if (position_ == POSITION_L ||
        position_ == POSITION_R ||
        position_ == POSITION_B ||
        position_ == POSITION_T ||
        position_ == POSITION_INSIDE) {
      if      (position_ == POSITION_L)
        expand(0, -delta.y, 0, delta.y);
      else if (position_ == POSITION_R)
        expand(0, -delta.y, 0, delta.y);
      else if (position_ == POSITION_B)
        expand(-delta.x, 0, delta.x, 0);
      else if (position_ == POSITION_T)
        expand(-delta.x, 0, delta.x, 0);
      else if (position_ == POSITION_INSIDE)
        move(delta.x, delta.y);
    }
    else {
      CPoint2D c = bbox_.getCenter();

      double dx1 = pos_.x - c.x, dy1 = pos_.y - c.y;
      double dx2 = pos .x - c.x, dy2 = pos .y - c.y;

      double a1 = atan2(dy1, dx1);
      double a2 = atan2(dy2, dx2);

      double da = a2 - a1;

      rotate(da, c);
    }
  }

  pos_ = pos;
}

void
CRendererRegion2D::
release(const CPoint2D &pos)
{
  if (pos == press_pos_)
    mode_ = (mode_ == MODE_SIZE ? MODE_ROTATE : MODE_SIZE);

  pos_ = pos;
}

bool
CRendererRegion2D::
inside(const CPoint2D &pos)
{
  double x1 = bbox_.getXMin(), y1 = bbox_.getYMin();
  double x2 = bbox_.getXMax(), y2 = bbox_.getYMax();

  return (pos.x >= x1 && pos.x <= x2 && pos.y >= y1 && pos.y <= y2);
}

void
CRendererRegion2D::
move(double dx, double dy)
{
  bbox_.moveBy(CPoint2D(dx, dy));
}

void
CRendererRegion2D::
expand(double dx1, double dy1, double dx2, double dy2)
{
  bbox_.expand(dx1, dy1, dx2, dy2);
}

void
CRendererRegion2D::
rotate(double da, const CPoint2D &c)
{
  CPoint2D pmin = bbox_.getMin().rotate(c, da);
  CPoint2D pmax = bbox_.getMax().rotate(c, da);

  bbox_ = CBBox2D(pmin, pmax);
}

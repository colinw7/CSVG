#include <CPixelClip.h>
#include <CLine2D.h>
#include <CThrow.h>

class CPixelClip1 {
 public:
  typedef std::vector<CIPoint2D>    IPointList;
  typedef std::vector<IPointList *> IPointListList;

 private:
  CFillType      type_;
  IPointListList poly_points_list_;

 public:
  CPixelClip1();
  CPixelClip1(const CPixelClip1 &clip1);

  CPixelClip1(CFillType type, const IPointListList &poly_point_list);

 ~CPixelClip1();

  CFillType getType() const { return type_; }

  const IPointListList &getPolygons() const {
    return poly_points_list_;
  }

  uint getNumPolygons() const {
    return poly_points_list_.size();
  }

  uint getNumPoints() const {
    uint num = 0;

    uint num_polygons = poly_points_list_.size();

    for (uint i = 0; i < num_polygons; ++i)
      num += poly_points_list_[i]->size();

    return num;
  }

  IPointList *getPolygonPoints(uint i) {
    return poly_points_list_[i];
  }

  int getPolygonSize(uint i) const {
    return poly_points_list_[i]->size();
  }

  void init();

  const CPixelClip1 &operator=(const CPixelClip1 &clip1);
};

//-------------

CPixelClipMgr::
CPixelClipMgr()
{
}

void
CPixelClipMgr::
resetLines()
{
  lines_.clear();
}

void
CPixelClipMgr::
addClipLine(const CIPoint2D &point1, const CIPoint2D &point2)
{
  lines_.push_back(CILine2D(point1, point2));
}

void
CPixelClipMgr::
copyLines1()
{
  uint num_lines = lines_.size();

  lines1_.resize(num_lines);

  for (uint i = 0; i < num_lines; ++i)
    lines1_[i] = lines_[i];
}

//-------------

CPixelClip::
CPixelClip()
{
  mgr_ = CPixelClipMgrInst;

  init();
}

CPixelClip::
CPixelClip(const CPixelClip &clip) :
 mgr_(clip.mgr_)
{
  assign(clip);
}

CPixelClip::
~CPixelClip()
{
  term();
}

const CPixelClip &
CPixelClip::
operator=(const CPixelClip &clip)
{
  assign(clip);

  return *this;
}

void
CPixelClip::
init()
{
  uint num_clips = clips_.size();

  for (uint i = 0; i < num_clips; ++i)
    clips_[i].init();

  clips_.clear();
}

void
CPixelClip::
assign(const CPixelClip &clip)
{
  uint num_clips = clip.clips_.size();

  clips_.resize(num_clips);

  for (uint i = 0 ; i < num_clips; ++i)
    clips_[i] = clip.clips_[i];
}

void
CPixelClip::
term()
{
  clips_.clear();
}

void
CPixelClip::
processLine(int x1, int y1, int x2, int y2,
            int **xx1, int **yy1, int **xx2, int **yy2, uint *num)
{
  return processLine1(clips_, x1, y1, x2, y2, xx1, yy1, xx2, yy2, num);
}

void
CPixelClip::
processLine1(Clip1List &clips, int x1, int y1, int x2, int y2,
             int **xx1, int **yy1, int **xx2, int **yy2, uint *num)
{
  static std::vector<int> ret_x1;
  static std::vector<int> ret_y1;
  static std::vector<int> ret_x2;
  static std::vector<int> ret_y2;

  mgr_->resetLines();

  mgr_->addClipLine(CIPoint2D(x1, y1), CIPoint2D(x2, y2));

  uint num_clips = clips.size();

  for (uint i = 0; i < num_clips; ++i) {
    if (mgr_->getNumLines() <= 0)
      break;

    mgr_->copyLines1();

    mgr_->resetLines();

    uint num_lines1 = mgr_->getNumLines1();

    for (uint j = 0; j < num_lines1; ++j)
      processLine2(clips[i], mgr_->getLine1(j));
  }

  uint num_lines = mgr_->getNumLines();

  ret_x1.resize(num_lines);
  ret_y1.resize(num_lines);
  ret_x2.resize(num_lines);
  ret_y2.resize(num_lines);

  for (uint i = 0; i < num_lines; ++i) {
    const CILine2D &line = mgr_->getLine(i);

    ret_x1[i] = line.start().x;
    ret_y1[i] = line.start().y;
    ret_x2[i] = line.end  ().x;
    ret_y2[i] = line.end  ().y;
  }

  *xx1 = &ret_x1[0];
  *yy1 = &ret_y1[0];
  *xx2 = &ret_x2[0];
  *yy2 = &ret_y2[0];
  *num = num_lines;
}

void
CPixelClip::
processLine2(CPixelClip1 &clip1, const CILine2D &line)
{
  static std::vector<CIPoint2D> points;

  points.resize(clip1.getNumPoints() + 2);

  points[0].x = line.start().x;
  points[0].y = line.start().y;
  points[1].x = line.end  ().x;
  points[1].y = line.end  ().y;

  uint ni = 2;

  uint num_polygons = clip1.getNumPolygons();

  for (uint j = 0; j < num_polygons; ++j) {
    IPointList *point_list = clip1.getPolygonPoints(j);

    uint num_points = point_list->size();

    if (num_points < 3)
      continue;

    uint i1 = num_points - 1;
    uint i2 = 0;

    for ( ; i2 < num_points; i1 = i2, ++i2) {
      CIPoint2D &point1 = (*point_list)[i1];
      CIPoint2D &point2 = (*point_list)[i2];

      CLine2D line1(points[0].x, points[0].y, points[1].x, points[1].y);
      CLine2D line2(point1.x, point1.y, point2.x, point2.y);

      CPoint2D ipoint;
      double   mu1, mu2;

      if (line1.intersect(line2, ipoint, &mu1, &mu2) ==
           CMathGen::INTERSECT_NONE)
        continue;

      if (mu1 < 0.0 || mu1 > 1.0)
        continue;

      if (mu2 < 0.0 || mu2 > 1.0)
        continue;

      points[ni++] = CIPoint2D(CMathGen::Round(ipoint.x),
                               CMathGen::Round(ipoint.y));
    }
  }

  //-------

  uint i;

  do {
    i = 0;

    for (uint i1 = i; i1 < ni - 1; ++i1) {
      if (points[i1].x > points[i1 + 1].x) {
        std::swap(points[i1], points[i1 + 1]);

        i = i1;
      }
    }
  } while (i > 0 && i < ni - 1);

  for (i = 0; i < ni - 1; ++i) {
    int xm = (points[i].x + points[i + 1].x)/2;
    int ym = (points[i].y + points[i + 1].y)/2;

    if (isInside(clip1, xm, ym))
      mgr_->addClipLine(points[i], points[i + 1]);
  }
}

struct CPixelClipIData {
  int x, xm, o;
};

bool
CPixelClip::
isInside(CPixelClip1 &clip1, int px, int py) const
{
  typedef std::vector<CPixelClipIData> IDataList;

  static IDataList idata_list;

  //-------

  uint poly_num_xy = clip1.getNumPoints();

  if (poly_num_xy <= 0)
    return false;

  idata_list.resize(poly_num_xy);

  //-------

  uint ni = 0;

  uint num_polygons = clip1.getNumPolygons();

  for (uint j = 0; j < num_polygons; ++j) {
    IPointList *points = clip1.getPolygonPoints(j);

    uint num_points = points->size();

    if (num_points < 3)
      continue;

    uint i1 = num_points - 1;
    uint i2 = 0;

    int x1 = (*points)[i1].x;
    int y1 = (*points)[i1].y;

    int x, xm, o;

    for ( ; i2 < num_points; i1 = i2, ++i2) {
      int x2 = (*points)[i2].x;
      int y2 = (*points)[i2].y;

      if ((y1 < py && y2 < py) || (y1 > py && y2 > py) || y1 == y2)
        goto next;

      xm = (x1 + x2)/2;

      if      (py == y1)
        x = x1;
      else if (py == y2)
        x = x2;
      else {
        double factor = double(x2 - x1)/double(y2 - y1);

        x = CMathGen::Round((py - y1)*factor + x1);
      }

      o = CMathGen::sign(long(y2 - y1));

      idata_list[ni].x  = x;
      idata_list[ni].xm = xm;
      idata_list[ni].o  = o;

      ++ni;

 next:
      x1 = x2;
      y1 = y2;
    }
  }

  if (ni == 0)
    return false;

  //------

  // Sort

  uint i;

  do {
    i = 0;

    for (uint i1 = i, i2 = i1 + 1; i1 < ni - 1; i1 = i2++) {
      if ((idata_list[i1].x >  idata_list[i2].x) ||
          (idata_list[i1].x  == idata_list[i2].x  &&
           idata_list[i1].xm >  idata_list[i2].xm)) {
        std::swap(idata_list[i1], idata_list[i2]);

        i = i1;

        break;
      }
    }
  } while (i > 0 && i < ni - 1);

  // Remove duplicates

  i = 0;

  for (uint i1 = i, i2 = i1 + 1; i2 < ni; i1 = i2++) {
    if (idata_list[i1].x  == idata_list[i2].x  &&
        idata_list[i1].xm == idata_list[i2].xm &&
        idata_list[i1].o  == idata_list[i2].o) {
      for (uint j1 = i2, j2 = j1 + 1; j2 < ni; j1 = j2++)
        idata_list[j1] = idata_list[j2];

      --ni;
      --i2;
    }
  }

  idata_list.resize(ni);

  //------

  if (clip1.getType() == FILL_TYPE_WINDING) {
    int i1 = 0;

    for (uint i = 0; i < ni; ++i)
      if (idata_list[i].x < px)
        i1 += idata_list[i].o;

    return (i1 != 0);
  }
  else {
    if (px < idata_list[0].x)
      return false;

    uint j = 1;

    for (uint i = 1; i < ni; ++i) {
      if (px > idata_list[i - 1].x && px < idata_list[i].x)
        return (j == 1);

      j = 1 - j;
    }

    return (j == 1);
  }
}

bool
CPixelClip::
isEmpty() const
{
  return clips_.empty();
}

uint
CPixelClip::
getSize() const
{
  return clips_.size();
}

void
CPixelClip::
addPolygons(CFillType type, const IPointListList &poly_point_list)
{
  CPixelClip1 clip1(type, poly_point_list);

  clips_.push_back(clip1);
}

const CPixelClip::IPointListList &
CPixelClip::
getPolygons(uint i) const
{
  if (i < 1 || i > clips_.size())
    CTHROW("Invalid Subscript");

  const CPixelClip1 &clip1 = clips_[i - 1];

  return clip1.getPolygons();
}

uint
CPixelClip::
getNumPolygons(uint i) const
{
  if (i < 1 || i > clips_.size())
    CTHROW("Invalid Subscript");

  const CPixelClip1 &clip1 = clips_[i - 1];

  return clip1.getNumPolygons();
}

uint
CPixelClip::
getNumPolygonsPoints(uint i) const
{
  if (i < 1 || i > clips_.size())
    CTHROW("Invalid Subscript");

  const CPixelClip1 &clip1 = clips_[i - 1];

  uint num = 0;

  uint num_polygons = clip1.getNumPolygons();

  for (uint j = 0; j < num_polygons; ++j)
    num += clip1.getPolygonSize(j);

  return num;
}

//---------------

CPixelClip1::
CPixelClip1()
{
  type_ = FILL_TYPE_WINDING;
}

CPixelClip1::
CPixelClip1(const CPixelClip1 &clip)
{
  type_ = clip.type_;

  uint num_polygons1 =      poly_points_list_.size();
  uint num_polygons2 = clip.poly_points_list_.size();

  poly_points_list_.resize(num_polygons2);

  for (uint i = 0; i < num_polygons2; ++i) {
    if (i >= num_polygons1)
      poly_points_list_[i] = new IPointList;

    uint num_points2 = clip.poly_points_list_[i]->size();

    poly_points_list_[i]->resize(num_points2);

          IPointList *points1 =      poly_points_list_[i];
    const IPointList *points2 = clip.poly_points_list_[i];

    for (uint j = 0; j < num_points2; ++j)
      (*points1)[j] = (*points2)[j];
  }

  poly_points_list_.resize(num_polygons2);
}

CPixelClip1::
CPixelClip1(CFillType type, const IPointListList &poly_points_list)
{
  type_ = type;

  uint num_polygons1 = poly_points_list_.size();
  uint num_polygons2 = poly_points_list .size();

  poly_points_list_.resize(num_polygons2);

  for (uint i = 0; i < num_polygons2; ++i) {
    if (i >= num_polygons1)
      poly_points_list_[i] = new IPointList;

    uint num_points2 = poly_points_list[i]->size();

    poly_points_list_[i]->resize(num_points2);

          IPointList *points1 = poly_points_list_[i];
    const IPointList *points2 = poly_points_list [i];

    for (uint j = 0; j < num_points2; ++j)
      (*points1)[j] = (*points2)[j];
  }

  poly_points_list_.resize(num_polygons2);
}

CPixelClip1::
~CPixelClip1()
{
}

void
CPixelClip1::
init()
{
  uint num_polygons = poly_points_list_.size();

  for (uint i = 0; i < num_polygons; ++i)
    poly_points_list_[i]->clear();

  poly_points_list_.clear();
}

const CPixelClip1 &
CPixelClip1::
operator=(const CPixelClip1 &clip1)
{
  type_ = clip1.type_;

  uint num_polygons1 =       poly_points_list_.size();
  uint num_polygons2 = clip1.poly_points_list_.size();

  poly_points_list_.resize(num_polygons2);

  for (uint i = 0; i < num_polygons2; ++i) {
    if (i >= num_polygons1)
      poly_points_list_[i] = new IPointList;

    uint num_points2 = clip1.poly_points_list_[i]->size();

    poly_points_list_[i]->resize(num_points2);

          IPointList *points1 =       poly_points_list_[i];
    const IPointList *points2 = clip1.poly_points_list_[i];

    for (uint j = 0; j < num_points2; ++j)
      (*points1)[j] = (*points2)[j];
  }

  poly_points_list_.resize(num_polygons2);

  return *this;
}

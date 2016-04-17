#include <CMathGeom2D.h>
#include <CMathGen.h>
#include <CPoint2D.h>
#include <CLine2D.h>
#include <CArcToBezier.h>
#include <CBezierToLine.h>

using std::vector;
using std::list;
using std::map;

namespace {
double EPSILON_E6 = 1E-6;
}

//! intersect polygon (points) and bbox and return the result in (ipoints) if successful
bool
CMathGeom2D::
IntersectPolygon(const vector<CPoint2D> &points, const CBBox2D &bbox, vector<CPoint2D> &ipoints)
{
  std::vector<CPoint2D> points1;

  points1.push_back(bbox.getLL());
  points1.push_back(bbox.getLR());
  points1.push_back(bbox.getUR());
  points1.push_back(bbox.getUL());

  return IntersectPolygons(points, points1, ipoints);
}

//! intersect two polygons (x1,y1,n1) and (x2,y2,n2) and return the
//! result in (xi, yi, ni) if successful
//!
//! Note: the caller is responsible for freeing xi and yi
bool
CMathGeom2D::
IntersectPolygons(const double *x1, const double *y1, uint n1,
                  const double *x2, const double *y2, uint n2,
                  double **xi, double **yi, uint *ni)
{
  *xi = NULL;
  *yi = NULL;
  *ni = 0;

  // convert input polygons to vector of points
  vector<CPoint2D> vpoints1, vpoints2;

  vpoints1.resize(n1);
  vpoints2.resize(n2);

  for (uint i = 0; i < n1; ++i) vpoints1[i] = CPoint2D(x1[i], y1[i]);
  for (uint i = 0; i < n2; ++i) vpoints2[i] = CPoint2D(x2[i], y2[i]);

  // call actual implementation
  vector<CPoint2D> vipoints;

  if (! IntersectPolygons(vpoints1, vpoints2, vipoints))
    return false;

  // convert result point vector to return array
  *ni = vipoints.size();
  *xi = new double [*ni + 1];
  *yi = new double [*ni + 1];

  for (uint i = 0; i < *ni; ++i) {
    (*xi)[i] = vipoints[i].x;
    (*yi)[i] = vipoints[i].y;
  }

  return true;
}

//! intersect two polygons (points1,num_points1) and (points2,num_points2)
//! and return the result in (ipoints, num_ipoints) if successful
//!
//! Note: *num_ipoints is expected to contain the size of the ipoints array
//! the routine will only return the result if this is large enough to
//! contain all the intersection points
bool
CMathGeom2D::
IntersectPolygons(const CPoint2D *points1, uint num_points1,
                  const CPoint2D *points2, uint num_points2,
                  CPoint2D *ipoints, uint *num_ipoints)
{
  // convert input polygons to vector of points
  vector<CPoint2D> vpoints1, vpoints2;

  vpoints1.resize(num_points1);
  vpoints2.resize(num_points2);

  for (uint i = 0; i < num_points1; ++i) vpoints1[i] = points1[i];
  for (uint i = 0; i < num_points2; ++i) vpoints2[i] = points2[i];

  // call actual implementation
  vector<CPoint2D> vipoints;

  if (! IntersectPolygons(vpoints1, vpoints2, vipoints)) {
    *num_ipoints = 0;
    return false;
  }

  // store result point vector in return array if enough room
  uint ni = vipoints.size();

  if (ni > *num_ipoints) { // not enough room
    *num_ipoints = 0;
    return false;
  }

  for (uint i = 0; i < ni; ++i)
    ipoints[i] = vipoints[i];

  return true;
}

//! intersect two polygons (points1) and (points2) and return the
//! result in (ipoints) if successful
//!
//! Note: this the main implementation - all variants call this
bool
CMathGeom2D::
IntersectPolygons(const vector<CPoint2D> &points1, const vector<CPoint2D> &points2,
                  vector<CPoint2D> &ipoints)
{
  static CPoint2D *f[2];
  static uint      num_f;

  ipoints.clear();

  uint num_points1 = points1.size();
  uint num_points2 = points2.size();

  // fail if polygons are degenerate
  if (num_points1 < 3 || num_points2 < 3)
    return false;

  CPolygonOrientation orient1 = PolygonOrientation(points1);
  CPolygonOrientation orient2 = PolygonOrientation(points2);

  // max number of intersection
  uint ni = num_points1*num_points2;

  // make sure intersection buffer is large enough
  if (num_f < ni) {
    num_f = ni;

    delete [] f[0];
    delete [] f[1];

    f[0] = new CPoint2D [num_f];
    f[1] = new CPoint2D [num_f];
  }

  // store polygon one in start point array
  // Note: if orients don't match we invert the first polygon's point order
  int l1 = 0;

  ni = num_points1;

  if (orient1 == orient2) {
    for (uint i = 0; i < ni; ++i)
      f[l1][i] = points1[i];
  }
  else {
    for (uint i = 0, j = ni - 1; i < ni; ++i, --j)
      f[l1][i] = points1[j];
  }

  // intersect current set of points with each line (end1, end2)
  // of the second polygon (points2)
  CPoint2D end1 = points2[num_points2 - 1];

  for (uint i = 0; i < num_points2; ++i) {
    CPoint2D end2 = points2[i];

    // l2 is destination point index (inverse of current l1)
    int l2 = 1 - l1;

    // calc line coefficients
    double ca = end2.x - end1.x; // (x2 - x1), (y2 - y1)
    double cb = end1.y - end2.y; // (x2 - x1), (y2 - y1)
    double cc = -end1.x*cb - end1.y*ca; // -x1*(y2 - y1) - y1*(x2 - x1)

    // calc side of line for first point
    CPoint2D v1     = f[l1][ni - 1];
    double   fv1    = ca*v1.y + cb*v1.x + cc;
    double   absfv1 = fabs(fv1);

    int index1 = 0;

    if (absfv1 >= EPSILON_E6)
      index1 = CMathGen::sign(fv1)*orient2;

    int ni1 = 0;

    for (uint j = 0; j < ni; ++j) {
      // calc side of line for second point
      CPoint2D v2     = f[l1][j];
      double   fv2    = ca*v2.y + cb*v2.x + cc;
      double   absfv2 = fabs(fv2);

      int index2 = 0;

      if (absfv2 >= EPSILON_E6)
        index2 = CMathGen::sign(fv2)*orient2;

      // add start point
      if (index1 >= 0)
        f[l2][ni1++] = v1;

      // add intersection point (if changed sides)
      if (index1 != 0 && index1 != index2 && index2 != 0) {
        double delta = absfv1 + absfv2;

        double xi = (absfv2*v1.x + absfv1*v2.x)/delta;
        double yi = (absfv2*v1.y + absfv1*v2.y)/delta;

        f[l2][ni1++] = CPoint2D(xi, yi);
      }

      // move to next line
      v1     = v2;
      absfv1 = absfv2;
      index1 = index2;
    }

    // degenerate result so fail
    if (ni1 < 3)
      return false;

    l1   = l2;
    end1 = end2;
    ni   = ni1;
  }

  ipoints.resize(ni);

  for (uint i = 0; i < ni; ++i)
    ipoints[i] = f[l1][i];

  return true;
}

struct CutPoint {
  enum Type {
    POLYGON,
    INTERSECT
  };

  CPoint2D p;
  Type     type;
  uint     id;

  CutPoint() { }

  CutPoint(const CPoint2D &p1, Type type1, uint id1) :
   p(p1), type(type1), id(id1) {
  }
};

typedef vector<CutPoint>    CutPointArray;
typedef list<CutPointArray> CutPointArrayList;

//! intersect two polygons (points1) and (points2) and return the
//! result in (cpoints) if successful
bool
CMathGeom2D::
CutPolygons(const vector<CPoint2D> &points1, const vector<CPoint2D> &points2,
            vector<CPoint2D> &cpoints)
{
  std::vector< vector<CPoint2D> > cpoints1;

  if (! CutPolygons(points1, points2, cpoints1))
    return false;

  cpoints = cpoints1.front();

  return true;
}

bool
CMathGeom2D::
CutPolygons(const vector<CPoint2D> &points1, const vector<CPoint2D> &points2,
            std::vector< vector<CPoint2D> > &cpoints)
{
  static CutPoint *f[2];
  static uint      num_f;

  CutPointArrayList opoints;

  uint num_points1 = points1.size();
  uint num_points2 = points2.size();

  // fail if polygons are degenerate
  if (num_points1 < 3 || num_points2 < 3)
    return false;

  CPolygonOrientation orient1 = PolygonOrientation(points1);
  CPolygonOrientation orient2 = PolygonOrientation(points2);

  // max number of intersection
  uint ni = num_points1*num_points2;

  // make sure intersection buffer is large enough
  if (num_f < ni) {
    num_f = ni;

    delete [] f[0];
    delete [] f[1];

    f[0] = new CutPoint [num_f];
    f[1] = new CutPoint [num_f];
  }

  // store polygon one in start point array
  // Note: if orients don't match we invert the first polygon's point order
  int l1 = 0;

  ni = num_points1;

  if (orient1 == orient2) {
    for (uint i = 0; i < ni; ++i) {
      f[l1][i].p    = points1[i];
      f[l1][i].type = CutPoint::POLYGON;
      f[l1][i].id   = i;
    }
  }
  else {
    for (uint i = 0, j = ni - 1; i < ni; ++i, --j) {
      f[l1][i].p    = points1[j];
      f[l1][i].type = CutPoint::POLYGON;
      f[l1][i].id   = j;
    }
  }

  CutPointArray opoints1;

  uint ci = 0;

  // intersect current set of points with each line (end1, end2)
  // of the second polygon (points2)
  CPoint2D end1 = points2[num_points2 - 1];

  for (uint i = 0; i < num_points2; ++i) {
    opoints1.clear();

    CPoint2D end2 = points2[i];

    // l2 is destination point index (inverse of current l1)
    int l2 = 1 - l1;

    // calc line coefficients
    double ca = end2.x - end1.x; // (x2 - x1), (y2 - y1)
    double cb = end1.y - end2.y; // (x2 - x1), (y2 - y1)
    double cc = -end1.x*cb - end1.y*ca; // -x1*(y2 - y1) - y1*(x2 - x1)

    // calc side of line for first point
    CutPoint v1     = f[l1][ni - 1];
    double   fv1    = ca*v1.p.y + cb*v1.p.x + cc;
    double   absfv1 = fabs(fv1);

    int index1 = 0;

    if (absfv1 >= EPSILON_E6)
      index1 = CMathGen::sign(fv1)*orient2;

    int ni1 = 0;

    for (uint j = 0; j < ni; ++j) {
      // calc side of line for second point
      CutPoint v2     = f[l1][j];
      double   fv2    = ca*v2.p.y + cb*v2.p.x + cc;
      double   absfv2 = fabs(fv2);

      int index2 = 0;

      if (absfv2 >= EPSILON_E6)
        index2 = CMathGen::sign(fv2)*orient2;

      // add start point to inside
      if (index1 >= 0)
        f[l2][ni1++] = v1;

      // add outside point
      if (index1 <= 0)
        opoints1.push_back(v1);

      // add intersection point (if changed sides)
      if (index1 != 0 && index1 != index2 && index2 != 0) {
        double delta = absfv1 + absfv2;

        double xi = (absfv2*v1.p.x + absfv1*v2.p.x)/delta;
        double yi = (absfv2*v1.p.y + absfv1*v2.p.y)/delta;

        CPoint2D pi(xi, yi);

        CutPoint cp(pi, CutPoint::INTERSECT, ci++);

        // add intersection point to inside
        f[l2][ni1++] = cp;

        // add intersection point to outside
        opoints1.push_back(cp);
      }

      // move to next line
      v1     = v2;
      absfv1 = absfv2;
      index1 = index2;
    }

    // degenerate result so fail
    if (ni1 < 3)
      return false;

    l1   = l2;
    end1 = end2;
    ni   = ni1;

    // add outside polygon if not degenerate (should test >= 3 points)
    if (! opoints1.empty())
      opoints.push_back(opoints1);
  }

  //----

  // build outside boundaries stitching at common intersection points
  uint num_opoints = opoints.size();

  while (num_opoints >= 2) {
    CutPointArrayList::iterator p2 = opoints.begin();
    CutPointArrayList::iterator p1 = p2++;

    CutPointArray &opoints0 = *p1;
    CutPointArray &opoints1 = *p2;

    uint num_opoints1 = opoints0.size();
    uint num_opoints2 = opoints1.size();

    uint pos1  = 0;
    uint pos2  = 0;
    bool found = false;

    // find common points
    for (uint i = 0; i < num_opoints1; ++i) {
      for (uint j = 0; j < num_opoints2; ++j) {
        if (opoints0[i].type == opoints1[j].type &&
            opoints0[i].id   == opoints1[j].id) {
          pos1  = i;
          pos2  = j;
          found = true;
          break;
        }
      }

      if (found) break;
    }

    if (! found) break;

    // merge polygons at common point
    CutPointArray npoints;

    for (uint i = 0; i < pos1; ++i)
      npoints.push_back(opoints0[i]);

    for (uint j = pos2 + 1; j < num_opoints2; ++j)
      npoints.push_back(opoints1[j]);

    for (uint j = 0; j < pos2; ++j)
      npoints.push_back(opoints1[j]);

    for (uint i = pos1 + 1; i < num_opoints1; ++i)
      npoints.push_back(opoints0[i]);

    opoints0 = npoints;

    opoints.erase(p2);

    --num_opoints;
  }

  if (num_opoints < 1)
    return false;

  //----

  // set return array of cut polygons
  CutPointArrayList::const_iterator p1, p2;

  for (p1 = opoints.begin(), p2 = opoints.end(); p1 != p2; ++p1) {
    const CutPointArray &opoints1 = *p1;

    uint num_opoints1 = opoints1.size();

    vector<CPoint2D> cpoints1;

    for (uint j = 0; j < num_opoints1; ++j)
      cpoints1.push_back(opoints1[j].p);

    cpoints.push_back(cpoints1);
  }

  return true;
}

struct AddPoint {
  enum Type {
    POLY,
    INTERSECT,
    SKIP
  };

  double x, y;
  Type   type;
  uint   id1, id2;
  double s;

  AddPoint() { }

  AddPoint(double x1, double y1, Type type1, uint id11=0, uint id22=0, double s1=1.0) :
   x(x1), y(y1), type(type1), id1(id11), id2(id22), s(s1) {
  }

  friend bool operator<(const AddPoint &lhs, const AddPoint &rhs) {
    return (lhs.s < rhs.s);
  }
};

typedef vector<AddPoint> AddPointArray;
typedef list<AddPoint>   AddPointList;

//! add two polygons (points1) and (points2) and return the
//! result in (ipoints) if successful
bool
CMathGeom2D::
AddPolygons(const vector<CPoint2D> &points1, const vector<CPoint2D> &points2,
            vector<CPoint2D> &ipoints)
{
  ipoints.clear();

  uint num_points1 = points1.size();
  uint num_points2 = points2.size();

  // fail if polygons are degenerate
  if (num_points1 < 3 || num_points2 < 3)
    return false;

  // store polygon one in start point array
  // Note: if orients don't match we invert the first polygon's point order
  CPolygonOrientation orient1 = PolygonOrientation(points1);
  CPolygonOrientation orient2 = PolygonOrientation(points2);

  AddPointArray apoints1;
  AddPointArray apoints2;

  if (orient1 == orient2) {
    for (uint i = 0; i < num_points1; ++i)
      apoints1.push_back(AddPoint(points1[i].x, points1[i].y, AddPoint::POLY));
  }
  else {
    for (uint i = 0, j = num_points1 - 1; i < num_points1; ++i, --j)
      apoints1.push_back(AddPoint(points1[j].x, points1[j].y, AddPoint::POLY));
  }

  for (uint i = 0; i < num_points2; ++i)
    apoints2.push_back(AddPoint(points2[i].x, points2[i].y, AddPoint::POLY));

  // intersect first set of points with each line of the second set
  // of points to build first point set
  AddPointArray aipoints2;

  for (uint i1 = num_points2 - 1, i2 = 0; i2 < num_points2; i1 = i2++) {
    AddPoint &end1 = apoints2[i1];
    AddPoint &end2 = apoints2[i2];

    aipoints2.push_back(end1);

    AddPointList ti;

    // calc line coefficients
    double ca = end2.x - end1.x; // (x2 - x1), (y2 - y1)
    double cb = end1.y - end2.y; // (x2 - x1), (y2 - y1)
    double cc = -end1.x*cb - end1.y*ca; // -x1*(y2 - y1) - y1*(x2 - x1)

    for (uint j1 = num_points1 - 1, j2 = 0; j2 < num_points1; j1 = j2++) {
      // calc side of line for first point
      AddPoint &v1     = apoints1[j1];
      double    fv1    = ca*v1.y + cb*v1.x + cc;
      double    absfv1 = fabs(fv1);

      int index1 = 0;

      if (absfv1 >= EPSILON_E6)
        index1 = CMathGen::sign(fv1)*orient2;

      // calc side of line for second point
      AddPoint &v2     = apoints1[j2];
      double    fv2    = ca*v2.y + cb*v2.x + cc;
      double    absfv2 = fabs(fv2);

      int index2 = 0;

      if (absfv2 >= EPSILON_E6)
        index2 = CMathGen::sign(fv2)*orient2;

      // add intersection point (if changed sides)
      if (index1 != 0 && index1 != index2 && index2 != 0) {
        double delta = absfv1 + absfv2;

        double xi = (absfv2*v1.x + absfv1*v2.x)/delta;
        double yi = (absfv2*v1.y + absfv1*v2.y)/delta;

        double s;

        if (fabs(ca) > fabs(cb))
          s = (xi - end1.x)/ca;
        else
          s = (end1.y - yi)/cb;

        // ensure intersection is internal
        if (s >= 0.0 && s <= 1.0) {
          CPoint2D pi(xi, yi);

          AddPoint ap(xi, yi, AddPoint::INTERSECT, j1, i1, s);

          ti.push_back(ap);
        }
      }
    }

    // sort intersection points by line distance parameter and add to list
    ti.sort();

    AddPointList::const_iterator p1, p2;

    for (p1 = ti.begin(), p2 = ti.end(); p1 != p2; ++p1)
      aipoints2.push_back(*p1);
  }

  //----

  // intersect second set of points with each line of the first
  // set of points to build first point set
  AddPointArray aipoints1;

  for (uint j1 = num_points1 - 1, j2 = 0; j2 < num_points1; j1 = j2++) {
    AddPoint &end1 = apoints1[j1];
    AddPoint &end2 = apoints1[j2];

    aipoints1.push_back(end1);

    AddPointList ti;

    // calc line coefficients
    double ca = end2.x - end1.x; // (x2 - x1), (y2 - y1)
    double cb = end1.y - end2.y; // (x2 - x1), (y2 - y1)
    double cc = -end1.x*cb - end1.y*ca; // -x1*(y2 - y1) - y1*(x2 - x1)

    for (uint i1 = num_points2 - 1, i2 = 0; i2 < num_points2; i1 = i2++) {
      // calc side of line for first point
      AddPoint &v1     = apoints2[i1];
      double    fv1    = ca*v1.y + cb*v1.x + cc;
      double    absfv1 = fabs(fv1);

      int index1 = 0;

      if (absfv1 >= EPSILON_E6)
        index1 = CMathGen::sign(fv1)*orient2;

      // calc side of line for second point
      AddPoint &v2     = apoints2[i2];
      double    fv2    = ca*v2.y + cb*v2.x + cc;
      double    absfv2 = fabs(fv2);

      int index2 = 0;

      if (absfv2 >= EPSILON_E6)
        index2 = CMathGen::sign(fv2)*orient2;

      // add intersection point (if changed sides)
      if (index1 != 0 && index1 != index2 && index2 != 0) {
        double delta = absfv1 + absfv2;

        double xi = (absfv2*v1.x + absfv1*v2.x)/delta;
        double yi = (absfv2*v1.y + absfv1*v2.y)/delta;

        double s;

        if (fabs(ca) > fabs(cb))
          s = (xi - end1.x)/ca;
        else
          s = (end1.y - yi)/cb;

        // ensure intersection is internal
        if (s >= 0.0 && s <= 1.0) {
          CPoint2D pi(xi, yi);

          AddPoint ap(xi, yi, AddPoint::INTERSECT, j1, i1, s);

          ti.push_back(ap);
        }
      }
    }

    // sort intersection points by line distance parameter and add to list
    ti.sort();

    AddPointList::const_iterator p1, p2;

    for (p1 = ti.begin(), p2 = ti.end(); p1 != p2; ++p1)
      aipoints1.push_back(*p1);
  }

  //------

  // skip points inside other polygon

  uint num_aipoints1 = aipoints1.size();

  for (uint i = 0; i < num_aipoints1; ++i) {
    if (aipoints1[i].type != AddPoint::INTERSECT &&
        PointInsideConvex(aipoints1[i].x, aipoints1[i].y, &points2[0], num_points2))
      aipoints1[i].type = AddPoint::SKIP;
  }

  uint num_aipoints2 = aipoints2.size();

  for (uint i = 0; i < num_aipoints2; ++i) {
    if (aipoints2[i].type != AddPoint::INTERSECT &&
        PointInsideConvex(aipoints2[i].x, aipoints2[i].y, &points1[0], num_points1))
      aipoints2[i].type = AddPoint::SKIP;
  }

  //------

  // combine two list of points by switching lists at each intersection
  AddPointArray aipoints;

  uint i = 0;

  while (i < num_aipoints1) {
    if (aipoints1[i].type == AddPoint::SKIP) {
      ++i;
      continue;
    }

    aipoints.push_back(aipoints1[i]);

    if (aipoints1[i].type == AddPoint::INTERSECT) {
      // if intersection find matching point in other list
      uint j = 0;

      for ( ; j < num_aipoints2; ++j) {
        if (aipoints2[j].type != AddPoint::INTERSECT)
          continue;

        if (aipoints1[i].id1 == aipoints2[j].id1 &&
            aipoints1[i].id2 == aipoints2[j].id2)
          break;
      }

      // not found - fail
      if (j >= num_aipoints2)
        return false;

      // add points from next point after matching intersection
      // until next intersection
      ++j;

      if (j >= num_aipoints2) j = 0;

      uint k = 0;

      while (aipoints2[j].type != AddPoint::INTERSECT) {
        if (aipoints2[j].type != AddPoint::SKIP)
          aipoints.push_back(aipoints2[j]);

        ++j;

        if (j >= num_aipoints2) j = 0;

        ++k;

        if (k >= num_aipoints2) // catch infinite loop - just in case
          return false;
      }

      // find matching intersection in original list to complete the detour
      ++i;

      while (i < num_aipoints1) {
        if (aipoints1[i].type != AddPoint::INTERSECT) {
          ++i;
          continue;
        }

        if (aipoints1[i].id1 != aipoints2[j].id1 ||
            aipoints1[i].id2 != aipoints2[j].id2)
          return false;

        break;
      }

      if (i >= num_aipoints1)
        return false;

      aipoints.push_back(aipoints1[i]);

      ++i;
    }
    else
      ++i;
  }

  //------

  // set the return array
  uint num_aipoints = aipoints.size();

  for (uint i = 0; i < num_aipoints; ++i)
    ipoints.push_back(CPoint2D(aipoints[i].x, aipoints[i].y));

  //------

  return true;
}

//--------------------------

bool
CMathGeom2D::
PointInsideConvex(const CPoint2D &point, const vector<CPoint2D> &points)
{
  return PointInsideConvex(point, &points[0], points.size());
}

//! point inside polygon
//! Note: polygon must be convex
bool
CMathGeom2D::
PointInsideConvex(const CPoint2D &point, const CPoint2D *points, uint num_points)
{
  CPolygonOrientation orient = PolygonOrientation(points, num_points);

  if (orient == CPOLYGON_ORIENTATION_UNKNOWN) return false;

  if (orient == CPOLYGON_ORIENTATION_ANTICLOCKWISE) {
    int i1 = num_points - 1;

    // iterate forwards through polygon lines (i1 -> i2)
    for (int i2 = 0; i2 < (int) num_points; i1 = i2++) {
      // test orientation of triangle made from point and
      // line matches polygon orientation
      double f = (points[i2].x - points[i1].x)*(point.y - points[i1].y) -
                 (points[i2].y - points[i1].y)*(point.x - points[i1].x);

      if (f < 0)
        return false;
    }
  }
  else {
    int i1 = 0;

    // iterate backwards through polygon lines (i1 -> i2)
    for (int i2 = num_points - 1; i2 >= 0; i1 = i2--) {
      // test orientation of triangle made from point and
      // line matches polygon orientation
      double f = (points[i2].x - points[i1].x)*(point.y - points[i1].y) -
                 (points[i2].y - points[i1].y)*(point.x - points[i1].x);

      if (f < 0)
        return false;
    }
  }

  return true;
}

//! point inside polygon
//! (Calls above)
bool
CMathGeom2D::
PointInsideConvex(double x, double y, const CPoint2D *points, uint num_points)
{
  return PointInsideConvex(CPoint2D(x, y), points, num_points);
}

//! point inside polygon
//! Note: polygon must be convex
bool
CMathGeom2D::
PointInsideConvex(double x, double y, const double *px, const double *py, uint np)
{
  CPolygonOrientation orient = PolygonOrientation(px, py, np);

  if (orient == CPOLYGON_ORIENTATION_UNKNOWN) return false;

  if (orient == CPOLYGON_ORIENTATION_ANTICLOCKWISE) {
    int i1 = np - 1;

    // iterate forwards through polygon lines (i1 -> i2)
    for (int i2 = 0; i2 < (int) np; i1 = i2++) {
      // test orientation of triangle made from point and
      // line matches polygon orientation
      double f = (px[i2] - px[i1])*(y - py[i1]) - (py[i2] - py[i1])*(x - px[i1]);

      if (f < 0)
        return false;
    }
  }
  else {
    int i1 = 0;

    // iterate backwards through polygon lines (i1 -> i2)
    for (int i2 = np - 1; i2 >= 0; i1 = i2--) {
      // test orientation of triangle made from point and
      // line matches polygon orientation
      double f = (px[i2] - px[i1])*(y - py[i1]) - (py[i2] - py[i1])*(x - px[i1]);

      if (f < 0)
        return false;
    }
  }

  return true;
}

bool
CMathGeom2D::
PointInsideEvenOdd(const CPoint2D &point, const vector<CPoint2D> &points)
{
  return PointInsideEvenOdd(point, &points[0], points.size());
}

//! point inside polygon (variant 1)
//!
//! count number of intersections of horizontal line at specified
//! point with polygons lines to the right. If convex then this should
//! be odd if inside
bool
CMathGeom2D::
PointInsideEvenOdd(const CPoint2D &point, const CPoint2D *points, uint num_points)
{
  double xinters;

  int counter = 0;

  int             i2     = num_points - 1;
  const CPoint2D *point2 = &points[i2];

  // iterate through all lines of the polygon
  for (int i1 = 0; i1 < (int) num_points; ++i1) {
    const CPoint2D *point1 = &points[i1];

    // intersect current line with horizontal line at inside point
    if (point.y > std::min(point1->y, point2->y)) {
      if (point.y <= std::max(point1->y, point2->y)) {
        if (point.x <= std::max(point1->x, point2->x)) {
          if (point1->y != point2->y) {
            // if we have an intersection, increase count
            xinters = (point.y   - point1->y)*(point2->x - point1->x)/
                      (point2->y - point1->y) + point1->x;

            if (point1->x == point2->x || point.x <= xinters)
              ++counter;
          }
        }
      }
    }

    // next line
    i2     = i1;
    point2 = point1;
  }

  // if odd then success
  return ((counter % 2) != 0);
}

//! point inside polygon (variant 1)
//! (calls above)
bool
CMathGeom2D::
PointInsideEvenOdd(double x, double y, const CPoint2D *points, uint num_points)
{
  return PointInsideEvenOdd(CPoint2D(x, y), points, num_points);
}

//! point inside polygon (variant 1)
//!
//! count number of intersections of horizontal line at specified
//! point with polygons lines to the right. If convex then this should
//! be odd if inside
bool
CMathGeom2D::
PointInsideEvenOdd(double x, double y, const double *xp, const double *yp, uint np)
{
  double xinters, x1, y1, x2, y2;

  int counter = 0;

  int i2 = np - 1;

  x2 = xp[i2]; y2 = yp[i2];

  // iterate through all lines of the polygon
  for (int i1 = 0; i1 < (int) np; i2 = i1++, x2 = x1, y2 = y1) {
    x1 = xp[i1]; y1 = yp[i1];

    // intersect current line with horizontal line at inside point
    if (y > std::min(y1, y2)) {
      if (y <= std::max(y1, y2)) {
        if (x <= std::max(x1, x2)) {
          if (y1 != y2) {
            // if we have an intersection, increase count
            xinters = (y - y1)*(x2 - x1)/(y2 - y1) + x1;

            if (x1 == x2 || x <= xinters)
              ++counter;
          }
        }
      }
    }
  }

  // if odd then success
  return ((counter % 2) != 0);
}

#if 0
// Old Variation of above
// Use boolean instead of count to trace even/odd

bool
CMathGeom2D::
PointInside1(double x, double y, double *xp, double *yp, uint np)
{
  bool c = false;

  int i2 = np - 1;

  double dx, dy;

  x2 = xp[i2]; y2 = yp[i2];

  // iterate through all lines of the polygon
  for (int i1 = 0; i1 < (int) np; i2 = i1++, x2 = x1, y2 = y1) {
    x1 = xp[i1]; y1 = yp[i1];

    // intersect current line with horizontal line at inside point
    if (((y1 <= y) && (y < y2)) || ((y2 <= y) && (y < y1))) {
      dy = y2 - y1;

      if (fabs(dy) < EPSILON_E6)
        continue;

      dx = x2 - x1;

      // if we have an intersection, flip bit
      if ((x < dx*(y - y1)/dy + x1))
        c = ! c;
    }
  }

  return c;
}
#endif

bool
CMathGeom2D::
PointInsideByAngle(const CPoint2D &point, const vector<CPoint2D> &points)
{
  return PointInsideByAngle(point, &points[0], points.size());
}

//! point inside polygon (variant 2)
//!
//! calc angle made by point and ends of each polygon line
//! if angle total is greater or equal to PI (180 degrees) it's inside
bool
CMathGeom2D::
PointInsideByAngle(double x, double y, const double *xp, const double *yp, uint np)
{
  double x1, y1, x2, y2;

  double angle = 0;

  int i = np - 1;

  // iterate through all lines of the polygon
  for (int j = 0; j < (int) np; i = j++) {
    // add angle between two vectors from point to ends of line
    x1 = xp[i] - x;
    y1 = yp[i] - y;
    x2 = xp[j] - x;
    y2 = yp[j] - y;

    angle += IncludedAngle(x1, y1, x2, y2);
  }

  return (fabs(angle) >= M_PI);
}

//! point inside polygon (variant 2)
//! (calls above)
bool
CMathGeom2D::
PointInsideByAngle(double x, double y, const CPoint2D *points, uint num_points)
{
  return PointInsideByAngle(CPoint2D(x, y), points, num_points);
}

//! point inside polygon (variant 2)
//!
//! calc angle made by point and ends of each polygon line
//! if angle total is greater or equal to PI (180 degrees) it's inside
bool
CMathGeom2D::
PointInsideByAngle(const CPoint2D &point, const CPoint2D *points, uint num_points)
{
  double x1, y1, x2, y2;

  double angle = 0;

  int i = num_points - 1;

  // iterate through all lines of the polygon
  for (int j = 0; j < (int) num_points; i = j++) {
    // add angle between two vectors from point to ends of line
    x1 = points[i].x - point.x;
    y1 = points[i].y - point.y;
    x2 = points[j].x - point.x;
    y2 = points[j].y - point.y;

    angle += IncludedAngle(x1, y1, x2, y2);
  }

  return (fabs(angle) >= M_PI);
}

//-------

//! point inside polygon (variant 3)
//!
//! check if point is inside using single line test using
//! algorithm from PointInsideConvex
bool
CMathGeom2D::
PointInsideConvex1(double x, double y, const double *xp, const double *yp, uint np)
{
  CPolygonOrientation orient = PolygonOrientation(xp, yp, np);

  double d = (y - yp[0])*(xp[1] - xp[0]) - (x - xp[0])*(yp[1] - yp[0]);

  return ((d < 0 && orient < 0) || (d > 0 && orient > 0)); // TODO: test
}

//-------

//! Check if point is inside rectangle
bool
CMathGeom2D::
PointInsideRect(double x, double y, double xmin, double ymin, double xmax, double ymax)
{
  // ensure rectangle is properly ordered (assert ?)
  if (xmin > xmax) std::swap(xmin, xmax);
  if (ymin > ymax) std::swap(ymin, ymax);

  return (x >= xmin && x <= xmax && y >= ymin && y <= ymax);
}

//! Check if point is inside rectangle
//! (calls above)
bool
CMathGeom2D::
PointInsideRect(const CPoint2D &point, const CBBox2D &rect)
{
  // TODO: needed - CBBox2D has this code already
  return PointInsideRect(point.x, point.y, rect.getXMin(), rect.getYMin(),
                         rect.getXMax(), rect.getYMax());
}

//-------

//! Arc through three points ?
bool
CMathGeom2D::
ArcThrough(const CPoint2D &point1, const CPoint2D &point2, const CPoint2D &point3, double r,
           CPoint2D &center, CPoint2D &t1, CPoint2D &t2)
{
  return ArcThrough(point1.x, point1.y, point2.x, point2.y,
                    point3.x, point3.y, r, &center.x, &center.y,
                    &t1.x, &t1.y, &t2.x, &t2.y);
}

//! Arc through three points ?
bool
CMathGeom2D::
ArcThrough(double x1, double y1, double x2, double y2, double x3, double y3, double r,
           double *xc, double *yc, double *xt1, double *yt1, double *xt2, double *yt2)
{
  return ArcThrough(x1, y1, x2, y2, x3, y3, r, r, xc, yc, xt1, yt1, xt2, yt2);
}

//! Arc through three points ?
bool
CMathGeom2D::
ArcThrough(double x1, double y1, double x2, double y2, double x3, double y3, double xr, double yr,
           double *xc, double *yc, double *xt1, double *yt1, double *xt2, double *yt2)
{
  if (xr <= 0.0 || yr <= 0.0)
    return false;

  double s1 = (x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1);
  double s2 = (x3 - x2)*(x3 - x2) + (y3 - y2)*(y3 - y2);
  double s3 = (x3 - x1)*(x3 - x1) + (y3 - y1)*(y3 - y1);

  if (s1 == 0.0 || s2 == 0.0)
    return false;

  double d1 = sqrt(s1);
  double d2 = sqrt(s2);

  double cs = fabs((s1 + s2 - s3)/(2.0*d1*d2));

  double theta = CMathGen::acos(cs);

  if (s1 + s2 - s3 < 0)
    theta = M_PI - theta;

  if (theta == 0.0)
    return false;

  double xd = 0.0;
  double yd = 0.0;

  if (theta != M_PI)  {
    xd = xr/tan(theta/2.0);
    yd = yr/tan(theta/2.0);
  }

  *xt1 = x2 + xd*(x1 - x2)/d1;
  *yt1 = y2 + yd*(y1 - y2)/d1;

  *xt2 = x2 + xd*(x3 - x2)/d2;
  *yt2 = y2 + yd*(y3 - y2)/d2;

  double dc = Hypot(xr, xd);

  double xtm = (*xt1 + *xt2)/2.0;
  double ytm = (*yt1 + *yt2)/2.0;

  double dm = Hypot(xtm - x2, ytm - y2);

  *xc = x2 + dc*(xtm - x2)/dm;
  *yc = y2 + dc*(ytm - y2)/dm;

  return true;
}

bool
CMathGeom2D::
ConvertFromSVGArc(double x1, double y1, double x2, double y2, double phi, double rx, double ry,
                  int fa, int fs, double *cx, double *cy, double *theta, double *delta)
{
  if (x1 == x2 && y1 == y2) return false;

  if (rx == 0.0 || ry == 0.0) return false;

  // Step 1

  double dx2 = (x1 - x2)/2.0;
  double dy2 = (y1 - y2)/2.0;

  double phi1 = CMathGen::DegToRad(phi);

  double c = cos(phi1);
  double s = sin(phi1);

  double xx =  c*dx2 + s*dy2;
  double yy = -s*dx2 + c*dy2;

  rx = fabs(rx);
  ry = fabs(ry);

  double rx2 = rx*rx;
  double ry2 = ry*ry;
  double xx2 = xx*xx;
  double yy2 = yy*yy;

  double rc = xx2/rx2 + yy2/ry2;

  if (rc > 1) {
    double rc2 = sqrt(rc);

    rx *= rc2;
    ry *= rc2;

    rx2 = rx*rx;
    ry2 = ry*ry;
  }

  // Step 2

  int sign = (fa == fs) ? -1 : 1;

  double sq = (rx2*ry2 - rx2*yy2 - ry2*xx2)/(rx2*yy2 + ry2*xx2);

  sq = (sq < 0) ? 0 : sq;

  double coef = sign*sqrt(sq);

  double cx1 =  coef*((rx*yy)/ry);
  double cy1 = -coef*((ry*xx)/rx);

  // Step 3

  double sx2 = (x1 + x2)/2.0;
  double sy2 = (y1 + y2)/2.0;

  *cx = sx2 + c*cx1 - s*cy1;
  *cy = sy2 + s*cx1 + c*cy1;

  // Step 4

  double ux = ( xx - cx1)/rx;
  double uy = ( yy - cy1)/ry;

  double vx = (-xx - cx1)/rx;
  double vy = (-yy - cy1)/ry;

  double mod_u = Hypot(ux, uy);
  double mod_v = ux;

  sign = (uy < 0) ? -1 : 1;

  *theta = sign*acos(mod_v/mod_u);

  *theta = CMathGen::RadToDeg(*theta);

  while (*theta >  360)
    *theta -= 360;

  while (*theta < -360)
    *theta += 360;

  mod_u = sqrt((ux*ux + uy*uy) * (vx*vx + vy*vy));
  mod_v = ux*vx + uy*vy;

  sign = ((ux*vy - uy*vx) < 0) ? -1 : 1;

  *delta = sign*acos(mod_v/mod_u);

  *delta = CMathGen::RadToDeg(*delta);

  if      (fs == 0 && *delta > 0)
    *delta -= 360;
  else if (fs == 1 && *delta < 0)
    *delta += 360;

  while (*delta > 360)
    *delta -= 360;

  while (*delta < -360)
    *delta += 360;

  return true;
}

bool
CMathGeom2D::
ConvertToSVGArc(double cx, double cy, double rx, double ry, double theta, double delta,
                double phi, double *x0, double *y0, double *x1, double *y1, int *fa, int *fs)
{
  // Convert angles to radians

  double theta1 = CMathGen::DegToRad(theta);
  double theta2 = CMathGen::DegToRad(theta + delta);
  double phi_r  = CMathGen::DegToRad(phi);

  // Figure out the coordinates of the beginning and ending points

  *x0 = cx + cos(phi_r) * rx * cos(theta1) + sin(-phi_r) * ry * sin(theta1);
  *y0 = cy + sin(phi_r) * rx * cos(theta1) + cos( phi_r) * ry * sin(theta1);

  *x1 = cx + cos(phi_r) * rx * cos(theta2) + sin(-phi_r) * ry * sin(theta2);
  *y1 = cy + sin(phi_r) * rx * cos(theta2) + cos( phi_r) * ry * sin(theta2);

  *fa = (delta > 180) ? 1 : 0;
  *fs = (delta > 0  ) ? 1 : 0;

  return true;
}

//---------

bool
CMathGeom2D::
LinesAreCoincident(const CLine2D &line1, const CLine2D &line2)
{
  return LinesAreCoincident(line1.start(), line1.end(), line2.start(), line2.end());
}

bool
CMathGeom2D::
LinesAreCoincident(const CPoint2D &p11, const CPoint2D &p12,
                   const CPoint2D &p21, const CPoint2D &p22)
{
  return LinesAreCoincident(p11.x, p11.y, p12.x, p12.y, p21.x, p21.y, p22.x, p22.y);
}

bool
CMathGeom2D::
LinesAreCoincident(double x11, double y11, double x21, double y21,
                   double x12, double y12, double x22, double y22)
{
  // check if parallel
  double offset_x1 = x21 - x11;
  double offset_y1 = y21 - y11;
  double offset_x2 = x22 - x12;
  double offset_y2 = y22 - y12;

  double delta = offset_x1*offset_y2 - offset_y1*offset_x2;

  if (fabs(delta) > EPSILON_E6) return false;

  // check if one point on second line satisfies first

  if (fabs(offset_x1) > fabs(offset_y1)) {
    double m = offset_y1/offset_x1;

    double ty12 = m*(x12 - x11) + y11;

    if (fabs(ty12 - y12) > EPSILON_E6) return false;
  }
  else {
    double m = offset_x1/offset_y1;

    double tx12 = m*(y12 - y11) + x11;

    if (fabs(tx12 - x12) > EPSILON_E6) return false;
  }

  return true;
}

//---------

//! Find intersection between two lines
//!
//! Note: intersection is done on infinite line through line points
//! so intersection need not be on the line segment. Use mu1 and mu2
//! if you need to restrict to line segment. mu1 and mu2 need to be in
//! range 0->1 for intersect in segment
bool
CMathGeom2D::
IntersectLine(double x11, double y11, double x21, double y21,
              double x12, double y12, double x22, double y22,
              double *xi, double *yi, double *mu1, double *mu2)
{
  double offset_x1 = x21 - x11;
  double offset_y1 = y21 - y11;
  double offset_x2 = x22 - x12;
  double offset_y2 = y22 - y12;

  double delta = offset_x1*offset_y2 - offset_y1*offset_x2;

  if (fabs(delta) < EPSILON_E6) { // parallel
    if (xi != NULL) *xi = 0.0;
    if (yi != NULL) *yi = 0.0;

    if (mu1 != NULL) *mu1 = 0.0;
    if (mu2 != NULL) *mu2 = 0.0;

    return false;
  }

  double idelta = 1.0/delta;

  double dx = x12 - x11;
  double dy = y12 - y11;

  double m1 = (dx*offset_y1 - dy*offset_x1)*idelta;
  double m2 = (dx*offset_y2 - dy*offset_x2)*idelta;

  if (xi != NULL) *xi = x11 + m2*offset_x1;
  if (yi != NULL) *yi = y12 + m1*offset_y2;

  if (mu1 != NULL) *mu1 = m2;
  if (mu2 != NULL) *mu2 = m1;

  return true;
}

//! Find intersection between two lines
//!
//! Note: intersection is done on infinite line through line points
//! so intersection need not be on the line segment. Use mu1 and mu2
//! if you need to restrict to line segment. mu1 and mu2 need to be in
//! range 0->1 for intersect in segment
bool
CMathGeom2D::
IntersectLine(const CPoint2D &line1s, const CPoint2D &line1e,
              const CPoint2D &line2s, const CPoint2D &line2e,
              CPoint2D *point, double *mu1, double *mu2)
{
  if (point != NULL)
    return IntersectLine(line1s.x, line1s.y, line1e.x, line1e.y,
                         line2s.x, line2s.y, line2e.x, line2e.y,
                         &point->x, &point->y, mu1, mu2);
  else
    return IntersectLine(line1s.x, line1s.y, line1e.x, line1e.y,
                         line2s.x, line2s.y, line2e.x, line2e.y,
                         NULL, NULL, mu1, mu2);
}

bool
CMathGeom2D::
IntersectLine(const CLine2D &line1, const CLine2D &line2,
              CPoint2D *point, double *mu1, double *mu2)
{
  return IntersectLine(line1.start(), line1.end(), line2.start(), line2.end(), point, mu1, mu2);
}

//-------

//! Orientation on polygon - clockwise/anti-clockwise
CPolygonOrientation
CMathGeom2D::
PolygonOrientation(const double *x, const double *y, uint num_xy)
{
  int i = 2;

  while (i < (int) num_xy) {
    CPolygonOrientation orient =
      PolygonOrientation(x[i - 2], y[i - 2], x[i - 1], y[i - 1], x[i], y[i]);

    if (orient != CPOLYGON_ORIENTATION_UNKNOWN)
      return orient;

    ++i;
  }

  return CPOLYGON_ORIENTATION_UNKNOWN;
}

//! Orientation on polygon - clockwise/anti-clockwise
CPolygonOrientation
CMathGeom2D::
PolygonOrientation(const CPoint2D *points, uint num_points)
{
  int i = 2;

  while (i < (int) num_points) {
    CPolygonOrientation orient = PolygonOrientation(points[i - 2], points[i - 1], points[i]);

    if (orient != CPOLYGON_ORIENTATION_UNKNOWN)
      return orient;

    ++i;
  }

  return CPOLYGON_ORIENTATION_UNKNOWN;
}

//! Orientation on polygon - clockwise/anti-clockwise
CPolygonOrientation
CMathGeom2D::
PolygonOrientation(const vector<CPoint2D> &points)
{
  int i = 2;

  uint num_points = points.size();

  while (i < (int) num_points) {
    CPolygonOrientation orient = PolygonOrientation(points[i - 2], points[i - 1], points[i]);

    if (orient != CPOLYGON_ORIENTATION_UNKNOWN)
      return orient;

    ++i;
  }

  return CPOLYGON_ORIENTATION_UNKNOWN;
}

//! Orientation on polygon - clockwise/anti-clockwise
CPolygonOrientation
CMathGeom2D::
PolygonOrientation(double x1, double y1, double x2, double y2, double x3, double y3)
{
  double dx1 = x2 - x1;
  double dy1 = y2 - y1;

  double dx2 = x3 - x2;
  double dy2 = y3 - y2;

  return (CPolygonOrientation) CMathGen::sign(dx1*dy2 - dy1*dx2);
}

//! Orientation on polygon - clockwise/anti-clockwise
CPolygonOrientation
CMathGeom2D::
PolygonOrientation(const CPoint2D &point1, const CPoint2D &point2, const CPoint2D &point3)
{
  CVector2D d1(point1, point2);
  CVector2D d2(point2, point3);

  return (CPolygonOrientation) CMathGen::sign(d1.getX()*d2.getY() - d1.getY()*d2.getX());
}

//-------

//! Check if polygon is convex
//!
//! For a convex polygon no line intersect with any
//! other line except at vertices
bool
CMathGeom2D::
PolygonIsConvex(const vector<CLine2D> &lines)
{
  uint num_lines = lines.size();

  if (num_lines < 3)
    return false;

  int    s1, s2;
  double dx1, dy1, dx2, dy2;

  bool s1_set = false;

  int i1 = num_lines - 1;

  // iterate through all polygon line pairs
  for (uint i2 = 0; i2 < num_lines; i1 = i2++) {
    // check orientation
    dx1 = lines[i1].end().x - lines[i1].start().x;
    dy1 = lines[i1].end().y - lines[i1].start().y;
    dx2 = lines[i2].end().x - lines[i2].start().x;
    dy2 = lines[i2].end().y - lines[i2].start().y;

    // if orientation not consistent then non-convex
    if (! s1_set)
      s1 = CMathGen::sign(dx1*dy2 - dy1*dx2);
    else {
      s2 = CMathGen::sign(dx1*dy2 - dy1*dx2);

      if (s1 != s2)
        return false;
    }
  }

  return true;
}

//! Check if polygon is convex
//!
//! For a convex polygon no line intersect with any
//! other line except at vertices
bool
CMathGeom2D::
PolygonIsConvex(const list<CLine2D> &lines)
{
  uint num_lines = lines.size();

  if (num_lines < 3)
    return false;

  vector<CLine2D> lines1;

  copy(lines.begin(), lines.end(), back_inserter(lines1));

  return PolygonIsConvex(lines1);
}

//! Check if polygon is convex
//!
//! For a convex polygon no line intersect with any
//! other line except at vertices
bool
CMathGeom2D::
PolygonIsConvex(const double *x, const double *y, int num_xy)
{
  if (num_xy < 3)
    return false;

  int i1 = num_xy - 2;
  int i2 = num_xy - 1;

  bool s1_set = false;

  int    s1, s2;
  double dx1, dy1, dx2, dy2;

  // iterate through all polygon line pairs
  for (int i3 = 0; i3 < num_xy; i1 = i2, i2 = i3++) {
    dx1 = x[i2] - x[i1];
    dy1 = y[i2] - y[i1];
    dx2 = x[i3] - x[i2];
    dy2 = y[i3] - y[i2];

    // if orientation not consistent then non-convex
    if (! s1_set) {
      s1 = CMathGen::sign(dx1*dy2 - dy1*dx2);

      s1_set = true;
    }
    else {
      s2 = CMathGen::sign(dx1*dy2 - dy1*dx2);

      if (s1 != s2)
        return false;
    }
  }

  return true;
}

//-------

//! Calc polygon area (triangle)
double
CMathGeom2D::
PolygonArea(double x1, double y1, double x2, double y2, double x3, double y3)
{
  return fabs(0.5*TriangleArea2(x1, y1, x2, y2, x3, y3));
}

//! Calc polygon area (quad)
double
CMathGeom2D::
PolygonArea(double x1, double y1, double x2, double y2,
            double x3, double y3, double x4, double y4)
{
  return fabs(0.5*PolygonArea2(x1, y1, x2, y2, x3, y3, x4, y4));
}

//! Calc polygon area (generic)
double
CMathGeom2D::
PolygonArea(const double *x, const double *y, uint num_xy)
{
  return fabs(0.5*PolygonArea2(x, y, num_xy));
}

//! Calc polygon area (generic)
double
CMathGeom2D::
PolygonArea(const CPoint2D *points, uint num_points)
{
  return fabs(0.5*PolygonArea2(points, num_points));
}

//! Calc polygon area (generic)
double
CMathGeom2D::
PolygonArea(const vector<CPoint2D> &points)
{
  return fabs(0.5*PolygonArea2(points));
}

//! Calc 2 * polygon area (quad)
double
CMathGeom2D::
PolygonArea2(double x1, double y1, double x2, double y2,
             double x3, double y3, double x4, double y4)
{
  return (x4*y1 - y4*x1 + x1*y2 - y1*x2 +
          x2*y3 - y2*x3 + x3*y4 - y3*x4);
}

//! Calc 2 * polygon area (generic)
double
CMathGeom2D::
PolygonArea2(const double *x, const double *y, uint num_xy)
{
  double area = 0.0;

  int i1 = num_xy - 1;
  int i2 = 0;

  for ( ; i2 < (int) num_xy; i1 = i2++)
    area += x[i1]*y[i2] - y[i1]*x[i2];

  return area;
}

//! Calc 2 * polygon area (generic)
double
CMathGeom2D::
PolygonArea2(const CPoint2D *points, uint num_points)
{
  double area = 0.0;

  int i1 = num_points - 1;
  int i2 = 0;

  for ( ; i2 < (int) num_points; i1 = i2++)
    area += points[i1].x*points[i2].y - points[i1].y*points[i2].x;

  return area;
}

//! Calc 2 * polygon area (generic)
double
CMathGeom2D::
PolygonArea2(const vector<CPoint2D> &points)
{
  uint num_points = points.size();

  double area = 0.0;

  int i1 = num_points - 1;
  int i2 = 0;

  for ( ; i2 < (int) num_points; i1 = i2++)
    area += points[i1].x*points[i2].y - points[i1].y*points[i2].x;

  return area;
}

//! Calc 2 * polygon area (generic)
double
CMathGeom2D::
PolygonArea2(const list<CPoint2D> &points)
{
  double area = 0.0;

  list<CPoint2D>::const_iterator ps = points.begin();
  list<CPoint2D>::const_iterator pe = points.end();

  list<CPoint2D>::const_iterator p3 = ps;
  list<CPoint2D>::const_iterator p1 = p3++;
  list<CPoint2D>::const_iterator p2 = p3++;

  while (p1 != pe) {
    area += TriangleArea2(*p1, *p2, *p3);

    p1 = p2;
    p2 = p3++;

    if (p3 == pe) p3 = ps;
  }

  return area;
}

//-------

//! Calc triangle area
double
CMathGeom2D::
TriangleArea(const CPoint2D &point1, const CPoint2D &point2, const CPoint2D &point3)
{
  return fabs(0.5*TriangleArea2(point1, point2, point3));
}

//! Calc triangle area
double
CMathGeom2D::
TriangleArea(double x1, double y1, double x2, double y2, double x3, double y3)
{
  return fabs(0.5*TriangleArea2(x1, y1, x2, y2, x3, y3));
}

//! Calc 2 * triangle area
double
CMathGeom2D::
TriangleArea2(const CPoint2D &point1, const CPoint2D &point2, const CPoint2D &point3)
{
  return (point2.x - point1.x)*(point3.y - point1.y) -
         (point3.x - point1.x)*(point2.y - point1.y);
}

//! Calc 2 * triangle area
double
CMathGeom2D::
TriangleArea2(double x1, double y1, double x2, double y2, double x3, double y3)
{
  return (x2 - x1)*(y3 - y1) - (x3 - x1)*(y2 - y1);
}

//-------

//! Calc centroid of polygon
void
CMathGeom2D::
PolygonCentroid(const double *x, const double *y, int num_xy, double *xc, double *yc)
{
  *xc = 0.0;
  *yc = 0.0;

  double area = 0.0;

  for (int i = num_xy - 1, j = 0; j < num_xy; i = j++) {
    double xy = (x[i]*y[j]) - (y[i]*x[j]);

    *xc += (x[i] + x[j])*xy;
    *yc += (y[i] + y[j])*xy;

    area += xy;
  }

  double f = 3.0*area;

  *xc /= f;
  *yc /= f;
}

//! Calc centroid of polygon
void
CMathGeom2D::
PolygonCentroid(const std::vector<CPoint2D> &points, CPoint2D &p)
{
  p.x = 0.0;
  p.y = 0.0;

  int num_xy = points.size();

  double area = 0.0;

  for (int i = num_xy - 1, j = 0; j < num_xy; i = j++) {
    double xy = (points[i].x*points[j].y) - (points[i].y*points[j].x);

    p.x += (points[i].x + points[j].x)*xy;
    p.y += (points[i].y + points[j].y)*xy;

    area += xy;
  }

  double f = 3.0*area;

  p.x /= f;
  p.y /= f;
}

//-------

//! Calc included angle between two connected lines
double
CMathGeom2D::
IncludedAngle(double x1, double y1, double x2, double y2, double x3, double y3)
{
  double s1 = (x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1);
  double s2 = (x3 - x2)*(x3 - x2) + (y3 - y2)*(y3 - y2);

  if (s1 == 0.0 || s2 == 0.0)
    return 0.0;

  double d1 = sqrt(s1);
  double d2 = sqrt(s2);

  double s3 = (x3 - x1)*(x3 - x1) + (y3 - y1)*(y3 - y1);

  double cs = fabs((s1 + s2 - s3)/(2.0*d1*d2));

  double theta = CMathGen::acos(cs);

  return theta;
}

//! Calc included angle between two connected lines
double
CMathGeom2D::
IncludedAngle(const CPoint2D &point1, const CPoint2D &point2, const CPoint2D &point3)
{
  double s1 = (point2.x - point1.x)*(point2.x - point1.x) +
              (point2.y - point1.y)*(point2.y - point1.y);
  double s2 = (point3.x - point2.x)*(point3.x - point2.x) +
              (point3.y - point2.y)*(point3.y - point2.y);

  if (s1 == 0.0 || s2 == 0.0)
    return 0.0;

  double d1 = sqrt(s1);
  double d2 = sqrt(s2);

  double s3 = (point3.x - point1.x)*(point3.x - point1.x) +
              (point3.y - point1.y)*(point3.y - point1.y);

  double cs = fabs((s1 + s2 - s3)/(2.0*d1*d2));

  double theta = CMathGen::acos(cs);

  return theta;
}

//! Calc included angle between two lines
double
CMathGeom2D::
IncludedAngle(double x1, double y1, double x2, double y2)
{
  double theta1 = atan2(y1, x1);
  double theta2 = atan2(y2, x2);

  double dtheta = theta2 - theta1;

  while (dtheta > M_PI)
    dtheta -= 2*M_PI;

  while (dtheta < -M_PI)
    dtheta += 2*M_PI;

  return dtheta;
}

//-------

//! Clip line in region
bool
CMathGeom2D::
clipLine(const CLine2D &line, const CBBox2D &bbox, CLine2D &cline)
{
  double x1 = line.start().x;
  double y1 = line.start().y;
  double x2 = line.end  ().x;
  double y2 = line.end  ().y;

  if (clipLine(bbox.getXMin(), bbox.getYMin(), bbox.getXMax(), bbox.getYMax(),
               &x1, &y1, &x2, &y2)) {
    cline = CLine2D(x1, y1, x2, y2);

    return true;
  }

  return false;
}

//! Clip line (x1,y1) -> (x2,y2) in region (xmin,xmax,ymin,ymax)
bool
CMathGeom2D::
clipLine(int xmin, int ymin, int xmax, int ymax, int *x1, int *y1, int *x2, int *y2)
{
  double rx1 = *x1;
  double ry1 = *y1;
  double rx2 = *x2;
  double ry2 = *y2;

  if (! clipLine(xmin, ymin, xmax, ymax, &rx1, &ry1, &rx2, &ry2))
    return false;

  *x1 = (int) rx1;
  *y1 = (int) ry1;
  *x2 = (int) rx2;
  *y2 = (int) ry2;

  if (*x1 < xmin || *x1 > xmax || *y1 < ymin || *y1 > ymax ||
      *x2 < xmin || *x2 > xmax || *y2 < ymin || *y2 > ymax)
    return false;

  return true;
}

//! Clip line (x1,y1) -> (x2,y2) in region (xmin,xmax,ymin,ymax)
bool
CMathGeom2D::
clipLine(double xmin, double ymin, double xmax, double ymax,
         double *x1, double *y1, double *x2, double *y2)
{
  bool     intersect;
  ClipZone zone1, zone2;

  if (clipBySector(xmin, ymin, xmax, ymax, x1, y1, x2, y2, &zone1, &zone2, &intersect))
    return intersect;

  double x, y;

  if      (zone1 & CLIP_ZONE_BOT) {
    y = ymin;
    x = *x1 + (*x2 - *x1)*(y - *y1)/(*y2 - *y1);

    *x1 = x;
    *y1 = y;

    zone1 = getClipZone(*x1, *y1, xmin, ymin, xmax, ymax);
  }
  else if (zone1 & CLIP_ZONE_TOP) {
    y = ymax;
    x = *x1 + (*x2 - *x1)*(y - *y1)/(*y2 - *y1);

    *x1 = x;
    *y1 = y;

    zone1 = getClipZone(*x1, *y1, xmin, ymin, xmax, ymax);
  }

  if      (zone1 & CLIP_ZONE_LEFT) {
    x = xmin;
    y = *y1 + (*y2 - *y1)*(x - *x1)/(*x2 - *x1);

    *y1 = y;
    *x1 = x;
  }
  else if (zone1 & CLIP_ZONE_RIGHT) {
    x = xmax;
    y = *y1 + (*y2 - *y1)*(x - *x1)/(*x2 - *x1);

    *y1 = y;
    *x1 = x;
  }

  if      (zone2 & CLIP_ZONE_BOT) {
    y = ymin;
    x = *x2 + (*x1 - *x2)*(y - *y2)/(*y1 - *y2);

    *x2 = x;
    *y2 = y;

    zone2 = getClipZone(*x2, *y2, xmin, ymin, xmax, ymax);
  }
  else if (zone2 & CLIP_ZONE_TOP) {
    y = ymax;
    x = *x2 + (*x1 - *x2)*(y - *y2)/(*y1 - *y2);

    *x2 = x;
    *y2 = y;

    zone2 = getClipZone(*x2, *y2, xmin, ymin, xmax, ymax);
  }

  if      (zone2 & CLIP_ZONE_LEFT) {
    x = xmin;
    y = *y2 + (*y1 - *y2)*(x - *x2)/(*x1 - *x2);

    *y2 = y;
    *x2 = x;
  }
  else if (zone2 & CLIP_ZONE_RIGHT) {
    x = xmax;
    y = *y2 + (*y1 - *y2)*(x - *x2)/(*x1 - *x2);

    *y2 = y;
    *x2 = x;
  }

  if (fabs(*x2 - *x1) < EPSILON_E6 &&
      fabs(*y2 - *y1) < EPSILON_E6)
    return false;

  return true;
}

//! Clip line (x1,y1) -> (x2,y2) in region (xmin,xmax,ymin,ymax)
bool
CMathGeom2D::
clipLine1(double xmin, double ymin, double xmax, double ymax,
          double *x1, double *y1, double *x2, double *y2)
{
  bool     intersect;
  ClipZone zone1, zone2;

  if (clipBySector(xmin, ymin, xmax, ymax, x1, y1, x2, y2, &zone1, &zone2, &intersect))
    return intersect;

  double dx = *x2 - *x1;
  double dy = *y2 - *y1;

  double r1 = (-xmin + *x1)/-dx;
  double r2 = ( xmax - *x1)/ dx;
  double r3 = ( ymax - *y1)/ dy;
  double r4 = (-ymin + *y1)/-dy;

  double t1 = 0;
  double t2 = 1;

  if (dx < 0) { t1 = std::max(t1, r2); t2 = std::min(t2, r1); }
  else        { t1 = std::max(t1, r1); t2 = std::min(t2, r2); }

  if (dy < 0) { t1 = std::max(t1, r3); t2 = std::min(t2, r4); }
  else        { t1 = std::max(t1, r4); t2 = std::min(t2, r3); }

  *x2 = *x1 + t2*dx;
  *y2 = *y1 + t2*dy;
  *x1 = *x1 + t1*dx;
  *y1 = *y1 + t1*dy;

  if (fabs(*x2 - *x1) < EPSILON_E6 &&
      fabs(*y2 - *y1) < EPSILON_E6)
    return false;

  return true;
}

//! get clip sector for start and end points of line
bool
CMathGeom2D::
clipBySector(double xmin, double ymin, double xmax, double ymax,
             double *x1, double *y1, double *x2, double *y2,
             ClipZone *zone1, ClipZone *zone2, bool *intersect)
{
  if (xmin >= xmax || ymin >= ymax) {
    *intersect = false;

    return true;
  }

  if (*x1 == *x2) {
    if (*x1 < xmin || *x1 > xmax) {
      *intersect = false;

      return true;
    }

    if      (*y1 < ymin)
      *y1 = ymin;
    else if (*y1 > ymax)
      *y1 = ymax;

    if      (*y2 < ymin)
      *y2 = ymin;
    else if (*y2 > ymax)
      *y2 = ymax;

    if (fabs(*x2 - *x1) < EPSILON_E6 &&
        fabs(*y2 - *y1) < EPSILON_E6) {
      *intersect = false;

      return true;
    }

    *intersect = true;

    return true;
  }

  if (*y1 == *y2) {
    if (*y1 < ymin || *y1 > ymax) {
      *intersect = false;

      return true;
    }

    if      (*x1 < xmin)
      *x1 = xmin;
    else if (*x1 > xmax)
      *x1 = xmax;

    if      (*x2 < xmin)
      *x2 = xmin;
    else if (*x2 > xmax)
      *x2 = xmax;

    if (fabs(*x2 - *x1) < EPSILON_E6 &&
        fabs(*y2 - *y1) < EPSILON_E6) {
      *intersect = false;

      return true;
    }

    *intersect = true;

    return true;
  }

  *zone1 = getClipZone(*x1, *y1, xmin, ymin, xmax, ymax);
  *zone2 = getClipZone(*x2, *y2, xmin, ymin, xmax, ymax);

  if (*zone1 & *zone2) {
    *intersect = false;

    return true;
  }

  return false;
}

//! get clip zone for point
CMathGeom2D::ClipZone
CMathGeom2D::
getClipZone(double x, double y, double xmin, double ymin, double xmax, double ymax)
{
  ClipZone zone;

  if      (x < xmin) {
    if      (y < ymin)
      zone = CLIP_ZONE_9;
    else if (y <= ymax)
      zone = CLIP_ZONE_1;
    else
      zone = CLIP_ZONE_5;
  }
  else if (x <= xmax) {
    if      (y < ymin)
      zone = CLIP_ZONE_8;
    else if (y <= ymax)
      zone = CLIP_ZONE_0;
    else
      zone = CLIP_ZONE_4;
  }
  else {
    if      (y < ymin)
      zone = CLIP_ZONE_10;
    else if (y <= ymax)
      zone = CLIP_ZONE_2;
    else
      zone = CLIP_ZONE_6;
  }

  return zone;
}

//-------

bool
CMathGeom2D::
PointLineDistance(const CPoint2D &point, const CLine2D &line, double *dist)
{
  CVector2D pl(line.start(), point);

  CVector2D l = line.vector();

  double u1 = pl.dotProduct(l);
  double u2 = l .lengthSqr();

  if (u2 <= 0.0) {
    *dist = PointPointDistance(point, line.start());
    return false;
  }

  double u = u1/u2;

  if      (u < 0.0) {
    *dist = PointPointDistance(point, line.start());
    return false;
  }
  else if (u > 1.0) {
    *dist = PointPointDistance(point, line.end());
    return false;
  }
  else {
    CPoint2D intersection = line.start() + u*l;

    *dist = PointPointDistance(point, intersection);

    return true;
  }
}

//-------

//! distance between two points
double
CMathGeom2D::
PointPointDistance(const CPoint2D &point1, const CPoint2D &point2)
{
  double dx = point1.x - point2.x;
  double dy = point1.y - point2.y;

  return Hypot(dx, dy);
}

//-------

double
CMathGeom2D::
Hypot(double a, double b)
{
  double r;

  if      (abs(a) > abs(b)) {
    r = b/a;
    r = abs(a)*sqrt(1 + r*r);
  }
  else if (b != 0) {
    r = a/b;
    r = abs(b)*sqrt(1 + r*r);
  }
  else {
    r = 0.0;
  }

  return r;
}

//-------

struct SlicePoint {
  enum Type { NONE, NORMAL, INTERSECT };

  SlicePoint(Type t1, const CPoint2D &p1) :
   t(t1), p(p1) {
  }

  Type     t;
  CPoint2D p;
};

bool
CMathGeom2D::
SlicePolygonByLines(const vector<CPoint2D> &poly, const CLine2D &line,
                    vector< vector<CPoint2D> > &opolys)
{
  map<uint,CPoint2D> ipoints;

  if (! PolygonLineIntersect(poly, line, ipoints))
    return false;

  vector<SlicePoint> tpoints2[2];

  int tind = 0;

  uint num_points = poly.size();

  int i1 = num_points - 1;

  for (int i2 = 0; i2 < (int) num_points; i1 = i2, ++i2) {
    tpoints2[tind].push_back(SlicePoint(SlicePoint::NORMAL, poly[i1]));

    map<uint,CPoint2D>::const_iterator p = ipoints.find(i2);

    if (p != ipoints.end()) {
      tpoints2[tind].push_back(SlicePoint(SlicePoint::INTERSECT, (*p).second));

      tind = 1 - tind;

      tpoints2[tind].push_back(SlicePoint(SlicePoint::INTERSECT, (*p).second));
    }
  }

  for (uint j = 0; j < 2; ++j) {
    SlicePoint::Type last_t = SlicePoint::NONE;
    CPoint2D         last_p;

    vector<CPoint2D> opoly;

    vector<SlicePoint> &tpoints = tpoints2[j];

    uint num_tpoints = tpoints.size();

    for (uint i = 0; i < num_tpoints; ++i) {
      SlicePoint &p = tpoints[i];

      if (p.t == SlicePoint::INTERSECT && last_t == SlicePoint::INTERSECT) {
        CPoint2D p1 = (p.p + last_p)/2.0;

        if (! PointInsideEvenOdd(p1, poly)) {
          opolys.push_back(opoly);

          opoly.clear();
        }
      }

      opoly.push_back(p.p);

      last_t = p.t;
      last_p = p.p;
    }

    opolys.push_back(opoly);
  }

  return true;
}

//! intersection between polygon and line
bool
CMathGeom2D::
PolygonLineIntersect(const vector<CPoint2D> &points, const CLine2D &line,
                     vector<CPoint2D> &ipoints)
{
  map<uint,CPoint2D> ipoints1;

  PolygonLineIntersect(points, line, ipoints1);

  map<uint,CPoint2D>::const_iterator p1, p2;

  for (p1 = ipoints1.begin(), p2 = ipoints1.end(); p1 != p2; ++p1)
    ipoints.push_back((*p1).second);

  return true;
}

bool
CMathGeom2D::
PolygonLineIntersect(const vector<CPoint2D> &points, const CLine2D &line,
                     map<uint,CPoint2D> &ipoints)
{
  ipoints.clear();

  uint num_points = points.size();

  if (num_points < 3) return false;

  double mu1, mu2;

  int i1 = num_points - 1;

  for (int i2 = 0; i2 < (int) num_points; i1 = i2, ++i2) {
    CPoint2D ipoint;

    IntersectLine(points[i1], points[i2], line.start(), line.end(), &ipoint, &mu1, &mu2);

    if (mu1 < 0.0 || mu1 > 1.0) continue;
    if (mu2 < 0.0 || mu2 > 1.0) continue;

    ipoints[i2] = ipoint;
  }

  return (! ipoints.empty());
}

//! intersection between polygon and line
bool
CMathGeom2D::
PolygonLineIntersect(const double *x, const double *y, uint nxy,
                     double x1, double y1, double x2, double y2,
                     double *xi, double *yi, uint *num_i)
{
  double xi1, yi1, mu1, mu2;

  *num_i = 0;

  int i1 = nxy - 1;

  for (int i2 = 0; i2 < (int) nxy; i1 = i2, ++i2) {
    IntersectLine(x[i1], y[i1], x[i2], y[i2], x1, y1, x2, y2, &xi1, &yi1, &mu1, &mu2);

    if (mu1 < 0.0 || mu1 > 1.0)
      continue;

    if (mu2 < 0.0 || mu2 > 1.0)
      continue;

    if (xi != NULL) xi[*num_i] = xi1;
    if (yi != NULL) yi[*num_i] = yi1;

    ++(*num_i);
  }

  return (*num_i > 0);
}

//-------

//! intersection between circle and line
bool
CMathGeom2D::
CircleLineIntersect(double xc, double yc, double r, double x1, double y1, double x2, double y2,
                    double *xi1, double *yi1, double *xi2, double *yi2, uint *num_i)
{
  // Transform to origin

  x1 -= xc; y1 -= yc;
  x2 -= xc; y2 -= yc;

  // Calculate discriminant

  double dx = x2 - x1;
  double dy = y2 - y1;

  double dr2 = dx*dx + dy*dy;

  if (dr2 <= 0.0)
    return false;

  double idr2 = 1.0/dr2;

  double dd = x1*y2 - x2*y1;

  double dd2 = dd*dd;

  double r2 = r*r;

  double dis = r2*dr2 - dd2;

  if (dis < 0.0) {
    *num_i = 0;

    return false;
  }

  // Calculate intersection

  if (dis != 0) {
    double sdy = (dy < 0.0 ? -1.0 : 1.0);

    dis = sqrt(dis);

    double dd_idr2 = dd*idr2;

    double dddx_idr2 = dd_idr2*dx;
    double dddy_idr2 = dd_idr2*dy;

    double sdydis_idr2 = sdy*dis*idr2;

    double sdydxdis_idr2 = sdydis_idr2*dx;
    double sdydydis_idr2 = sdydis_idr2*dy;

    if (xi1 != NULL)
      *xi1 =  dddy_idr2 - sdydxdis_idr2 + xc;

    if (yi1 != NULL)
      *yi1 = -dddx_idr2 - sdydydis_idr2 + yc;

    if (xi2 != NULL)
      *xi2 =  dddy_idr2 + sdydxdis_idr2 + xc;

    if (yi2 != NULL)
      *yi2 = -dddx_idr2 + sdydydis_idr2 + yc;

    *num_i = 2;
  }
  else {
    *num_i = 1;

    double dd_idr2 = dd*idr2;

    if (xi1 != NULL)
      *xi1 =  dd_idr2*dy + xc;

    if (yi1 != NULL)
      *yi1 = -dd_idr2*dx + yc;

    if (xi2 != NULL)
      *xi2 = *xi1;

    if (yi2 != NULL)
      *yi2 = *yi1;
  }

  return true;
}

//-------

//! three points to circle
bool
CMathGeom2D::
ThreePointCircle(const CPoint2D &point1, const CPoint2D &point2,
                 const CPoint2D &point3, CPoint2D &center, double &radius)
{
  return ThreePointCircle(point1.x, point1.y, point2.x, point2.y, point3.x, point3.y,
                          &center.x, &center.y, &radius);
}

//! three points to circle
bool
CMathGeom2D::
ThreePointCircle(double x1, double y1, double x2, double y2, double x3, double y3,
                 double *xc, double *yc, double *r)
{
  double x21 = x2 - x1;
  double y21 = y2 - y1;
  double x31 = x3 - x1;
  double y31 = y3 - y1;
  double x32 = x3 - x2;
  double y32 = y3 - y2;

  if      (! IsPerpendicular(x21, y21, x31, y31))
    ThreePointCircle1(x1, y1, x2, y2, x3, y3, xc, yc, r);
  else if (! IsPerpendicular(x21, y21, x32, y32))
    ThreePointCircle1(x2, y2, x1, y1, x3, y3, xc, yc, r);
  else if (! IsPerpendicular(x31, y31, x21, y21))
    ThreePointCircle1(x3, y3, x1, y1, x2, y2, xc, yc, r);
  else if (! IsPerpendicular(x31, y31, x32, y32))
    ThreePointCircle1(x1, y1, x3, y3, x2, y2, xc, yc, r);
  else if (! IsPerpendicular(x32, y32, x21, y21))
    ThreePointCircle1(x3, y3, x2, y2, x1, y1, xc, yc, r);
  else if (! IsPerpendicular(x32, y32, x31, y31))
    ThreePointCircle1(x2, y2, x3, y3, x1, y1, xc, yc, r);
  else
    return false;

  return true;
}

//! check if lines (x1,y1) -> (x2,y2) and (x2,x2) -> (x3,y3) are
//! perpendicular using differences
bool
CMathGeom2D::
IsPerpendicular(double x21, double y21, double x32, double y32)
{
  if (fabs(x21) <= EPSILON_E6 &&
      fabs(y32) <= EPSILON_E6)
    return false;

  if (fabs(y21) <= EPSILON_E6)
    return true;

  if (fabs(y32) <= EPSILON_E6)
    return true;

  if (fabs(x21) <= EPSILON_E6)
    return true;

  if (fabs(x32) <= EPSILON_E6)
    return true;

  return false ;
}

bool
CMathGeom2D::
ThreePointCircle1(double x1, double y1, double x2, double y2,
                  double x3, double y3, double *xc, double *yc, double *r)
{
  double x21 = x2 - x1;
  double y21 = y2 - y1;
  double x32 = x3 - x2;
  double y32 = y3 - y2;

  double xm1 = 0.5*(x1 + x2);
  double ym1 = 0.5*(y1 + y2);

  if (fabs(x21) <= EPSILON_E6 &&
      fabs(y32) <= EPSILON_E6) {
    *xc = xm1;
    *yc = ym1;

    double dx = x1 - *xc;
    double dy = y1 - *yc;

    *r = Hypot(dx, dy);

    return true;
  }

  double ma = y21/x21;
  double mb = y32/x32;

  if (fabs(ma - mb) <= EPSILON_E6)
    return false;

  double xm2 = 0.5*(x2 + x3);
  double yd2 = 0.5*(y1 - y3);

  *xc = (ma*mb*yd2 + mb*xm1 - ma*xm2)/(mb - ma);
  *yc = (xm1 - *xc)/ma + ym1;

  double dx = x1 - *xc;
  double dy = y1 - *yc;

  *r = Hypot(dx, dy);

  return true;
}

//-----------------

bool
CMathGeom2D::
CircleCircleIntersect(double x1, double y1, double r1, double x2, double y2, double r2,
                      double *xi1, double *yi1, double *xi2, double *yi2)
{
  // distance between circle centers
  double dx = x2 - x1;
  double dy = y2 - y1;

  double d = sqrt(dx*dx + dy*dy);

  double sr12 = r1 + r2;
  double dr12 = fabs(r1 - r2);

  if (d > sr12) return false; // separate
  if (d < dr12) return false; // contained

  if (d == 0.0 && r1 == r2) return false; // coincident

  // (x3,y3) is the point where the line through the circle intersection points
  // crosses the line between the circle centers.

  // calc distance from point 1 to point 3.
  double a = (r1*r1 - r2*r2 + d*d)/(2.0*d);

  // calc distance from point 3 to either intersection points
  double h = sqrt(r1*r1 - a*a);

  // calc the coordinates of point 3
  double x3 = x1 + a*dx/d;
  double y3 = y1 + a*dy/d;

  // calc the offsets of the intersection points from point 3
  double rx = -h*dy/d;
  double ry =  h*dx/d;

  // calc the absolute intersection points
  *xi1 = x3 + rx;
  *xi2 = x3 - rx;
  *yi1 = y3 + ry;
  *yi2 = y3 - ry;

  return 1;
}

//-----------------

//! convert arc to array of beziers
void
CMathGeom2D::
ArcToBeziers(double x, double y, double rx, double ry,
             double angle1, double angle2, vector<C3Bezier2D> &beziers)
{
  CArcToBezier a_to_b;

  a_to_b.calc(x, y, rx, ry, angle1, angle2);

  uint num_beziers = a_to_b.getNumBeziers();

  beziers.resize(num_beziers);

  for (uint i = 0; i < num_beziers; ++i)
    beziers[i] = a_to_b.getBezier(i);
}

void
CMathGeom2D::
ArcNToBeziers(double x, double y, double rx, double ry,
              double angle1, double angle2, vector<C3Bezier2D> &beziers)
{
  CArcToBezier a_to_b;

  a_to_b.calcN(x, y, rx, ry, angle1, angle2);

  uint num_beziers = a_to_b.getNumBeziers();

  beziers.resize(num_beziers);

  for (uint i = 0; i < num_beziers; ++i)
    beziers[i] = a_to_b.getBezier(i);
}

//! convert bezier to array of lines
void
CMathGeom2D::
BezierToLines(const C2Bezier2D &bezier, vector<CPoint2D> &points, double tol)
{
  CBezierToLine b_to_l;

  if (tol > 0.0) b_to_l.setTolerance(tol);

  b_to_l.calc(bezier);

  uint num_points = b_to_l.getNumPoints();

  points.resize(num_points);

  for (uint i = 0; i < num_points; ++i)
    points[i] = b_to_l.getPoint(i);
}

void
CMathGeom2D::
BezierToLines(const C3Bezier2D &bezier, vector<CPoint2D> &points, double tol)
{
  CBezierToLine b_to_l;

  if (tol > 0.0) b_to_l.setTolerance(tol);

  b_to_l.calc(bezier);

  uint num_points = b_to_l.getNumPoints();

  points.resize(num_points);

  for (uint i = 0; i < num_points; ++i)
    points[i] = b_to_l.getPoint(i);
}

//-------------

CMathGeom2D::PointPosition
CMathGeom2D::
PointLinePosition(const CLine2D &line, const CPoint2D &point)
{
  return PointLinePosition(line.start(), line.end(), point);
}

CMathGeom2D::PointPosition
CMathGeom2D::
PointLinePosition(const CPoint2D &lpoint1, const CPoint2D &lpoint2, const CPoint2D &point)
{
  double area = TriangleArea2(lpoint1, lpoint2, point);

  if      (area >  EPSILON_E6) return POINT_POSITION_LEFT;
  else if (area < -EPSILON_E6) return POINT_POSITION_RIGHT;
  else                         return POINT_POSITION_ON;
}

bool
CMathGeom2D::
PointLineLeft(const CLine2D &line, const CPoint2D &point)
{
  return PointLineLeft(line.start(), line.end(), point);
}

bool
CMathGeom2D::
PointLineLeft(const CPoint2D &lpoint1, const CPoint2D &lpoint2, const CPoint2D &point)
{
  return TriangleArea2(lpoint1, lpoint2, point) > 0.0;
}

bool
CMathGeom2D::
PointLineRight(const CLine2D &line, const CPoint2D &point)
{
  return PointLineRight(line.start(), line.end(), point);
}

bool
CMathGeom2D::
PointLineRight(const CPoint2D &lpoint1, const CPoint2D &lpoint2, const CPoint2D &point)
{
  return TriangleArea2(lpoint1, lpoint2, point) < 0.0;
}

bool
CMathGeom2D::
PointLineOn(const CLine2D &line, const CPoint2D &point)
{
  return PointLineOn(line.start(), line.end(), point) == 0.0;
}

bool
CMathGeom2D::
PointLineOn(const CPoint2D &lpoint1, const CPoint2D &lpoint2, const CPoint2D &point)
{
  return TriangleArea2(lpoint1, lpoint2, point) == 0.0;
}

bool
CMathGeom2D::
PointLineLeftOn(const CLine2D &line, const CPoint2D &point)
{
  return PointLineLeftOn(line.start(), line.end(), point) >= 0.0;
}

bool
CMathGeom2D::
PointLineLeftOn(const CPoint2D &lpoint1, const CPoint2D &lpoint2, const CPoint2D &point)
{
  return TriangleArea2(lpoint1, lpoint2, point) >= 0.0;
}

bool
CMathGeom2D::
PointLineRightOn(const CLine2D &line, const CPoint2D &point)
{
  return PointLineRightOn(line.start(), line.end(), point);
}

bool
CMathGeom2D::
PointLineRightOn(const CPoint2D &lpoint1, const CPoint2D &lpoint2, const CPoint2D &point)
{
  return TriangleArea2(lpoint1, lpoint2, point) >= 0.0;
}

bool
CMathGeom2D::
Collinear(const CPoint2D &point1, const CPoint2D &point2, const CPoint2D &point3)
{
  return PointLinePosition(point1, point2, point3) == POINT_POSITION_ON;
}

//-------

bool
CMathGeom2D::
IntersectsProperly(const CPoint2D &l1point1, const CPoint2D &l1point2,
                   const CPoint2D &l2point1, const CPoint2D &l2point2)
{
  double area11 = TriangleArea2(l1point1, l1point2, l2point1);

  if (area11 == 0.0) return false;

  double area12 = TriangleArea2(l1point1, l1point2, l2point2);

  if (area12 == 0.0) return false;

  double area21 = TriangleArea2(l2point1, l2point2, l1point1);

  if (area21 == 0.0) return false;

  double area22 = TriangleArea2(l2point1, l2point2, l1point2);

  if (area22 == 0.0) return false;

  return ((area11*area12 < 0.0) && (area21*area22 < 0.0));
}

bool
CMathGeom2D::
Intersects(const CPoint2D &l1point1, const CPoint2D &l1point2,
           const CPoint2D &l2point1, const CPoint2D &l2point2)
{
  double area11 = TriangleArea2(l1point1, l1point2, l2point1);

  if (area11 == 0.0 && PointBetween(l1point1, l1point2, l2point1))
    return true;

  double area12 = TriangleArea2(l1point1, l1point2, l2point2);

  if (area12 == 0.0 && PointBetween(l1point1, l1point2, l2point2))
    return true;

  double area21 = TriangleArea2(l2point1, l2point2, l1point1);

  if (area21 == 0.0 && PointBetween(l2point1, l2point2, l1point1))
    return true;

  double area22 = TriangleArea2(l2point1, l2point2, l1point2);

  if (area22 == 0.0 && PointBetween(l2point1, l2point2, l1point1))
    return true;

  return ((area11*area12 < 0.0) && (area21*area22 < 0.0));
}

bool
CMathGeom2D::
PointBetween(const CPoint2D &lpoint1, const CPoint2D &lpoint2, const CPoint2D &point)
{
  if (lpoint1.x != lpoint2.x)
    return (lpoint1.x <= point.x && point.x <= lpoint2.x) ||
           (lpoint1.x >= point.x && point.x >= lpoint2.x);
  else
    return (lpoint1.x <= point.y && point.x <= lpoint2.y) ||
           (lpoint1.x >= point.y && point.x >= lpoint2.y);
}

CPoint2D
CMathGeom2D::
RotatePoint(const CPoint2D &point, double angle, const CPoint2D &o)
{
  double s = sin(angle);
  double c = cos(angle);

  double x1 = point.x - o.x;
  double y1 = point.y - o.y;

  double x2 = x1*c - y1*s;
  double y2 = x1*s + y1*c;

  return CPoint2D(x2 + o.x, y2 + o.y);
}

void
CMathGeom2D::
PointsRange(const vector<CPoint2D> &points, CPoint2D &min_point, CPoint2D &max_point)
{
  uint num_points = points.size();

  assert(num_points > 0);

  min_point = points[0];
  max_point = min_point;

  for (uint i = 1; i < num_points; ++i) {
    min_point.x = std::min(min_point.x, points[i].x);
    min_point.y = std::min(min_point.y, points[i].y);
    max_point.x = std::max(max_point.x, points[i].x);
    max_point.y = std::max(max_point.y, points[i].y);
  }
}

void
CMathGeom2D::
PointsRange(const vector<CIPoint2D> &points, CIPoint2D &min_point, CIPoint2D &max_point)
{
  uint num_points = points.size();

  assert(num_points > 0);

  min_point = points[0];
  max_point = min_point;

  for (uint i = 1; i < num_points; ++i) {
    min_point.x = std::min(min_point.x, points[i].x);
    min_point.y = std::min(min_point.y, points[i].y);
    max_point.x = std::max(max_point.x, points[i].x);
    max_point.y = std::max(max_point.y, points[i].y);
  }
}

// ellipse center (cx,cy), radii (xr,yr)
void
CMathGeom2D::
EllipsePointAtAngle(double cx, double cy, double xr, double yr, double a, double *x, double *y)
{
  double c = cos(a);
  double s = sin(a);

  // intersect line (0,0)->(c,s) with ellipse center (0,0), radius (xr,yr)

  double n = xr*yr;
  double d = Hypot(xr*s, yr*c);

  double f = n/d;

  *x = c*f;
  *y = s*f;

  *x += cx;
  *y += cy;
}

//-------

bool
CMathGeom2D::
BezierLineIntersect(const C2Bezier2D &bezier, const CLine2D &line,
                    std::vector<CPoint2D> &ipoints, double tol)
{
  std::vector<CPoint2DParam2> ibpoints;

  if (! BezierLineIntersect(bezier, line, 0.0, 1.0, ibpoints, tol))
    return false;

  uint num_ibpoints = ibpoints.size();

  ipoints.resize(num_ibpoints);

  for (uint i = 0; i < num_ibpoints; ++i)
    ipoints[i] = ibpoints[i].p;

  return true;
}

bool
CMathGeom2D::
BezierLineIntersect(const C2Bezier2D &bezier, const CLine2D &line,
                    std::vector<CPoint2DParam2> &ibpoints, double tol)
{
  return BezierLineIntersect(bezier, line, 0, 1, ibpoints, tol);
}

bool
CMathGeom2D::
BezierLineIntersect(const C2Bezier2D &bezier, const CLine2D &line, double t1, double t2,
                    std::vector<CPoint2DParam2> &ibpoints, double tol)
{
  const CPoint2D &p1 = bezier.getFirstPoint  ();
  const CPoint2D &p2 = bezier.getControlPoint();
  const CPoint2D &p3 = bezier.getLastPoint   ();

  const CPoint2D &lp1 = line.start();
  const CPoint2D &lp2 = line.end  ();

  double a1 = fabs(IncludedAngle(p2, p1, p3));
  double a2 = fabs(IncludedAngle(p2, p3, p1));

  if (a1 < tol && a2 < tol) {
    CPoint2D ipoint;
    double   mu1, mu2;

    if (IntersectLine(p1, p3, lp1, lp2, &ipoint, &mu1, &mu2) &&
        mu1 >= 0.0 && mu1 <= 1.0 && mu2 >= 0.0 && mu2 <= 1.0) {
      double it1 = t1 + mu1*(t2 - t1);
      double it2 = mu2;

      AddUniquePoint(ibpoints, CPoint2DParam2(ipoint, it1, it2));

      return true;
    }

    return false;
  }
  else {
    double tt = (t1 + t2)/2.0;

    C2Bezier2D bezier1, bezier2;

    bezier.split(bezier1, bezier2);

    bool i1 = BezierLineIntersect(bezier1, line, t1, tt, ibpoints, tol);
    bool i2 = BezierLineIntersect(bezier2, line, tt, t2, ibpoints, tol);

    return (i1 || i2);
  }
}

bool
CMathGeom2D::
BezierBezierIntersect(const C2Bezier2D &bezier1, const C2Bezier2D &bezier2,
                      std::vector<CPoint2D> &ipoints, double tol)
{
  std::vector<CPoint2DParam2> ibpoints;

  if (! BezierBezierIntersect(bezier1, 0.0, 1.0, bezier2, 0.0, 1.0, ibpoints, tol))
    return false;

  uint num_ibpoints = ibpoints.size();

  ipoints.resize(num_ibpoints);

  for (uint i = 0; i < num_ibpoints; ++i)
    ipoints[i] = ibpoints[i].p;

  return true;
}

bool
CMathGeom2D::
BezierBezierIntersect(const C2Bezier2D &bezier1, const C2Bezier2D &bezier2,
                      std::vector<CPoint2DParam2> &ibpoints, double tol)
{
  return BezierBezierIntersect(bezier1, 0, 1, bezier2, 0, 1, ibpoints, tol);
}

bool
CMathGeom2D::
BezierBezierIntersect(const C2Bezier2D &bezier1, double t11, double t12,
                      const C2Bezier2D &bezier2, double t21, double t22,
                      std::vector<CPoint2DParam2> &ibpoints, double tol)
{
  CBBox2D bbox1, bbox2;

  bezier1.getHullBBox(bbox1);
  bezier2.getHullBBox(bbox2);

  if (! bbox1.overlaps(bbox2))
    return false;

  const CPoint2D &p11 = bezier1.getFirstPoint  ();
  const CPoint2D &p12 = bezier1.getControlPoint();
  const CPoint2D &p13 = bezier1.getLastPoint   ();

  const CPoint2D &p21 = bezier2.getFirstPoint  ();
  const CPoint2D &p22 = bezier2.getControlPoint();
  const CPoint2D &p23 = bezier2.getLastPoint   ();

  double a11 = fabs(IncludedAngle(p12, p11, p13));
  double a12 = fabs(IncludedAngle(p12, p13, p11));
  double a21 = fabs(IncludedAngle(p22, p21, p23));
  double a22 = fabs(IncludedAngle(p22, p23, p21));

  if      (a11 < tol && a12 < tol && a21 < tol && a22 < tol) {
    CPoint2D ipoint;
    double   mu1, mu2;

    if (IntersectLine(p11, p13, p21, p23, &ipoint, &mu1, &mu2) &&
        mu1 >= 0.0 && mu1 <= 1.0 && mu2 >= 0.0 && mu2 <= 1.0) {
      double it1 = t11 + mu1*(t12 - t11);
      double it2 = t21 + mu2*(t22 - t21);

      AddUniquePoint(ibpoints, CPoint2DParam2(ipoint, it1, it2));

      return true;
    }

    return false;
  }
  else if (a11 < tol && a12 < tol)
    return BezierLineIntersect(bezier2, CLine2D(p11, p13), t21, t22, ibpoints, tol);
  else if (a21 < tol && a22 < tol)
    return BezierLineIntersect(bezier1, CLine2D(p21, p23), t11, t12, ibpoints, tol);
  else if (a11 + a12 < a21 + a22) {
    C2Bezier2D bezier21, bezier22;

    bezier2.split(bezier21, bezier22);

    double tt = (t21 + t22)/2;

    bool i1 = BezierBezierIntersect(bezier1, t11, t12, bezier21, t21, tt, ibpoints, tol);
    bool i2 = BezierBezierIntersect(bezier1, t11, t12, bezier22, tt, t22, ibpoints, tol);

    return (i1 || i2);
  }
  else {
    C2Bezier2D bezier11, bezier12;

    bezier1.split(bezier11, bezier12);

    double tt = (t11 + t12)/2;

    bool i1 = BezierBezierIntersect(bezier2, t21, t22, bezier11, t11, tt, ibpoints, tol);
    bool i2 = BezierBezierIntersect(bezier2, t21, t22, bezier12, tt, t12, ibpoints, tol);

    return (i1 || i2);
  }
}

//------

bool
CMathGeom2D::
BezierLineIntersect(const C3Bezier2D &bezier, const CLine2D &line,
                    std::vector<CPoint2D> &ipoints, double tol)
{
  std::vector<CPoint2DParam2> ibpoints;

  if (! BezierLineIntersect(bezier, line, 0.0, 1.0, ibpoints, tol))
    return false;

  uint num_ibpoints = ibpoints.size();

  ipoints.resize(num_ibpoints);

  for (uint i = 0; i < num_ibpoints; ++i)
    ipoints[i] = ibpoints[i].p;

  return true;
}

bool
CMathGeom2D::
BezierLineIntersect(const C3Bezier2D &bezier, const CLine2D &line,
                    std::vector<CPoint2DParam2> &ibpoints, double tol)
{
  return BezierLineIntersect(bezier, line, 0, 1, ibpoints, tol);
}

bool
CMathGeom2D::
BezierLineIntersect(const C3Bezier2D &bezier, const CLine2D &line, double t1, double t2,
                    std::vector<CPoint2DParam2> &ibpoints, double tol)
{
  const CPoint2D &p1 = bezier.getFirstPoint   ();
  const CPoint2D &p2 = bezier.getControlPoint1();
  const CPoint2D &p3 = bezier.getControlPoint2();
  const CPoint2D &p4 = bezier.getLastPoint    ();

  const CPoint2D &lp1 = line.start();
  const CPoint2D &lp2 = line.end  ();

  double a1 = fabs(IncludedAngle(p2, p1, p4));
  double a2 = fabs(IncludedAngle(p3, p4, p1));

  if (a1 < tol && a2 < tol) {
    CPoint2D ipoint;
    double   mu1, mu2;

    if (IntersectLine(p1, p4, lp1, lp2, &ipoint, &mu1, &mu2) &&
        mu1 >= 0.0 && mu1 <= 1.0 && mu2 >= 0.0 && mu2 <= 1.0) {
      double it1 = t1 + mu1*(t2 - t1);
      double it2 = mu2;

      AddUniquePoint(ibpoints, CPoint2DParam2(ipoint, it1, it2));

      return true;
    }

    return false;
  }
  else {
    double tt = (t1 + t2)/2.0;

    C3Bezier2D bezier1, bezier2;

    bezier.split(bezier1, bezier2);

    bool i1 = BezierLineIntersect(bezier1, line, t1, tt, ibpoints, tol);
    bool i2 = BezierLineIntersect(bezier2, line, tt, t2, ibpoints, tol);

    return (i1 || i2);
  }
}

bool
CMathGeom2D::
BezierBezierIntersect(const C3Bezier2D &bezier1, const C3Bezier2D &bezier2,
                      std::vector<CPoint2D> &ipoints, double tol)
{
  std::vector<CPoint2DParam2> ibpoints;

  if (! BezierBezierIntersect(bezier1, 0.0, 1.0, bezier2, 0.0, 1.0, ibpoints, tol))
    return false;

  uint num_ibpoints = ibpoints.size();

  ipoints.resize(num_ibpoints);

  for (uint i = 0; i < num_ibpoints; ++i)
    ipoints[i] = ibpoints[i].p;

  return true;
}

bool
CMathGeom2D::
BezierBezierIntersect(const C3Bezier2D &bezier1, const C3Bezier2D &bezier2,
                      std::vector<CPoint2DParam2> &ibpoints, double tol)
{
  return BezierBezierIntersect(bezier1, 0, 1, bezier2, 0, 1, ibpoints, tol);
}

bool
CMathGeom2D::
BezierBezierIntersect(const C3Bezier2D &bezier1, double t11, double t12,
                      const C3Bezier2D &bezier2, double t21, double t22,
                      std::vector<CPoint2DParam2> &ibpoints, double tol)
{
  CBBox2D bbox1, bbox2;

  bezier1.getHullBBox(bbox1);
  bezier2.getHullBBox(bbox2);

  if (! bbox1.overlaps(bbox2))
    return false;

  const CPoint2D &p11 = bezier1.getFirstPoint   ();
  const CPoint2D &p12 = bezier1.getControlPoint1();
  const CPoint2D &p13 = bezier1.getControlPoint2();
  const CPoint2D &p14 = bezier1.getLastPoint    ();

  const CPoint2D &p21 = bezier2.getFirstPoint   ();
  const CPoint2D &p22 = bezier2.getControlPoint1();
  const CPoint2D &p23 = bezier2.getControlPoint2();
  const CPoint2D &p24 = bezier2.getLastPoint    ();

  double a11 = fabs(IncludedAngle(p12, p11, p14));
  double a12 = fabs(IncludedAngle(p13, p14, p11));
  double a21 = fabs(IncludedAngle(p22, p21, p24));
  double a22 = fabs(IncludedAngle(p23, p24, p21));

  if      (a11 < tol && a12 < tol && a21 < tol && a22 < tol) {
    CPoint2D ipoint;
    double   mu1, mu2;

    if (IntersectLine(p11, p14, p21, p24, &ipoint, &mu1, &mu2) &&
        mu1 >= 0.0 && mu1 <= 1.0 && mu2 >= 0.0 && mu2 <= 1.0) {
      double it1 = t11 + mu1*(t12 - t11);
      double it2 = t21 + mu2*(t22 - t21);

      AddUniquePoint(ibpoints, CPoint2DParam2(ipoint, it1, it2));

      return true;
    }

    return false;
  }
  else if (a11 < tol && a12 < tol)
    return BezierLineIntersect(bezier2, CLine2D(p11, p14), t21, t22, ibpoints, tol);
  else if (a21 < tol && a22 < tol)
    return BezierLineIntersect(bezier1, CLine2D(p21, p24), t11, t12, ibpoints, tol);
  else if (a11 + a12 < a21 + a22) {
    C3Bezier2D bezier21, bezier22;

    bezier2.split(bezier21, bezier22);

    double tt = (t21 + t22)/2;

    bool i1 = BezierBezierIntersect(bezier1, t11, t12, bezier21, t21, tt, ibpoints, tol);
    bool i2 = BezierBezierIntersect(bezier1, t11, t12, bezier22, tt, t22, ibpoints, tol);

    return (i1 || i2);
  }
  else {
    C3Bezier2D bezier11, bezier12;

    bezier1.split(bezier11, bezier12);

    double tt = (t11 + t12)/2;

    bool i1 = BezierBezierIntersect(bezier2, t21, t22, bezier11, t11, tt, ibpoints, tol);
    bool i2 = BezierBezierIntersect(bezier2, t21, t22, bezier12, tt, t12, ibpoints, tol);

    return (i1 || i2);
  }
}

void
CMathGeom2D::
AddUniquePoint(std::vector<CPoint2DParam2> &points, const CPoint2DParam2 &p)
{
  uint num = points.size();

  for (uint i = 0; i < num; ++i)
    if (points[i].p.equal(p.p, EPSILON_E6))
      return;

  points.push_back(p);
}

//------

bool
CMathGeom2D::
LineToPolygon(const CPoint2D &p1, const CPoint2D &p2, std::vector<CPoint2D> &points,
              double line_width)
{
  double x21 = p2.x - p1.x;
  double y21 = p2.y - p1.y;

  if (x21 == 0.0 && y21 == 0.0) {
    points.resize(0);
    return false;
  }

  if (line_width <= 0.0)
    line_width = 1.0;

  points.resize(4);

  double g = atan2(y21, x21);

  AddWidthToPoint(p1, g, line_width, points[0], points[3]);
  AddWidthToPoint(p2, g, line_width, points[1], points[2]);

  return true;
}

//------

bool
CMathGeom2D::
LineJoinToPolygon(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3,
                  CLineJoinType lineJoin, std::vector<CPoint2D> &points, double line_width)
{
  if (line_width <= 0.0) {
    points.resize(0);
    return false;
  }

  if      (lineJoin == LINE_JOIN_TYPE_MITRE)
    return MitreLineJoinToPolygon(p1, p2, p3, points, line_width);
  else if (lineJoin == LINE_JOIN_TYPE_ROUND)
    return RoundLineJoinToPolygon(p1, p2, p3, points);
  else
    return BevelLineJoinToPolygon(p1, p2, p3, points);
}

bool
CMathGeom2D::
MitreLineJoinToPolygon(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3,
                       std::vector<CPoint2D> &points, double line_width, double mitre_limit)
{
  if (mitre_limit == 1.0)
    return BevelLineJoinToPolygon(p1, p2, p3, points);

  double theta = IncludedAngle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);

  double s = sin(theta/2.0);

  if (s == 0.0 || 1.0/s > mitre_limit)
    return BevelLineJoinToPolygon(p1, p2, p3, points);

  //------

  double x_diff = p2.x - p1.x;
  double y_diff = p2.y - p1.y;

  if (x_diff == 0.0 && y_diff == 0.0) return false;

  double g = atan2(y_diff, x_diff);

  CPoint2D pp1[4];

  AddWidthToPoint(p1, g, line_width, pp1[0], pp1[3]);
  AddWidthToPoint(p2, g, line_width, pp1[1], pp1[2]);

  //------

  x_diff = p3.x - p2.x;
  y_diff = p3.y - p2.y;

  if (x_diff == 0.0 && y_diff == 0.0) return false;

  g = atan2(y_diff, x_diff);

  CPoint2D pp2[4];

  AddWidthToPoint(p2, g, line_width, pp2[0], pp2[3]);
  AddWidthToPoint(p3, g, line_width, pp2[1], pp2[2]);

  //------

  CPoint2D pi[4];

  CLine2D line1(pp1[0], pp1[1]);
  CLine2D line2(pp2[0], pp2[1]);
  CLine2D line3(pp2[3], pp2[2]);
  CLine2D line4(pp1[3], pp1[2]);

  if (! line1.intersect(line2, pi[0])) return false;
  if (! line1.intersect(line3, pi[1])) return false;
  if (! line4.intersect(line2, pi[3])) return false;
  if (! line4.intersect(line3, pi[2])) return false;

  //------

  points.resize(7);

  points[0].x = pp2[0].x; points[0].y = pp2[0].y;
  points[1].x = pi [0].x; points[1].y = pi [0].y;
  points[2].x = pp1[1].x; points[2].y = pp1[1].y;
  points[3].x = p2    .x; points[3].y = p2    .y;
  points[4].x = pp2[3].x; points[4].y = pp2[3].y;
  points[5].x = pi [2].x; points[5].y = pi [2].y;
  points[6].x = pp1[2].x; points[6].y = pp1[2].y;

  return true;
}

bool
CMathGeom2D::
RoundLineJoinToPolygon(const CPoint2D &/*p1*/, const CPoint2D &/*p2*/, const CPoint2D &/*p3*/,
                       std::vector<CPoint2D> &points)
{
  // TODO
  points.resize(0);

  return false;
}

bool
CMathGeom2D::
BevelLineJoinToPolygon(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3,
                       std::vector<CPoint2D> &points, double line_width)
{
  double x_diff = p2.x - p1.x;
  double y_diff = p2.y - p1.y;

  if (x_diff == 0.0 && y_diff == 0.0) return false;

  double g = atan2(y_diff, x_diff);

  AddWidthToPoint(p2, g, line_width, points[0], points[2]);

  //------

  x_diff = p2.x - p3.x;
  y_diff = p2.y - p3.y;

  if (x_diff == 0.0 && y_diff == 0.0) return false;

  g = atan2(y_diff, x_diff);

  AddWidthToPoint(p2, g, line_width, points[3], points[1]);

  return true;
}

void
CMathGeom2D::
AddWidthToPoint(const CPoint2D &p, double g, double line_width, CPoint2D &p1, CPoint2D &p2)
{
  double dx = line_width*sin(g)/2.0;
  double dy = line_width*cos(g)/2.0;

  p1.x = p.x + dx;
  p1.y = p.y - dy;
  p2.x = p.x - dx;
  p2.y = p.y + dy;
}

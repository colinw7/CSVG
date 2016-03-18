#ifndef CPIXEL_CLIP_H
#define CPIXEL_CLIP_H

#include <CSingleton.h>
#include <CILine2D.h>
#include <CFillType.h>
#include <vector>

#define CPixelClipMgrInst CPixelClipMgr::getInstancePtr()

class CPixelClip1;

class CPixelClipMgr : public CSingleton<CPixelClipMgr> {
 private:
  typedef std::vector<CILine2D> ClipLineList;

  ClipLineList lines_;
  ClipLineList lines1_;

 protected:
  friend class CSingleton<CPixelClipMgr>;

  CPixelClipMgr();

 public:
  uint getNumLines () const { return lines_ .size(); }
  uint getNumLines1() const { return lines1_.size(); }

  const CILine2D &getLine (uint i) const { return lines_ [i]; }
  const CILine2D &getLine1(uint i) const { return lines1_[i]; }

  void resetLines();

  void addClipLine(const CIPoint2D &point1, const CIPoint2D &point2);

  void copyLines1();
};

class CPixelClip {
 public:
  typedef std::vector<CIPoint2D>    IPointList;
  typedef std::vector<IPointList *> IPointListList;
  typedef std::vector<CPixelClip1>  Clip1List;

 private:
  CPixelClipMgr *mgr_;
  Clip1List      clips_;

 public:
  CPixelClip();

  CPixelClip(const CPixelClip &clip);

 ~CPixelClip();

  const CPixelClip &operator=(const CPixelClip &clip);

 private:
  void assign(const CPixelClip &clip);

 public:
  void resize(uint new_num_clips);

  void init();
  void term();

  void processLine(int x1, int y1, int x2, int y2,
                   int **xx1, int **yy1, int **xx2, int **yy2, uint *num);

 private:
  void processLine1(Clip1List &clips, int x1, int y1, int x2, int y2,
                    int **xx1, int **yy1, int **xx2, int **yy2, uint *num);

  void processLine2(CPixelClip1 &clip1, const CILine2D &line);

 public:
  bool isInside(CPixelClip1 &clip1, int px, int py) const;

  bool isEmpty() const;

  uint getSize() const;

  void addPolygons(CFillType type, const IPointListList &poly_point_list);

  const IPointListList &getPolygons(uint i) const;

  uint getNumPolygons(uint i) const;
  uint getNumPolygonsPoints(uint i) const;
};

#endif

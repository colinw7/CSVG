#ifndef CSVGAnimateMotion_H
#define CSVGAnimateMotion_H

#include <CSVGAnimateBase.h>
#include <CSVGPathPart.h>

class CSVGAnimateMotion : public CSVGAnimateBase {
 public:
  CSVG_OBJECT_DEF("animateMotion", CSVGObjTypeId::ANIMATE_MOTION)

  CSVGAnimateMotion(CSVG &svg);
  CSVGAnimateMotion(const CSVGAnimateMotion &animateMotion);

  CSVGAnimateMotion *dup() const override;

  const CSVGPathPartList &getPath() const { return path_; }
  void setPath(const CSVGPathPartList &path) { path_ = path; }

  std::string getKeyPoints() const { return keyPoints_.getValue(""); }
  void setKeyPoints(const std::string &s) { keyPoints_ = s; }

  std::string getRotate() const { return rotate_.getValue(""); }
  void setRotate(const std::string &s) { rotate_ = s; }

  std::string getOrigin() const { return origin_.getValue(""); }
  void setOrigin(const std::string &s) { origin_ = s; }

  bool processOption(const std::string &name, const std::string &value) override;

  void animate(double t) override;

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGAnimateMotion &animateMotion);

 private:
  CSVGPathPartList path_;
  COptString       keyPoints_;
  COptString       rotate_;
  COptString       origin_;
};

#endif

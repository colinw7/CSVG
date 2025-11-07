#ifndef CSVGAnimateMotion_H
#define CSVGAnimateMotion_H

#include <CSVGAnimateBase.h>
#include <CSVGPathPart.h>

class CSVGAnimateMotion : public CSVGAnimateBase, public CSVGPrintBase<CSVGAnimateMotion> {
 public:
  CSVG_OBJECT_DEF("animateMotion", CSVGObjTypeId::ANIMATE_MOTION)

  CSVGAnimateMotion(CSVG &svg);
  CSVGAnimateMotion(const CSVGAnimateMotion &animateMotion);

  CSVGAnimateMotion *dup() const override;

  const CSVGPathPartList &getPath() const { return path_; }
  void setPath(const CSVGPathPartList &path) { path_ = path; }

  std::string getKeyPoints() const { return keyPoints_.value_or(""); }
  void setKeyPoints(const std::string &s) { keyPoints_ = s; }

  std::string getRotate() const { return rotate_.value_or(""); }
  void setRotate(const std::string &s) { rotate_ = s; }

  std::string getOrigin() const { return origin_.value_or(""); }
  void setOrigin(const std::string &s) { origin_ = s; }

  bool processOption(const std::string &name, const std::string &value) override;

  void animate(double t) override;

  void print(std::ostream &os, bool hier=false) const override;

  void accept(CSVGVisitor *visitor) override { visitor->visit(this); }

 private:
  CSVGPathPartList           path_;
  std::optional<std::string> keyPoints_;
  std::optional<std::string> rotate_;
  std::optional<std::string> origin_;
};

#endif

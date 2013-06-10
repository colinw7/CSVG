class CStrParse;
class CSVGPathPart;

class CSVGClipPath : public CSVGObject {
 public:
  typedef std::vector<CSVGPathPart *> PartList;

 private:
  PartList parts_;

 public:
  CSVG_OBJECT_DEF("clipPath", CSVG_OBJ_TYPE_CLIP_PATH)

  CSVGClipPath(CSVG &svg);
  CSVGClipPath(const CSVGClipPath &path);

  CSVGClipPath *dup() const;

  bool processOption(const std::string &name, const std::string &value);

  void draw();

  void print(std::ostream &os) const;

  friend std::ostream &operator<<(std::ostream &os, const CSVGClipPath &path);
};

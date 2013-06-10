class CStrParse;
class CSVGPathPart;

class CSVGPath : public CSVGObject {
 public:
  typedef std::vector<CSVGPathPart *> PartList;

 protected:
  PartList parts_;

 public:
  CSVG_OBJECT_DEF("path", CSVG_OBJ_TYPE_PATH)

  CSVGPath(CSVG &svg);
  CSVGPath(const CSVGPath &path);

  CSVGPath *dup() const;

  bool processOption(const std::string &name, const std::string &value);

  const PartList &getPartList() const { return parts_; }

  void draw();

  bool getBBox(CBBox2D &bbox) const;

  void print(std::ostream &os) const;

  friend std::ostream &operator<<(std::ostream &os, const CSVGPath &path);
};

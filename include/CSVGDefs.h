class CXMLToken;

class CSVGDefs : public CSVGObject {
 public:
  typedef std::vector<const CXMLToken *> TokenList;

 private:
  TokenList tokens_;

 public:
  CSVG_OBJECT_DEF("defs", CSVG_OBJ_TYPE_DEFS)

  CSVGDefs(CSVG &svg);
  CSVGDefs(const CSVGDefs &defs);

  CSVGDefs *dup() const;

  bool processOption(const std::string &name, const std::string &value);

  bool isDrawable() { return false; }

  void draw();

  void print(std::ostream &os) const;

  friend std::ostream &operator<<(std::ostream &os, const CSVGDefs &defs);
};

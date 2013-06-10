class CSVGFeFunc : public CSVGObject {
 public:
  enum Type {
    IDENTITY,
    LINEAR,
    GAMMA,
    TABLE,
    DISCRETE
  };

 private:
  typedef std::vector<double> ValueTable;

  CColorComponent component_;
  Type            type_;
  // linear
  double          slope_;
  double          intercept_;
  // gamma
  double          amplitude_;
  double          exponent_;
  double          offset_;
  // table/discrete
  ValueTable      table_;

 public:
  CSVG_OBJECT_DEF("feFunc", CSVG_OBJ_TYPE_FE_FUNC)

  CSVGFeFunc(CSVG &svg, CColorComponent component);
  CSVGFeFunc(const CSVGFeFunc &func);

  CSVGFeFunc *dup() const;

  bool processOption(const std::string &name, const std::string &value);

  void draw();

  CImagePtr filterImage(CImagePtr src_image);

  void print(std::ostream &os) const;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFeFunc &filter);
};

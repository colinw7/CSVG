class CSVGUse : public CSVGObject {
 private:
  CSVGXLink        xlink_;
  COptValT<double> x_;
  COptValT<double> y_;
  COptValT<double> width_;
  COptValT<double> height_;

 public:
  CSVG_OBJECT_DEF("use", CSVG_OBJ_TYPE_USE)

  CSVGUse(CSVG &svg);
  CSVGUse(const CSVGUse &use);

 ~CSVGUse();

  CSVGUse *dup() const;

  CSVGObject *getObject() const;

  bool processOption(const std::string &name, const std::string &value);

  void termParse();

  void moveBy(const CVector2D &delta);

  void draw();

  void print(std::ostream &os) const;

  friend std::ostream &operator<<(std::ostream &os, const CSVGUse &use);
};

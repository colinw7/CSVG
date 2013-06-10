class CSVGSymbol : public CSVGObject {
 public:
  CSVG_OBJECT_DEF("symbol", CSVG_OBJ_TYPE_SYMBOL)

  CSVGSymbol(CSVG &svg);
  CSVGSymbol(const CSVGSymbol &symbol);

  CSVGSymbol *dup() const;

  bool processOption(const std::string &name, const std::string &value);

  void moveBy(const CVector2D &delta);
  void resizeTo(const CSize2D &size);
  void rotateBy(double da, const CPoint2D &c);

  void draw();

  void print(std::ostream &os) const;

  friend std::ostream &operator<<(std::ostream &os, const CSVGSymbol &group);
};

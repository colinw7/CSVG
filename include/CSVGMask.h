class CSVGMask : public CSVGObject {
 private:
  CSVGObject     *object_;
  double          x_, y_;
  double          width_, height_;
  CSVGCoordUnits  units_;

 public:
  CSVG_OBJECT_DEF("mask", CSVG_OBJ_TYPE_MASK)

  CSVGMask(CSVG &svg);
  CSVGMask(const CSVGMask &mask);

  CSVGMask *dup() const;

  bool processOption(const std::string &name, const std::string &value);

  void draw();

  void objectDraw(const CSVGObject &object);

  void print(std::ostream &os) const;

  friend std::ostream &operator<<(std::ostream &os, const CSVGMask &mask);

 private:
  CSVGMask &operator=(const CSVGMask &rhs);
};

class CSVGMarker : public CSVGObject {
 private:
  double refX_, refY_;
  double markerWidth_, markerHeight_;

 public:
  CSVG_OBJECT_DEF("marker", CSVG_OBJ_TYPE_MARKER)

  CSVGMarker(CSVG &svg);
  CSVGMarker(const CSVGMarker &marker);

  CSVGMarker *dup() const;

  bool processOption(const std::string &name, const std::string &value);

  void draw();

  void drawMarker(double x, double y, double angle);

  void print(std::ostream &os) const;

  friend std::ostream &operator<<(std::ostream &os, const CSVGMarker &marker);
};

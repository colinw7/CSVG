class CSVGBlock : public CSVGObject {
 private:
  COptValT<double> x_;
  COptValT<double> y_;
  COptValT<double> width_;
  COptValT<double> height_;
  CHAlignType      halign_;
  CVAlignType      valign_;
  CSVGScale        scale_;

 public:
  CSVG_OBJECT_DEF("block", CSVG_OBJ_TYPE_BLOCK)

  CSVGBlock(CSVG &svg);
  CSVGBlock(const CSVGBlock &block);

  CSVGBlock *dup() const;

  double getX() const;
  double getY() const;

  double getWidth() const;
  double getHeight() const;

  CHAlignType getHAlign() const { return halign_; }
  CVAlignType getVAlign() const { return valign_; }
  CSVGScale   getScale () const { return scale_ ; }

  void setSize(const CSize2D &size);

  bool processOption(const std::string &name, const std::string &value);

  void draw();

  bool getBBox(CBBox2D &bbox) const;

  void print(std::ostream &os) const;

  friend std::ostream &operator<<(std::ostream &os, const CSVGBlock &block);
};

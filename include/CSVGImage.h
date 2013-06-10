class CSVGImage : public CSVGObject {
 private:
  CSVGXLink xlink_;
  CPoint2D  pos_;
  CSize2D   size_;

 public:
  CSVG_OBJECT_DEF("image", CSVG_OBJ_TYPE_IMAGE)

  CSVGImage(CSVG &svg);
  CSVGImage(const CSVGImage &image);

 ~CSVGImage();

  CSVGImage *dup() const;

  CImagePtr getImage() const;

  CPoint2D getPosition() const { return pos_; }

  bool processOption(const std::string &name, const std::string &value);

  bool getSize(CSize2D &size) const;

  void setOrigin(const CPoint2D &point);
  void setSize(const CSize2D &size);

  void setImage(const std::string &filename);

  void draw();

  bool getBBox(CBBox2D &bbox) const;

  void moveBy(const CVector2D &delta);
  void resizeTo(const CSize2D &size);

  void print(std::ostream &os) const;

  friend std::ostream &operator<<(std::ostream &os, const CSVGImage &image);
};

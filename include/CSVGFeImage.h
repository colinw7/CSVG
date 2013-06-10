class CSVGFeImage : public CSVGFilter {
 private:
  CSVGXLink   xlink_;
  std::string filter_out_;

 public:
  CSVG_OBJECT_DEF("feImage", CSVG_OBJ_TYPE_FE_IMAGE)

  CSVGFeImage(CSVG &svg);
  CSVGFeImage(const CSVGFeImage &image);

  CSVGFeImage *dup() const;

  bool processOption(const std::string &name, const std::string &value);

  void draw();

  CImagePtr filterImage(CImagePtr src_image);

  void print(std::ostream &os) const;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFeImage &filter);
};

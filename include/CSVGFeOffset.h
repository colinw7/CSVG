class CSVGFeOffset : public CSVGFilter {
 private:
  std::string filter_in_;
  std::string filter_out_;
  double      dx_, dy_;

 public:
  CSVG_OBJECT_DEF("feOffset", CSVG_OBJ_TYPE_FE_OFFSET)

  CSVGFeOffset(CSVG &svg);
  CSVGFeOffset(const CSVGFeOffset &offset);

  CSVGFeOffset *dup() const;

  bool processOption(const std::string &name, const std::string &value);

  void draw();

  CImagePtr filterImage(CImagePtr src_image);

  void print(std::ostream &os) const;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFeOffset &filter);
};

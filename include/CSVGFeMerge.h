class CSVGFeMerge : public CSVGFilter {
 private:
  std::string filter_in_;
  std::string filter_out_;

 public:
  CSVG_OBJECT_DEF("feMerge", CSVG_OBJ_TYPE_FE_MERGE)

  CSVGFeMerge(CSVG &svg);
  CSVGFeMerge(const CSVGFeMerge &merge);

  CSVGFeMerge *dup() const;

  bool processOption(const std::string &name, const std::string &value);

  void draw();

  CImagePtr filterImage(CImagePtr src_image);

  void print(std::ostream &os) const;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFeMerge &filter);
};

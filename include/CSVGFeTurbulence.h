class CSVGFeTurbulence : public CSVGFilter {
 private:
  bool        fractal_;
  double      baseFreq_;
  int         numOctaves_;
  int         seed_;
  std::string filter_in_;
  std::string filter_out_;

 public:
  CSVG_OBJECT_DEF("feTurbulence", CSVG_OBJ_TYPE_FE_TURBULENCE)

  CSVGFeTurbulence(CSVG &svg);
  CSVGFeTurbulence(const CSVGFeTurbulence &merge);

  CSVGFeTurbulence *dup() const;

  bool processOption(const std::string &name, const std::string &value);

  void draw();

  CImagePtr filterImage(CImagePtr src_image);

  void print(std::ostream &os) const;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFeTurbulence &filter);
};

class CSVGFeMergeNode : public CSVGObject {
 private:
  std::string filter_in_;
  std::string filter_out_;

 public:
  CSVG_OBJECT_DEF("feMergeNode", CSVG_OBJ_TYPE_FE_MERGE_NODE)

  CSVGFeMergeNode(CSVG &svg);
  CSVGFeMergeNode(const CSVGFeMergeNode &merge);

  CSVGFeMergeNode *dup() const;

  const std::string &getFilterIn () const { return filter_in_ ; }
  const std::string &getFilterOut() const { return filter_out_; }

  bool processOption(const std::string &name, const std::string &value);

  bool isDrawable() { return false; }

  void draw();

  void print(std::ostream &os) const;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFeMergeNode &filter);
};

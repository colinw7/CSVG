#ifndef CSVGObjectMarker_H
#define CSVGObjectMarker_H

class CSVGObject;

class CSVGObjectMarker {
 public:
  CSVGObjectMarker() { }

  CSVGObject *getStart() const { return start_; }
  void setStart(CSVGObject *m) { start_ = m; }

  CSVGObject *getMid() const { return mid_; }
  void setMid(CSVGObject *m) { mid_ = m; }

  CSVGObject *getEnd() const { return end_; }
  void setEnd(CSVGObject *m) { end_ = m; }

 private:
  CSVGObject *start_ { nullptr };
  CSVGObject *mid_   { nullptr };
  CSVGObject *end_   { nullptr };
};

#endif

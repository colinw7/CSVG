#ifndef CQSVGFontObj_H
#define CQSVGFontObj_H

#include <CSVGFontObj.h>
#include <QFont>

class CQSVGFontObj : public CSVGFontObj {
 public:
  CQSVGFontObj(const CSVGFontDef &def);

  const QFont &font() const { return qfont_; }

  void textSize(const std::string &text, double *w, double *a, double *d) const;

 private:
  void initPointSize(const CSVGFontDef &def);

 private:
  QFont          qfont_;
  double         size_ { 72 };
  mutable double scale_ { 1 };
};

#endif

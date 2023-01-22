#ifndef CQSVGParser_H
#define CQSVGParser_H

#include <QString>

class CQSVGParser {
 public:
  struct TransformData {
    bool    enabled { false };
    QString id;
    QString angle;
  };

 public:
  CQSVGParser();

  bool isDump() const { return dump_; }
  void setDump(bool b) { dump_ = b; }

  bool isPath() const { return path_; }
  void setPath(bool b) { path_ = b; }

  bool isEncode() const { return encode_; }
  void setEncode(bool b) { encode_ = b; }

  bool isXmlHeader() const { return xmlHeader_; }
  void setXmlHeader(bool b) { xmlHeader_ = b; }

  bool isTransform() const { return transformData_.enabled; }
  void setTransform(bool b, const QString &id, const QString &angle) {
    transformData_.enabled = b;
    transformData_.id      = id;
    transformData_.angle   = angle;
  }

  const TransformData &transformData() const { return transformData_; }

  const QString &filename() const { return filename_; }

  bool parse(const QString &filename);

 private:
  bool          dump_      { false };
  bool          path_      { false };
  bool          encode_    { false };
  bool          xmlHeader_ { false };
  TransformData transformData_;
  QString       filename_;
};

#endif

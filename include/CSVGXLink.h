#ifndef CSVGXLink_H
#define CSVGXLink_H

#include <CImageLib.h>

class CSVGObject;

class CSVGXLink {
 public:
  CSVGXLink(CSVGObject *parent, const std::string &str="") :
   parent_(parent), resolved_(false), str_(str) {
  }

  CSVGXLink(CSVGObject *parent, CSVGObject *object) :
   parent_(parent), resolved_(true), object_(object) {
  }

  CSVGXLink(CSVGObject *parent, CImagePtr image) :
   parent_(parent), resolved_(true), image_(image) {
  }

  CSVGXLink(const CSVGXLink &xlink);

  CSVGXLink &operator=(const CSVGXLink &xlink);

  bool isResolved() const { return resolved_; }

  std::string str() const { return str_; }

  bool isObject() const {
    resolve();

    return (object_ != 0);
  }

  CSVGObject *getObject() const {
    resolve();

    return object_;
  }

  void setObject(CSVGObject *object) {
    object_   = object;
    resolved_ = true;
  }

  bool isImage() const {
    resolve();

    return (image_.isValid());
  }

  CImagePtr getImage() const {
    resolve();

    return image_;
  }

  void setImage(CImagePtr image) {
    image_    = image;
    resolved_ = true;
  }

 private:
  void resolve() const;

 private:
  CSVGObject  *parent_   { 0 };
  bool         resolved_ { false };
  std::string  str_;
  CSVGObject  *object_   { 0 };
  CImagePtr    image_;
};

#endif

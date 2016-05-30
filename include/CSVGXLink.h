#ifndef CSVGXLink_H
#define CSVGXLink_H

#include <CMatrixStack2D.h>
#include <string>

class CSVGObject;
class CSVGBuffer;

class CSVGXLink {
 public:
  CSVGXLink(CSVGObject *parent, const std::string &str="") :
   parent_(parent), resolved_(false), str_(str) {
  }

  CSVGXLink(CSVGObject *parent, CSVGObject *object) :
   parent_(parent), resolved_(true), object_(object) {
  }

  CSVGXLink(const CSVGXLink &xlink);

  CSVGXLink &operator=(const CSVGXLink &xlink);

  bool isResolved() const { return resolved_; }

  std::string str() const { return str_; }

  bool isObject() const;

  CSVGObject *getObject() const;

  void setObject(CSVGObject *object);

  bool isImage() const;

  CSVGBuffer *getImageBuffer() const;

  void setImage(CSVGBuffer *buffer);

  bool getImage(CSVGBuffer *buffer) const;

 private:
  void resolve() const;

  void initImageBuffer();

 private:
  CSVGObject  *parent_      { 0 };
  bool         resolved_    { false };
  std::string  str_;
  CSVGObject  *object_      { 0 };
  bool         isImage_     { false };
  CSVGBuffer*  imageBuffer_ { 0 };
  double       xscale_      { 0 };
  double       yscale_      { 0 };
};

#endif

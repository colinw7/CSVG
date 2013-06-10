#ifndef CFONT_H
#define CFONT_H

#include <CRefPtr.h>
#include <CFontStyle.h>
#include <sys/types.h>
#include <map>

#define CFONT_DEF_SIZE 14

class CPixelRenderer;

struct CFontDefKey {
  CFontStyle style;
  uint       size;
  uint       angle;

  CFontDefKey(CFontStyle style1, uint size1, uint angle1) :
   style(style1), size(size1), angle(angle1) {
  }

  bool operator<(const CFontDefKey &key) const {
    if (style < key.style) return true;
    if (style > key.style) return false;

    if (size < key.size) return true;
    if (size > key.size) return false;

    if (angle < key.angle) return true;
    if (angle > key.angle) return false;

    return false;
  }
};

struct CFontDef {
  CFontStyle style;
  double     size;
  double     angle;
  uint       x_res;
  uint       y_res;

  CFontDef(CFontStyle style1=CFONT_STYLE_NORMAL, double size1=CFONT_DEF_SIZE,
           double angle1=0, uint x_res1=100, uint y_res1=100) :
   style(style1), size(size1), angle(angle1), x_res(x_res1), y_res(y_res1) {
  }

  CFontDef(const CFontDef &font_def) :
   style(font_def.style), size(font_def.size), angle(font_def.angle),
   x_res(font_def.x_res), y_res(font_def.y_res) {
  }
};

class CFontFamily {
 private:
  typedef std::map<std::string,CFontFamily *> FamilyMap;
  typedef std::map<CFontDefKey,CFontDef *>    FontDefMap;

  std::string name_;
  FontDefMap  font_defs_;

 public:
  static CFontFamily &lookup(const std::string &name);

  CFontFamily(const std::string &name);
  CFontFamily(const CFontFamily &family);
 ~CFontFamily();

  const CFontFamily &operator=(const CFontFamily &family);

  const std::string &getName() const { return name_; }

  CFontDef &lookupFontDef(CFontStyle style, uint size=CFONT_DEF_SIZE, uint angle=0);
};

class CFont;

typedef CRefPtr<CFont> CFontPtr;

class CFont {
 private:
  uint        id_;
  std::string family_;
  CFontDef    font_def_;
  double      char_angle_;
  std::string x_font_name_;

  friend class CFontMgr;

 protected:
  CFont(const std::string &family, CFontStyle style, double size,
        double angle=0, double char_angle=0, int x_res=100, int y_res=100);

  explicit CFont(const std::string &full_name);

 public:
  virtual ~CFont() { }

 protected:
  CFont(const CFont &font);

  CFont &operator=(const CFont &font);

  friend class CRefPtr<CFont>;

  static uint getNextId() {
    static uint id;

    return ++id;
  }

 public:
  virtual CFontPtr dup() const;
  virtual CFontPtr dup(const std::string &family, CFontStyle style, double size,
                       double angle, double char_angle, int x_res, int y_res) const;
  virtual CFontPtr dup(const std::string &full_name) const;

 public:
  virtual uint               getId    () const { return id_                  ; }
  virtual const std::string &getFamily() const { return family_              ; }

  virtual CFontStyle    getStyle     () const { return font_def_.style      ; }
  virtual double        getSize      () const { return font_def_.size       ; }
  virtual uint          getISize     () const { return uint(font_def_.size) ; }
  virtual double        getAngle     () const { return font_def_.angle      ; }
  virtual uint          getIAngle    () const { return uint(font_def_.angle); }
  virtual double        getCharAngle () const { return char_angle_          ; }
  virtual uint          getICharAngle() const { return uint(char_angle_)    ; }
  virtual uint          getXRes      () const { return font_def_.x_res      ; }
  virtual uint          getYRes      () const { return font_def_.y_res      ; }

 protected:
  virtual void setSize(double size) { font_def_.size = size; }

 public:
  CFontPtr rotated(double dangle=90) const;

 public:
  virtual double getCharWidth() const {
    return getSize();
  }

  virtual uint getICharWidth() const {
    return uint(getCharWidth());
  }

  virtual double getCharAscent() const {
    return getSize();
  }

  virtual uint getICharAscent() const {
    return uint(getCharAscent());
  }

  virtual double getCharDescent() const {
    return 0;
  }

  virtual uint getICharDescent() const {
    return uint(getCharDescent());
  }

  virtual double getCharHeight() const {
    return getCharAscent() + getCharDescent();
  }

  virtual uint getICharHeight() const {
    return uint(getCharHeight());
  }

  virtual double getStringWidth(const std::string &str) const {
    return str.size()*getCharWidth();
  }

  virtual uint getIStringWidth(const std::string &str) const {
    return uint(getStringWidth(str));
  }

  virtual void getStringBBox(const std::string &str, double x[4], double y[4]) const;

  virtual void getIStringBBox(const std::string &str, int x[4], int y[4]) const;

  virtual bool isProportional() const {
    return false;
  }

  virtual bool isBold() const {
    return (getStyle() & CFONT_STYLE_BOLD);
  }

  virtual bool isItalic() const {
    return (getStyle() & CFONT_STYLE_ITALIC);
  }

  virtual double getCharAspect() const {
    return (getCharWidth()/getCharHeight());
  }

  virtual std::string getXFontName() const;

  void print(std::ostream &os) const {
    os << getFamily() << ":" << fontStyleToString(getStyle()) << ":" <<
          getSize() << ":" << getAngle() << ":" << getCharAngle() << ":" <<
          getXRes() << ":" << getYRes();
  }

  friend std::ostream &operator<<(std::ostream &os, const CFont &font) {
    font.print(os);

    return os;
  }

  static bool decodeXFontName(const std::string &x_font_name, std::string &family,
                              CFontStyle &style, uint &size, uint &x_res, uint &y_res);

  static std::string encodeXFontName(const std::string &family, CFontStyle style, uint size);

  std::string encodeFontName();

  static std::string encodeFontName(const std::string &family, CFontStyle style, uint size);

  static bool decodeFontName(const std::string &name, std::string &family,
                             CFontStyle &style, uint &size);

 private:
  std::string buildXFontName() const;

 public:
  static CFontStyle  stringToFontStyle(const std::string &style);
  static std::string fontStyleToString(CFontStyle style);
};

class CFontSet {
 private:
  std::string family_;
  uint        size_;
  CFontPtr    normal_;
  CFontPtr    bold_;
  CFontPtr    italic_;
  CFontPtr    boldi_;

 public:
  CFontSet(const std::string &family="courier", uint size=12);

  void setup(const std::string &family, uint size);

  const std::string &getFamily() const { return family_; }

  uint getSize() const { return size_; }

  CFontPtr getFont(CFontStyle style) const;
};

#endif

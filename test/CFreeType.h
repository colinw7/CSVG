#ifndef CFREE_TYPE_H
#define CFREE_TYPE_H

#ifndef FT_FREETYPE_H
typedef struct FT_LibraryRec_ *FT_Library;
typedef struct FT_FaceRec_    *FT_Face;
typedef struct FT_Bitmap_      FT_Bitmap;
typedef struct FT_Vector_      FT_Vector;
#endif

#include <CPoint2D.h>
#include <CFont.h>
#include <CConfig.h>
#include <CImageLib.h>
#include <string>

#define CFreeTypeMgrInst CFreeTypeMgr::getInstance()

class CFreeType;

class CFreeTypeImageRenderer {
 public:
  CFreeTypeImageRenderer() { }

  virtual ~CFreeTypeImageRenderer() { }

  virtual void drawPoint(const CIPoint2D &p, double gray) = 0;
};

class CFreeTypePathRenderer {
 public:
  CFreeTypePathRenderer() :
   current_(0,0), current_set_(false) {
  }

  virtual ~CFreeTypePathRenderer() { }

  virtual void init() = 0;

  virtual void moveTo(const CPoint2D &p) = 0;
  virtual void lineTo(const CPoint2D &p) = 0;
  virtual void bezier2To(const CPoint2D &p1, const CPoint2D &p2) = 0;
  virtual void bezier3To(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3) = 0;

  virtual void close() = 0;

  virtual void stroke() = 0;
  virtual void fill() = 0;

  void setCurrentPoint(const CPoint2D &p) {
    current_     = p;
    current_set_ = true;
  }

  bool getCurrentPoint(CPoint2D &p) const {
    p = current_;

    return current_set_;
  }

  void resetCurrentPoint() {
    current_set_ = false;
  }

 private:
  CPoint2D current_;
  bool     current_set_;
};

class CFreeTypeMgr {
 private:
  CFreeType *free_type_;

 public:
  static CFreeTypeMgr *getInstance();

 private:
  CFreeTypeMgr();
 ~CFreeTypeMgr();

 public:
  CFontPtr getFont() const;

  bool setFont(const char *fontName, CFontStyle style, double size);
  bool setFont(CFontPtr font);

  bool drawString(CFreeTypeImageRenderer *renderer, const char *s, double *x, double *y);
  bool drawChar(CFreeTypeImageRenderer *renderer, char c, double *x, double *y);

  bool imageString(CImagePtr image, const char *s, const CRGBA &color);
  bool imageChar(CImagePtr image, char c, const CRGBA &color);

  bool strokeString(CFreeTypePathRenderer *renderer, const char *s, double *x, double *y);
  bool fillString(CFreeTypePathRenderer *renderer, const char *s, double *x, double *y);
  bool pathString(CFreeTypePathRenderer *path, const char *s, double *x, double *y);

  bool strokeChar(CFreeTypePathRenderer *renderer, char c, double *x, double *y);
  bool fillChar(CFreeTypePathRenderer *renderer, char c, double *x, double *y);

  bool pathChar(CFreeTypePathRenderer *path, char c, double *x, double *y);

 private:
  CFreeTypeMgr(const CFreeTypeMgr &rhs);
  CFreeTypeMgr &operator=(const CFreeTypeMgr &rhs);
};

class CFreeType {
 public:
  CFreeType();
 ~CFreeType();

  CFontPtr getFont() const { return font_; }

  bool setFont(const char *fontName, CFontStyle style, double size);
  bool setFont(CFontPtr font);

  bool drawString(CFreeTypeImageRenderer *renderer, const char *s, double *x, double *y);
  bool drawChar(CFreeTypeImageRenderer *renderer, char c, double *x, double *y);

  bool imageString(CImagePtr image, const char *s, const CRGBA &color);
  bool imageChar(CImagePtr image, char c, const CRGBA &color);

  bool strokeString(CFreeTypePathRenderer *renderer, const char *s, double *x, double *y);
  bool fillString(CFreeTypePathRenderer *renderer, const char *s, double *x, double *y);
  bool pathString(CFreeTypePathRenderer *path, const char *s, double *x, double *y);

  bool strokeChar(CFreeTypePathRenderer *renderer, char c, double *x, double *y);
  bool fillChar(CFreeTypePathRenderer *renderer, char c, double *x, double *y);

  bool pathChar(CFreeTypePathRenderer *path, char c, double *x, double *y);

 private:
  bool init();

  std::string encodeFontName(const char *fontName, CFontStyle style);

  bool getChar(char c);

  void drawBitmap(CFreeTypeImageRenderer *renderer, FT_Bitmap *bitmap, double x, double y);

  bool imageChar1(CImagePtr image, char c, int *x1, int *y1, int *x2, int *y2, const CRGBA &color);

  void imageBitmap(CImagePtr image, FT_Bitmap *bitmap, const CRGBA &color);

  static int traceMoveto(FT_Vector *v, CFreeType *th);
  static int traceLineto(FT_Vector *v, CFreeType *th);
  static int traceBezier2(FT_Vector *v1, FT_Vector *v2, CFreeType *th);
  static int traceBezier3(FT_Vector *v1, FT_Vector *v2, FT_Vector *v3, CFreeType *th);

 private:
  CFreeType(const CFreeType &rhs);
  CFreeType &operator=(const CFreeType &rhs);

 private:
  CConfig    config_;
  FT_Library library_;
  FT_Face    face_;
  CFontPtr   font_;

  mutable CPoint2D              pos_;
  mutable CFreeTypePathRenderer *renderer_;
};

#endif

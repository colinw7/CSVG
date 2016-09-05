#include <CFreeType.h>
#include <CFontMgr.h>
#include <CAutoPtr.h>

#include <ft2build.h>
#include <freetype/ftoutln.h>
#include FT_FREETYPE_H

#include <vector>

CFreeTypeMgr *
CFreeTypeMgr::
getInstance()
{
  static CFreeTypeMgr *instance;

  if (! instance)
    instance = new CFreeTypeMgr;

  return instance;
}

CFreeTypeMgr::
CFreeTypeMgr()
{
  free_type_ = new CFreeType;
}

CFreeTypeMgr::
~CFreeTypeMgr()
{
  delete free_type_;
}

CFontPtr
CFreeTypeMgr::
getFont() const
{
  return free_type_->getFont();
}

bool
CFreeTypeMgr::
setFont(const char *fontName, CFontStyle style, double size)
{
  return free_type_->setFont(fontName, style, size);
}

bool
CFreeTypeMgr::
setFont(CFontPtr font)
{
  return free_type_->setFont(font);
}

bool
CFreeTypeMgr::
drawString(CFreeTypeImageRenderer *renderer, const char *s, double *x, double *y)
{
  return free_type_->drawString(renderer, s, x, y);
}

bool
CFreeTypeMgr::
drawChar(CFreeTypeImageRenderer *renderer, char c, double *x, double *y)
{
  return free_type_->drawChar(renderer, c, x, y);
}

bool
CFreeTypeMgr::
imageString(CImagePtr image, const char *s, const CRGBA &color)
{
  return free_type_->imageString(image, s, color);
}

bool
CFreeTypeMgr::
imageChar(CImagePtr image, char c, const CRGBA &color)
{
  return free_type_->imageChar(image, c, color);
}

bool
CFreeTypeMgr::
strokeString(CFreeTypePathRenderer *renderer, const char *s, double *x, double *y)
{
  return free_type_->strokeString(renderer, s, x, y);
}

bool
CFreeTypeMgr::
fillString(CFreeTypePathRenderer *renderer, const char *s, double *x, double *y)
{
  return free_type_->fillString(renderer, s, x, y);
}

bool
CFreeTypeMgr::
pathString(CFreeTypePathRenderer *renderer, const char *s, double *x, double *y)
{
  return free_type_->pathString(renderer, s, x, y);
}

bool
CFreeTypeMgr::
strokeChar(CFreeTypePathRenderer *renderer, char c, double *x, double *y)
{
  return free_type_->strokeChar(renderer, c, x, y);
}

bool
CFreeTypeMgr::
fillChar(CFreeTypePathRenderer *renderer, char c, double *x, double *y)
{
  return free_type_->fillChar(renderer, c, x, y);
}

bool
CFreeTypeMgr::
pathChar(CFreeTypePathRenderer *renderer, char c, double *x, double *y)
{
  return free_type_->pathChar(renderer, c, x, y);
}

//----------------

CFreeType::
CFreeType() :
 config_("CFreeType"), library_(0), face_(0)
{
  init();
}

CFreeType::
~CFreeType()
{
}

bool
CFreeType::
init()
{
  renderer_ = NULL;

  int error = FT_Init_FreeType(&library_);

  if (error) {
    std::cerr << "An error occurred during library initialization" << std::endl;
    return false;
  }

  CFontPtr font = CFontMgrInst->lookupFont("arial", CFONT_STYLE_NORMAL, 14);

  setFont(font);

  return true;
}

bool
CFreeType::
setFont(const char *family, CFontStyle style, double size)
{
  CFontPtr font = CFontMgrInst->lookupFont(family, style, size);

  return setFont(font);
}

bool
CFreeType::
setFont(CFontPtr font)
{
  if (! font.isValid())
    return false;

  font_ = font;

  const std::string &family = font->getFamily();
  CFontStyle        style  = font->getStyle();
  double            size   = font->getSize();

  std::string fontName = encodeFontName(family.c_str(), style);

  int face_index = 0;

  int error = FT_New_Face(library_, fontName.c_str(), face_index, &face_);
  //int error = FT_New_Memory_Face(library_, buffer, size, face_index, &face_);

  if (error) {
    std::cerr << "Can't set font " << fontName << std::endl;

    fontName = encodeFontName("arial", style);

    error = FT_New_Face(library_, fontName.c_str(), face_index, &face_);

    if (error)
      return false;
  }

  int char_width  = 0;
  int char_height = int(64*size);
  int x_res       = 75; // dpi
  int y_res       = 75;

  error = FT_Set_Char_Size(face_, char_width, char_height, x_res, y_res);

  // error = FT_Set_Pixel_Sizes(face_, char_width, char_height);

  return true;
}

std::string
CFreeType::
encodeFontName(const char *family, CFontStyle style)
{
  std::string fontName = family;

  if (style & CFONT_STYLE_BOLD)
    fontName += "-bold";

  if (style & CFONT_STYLE_ITALIC)
    fontName += "-italic";

  std::string fontPath;

  if (! config_.getValue(fontName, fontPath))
    fontPath = fontName;

  return fontPath;
}

bool
CFreeType::
getChar(char c)
{
  if (! face_) return false;

  uint c1 = (c >= 0 ? c : 256 + c);

#if 0
  FT_UInt glyph_index = FT_Get_Char_Index(face_, c1);

  //if (glyph_index == 0) return false;

  // error = FT_Set_Transform(face_, &matrix, &delta);

  int load_flags = FT_LOAD_DEFAULT;

  int error = FT_Load_Glyph(face_, glyph_index, load_flags);

  if (error) return false;

  int render_mode = FT_RENDER_MODE_NORMAL;

  error = FT_Render_Glyph(face_->glyph, render_mode);

  if (error) return false;
#else
  int error = FT_Load_Char(face_, c1, FT_LOAD_RENDER);

  if (error) return false;
#endif

  return true;
}

bool
CFreeType::
drawString(CFreeTypeImageRenderer *renderer, const char *s, double *x, double *y)
{
  uint len = strlen(s);

  for (uint i = 0; i < len; ++i)
    (void) drawChar(renderer, s[i], x, y);

  return true;
}

bool
CFreeType::
drawChar(CFreeTypeImageRenderer *renderer, char c, double *x, double *y)
{
  if (! getChar(c)) return false;

  drawBitmap(renderer, &face_->glyph->bitmap, *x + face_->glyph->bitmap_left,
             *y - face_->glyph->bitmap_top);

  *x += face_->glyph->advance.x/64.0;
  *y += face_->glyph->advance.y/64.0;

  return true;
}

void
CFreeType::
drawBitmap(CFreeTypeImageRenderer *renderer, FT_Bitmap *bitmap, double x, double y)
{
  uchar *p = bitmap->buffer;

  for (uint r = 0; r < bitmap->rows; ++r) {
    uchar *p1 = p;

    for (uint c = 0; c < bitmap->width; ++c, ++p1) {
      uchar pixel = *p1;

      if (pixel == 0) continue;

      double gray = 1.0;

      if (bitmap->pixel_mode == FT_PIXEL_MODE_GRAY)
        gray = pixel/255.0;

      renderer->drawPoint(CIPoint2D(x + c, y + r), gray);
    }

    p += bitmap->pitch;
  }
}

struct ImageXY {
  CImagePtr image;
  int       x, y;
  int       l, ascent, descent;

  ImageXY(CImagePtr image1, int x1, int y1, int l1, int ascent1, int descent1) :
   image(image1), x(x1), y(y1), l(l1), ascent(ascent1), descent(descent1) {
  }
};

bool
CFreeType::
imageString(CImagePtr image, const char *s, const CRGBA &color)
{
  std::vector<ImageXY> char_images;

  int w = 0, ascent = 0, descent = 0;

  int x1 = 0, y1 = 0;

  uint len = strlen(s);

  for (uint i = 0; i < len; ++i) {
    y1 = 0;

    CImagePtr char_image = CImageMgrInst->createImage(CImageNoSrc());

    int x2 = x1;
    int y2 = y1;

    int l = 0, t = 0;

    (void) imageChar1(char_image, s[i], &l, &t, &x2, &y2, color);

    w = x2;

    int ascent1  = char_image->getHeight();
    int descent1 = 0;

    if (t < 0) {
      ascent1  += t;
      descent1  = -t;
    }

    ascent  = std::max( ascent,  ascent1);
    descent = std::max(descent, descent1);

    char_images.push_back(ImageXY(char_image, x1, y1, l, ascent1, descent1));

    x1 = x2;
  }

  image->setDataSize(w, ascent + descent);

  image->setRGBAData(CRGBA(0,0,0,0));

  uint n = char_images.size();

  for (uint i = 0; i < n; ++i) {
    ImageXY &char_image = char_images[i];

    int x = char_image.x + char_image.l;

    int y = ascent - char_image.ascent;

    char_images[i].image->copyTo(image, x, y);
  }

  return true;
}

bool
CFreeType::
imageChar(CImagePtr image, char c, const CRGBA &color)
{
  int l = 0, t = 0, x = 0, y = 0;

  return imageChar1(image, c, &l, &t, &x, &y, color);
}

bool
CFreeType::
imageChar1(CImagePtr image, char c, int *l, int *t, int *x, int *y, const CRGBA &color)
{
  if (! getChar(c)) return false;

  *l = face_->glyph->bitmap_left;

  if (face_->glyph->bitmap_top > 0)
    *t = face_->glyph->bitmap_top - face_->glyph->bitmap.rows;
  else
    *t = face_->glyph->bitmap_top;

  imageBitmap(image, &face_->glyph->bitmap, color);

  *x += face_->glyph->advance.x/64;
  *y += face_->glyph->advance.y/64;

  return true;
}

void
CFreeType::
imageBitmap(CImagePtr image, FT_Bitmap *bitmap, const CRGBA &color)
{
  image->setDataSize(bitmap->width, bitmap->rows);

  image->setRGBAData(CRGBA(0,0,0,0));

  uchar *p = bitmap->buffer;

  double r = color.getRed  ();
  double g = color.getGreen();
  double b = color.getBlue ();

  for (uint row = 0; row < bitmap->rows; ++row) {
    uchar *p1 = p;

    for (uint col = 0; col < bitmap->width; ++col, ++p1) {
      uchar pixel = *p1;

      if (pixel == 0) continue;

      if (bitmap->pixel_mode == FT_PIXEL_MODE_GRAY) {
        double gray = pixel/255.0;

        image->setRGBAPixel(col, row, CRGBA(r, g, b, gray));
      }
      else
        image->setRGBAPixel(col, row, CRGBA(r, g, b, 1));
    }

    p += bitmap->pitch;
  }
}

bool
CFreeType::
strokeString(CFreeTypePathRenderer *renderer, const char *s, double *x, double *y)
{
  uint len = strlen(s);

  for (uint i = 0; i < len; ++i)
    (void) strokeChar(renderer, s[i], x, y);

  return true;
}

bool
CFreeType::
fillString(CFreeTypePathRenderer *renderer, const char *s, double *x, double *y)
{
  uint len = strlen(s);

  for (uint i = 0; i < len; ++i)
    (void) fillChar(renderer, s[i], x, y);

  return true;
}

bool
CFreeType::
pathString(CFreeTypePathRenderer *renderer, const char *s, double *x, double *y)
{
  if (! font_.isValid())
    return false;

  const std::string &family = font_->getFamily();
  CFontStyle         style  = font_->getStyle();
  double             size   = font_->getSize();

  renderer->init();

  uint len = strlen(s);

  uint i = 0;

  while (i < len) {
    if (s[i] == '') {
      ++i;

      if (i >= len || s[i] != '[')
        continue;

      ++i;

      int num = 0;

      while (i < len && isdigit(s[i])) {
        num = 10*num + (s[i] - '0');

        ++i;
      }

      if (i >= len || s[i] != 'm')
        continue;

      ++i;

      CFontPtr font1;

      if      (num == 0)
        font1 = CFontMgrInst->lookupFont(family, CFONT_STYLE_NORMAL, size);
      else if (num == 1)
        font1 = CFontMgrInst->lookupFont(family, CFONT_STYLE_BOLD  , size);
      else if (num == 3)
        font1 = CFontMgrInst->lookupFont(family, CFONT_STYLE_ITALIC, size);

      if (font1.isValid())
        setFont(font1);
    }
    else {
      (void) pathChar(renderer, s[i], x, y);

      ++i;
    }
  }

  font_ = CFontMgrInst->lookupFont(family, style, size);

  return true;
}

bool
CFreeType::
strokeChar(CFreeTypePathRenderer *renderer, char c, double *x, double *y)
{
  renderer->init();

  if (! pathChar(renderer, c, x, y))
    return false;

  renderer->stroke();

  return true;
}

bool
CFreeType::
fillChar(CFreeTypePathRenderer *renderer, char c, double *x, double *y)
{
  renderer->init();

  if (! pathChar(renderer, c, x, y))
    return false;

  renderer->fill();

  return true;
}

bool
CFreeType::
pathChar(CFreeTypePathRenderer *renderer, char c, double *x, double *y)
{
  if (! face_) return false;

  renderer_ = renderer;

  uint c1 = (c >= 0 ? c : 256 + c);

  FT_UInt glyph_index = FT_Get_Char_Index(face_, c1);

  if (glyph_index == 0) return false;

  int error1 = FT_Load_Glyph(face_, glyph_index, FT_LOAD_NO_HINTING | FT_LOAD_NO_BITMAP);

  if (error1) return false;

  pos_ = CPoint2D(*x, *y);

  FT_Outline_Funcs outlineMethods = {
    (FT_Outline_MoveTo_Func ) CFreeType::traceMoveto,
    (FT_Outline_LineTo_Func ) CFreeType::traceLineto,
    (FT_Outline_ConicTo_Func) CFreeType::traceBezier2,
    (FT_Outline_CubicTo_Func) CFreeType::traceBezier3,
    0, // shift
    0  // delta
  };

  int error2 = FT_Outline_Decompose(&face_->glyph->outline, &outlineMethods,
                                    reinterpret_cast<void*>(this));

  if (error2)
    return false;

  renderer->close();

  *x += face_->glyph->advance.x/64.0;
  *y += face_->glyph->advance.y/64.0;

  return true;
}

int
CFreeType::
traceMoveto(FT_Vector *v, CFreeType *th)
{
  CPoint2D current;

  if (th->renderer_->getCurrentPoint(current))
    th->renderer_->close();

  double x = v->x/64.0;
  double y = v->y/64.0;

  CPoint2D p = th->pos_ + CPoint2D(x, y);

  th->renderer_->moveTo(p);

  th->renderer_->setCurrentPoint(p);

  return 0;
}

int
CFreeType::
traceLineto(FT_Vector *v, CFreeType *th)
{
  double x = v->x/64.0;
  double y = v->y/64.0;

  CPoint2D p = th->pos_ + CPoint2D(x, y);

  th->renderer_->lineTo(p);

  th->renderer_->setCurrentPoint(p);

  return 0;
}

int
CFreeType::
traceBezier2(FT_Vector *v1, FT_Vector *v2, CFreeType *th)
{
  double x1 = v1->x/64.0;
  double y1 = v1->y/64.0;
  double x2 = v2->x/64.0;
  double y2 = v2->y/64.0;

  CPoint2D p1 = th->pos_ + CPoint2D(x1, y1);
  CPoint2D p2 = th->pos_ + CPoint2D(x2, y2);

  th->renderer_->bezier2To(p1, p2);

  th->renderer_->setCurrentPoint(p2);

  return 0;
}

int
CFreeType::
traceBezier3(FT_Vector *v1, FT_Vector *v2, FT_Vector *v3, CFreeType *th)
{
  double x1 = v1->x/64.0;
  double y1 = v1->y/64.0;
  double x2 = v2->x/64.0;
  double y2 = v2->y/64.0;
  double x3 = v3->x/64.0;
  double y3 = v3->y/64.0;

  CPoint2D p1 = th->pos_ + CPoint2D(x1, y1);
  CPoint2D p2 = th->pos_ + CPoint2D(x2, y2);
  CPoint2D p3 = th->pos_ + CPoint2D(x3, y3);

  th->renderer_->bezier3To(p1, p2, p3);

  th->renderer_->setCurrentPoint(p3);

  return 0;
}

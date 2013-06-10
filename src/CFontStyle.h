#ifndef CFONT_STYLE_H
#define CFONT_STYLE_H

#include <CFlags.h>

enum CFontStyle {
  CFONT_STYLE_NORMAL      = 0,
  CFONT_STYLE_BOLD        = (1<<0),
  CFONT_STYLE_ITALIC      = (1<<1),
  CFONT_STYLE_UNDERLINE   = (1<<2),
  CFONT_STYLE_STRIKEOUT   = (1<<3),
  CFONT_STYLE_BOLD_ITALIC = (CFONT_STYLE_BOLD|CFONT_STYLE_ITALIC)
};

typedef CFlags<CFontStyle> CFontStyles;

#endif

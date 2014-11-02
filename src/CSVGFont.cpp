#include <CSVGI.h>

CSVGFont::
CSVGFont(CSVG &svg) :
 CSVGObject(svg),
 hxo_              (0),
 hyo_              (0),
 hdx_              (0),
 vxo_              (0),
 vyo_              (0),
 vdy_              (0),
 font_face_        (NULL),
 missing_glyph_    (NULL),
 char_glyph_map_   (),
 unicode_glyph_map_()
{
}

CSVGFont::
CSVGFont(const CSVGFont &font) :
 CSVGObject        (font),
 hxo_              (font.hxo_),
 hyo_              (font.hyo_),
 hdx_              (font.hdx_),
 vxo_              (font.vxo_),
 vyo_              (font.vyo_),
 vdy_              (font.vdy_),
 font_face_        (font.font_face_),
 missing_glyph_    (font.missing_glyph_),
 char_glyph_map_   (font.char_glyph_map_),
 unicode_glyph_map_(font.unicode_glyph_map_)
{
}

CSVGFont *
CSVGFont::
dup() const
{
  return new CSVGFont(*this);
}

bool
CSVGFont::
processOption(const string &opt_name, const string &opt_value)
{
  long integer;

  if      (svg_.integerOption(opt_name, opt_value, "horiz-origin-x", &integer))
    hxo_ = integer;
  else if (svg_.integerOption(opt_name, opt_value, "horiz-origin-y", &integer))
    hyo_ = integer;
  else if (svg_.integerOption(opt_name, opt_value, "horiz-adv-x", &integer))
    hdx_ = integer;
  else if (svg_.integerOption(opt_name, opt_value, "vert-origin-x", &integer))
    vxo_ = integer;
  else if (svg_.integerOption(opt_name, opt_value, "vert-origin-y", &integer))
    vyo_ = integer;
  else if (svg_.integerOption(opt_name, opt_value, "vert-adv-y", &integer))
    vdy_ = integer;
  else
    return CSVGObject::processOption(opt_name, opt_value);

  return true;
}

void
CSVGFont::
termParse()
{
  vector<CSVGObject *> objects;

  getChildrenOfType(CSVG_OBJ_TYPE_FONT_FACE, objects);

  if (objects.empty()) {
    CSVGLog() << "No font face for font";
    return;
  }

  font_face_ = dynamic_cast<CSVGFontFace *>(objects[0]);

  //------

  getChildrenOfType(CSVG_OBJ_TYPE_MISSING_GLYPH, objects);

  if (objects.empty()) {
    CSVGLog() << "No default glyph for font";
    return;
  }

  missing_glyph_ = dynamic_cast<CSVGMissingGlyph *>(objects[0]);

  //------

  getChildrenOfType(CSVG_OBJ_TYPE_GLYPH, objects);

  vector<CSVGObject *>::iterator p1 = objects.begin();
  vector<CSVGObject *>::iterator p2 = objects.end  ();

  for ( ; p1 != p2; ++p1) {
    CSVGGlyph *glyph = dynamic_cast<CSVGGlyph *>(*p1);

    string unicode = glyph->getUnicode();

    unicode_glyph_map_[unicode] = glyph;

    if (unicode.size() == 1)
      char_glyph_map_[unicode[0]] = glyph;
  }

  //------

  svg_.addFont(this);
}

void
CSVGFont::
draw()
{
}

CSVGGlyph *
CSVGFont::
getCharGlyph(char c) const
{
  CharGlyphMap::const_iterator p = char_glyph_map_.find(c);

  if (p != char_glyph_map_.end())
    return (*p).second;

  return missing_glyph_;
}

CSVGGlyph *
CSVGFont::
getUnicodeGlyph(const string &unicode) const
{
  UnicodeGlyphMap::const_iterator p = unicode_glyph_map_.find(unicode);

  if (p != unicode_glyph_map_.end())
    return (*p).second;

  return missing_glyph_;
}

void
CSVGFont::
print(ostream &os) const
{
  os << "font";
}

ostream &
operator<<(ostream &os, const CSVGFont &font)
{
  font.print(os);

  return os;
}

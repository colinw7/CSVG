#include <CSVGFont.h>
#include <CSVGGlyph.h>
#include <CSVGFontFace.h>
#include <CSVGMissingGlyph.h>
#include <CSVG.h>
#include <CSVGLog.h>

CSVGFont::
CSVGFont(CSVG &svg) :
 CSVGObject(svg)
{
}

CSVGFont::
CSVGFont(const CSVGFont &font) :
 CSVGObject        (font),
 hxo_              (font.hxo_),
 hyo_              (font.hyo_),
 hdx_              (font.hdx_),
 vdy_              (font.vdy_),
 vxo_              (font.vxo_),
 vyo_              (font.vyo_),
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
processOption(const std::string &opt_name, const std::string &opt_value)
{
  double real;

  if      (svg_.realOption(opt_name, opt_value, "horiz-origin-x", &real))
    hxo_ = real;
  else if (svg_.realOption(opt_name, opt_value, "horiz-origin-y", &real))
    hyo_ = real;
  else if (svg_.realOption(opt_name, opt_value, "horiz-adv-x", &real))
    hdx_ = real;
  else if (svg_.realOption(opt_name, opt_value, "vert-adv-y", &real))
    vdy_ = real;
  else if (svg_.realOption(opt_name, opt_value, "vert-origin-x", &real))
    vxo_ = real;
  else if (svg_.realOption(opt_name, opt_value, "vert-origin-y", &real))
    vyo_ = real;
  else
    return CSVGObject::processOption(opt_name, opt_value);

  return true;
}

void
CSVGFont::
termParse()
{
  std::vector<CSVGObject *> objects;

  getChildrenOfType(CSVGObjTypeId::FONT_FACE, objects);

  if (objects.empty()) {
    CSVGLog() << "No font face for font";
    return;
  }

  font_face_ = dynamic_cast<CSVGFontFace *>(objects[0]);

  //------

  getChildrenOfType(CSVGObjTypeId::MISSING_GLYPH, objects);

  if (objects.empty()) {
    CSVGLog() << "No default glyph for font";
    return;
  }

  missing_glyph_ = dynamic_cast<CSVGMissingGlyph *>(objects[0]);

  //------

  getChildrenOfType(CSVGObjTypeId::GLYPH, objects);

  for (const auto &o : objects) {
    auto *glyph = dynamic_cast<CSVGGlyph *>(o);
    if (! glyph) continue;

    auto unicode = glyph->getUnicode();

    unicode_glyph_map_[unicode] = glyph;

    if (unicode.size() == 1)
      char_glyph_map_[unicode[0]] = glyph;
  }

  //------

  svg_.addFont(this);
}

bool
CSVGFont::
draw()
{
  return false;
}

CSVGGlyph *
CSVGFont::
getCharGlyph(char c) const
{
  auto p = char_glyph_map_.find(c);

  if (p != char_glyph_map_.end())
    return (*p).second;

  return missing_glyph_;
}

CSVGGlyph *
CSVGFont::
getUnicodeGlyph(const std::string &unicode) const
{
  auto p = unicode_glyph_map_.find(unicode);

  if (p != unicode_glyph_map_.end())
    return (*p).second;

  return missing_glyph_;
}

void
CSVGFont::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<font";

    printValues(os);

    os << ">\n";

    printChildren(os, hier);

    os << "</font>\n";
  }
  else
    os << "font";
}

void
CSVGFont::
printValues(std::ostream &os, bool flat) const
{
  CSVGObject::printValues(os, flat);

  printNameValue(os, "horiz-origin-x", hxo_);
  printNameValue(os, "horiz-origin-y", hyo_);
  printNameValue(os, "horiz-adv-x"   , hdx_);
  printNameValue(os, "vert-adv-y"    , vdy_);
  printNameValue(os, "vert-origin-x" , vxo_);
  printNameValue(os, "vert-origin-y" , vyo_);
}

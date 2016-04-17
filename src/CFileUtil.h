#include <CFileType.h>
#include <CStrUtil.h>

class CFile;

namespace CFileUtil {
  inline CFileType getType(const std::string &filename) {
    struct stat file_stat;

    if (::stat(filename.c_str(), &file_stat) < 0)
      return CFILE_TYPE_NONE;

    if (S_ISFIFO(file_stat.st_mode)) return CFILE_TYPE_INODE_FIFO;
    if (S_ISCHR (file_stat.st_mode)) return CFILE_TYPE_INODE_CHR ;
    if (S_ISDIR (file_stat.st_mode)) return CFILE_TYPE_INODE_DIR ;
    if (S_ISBLK (file_stat.st_mode)) return CFILE_TYPE_INODE_BLK ;
    if (S_ISREG (file_stat.st_mode)) return CFILE_TYPE_INODE_REG ;
    if (S_ISLNK (file_stat.st_mode)) return CFILE_TYPE_INODE_LNK ;
    if (S_ISSOCK(file_stat.st_mode)) return CFILE_TYPE_INODE_SOCK;

    return CFILE_TYPE_NONE;
  }

  inline CFileType getType(CFile *file) {
    return file->getType();
  }

  inline std::string getTypeStr(CFileType) {
    return ""; // TODO
  }

  inline CFileType getImageType(CFile * /*file*/) {
    return CFILE_TYPE_NONE;
  }

  inline bool isImageType(CFileType) {
    return false; // TODO
  }

  inline CFileType getImageTypeFromName(const std::string &filename) {
    std::string suffix;

    size_t pos = filename.rfind('.');

    if (pos != std::string::npos)
      suffix = filename.substr(pos + 1);

    if      (CStrUtil::casecmp(suffix, "bmp") == 0)
      return CFILE_TYPE_IMAGE_BMP;
    else if (CStrUtil::casecmp(suffix, "eps") == 0)
      return CFILE_TYPE_APP_EPS;
    else if (CStrUtil::casecmp(suffix, "gif") == 0)
      return CFILE_TYPE_IMAGE_GIF;
    else if (CStrUtil::casecmp(suffix, "ico") == 0)
      return CFILE_TYPE_IMAGE_ICO;
    else if (CStrUtil::casecmp(suffix, "iff") == 0)
      return CFILE_TYPE_IMAGE_IFF;
    else if (CStrUtil::casecmp(suffix, "jpg" ) == 0 || CStrUtil::casecmp(suffix, "jpeg") == 0)
      return CFILE_TYPE_IMAGE_JPG;
    else if (CStrUtil::casecmp(suffix, "pcx") == 0)
      return CFILE_TYPE_IMAGE_PCX;
    else if (CStrUtil::casecmp(suffix, "png") == 0)
      return CFILE_TYPE_IMAGE_PNG;
    else if (CStrUtil::casecmp(suffix, "ppm") == 0)
      return CFILE_TYPE_IMAGE_PPM;
    else if (CStrUtil::casecmp(suffix, "psp") == 0)
      return CFILE_TYPE_IMAGE_PSP;
    else if (CStrUtil::casecmp(suffix, "ps") == 0)
      return CFILE_TYPE_IMAGE_PS;
    else if (CStrUtil::casecmp(suffix, "rgb") == 0 || CStrUtil::casecmp(suffix, "sgi") == 0)
      return CFILE_TYPE_IMAGE_SGI;
    else if (CStrUtil::casecmp(suffix, "tga") == 0)
      return CFILE_TYPE_IMAGE_TGA;
    else if (CStrUtil::casecmp(suffix, "tif") == 0)
      return CFILE_TYPE_IMAGE_TIF;
    else if (CStrUtil::casecmp(suffix, "xbm") == 0)
      return CFILE_TYPE_IMAGE_XBM;
    else if (CStrUtil::casecmp(suffix, "xpm") == 0)
      return CFILE_TYPE_IMAGE_XPM;
    else if (CStrUtil::casecmp(suffix, "xwd") == 0)
      return CFILE_TYPE_IMAGE_XWD;
    else
      return CFILE_TYPE_NONE;
  }

  inline std::string getPrefix(CFileType type) {
    std::string prefix = "??";

    switch (type) {
      case CFILE_TYPE_INODE_FIFO : prefix = "fifo"  ; break;
      case CFILE_TYPE_INODE_CHR  : prefix = "cdev"  ; break;
      case CFILE_TYPE_INODE_DIR  : prefix = ""      ; break;
      case CFILE_TYPE_INODE_BLK  : prefix = "bdev"  ; break;
      case CFILE_TYPE_INODE_REG  : prefix = "txt"   ; break;
      case CFILE_TYPE_INODE_LNK  : prefix = "lnk"   ; break;
      case CFILE_TYPE_INODE_SOCK : prefix = "socket"; break;

      case CFILE_TYPE_TEXT_PLAIN : prefix = "txt"   ; break;
      case CFILE_TYPE_TEXT_BINARY: prefix = "bin"   ; break;
      case CFILE_TYPE_TEXT_HTML  : prefix = "html"  ; break;
      case CFILE_TYPE_TEXT_XML   : prefix = "xml"   ; break;
      case CFILE_TYPE_TEXT_TAR   : prefix = "tar"   ; break;
      case CFILE_TYPE_TEXT_TGZ   : prefix = "tgz"   ; break;
      case CFILE_TYPE_TEXT_GZIP  : prefix = "gz"    ; break;

      case CFILE_TYPE_IMAGE_BMP  : prefix = "bmp"   ; break;
      case CFILE_TYPE_IMAGE_EPS  : prefix = "eps"   ; break;
      case CFILE_TYPE_IMAGE_GIF  : prefix = "gif"   ; break;
      case CFILE_TYPE_IMAGE_ICO  : prefix = "ico"   ; break;
      case CFILE_TYPE_IMAGE_IFF  : prefix = "iff"   ; break;
      case CFILE_TYPE_IMAGE_JPG  : prefix = "jpg"   ; break;
      case CFILE_TYPE_IMAGE_PCX  : prefix = "pcx"   ; break;
      case CFILE_TYPE_IMAGE_PNG  : prefix = "png"   ; break;
      case CFILE_TYPE_IMAGE_PPM  : prefix = "ppm"   ; break;
      case CFILE_TYPE_IMAGE_PS   : prefix = "ps"    ; break;
      case CFILE_TYPE_IMAGE_PSP  : prefix = "psp"   ; break;
      case CFILE_TYPE_IMAGE_SGI  : prefix = "rgb"   ; break;
      case CFILE_TYPE_IMAGE_SVG  : prefix = "svg"   ; break;
      case CFILE_TYPE_IMAGE_TGA  : prefix = "tga"   ; break;
      case CFILE_TYPE_IMAGE_TIF  : prefix = "tif"   ; break;
      case CFILE_TYPE_IMAGE_XBM  : prefix = "xbm"   ; break;
      case CFILE_TYPE_IMAGE_XPM  : prefix = "xpm"   ; break;
      case CFILE_TYPE_IMAGE_XWD  : prefix = "xwd"   ; break;

      case CFILE_TYPE_APP_EXEC   : prefix = "exe"   ; break;
      case CFILE_TYPE_APP_CORE   : prefix = ""      ; break;
      case CFILE_TYPE_APP_SH     : prefix = "sh"    ; break;
      case CFILE_TYPE_APP_CSH    : prefix = "csh"   ; break;
      case CFILE_TYPE_APP_KSH    : prefix = "ksh"   ; break;

      // TODO: Python, ...

      case CFILE_TYPE_SOUND_WAV  : prefix = "wav"   ; break;

      default:                                        break;
    }

    return prefix;
  }

  inline CFileType getImageType(const std::string &filename) {
    return getImageTypeFromName(filename);
  }
}

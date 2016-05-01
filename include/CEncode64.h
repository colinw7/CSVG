#ifndef CENCODE_64_H
#define CENCODE_64_H

#include <string>
#include <sys/types.h>

class CFile;

#define CEncode64Inst CEncode64::getInstance()

class CEncode64 {
 private:
  char base_64_chars_[65];
  char end_char_;

 public:
  static CEncode64 *getInstance();

  void setChars(const char base_64_chars[64]);

  void setEndChar(char c) { end_char_ = c; }

  bool encode(CFile *ifile, CFile *ofile);

  std::string encode(const std::string &str);

  bool encode(const unsigned char *in, size_t in_len, unsigned char *out, size_t out_size);

  std::string decode(const std::string &str);

 private:
  CEncode64();
 ~CEncode64();

  void encodeBlock(const unsigned char *in, unsigned char *out, size_t len);

  int addStr(unsigned char *to, size_t to_len, size_t to_max,
             const unsigned char *from, size_t from_len);

  bool decodeChar(char c, int *pos);
};

#endif

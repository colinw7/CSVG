#include <CEncode64.h>
#include <CFile.h>
#include <cstring>

#define BASE_64_LINE_MAX   60
#define BASE_64_LINE_MAX_4 (BASE_64_LINE_MAX / 4)

static const char
base_64_chars[] =
  "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
  "abcdefghijklmnopqrstuvwxyz"
  "0123456789+/";

static char end_char = '=';

CEncode64 *
CEncode64::
getInstance()
{
  static CEncode64 *instance;

  if (! instance)
    instance = new CEncode64;

  return instance;
}

CEncode64::
CEncode64()
{
  setChars(base_64_chars);

  setEndChar(end_char);
}

CEncode64::
~CEncode64()
{
}

void
CEncode64::
setChars(const char base_64_chars[64])
{
  memcpy(base_64_chars_, base_64_chars, 64);

  base_64_chars_[64] = '\0';
}

bool
CEncode64::
encode(CFile *ifile, CFile *ofile)
{
  uchar in[3], out[4];

  int num_out = 0;

  while (! ifile->eof()) {
    int len = 0;

    for (int i = 0; i < 3; i++) {
      in[i] = (u_char) ifile->getC();

      if (! ifile->eof())
        len++;
      else
        in[i] = '\0';
    }

    if (len) {
      encodeBlock(in, out, len);

      for (int i = 0; i < 4; i++)
        ofile->putC(out[i]);

      ++num_out;
    }

    if (num_out >= BASE_64_LINE_MAX_4 || ifile->eof()) {
      if (num_out > 0) {
        ofile->putC('\r');
        ofile->putC('\n');
      }

      num_out = 0;
    }

    if (ifile->error() || ofile->error())
      return false;
  }

  return true;
}

std::string
CEncode64::
encode(const std::string &str)
{
  uint size = str.size();

  uint num = size/3;

  char buffer[3], buffer1[4];

  std::string str1;

  uint j = 0;

  for (uint i = 0; i < num; ++i, j += 3) {
    buffer[0] = str[j + 0];
    buffer[1] = str[j + 1];
    buffer[2] = str[j + 2];

    encodeBlock((uchar *) buffer, (uchar *) buffer1, 3);

    str1 += std::string(buffer1, 4);
  }

  if (j < size) {
    num = size - j;

    for (uint i = 0; i < num; ++i)
      buffer[i] = str[j + i];

    encodeBlock((uchar *) buffer, (uchar *) buffer1, num);

    str1 += std::string(buffer1, 4);
  }

  return str1;
}

bool
CEncode64::
encode(const uchar *in, size_t in_len, uchar *out, size_t out_size)
{
  uchar buffer[5] = { 0 };

  size_t min_out_size = calcOutSize(in_len);

  if (out_size < min_out_size)
    return false;

  size_t out_len    = 0;
  size_t buffer_len = 0;

  while (in_len) {
    if (in_len > 3) {
      encodeBlock(in, buffer, 3);

      in     += 3;
      in_len -= 3;

      buffer_len = 3;
    }
    else {
      encodeBlock(in, buffer, in_len);

      in_len = 0;

      buffer_len = in_len;
    }

    out_len = addStr(out, out_len, out_size, buffer, buffer_len);
  }

  return true;
}

size_t
CEncode64::
calcOutSize(size_t in_len) const
{
#if 0
  return (in_len*(in_len/3)) + 2;
#else
  return (4*(in_len/3)) + 2;
#endif
}

void
CEncode64::
encodeBlock(const uchar *in, uchar *out, size_t len)
{
  out[0] = uchar(base_64_chars_[in[0] >> 2]);

  out[1] = uchar(base_64_chars_[((in[0] & 0x03) << 4) | ((in[1] & 0xf0) >> 4)]);

  out[2] = uchar(len > 1 ? base_64_chars_[((in[1] & 0x0f) << 2) |
                                          ((in[2] & 0xc0) >> 6)] : end_char_);

  out[3] = uchar(len > 2 ? base_64_chars_[in[2] & 0x3f] : end_char_);
}

int
CEncode64::
addStr(uchar *to, size_t to_len, size_t to_max, const uchar *from, size_t from_len)
{
  if (to_len >= to_max - 1)
    return (to_max - 1);

  size_t size_left = to_max - to_len - 1;

  if (from_len > size_left) {
    memcpy(&to[to_len], from, size_left);

    to[to_max] = '\0';

    return (to_max - 1);
  }
  else {
    memcpy(&to[to_len], from, from_len + 1);

    return (to_len + from_len);
  }
}

std::string
CEncode64::
decode(const std::string &str)
{
  int  buffer1[4];
  char buffer2[4];

  buffer2[3] = '\0';

  uint size = str.size();

  std::string str1;

  int j = 0;

  for (uint i = 0; i < size; ++i) {
    char c = str[i];

    int pos;

    if      (decodeChar(c, &pos)) {
      buffer1[j++] = pos;

      if (j == 4) {
        buffer2[0] = ((buffer1[0] & 0x3f) << 2) | ((buffer1[1] & 0x3f) >> 4);
        buffer2[1] = ((buffer1[1] & 0x0f) << 4) | ((buffer1[2] & 0x3f) >> 2);
        buffer2[2] = ((buffer1[2] & 0x03) << 6) | ((buffer1[3] & 0x3f) >> 0);

        str1 += std::string(buffer2, 3);

        j = 0;
      }
    }
    else if (c == end_char) {
      if      (j == 0 || j == 1) {
        // error
      }
      else if (j == 2) {
        buffer2[0] = ((buffer1[0] & 0x3f) << 2) | ((buffer1[1] & 0x3f) >> 4);

        str1 += std::string(buffer2, 1);
      }
      else if (j == 3) {
        buffer2[0] = ((buffer1[0] & 0x3f) << 2) | ((buffer1[1] & 0x3f) >> 4);
        buffer2[1] = ((buffer1[1] & 0x0f) << 4) | ((buffer1[2] & 0x3f) >> 2);

        str1 += std::string(buffer2, 2);
      }

      break;
    }
    else {
      continue;
    }
  }

  return str1;
}

bool
CEncode64::
decodeChar(char c, int *pos)
{
  char *p = strchr(base_64_chars_, c);

  if (p == 0) return false;

  *pos = p - base_64_chars_;

  return true;
}

#ifndef CBUFFER_H
#define CBUFFER_H

#include <algorithm>

// only for fundamental types !

template<typename T>
class CBufferT {
 private:
  T    *buffer_;
  uint  buffer_num_;
  uint  buffer_max_;

 public:
  CBufferT() :
   buffer_(NULL), buffer_num_(0), buffer_max_(0) {
  }

  explicit CBufferT(uint num) :
   buffer_(NULL), buffer_num_(0), buffer_max_(0) {
    resize(num);
  }

 ~CBufferT() {
    delete [] buffer_;
  }

  void resize(uint num) {
    if (num > buffer_max_) {
      uint  buffer_max = 2*buffer_max_ + num + 1;
      T    *buffer     = new T [buffer_max];

      if (buffer_ != NULL && buffer_num_ > 0)
        memcpy(buffer, buffer_, buffer_num_*sizeof(T));

      delete [] buffer_;

      buffer_max_ = buffer_max;
      buffer_     = buffer;
    }
  }

  T *dataP() const { return buffer_; }

  bool empty() const {
    return (buffer_num_ == 0);
  }

  uint size() const {
    return buffer_num_;
  }

  uint max_size() const {
    return buffer_max_;
  }

  T &operator[](int i) { return buffer_[i]; }
  T  operator[](int i) const { return buffer_[i]; }

  void growBy(uint num) {
    grow(buffer_num_ + num);
  }

  void grow(uint num) {
    resize(num);

    buffer_num_ = num;
  }

  void shrinkBy(uint num) {
    shrink(buffer_num_ - num);
  }

  void shrink(uint num) {
    resize(num);

    buffer_num_ = std::max(0U, num);
  }

  void push_back(const T &data) {
    resize(buffer_num_ + 1);

    buffer_[buffer_num_++] = data;
  }

  void clear() {
    buffer_num_ = 0;
  }
};

typedef CBufferT<int>    CIntBuffer;
typedef CBufferT<uint>   CUIntBuffer;
typedef CBufferT<char *> CCharPBuffer;
typedef CBufferT<double> CDoubleBuffer;

#endif

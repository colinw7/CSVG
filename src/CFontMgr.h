#ifndef CFONT_MGR_H
#define CFONT_MGR_H

#define CFontMgrInst CFontMgr::getInstance()

#include <CFont.h>
#include <list>

class CFontMgr {
 private:
  typedef std::list<CFontPtr> FontList;

  FontList font_list_;
  CFontPtr prototype_;
  bool     debug_;

 public:
  static CFontMgr *getInstance() {
    static CFontMgr *mgr;

    if (mgr == NULL)
      mgr = new CFontMgr;

    return mgr;
  }

  CFontMgr() :
   font_list_(), prototype_(), debug_(false) {
  }

 ~CFontMgr() { }

  void setPrototype(CFontPtr ptr);

  CFontPtr lookupFont(const std::string &family, CFontStyle style,
                      double size, double angle=0, double char_angle=0,
                      uint x_res=100, uint y_res=100);

  CFontPtr lookupFont(const std::string &full_name);

 private:
  bool addFont(CFontPtr font);
  bool deleteFont(CFontPtr font);
};

#endif

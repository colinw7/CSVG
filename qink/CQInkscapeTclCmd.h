#ifndef CQInkscapeTclCmd_H
#define CQInkscapeTclCmd_H

#include <CQTclCmd.h>

namespace CQInkscape {

class Console;

#define CQINKSCAPE_TCL_CMD(NAME) \
class NAME##TclCmd : public CQTclCmd::CmdProc { \
 public: \
  enum class ArgType { \
    None    = int(CQTclCmd::CmdArg::Type::None), \
    Boolean = int(CQTclCmd::CmdArg::Type::Boolean), \
    Integer = int(CQTclCmd::CmdArg::Type::Integer), \
    Real    = int(CQTclCmd::CmdArg::Type::Real), \
    String  = int(CQTclCmd::CmdArg::Type::String), \
    SBool   = int(CQTclCmd::CmdArg::Type::SBool), \
    Enum    = int(CQTclCmd::CmdArg::Type::Enum) \
  }; \
\
  using CmdArg = CQTclCmd::CmdArg; \
\
 public: \
  NAME##TclCmd(Console *console) : \
    CmdProc(console->tclCmdMgr()), console_(console) { } \
\
  bool exec(CQTclCmd::CmdArgs &args) override; \
\
  void addArgs(CQTclCmd::CmdArgs &args) override; \
\
  QStringList getArgValues(const QString &arg, \
                           const NameValueMap &nameValueMap=NameValueMap()) override; \
\
  CmdArg &addArg(CQTclCmd::CmdArgs &args, const QString &name, ArgType type, \
                 const QString &argDesc="", const QString &desc="") { \
    return args.addCmdArg(name, int(type), argDesc, desc); \
  } \
\
  void *data() const { return data_; } \
  void setData(void *p) { data_ = p; } \
\
 public: \
  Console *console_ { nullptr }; \
  void*    data_  { nullptr }; \
};

#define CQINKSCAPE_INST_TCL_CMD(NAME) \
class NAME##InstTclCmd : public CQTclCmd::CmdProc { \
 public: \
  enum class ArgType { \
    None    = int(CQTclCmd::CmdArg::Type::None), \
    Boolean = int(CQTclCmd::CmdArg::Type::Boolean), \
    Integer = int(CQTclCmd::CmdArg::Type::Integer), \
    Real    = int(CQTclCmd::CmdArg::Type::Real), \
    String  = int(CQTclCmd::CmdArg::Type::String), \
    SBool   = int(CQTclCmd::CmdArg::Type::SBool), \
    Enum    = int(CQTclCmd::CmdArg::Type::Enum) \
  }; \
\
  using CmdArg = CQTclCmd::CmdArg; \
\
 public: \
  NAME##InstTclCmd(Console *console, const QString &id) : \
   CmdProc(console->tclCmdMgr()), console_(console), id_(id) { } \
\
  bool exec(CQTclCmd::CmdArgs &args) override; \
\
  void addArgs(CQTclCmd::CmdArgs &args) override; \
\
  QStringList getArgValues(const QString &name, \
                           const NameValueMap &nameValueMap=NameValueMap()) override; \
\
  CmdArg &addArg(CQTclCmd::CmdArgs &args, const QString &name, ArgType type, \
                 const QString &argDesc="", const QString &desc="") { \
    return args.addCmdArg(name, int(type), argDesc, desc); \
  } \
\
  void *data() const { return data_; } \
  void setData(void *p) { data_ = p; } \
\
 public: \
  Console* console_ { nullptr }; \
  QString  id_; \
  void*    data_    { nullptr }; \
};

}

#endif

#ifndef CQTclCommand_H
#define CQTclCommand_H

#include <CQCommand.h>

class CQTcl;

namespace CQTclCmd {
class Mgr;
class CmdProc;
}

namespace CQTclCommand {

class ScrollArea;

//---

class CommandWidget : public CQCommand::CommandWidget {
 public:
  using OptionValues = std::map<std::string, std::string>;

 public:
  CommandWidget(ScrollArea *area);

 ~CommandWidget();

  CQTclCmd::Mgr *tclCmdMgr() const;

  void addCommand(const QString &name, CQTclCmd::CmdProc *proc);

  void executeCommand(const QString &line);

  int eval(const QString &line, bool showError=true, bool showResult=false);

  void textChanged() override;

  QColor posColor(int pos) const override;

  bool complete(const QString &text, int pos, QString &newText,
                CompleteMode completeMode) const override;

  bool completeCommand(const QString &lhs, const QString &str, const QString &rhs,
                       bool interactive, QString &command) const;

  bool completeOption(const QString &cmdName, const QString &lhs, const QString &str,
                      const QString &rhs, bool interactive, QString &option) const;

  bool completeArg(const QString &cmdName, const QString &option, const OptionValues &optionValues,
                   const QString &lhs, const QString &str, const QString &rhs,
                   bool interactive, QString &argText) const;

 protected:
  using Colors = std::vector<QColor>;

  CQTcl*         qtcl_ { nullptr };
  CQTclCmd::Mgr* mgr_  { nullptr };
  Colors         colors_;
};

//---

class ScrollArea : public CQCommand::ScrollArea {
 public:
  ScrollArea(QWidget *parent=nullptr);

  CQTclCmd::Mgr *tclCmdMgr() const;

  CQCommand::CommandWidget *createCommandWidget() const override;

  void executeCommand(const QString &line);

  int eval(const QString &line, bool showError=true, bool showResult=false);

  void addCommand(const QString &name, CQTclCmd::CmdProc *proc);

 protected:
  CommandWidget* widget_ { nullptr };
};

//---

}

//------

#include <CQTclCmd.h>

namespace CQTclCommand {
class ScrollArea;
}

//---

namespace CQTclCommand {

class TclCmdProc : public CQTclCmd::CmdProc {
 public:
  enum class ArgType {
    None    = int(CQTclCmd::CmdArg::Type::None),
    Boolean = int(CQTclCmd::CmdArg::Type::Boolean),
    Integer = int(CQTclCmd::CmdArg::Type::Integer),
    Real    = int(CQTclCmd::CmdArg::Type::Real),
    String  = int(CQTclCmd::CmdArg::Type::String),
    SBool   = int(CQTclCmd::CmdArg::Type::SBool),
    Enum    = int(CQTclCmd::CmdArg::Type::Enum)
  };

  using CmdArg = CQTclCmd::CmdArg;

 public:
  TclCmdProc(ScrollArea *area) :
   CQTclCmd::CmdProc(area->tclCmdMgr()), area_(area) {
  }

  void *data() const { return data_; }
  void setData(void *p) { data_ = p; }

  CmdArg &addArg(CQTclCmd::CmdArgs &args, const QString &name, ArgType type,
                 const QString &argDesc="", const QString &desc="") {
    return args.addCmdArg(name, int(type), argDesc, desc);
  }

 protected:
  ScrollArea *area_ { nullptr };
  void*       data_ { nullptr };
};

}

//---

#define CQTCL_CMD(NAME, CLASS, DATA) \
class NAME##TclCmd : public CLASS { \
 public: \
  NAME##TclCmd(DATA *clientData=nullptr) : \
   CLASS(clientData) { \
  } \
\
  DATA *area() const { return dynamic_cast<DATA *>(area_); } \
\
  bool exec(CQTclCmd::CmdArgs &args) override; \
\
  void addArgs(CQTclCmd::CmdArgs &args) override; \
\
  QStringList getArgValues(const QString &arg, \
                           const NameValueMap &nameValueMap=NameValueMap()) override; \
};

#define CQTCL_INST_TCL_CMD(NAME, CLASS, DATA) \
class NAME##InstTclCmd : public CLASS { \
 public: \
  NAME##InstTclCmd(DATA *clientData=nullptr, const QString &id) : \
   CLASS(clientData), id_(id) { } \
\
  DATA *area() const { return dynamic_cast<DATA *>(area_); } \
\
  bool exec(CQTclCmd::CmdArgs &args) override; \
\
  void addArgs(CQTclCmd::CmdArgs &args) override; \
\
  QStringList getArgValues(const QString &name, \
                           const NameValueMap &nameValueMap=NameValueMap()) override; \
\
 public: \
  QString id_; \
};

#endif

#include <CQInkscapeConsole.h>
#include <CQInkscapeWindow.h>
#include <CQInkscapeCanvas.h>

#include <CQCommand.h>
#include <CQTclCmd.h>
#include <CQTclUtil.h>
#include <CQUtil.h>
#include <CTclParse.h>
#include <CQStrUtil.h>

#include <CSVG.h>

#include <COSProcess.h>
#include <COSExec.h>

#include <QVBoxLayout>

#include <fstream>

//---

namespace {

QStringList tagNames() {
  auto names = QStringList() <<
    "a" << "animate" << "animateColor" << "animateMotion" << "animateTransform" <<
    "audio" << "circle" << "clipPath" << "color-profile" << "defs" << "desc" <<
    "ellipse" << "feBlend" << "feColorMatrix" << "feComponentTransfer" << "feComposite" <<
    "feConvolveMatrix" << "feDiffuseLighting" << "feDisplacementMap" << "feDistantLight" <<
    "feFlood" << "feFuncR" << "feFuncG" << "feFuncB" << "feFuncA" << "feGaussianBlur" <<
    "feImage" << "feMerge" << "feMergeNode" << "feMorphology" << "feOffset" <<
    "fePointLight" << "feSpecularLighting" << "feSpotLight" << "feTile" << "feTurbulence" <<
    "filter" << "font" << "font-face" << "font-face-src" << "font-face-uri" << "glyph" <<
    "g" << "hkern" << "image" << "line" << "linearGradient" << "marker" << "mask" <<
    "metadata" << "missing-glyph" << "mpath" << "path" << "pattern" << "polygon" <<
    "polyline" << "radialGradient" << "rect" << "script" << "set" << "stop" << "style" <<
    "switch" << "symbol" << "tbreak" << "text" << "textPath" << "title" << "tspan" << "use";
 /* "altGlyphDef" << "animation" << "cursor" << "discard" << "foreignObject" << */
 /* "prefetch" << "solidColor" << "textArea" << "video" << "view"; */
   return names;
}

}

//---

namespace CQInkscape {

ConsoleCommandWidget::
ConsoleCommandWidget(ConsoleScrollArea *) :
 CQTclCommand::CommandWidget(nullptr)
{
}

ConsoleCommandWidget::
~ConsoleCommandWidget()
{
}

void
ConsoleCommandWidget::
addCommand(const QString &name, CQTclCmd::CmdProc *proc)
{
  CQTclCommand::CommandWidget::addCommand(name, proc);
}

void
ConsoleCommandWidget::
executeCommand(const QString &line)
{
  CQTclCommand::CommandWidget::executeCommand(line);
}

int
ConsoleCommandWidget::
eval(const QString &line, bool showError, bool showResult)
{
  return CQTclCommand::CommandWidget::eval(line, showError, showResult);
}

bool
ConsoleCommandWidget::
complete(const QString &text, int pos, QString &newText, CompleteMode completeMode) const
{
  return CQTclCommand::CommandWidget::complete(text, pos, newText, completeMode);
}

//---

ConsoleScrollArea::
ConsoleScrollArea(Console *console) :
 CQTclCommand::ScrollArea(console), console_(console)
{
}

CQCommand::CommandWidget *
ConsoleScrollArea::
createCommandWidget() const
{
  auto *th = const_cast<ConsoleScrollArea *>(this);

  th->widget_ = new ConsoleCommandWidget(th);

  return widget_;
}

void
ConsoleScrollArea::
executeCommand(const QString &line)
{
  CQTclCommand::ScrollArea::executeCommand(line);
}

int
ConsoleScrollArea::
eval(const QString &line, bool showError, bool showResult)
{
  return CQTclCommand::ScrollArea::eval(line, showError, showResult);
}

void
ConsoleScrollArea::
addCommand(const QString &name, CQTclCmd::CmdProc *proc)
{
  CQTclCommand::ScrollArea::addCommand(name, proc);
}

//---

Console::
Console(Window *window) :
 window_(window)
{
  setObjectName("console");

  //---

  auto *layout = CQUtil::makeLayout<QVBoxLayout>(this, 0, 0);

  area_ = new ConsoleScrollArea(this);

  area_->getCommand()->setMinLines(10);
  area_->getCommand()->setPrompt("> ");

  connect(area_, SIGNAL(executeCommand(const QString &)),
          this, SLOT(executeCommand(const QString &)));

  layout->addWidget(area_);

  //---

  area_->addCommand("help", new HelpTclCmd(area_));

  area_->addCommand("get_svg_children" , new GetSVGChildrenTclCmd (area_));
  area_->addCommand("get_svg_data"     , new GetSVGDataTclCmd     (area_));
  area_->addCommand("set_svg_data"     , new SetSVGDataTclCmd     (area_));
  area_->addCommand("add_svg_object"   , new AddSVGObjectTclCmd   (area_));
  area_->addCommand("remove_svg_object", new RemoveSVGObjectTclCmd(area_));
  area_->addCommand("get_svg_selection", new GetSVGSelectionTclCmd(area_));
  area_->addCommand("move_svg_object"  , new MoveSVGObjectTclCmd  (area_));
  area_->addCommand("scale_svg_object" , new ScaleSVGObjectTclCmd (area_));
  area_->addCommand("rotate_svg_object", new RotateSVGObjectTclCmd(area_));
  area_->addCommand("write_svg_file"   , new WriteSVGFileTclCmd   (area_));
}

Console::
~Console()
{
}

void
Console::
executeCommand(const QString &line)
{
  area_->executeCommand(line);
}

void
Console::
loadTclFile(const QString &file)
{
  executeCommand(QString("source %1").arg(file));
}

QSize
Console::
sizeHint() const
{
  return QSize(1000, 500);
}

//---

void
HelpTclCmd::
addArgs(CQTclCmd::CmdArgs &argv)
{
  addArg(argv, "-hidden" , ArgType::Boolean, "show hidden");
  addArg(argv, "-verbose", ArgType::Boolean, "verbose help");
}

QStringList
HelpTclCmd::
getArgValues(const QString &, const NameValueMap &)
{
  return QStringList();
}

bool
HelpTclCmd::
exec(CQTclCmd::CmdArgs &argv)
{
  addArgs(argv);

  if (! argv.parse())
    return false;

  auto hidden  = argv.getParseBool("hidden");
  auto verbose = argv.getParseBool("verbose");

  //---

  const auto &pargs = argv.getParseArgs();

  QString pattern = (! pargs.empty() ? pargs[0].toString() : "");

  //---

  if (pattern.length())
    mgr()->help(pattern, verbose, hidden);
  else
    mgr()->helpAll(verbose, hidden);

  return true;
}

//---

void
GetSVGChildrenTclCmd::
addArgs(CQTclCmd::CmdArgs &argv)
{
  addArg(argv, "-parent", ArgType::String , "parent");
  addArg(argv, "-tag"   , ArgType::String , "tag name");
  addArg(argv, "-hier"  , ArgType::Boolean, "hierarchical");
}

QStringList
GetSVGChildrenTclCmd::
getArgValues(const QString &option, const NameValueMap &)
{
  if (option == "-tag")
    return tagNames();

  return QStringList();
}

bool
GetSVGChildrenTclCmd::
exec(CQTclCmd::CmdArgs &argv)
{
  addArgs(argv);

  bool rc;

  if (! argv.parse(rc))
    return rc;

  auto parent = argv.getParseStr ("parent");
  auto tag    = argv.getParseStr ("tag").toStdString();
  auto hier   = argv.getParseBool("hier");

  if (tag == "?") {
    auto names = GetSVGChildrenTclCmd::getArgValues("-tag");
    qtcl()->setResult(names);
    return true;
  }

  auto *svg = area()->console()->window()->svg();

  CSVGObject *parentObj = nullptr;

  if (parent == "" || parent == "root")
    parentObj = svg->getRoot();
  else {
    CSVGObject::ObjectArray objects;

    svg->getRoot()->getHierChildrenOfId(parent.toStdString(), objects);

    if (objects.size() == 1)
      parentObj = objects[0];
  }

  //---

  QStringList names;

  if (parentObj) {
    CSVGObject::ObjectArray children;

    if (hier)
      parentObj->getAllChildren(children);
    else {
      for (const auto &child : parentObj->children())
        children.push_back(child);
    }

    for (const auto &child : children) {
      if (tag != "" && child->getTagName() != tag)
        continue;

      auto id = child->getHierId(/*autoName*/true);

      names.push_back(QString::fromStdString(id));
    }
  }

  qtcl()->setResult(names);

  return true;
}

//---

void
GetSVGDataTclCmd::
addArgs(CQTclCmd::CmdArgs &argv)
{
  addArg(argv, "-id"  , ArgType::String , "tag id");
  addArg(argv, "-name", ArgType::String , "value name");
  addArg(argv, "-hier", ArgType::Boolean, "hierarchical");
}

QStringList
GetSVGDataTclCmd::
getArgValues(const QString &option, const NameValueMap &)
{
  if (option == "-name") {
    auto names = QStringList() <<
     "@id" << "@parent" << "@depth" << "@tag_name" << "@selected" << "@viewBox" <<
     "@style" << "@text" << "@string";
    return names;
  }

  return QStringList();
}

bool
GetSVGDataTclCmd::
exec(CQTclCmd::CmdArgs &argv)
{
  addArgs(argv);

  bool rc;

  if (! argv.parse(rc))
    return rc;

  auto id   = argv.getParseStr ("id");
  auto name = argv.getParseStr ("name");
  auto hier = argv.getParseBool("hier");

  auto *svg = area()->console()->window()->svg();

  CSVGObject::ObjectArray objects;

  if (id != "")
    svg->getRoot()->getHierChildrenOfId(id.toStdString(), objects);
  else
    objects.push_back(svg->getRoot());

  //---

  if (name == "?") {
    auto names = GetSVGDataTclCmd::getArgValues("-name");
    qtcl()->setResult(names);
    return true;
  }

  //---

  QVariantList values;

  for (const auto &obj : objects) {
    if      (name == "@id") {
      values.push_back(QString::fromStdString(obj->getId()));
    }
    else if (name == "@parent") {
      auto *parent = obj->getParent();

      std::string id;

      if (parent)
        id = parent->getHierId(/*autoName*/true);

      values.push_back(QString::fromStdString(id));
    }
    else if (name == "@depth") {
      values.push_back(obj->getDepth());
    }
    else if (name == "@tag_name") {
      values.push_back(QString::fromStdString(obj->getTagName()));
    }
    else if (name == "@selected") {
      values.push_back(obj->getSelected());
    }
    else if (name == "@viewBox") {
      values.push_back(QString::fromStdString(obj->getViewBox().toString()));
    }
    else if (name == "@style") {
      values.push_back(QString::fromStdString(obj->styleString(/*flat*/true)));
    }
    else if (name == "@text") {
      values.push_back(QString::fromStdString(obj->getText()));
    }
    else if (name == "@string") {
      values.push_back(QString::fromStdString(obj->toString(hier)));
    }
    else {
      auto value = obj->getNameValue(name.toStdString());

      if (value.isValid())
        values.push_back(QString::fromStdString(value.getValue()));
    }
  }

  qtcl()->setResult(values);

  return true;
}

//---

void
SetSVGDataTclCmd::
addArgs(CQTclCmd::CmdArgs &argv)
{
  addArg(argv, "-id"   , ArgType::String , "tag id");
  addArg(argv, "-name" , ArgType::String , "value name");
  addArg(argv, "-value", ArgType::String , "new value");
  addArg(argv, "-hier" , ArgType::Boolean, "hierarchical");
}

QStringList
SetSVGDataTclCmd::
getArgValues(const QString &, const NameValueMap &)
{
  return QStringList();
}

bool
SetSVGDataTclCmd::
exec(CQTclCmd::CmdArgs &argv)
{
  addArgs(argv);

  bool rc;

  if (! argv.parse(rc))
    return rc;

  auto id    = argv.getParseStr ("id");
  auto name  = argv.getParseStr ("name");
  auto value = argv.getParseStr ("value");
//auto hier  = argv.getParseBool("hier");

  auto *svg = area()->console()->window()->svg();

  CSVGObject::ObjectArray objects;

  if (id != "")
    svg->getRoot()->getHierChildrenOfId(id.toStdString(), objects);
  else
    objects.push_back(svg->getRoot());

  QVariantList values;

  for (const auto &obj : objects) {
    if      (name == "@text") {
      obj->setText(value.toStdString());
    }
    else if (name.left(6) == "style:")
      obj->setStyleValue(name.mid(6).toStdString(), value.toStdString());
    else
      obj->handleOption(name.toStdString(), value.toStdString());
  }

  qtcl()->setResult(values);

  area()->console()->window()->redraw(/*update*/true);

  return true;
}

//---

void
AddSVGObjectTclCmd::
addArgs(CQTclCmd::CmdArgs &argv)
{
  addArg(argv, "-parent", ArgType::String, "parent id");
  addArg(argv, "-tag"   , ArgType::String, "tag name");
  addArg(argv, "-id"    , ArgType::String, "new object id");
}

QStringList
AddSVGObjectTclCmd::
getArgValues(const QString &option, const NameValueMap &)
{
  if (option == "-tag")
    return tagNames();

  return QStringList();
}

bool
AddSVGObjectTclCmd::
exec(CQTclCmd::CmdArgs &argv)
{
  addArgs(argv);

  bool rc;

  if (! argv.parse(rc))
    return rc;

  auto parent = argv.getParseStr("parent");
  auto tag    = argv.getParseStr("tag");
  auto id     = argv.getParseStr("id");

  if (tag == "?") {
    auto names = AddSVGObjectTclCmd::getArgValues("-tag");
    qtcl()->setResult(names);
    return true;
  }

  auto *svg = area()->console()->window()->svg();

  CSVGObject *parentObj = nullptr;

  if (parent == "" || parent == "root")
    parentObj = svg->getRoot();
  else {
    CSVGObject::ObjectArray objects;

    svg->getRoot()->getHierChildrenOfId(parent.toStdString(), objects);

    if (objects.size() == 1)
      parentObj = objects[0];
    else
      return false;
  }

  auto *child = svg->createObjectByName(tag.toStdString());
  if (! child) return false;

  if (id != "")
    child->setId(id.toStdString());

  parentObj->addChildObject(child);

  auto id1 = child->getHierId(/*autoName*/true);

  qtcl()->setResult(QString::fromStdString(id1));

  area()->console()->window()->redraw(/*update*/true);

  return true;
}

//---

void
RemoveSVGObjectTclCmd::
addArgs(CQTclCmd::CmdArgs &argv)
{
  addArg(argv, "-id", ArgType::String, "object id");
}

QStringList
RemoveSVGObjectTclCmd::
getArgValues(const QString &, const NameValueMap &)
{
  return QStringList();
}

bool
RemoveSVGObjectTclCmd::
exec(CQTclCmd::CmdArgs &argv)
{
  addArgs(argv);

  bool rc;

  if (! argv.parse(rc))
    return rc;

  auto id = argv.getParseStr("id");

  auto *svg = area()->console()->window()->svg();

  CSVGObject::ObjectArray objects;

  svg->getRoot()->getHierChildrenOfId(id.toStdString(), objects);

  for (auto *obj : objects)
    obj->ungroupObject();

  area()->console()->window()->redraw(/*update*/true);

  return true;
}

//---

void
GetSVGSelectionTclCmd::
addArgs(CQTclCmd::CmdArgs &)
{
}

QStringList
GetSVGSelectionTclCmd::
getArgValues(const QString &, const NameValueMap &)
{
  return QStringList();
}

bool
GetSVGSelectionTclCmd::
exec(CQTclCmd::CmdArgs &argv)
{
  addArgs(argv);

  bool rc;

  if (! argv.parse(rc))
    return rc;

  auto *svg = area()->console()->window()->svg();

  CSVG::ObjectList objects;

  svg->getSelectedObjects(objects);

  QStringList names;

  for (const auto &obj : objects) {
    auto id = obj->getHierId(/*autoName*/true);

    names.push_back(QString::fromStdString(id));
  }

  qtcl()->setResult(names);

  return true;
}

//---

void
MoveSVGObjectTclCmd::
addArgs(CQTclCmd::CmdArgs &argv)
{
  addArg(argv, "-id", ArgType::String, "object id");
  addArg(argv, "-to", ArgType::String, "new position");
  addArg(argv, "-by", ArgType::String, "delta position");
}

QStringList
MoveSVGObjectTclCmd::
getArgValues(const QString &, const NameValueMap &)
{
  return QStringList();
}

bool
MoveSVGObjectTclCmd::
exec(CQTclCmd::CmdArgs &argv)
{
  addArgs(argv);

  bool rc;

  if (! argv.parse(rc))
    return rc;

  auto id = argv.getParseStr("id");
  auto to = argv.getParseStr("to");
  auto by = argv.getParseStr("by");

  auto *svg = area()->console()->window()->svg();

  CSVGObject::ObjectArray objects;

  if (id != "")
    svg->getRoot()->getHierChildrenOfId(id.toStdString(), objects);

  if (objects.empty()) {
    std::cerr << "Invalid object '" << id.toStdString() << "'\n";
    return false;
  }

  bool ok;

  if      (to != "") {
    QStringList strs;

    CQTclUtil::splitList(to, strs);

    if (strs.size() == 2) {
      double x = strs[0].toDouble(&ok);
      double y = strs[1].toDouble(&ok);

      for (const auto &obj : objects)
        obj->moveTo(CPoint2D(x, y));
    }
    else {
      std::cerr << "Invalid point '" << to.toStdString() << "'\n";
      return false;
    }
  }
  else if (by != "") {
    QStringList strs;

    CQTclUtil::splitList(by, strs);

    if (strs.size() == 2) {
      double x = strs[0].toDouble(&ok);
      double y = strs[1].toDouble(&ok);

      for (const auto &obj : objects)
        obj->moveBy(CVector2D(x, y));
    }
    else {
      std::cerr << "Invalid point '" << by.toStdString() << "'\n";
      return false;
    }
  }

  area()->console()->window()->redraw(/*update*/true);

  return true;
}

//---

void
ScaleSVGObjectTclCmd::
addArgs(CQTclCmd::CmdArgs &argv)
{
  addArg(argv, "-id", ArgType::String, "object id");
  addArg(argv, "-by", ArgType::String, "scale factor");
}

QStringList
ScaleSVGObjectTclCmd::
getArgValues(const QString &, const NameValueMap &)
{
  return QStringList();
}

bool
ScaleSVGObjectTclCmd::
exec(CQTclCmd::CmdArgs &argv)
{
  addArgs(argv);

  bool rc;

  if (! argv.parse(rc))
    return rc;

  auto id = argv.getParseStr("id");
  auto by = argv.getParseStr("by");

  auto *svg = area()->console()->window()->svg();

  CSVGObject::ObjectArray objects;

  if (id != "")
    svg->getRoot()->getHierChildrenOfId(id.toStdString(), objects);

  if (objects.empty()) {
    std::cerr << "Invalid object '" << id.toStdString() << "'\n";
    return false;
  }

  bool ok;

  if (by != "") {
    double s = by.toDouble(&ok);

    for (const auto &obj : objects)
      obj->scaleBy(s);
  }

  area()->console()->window()->redraw(/*update*/true);

  return true;
}

//---

void
RotateSVGObjectTclCmd::
addArgs(CQTclCmd::CmdArgs &argv)
{
  addArg(argv, "-id", ArgType::String, "object id");
  addArg(argv, "-by", ArgType::String, "delta angle");
  addArg(argv, "-at", ArgType::String, "rotation center");
}

QStringList
RotateSVGObjectTclCmd::
getArgValues(const QString &, const NameValueMap &)
{
  return QStringList();
}

bool
RotateSVGObjectTclCmd::
exec(CQTclCmd::CmdArgs &argv)
{
  addArgs(argv);

  bool rc;

  if (! argv.parse(rc))
    return rc;

  auto id = argv.getParseStr("id");
  auto by = argv.getParseStr("by");
  auto at = argv.getParseStr("at");

  auto *svg = area()->console()->window()->svg();

  CSVGObject::ObjectArray objects;

  if (id != "")
    svg->getRoot()->getHierChildrenOfId(id.toStdString(), objects);

  if (objects.empty()) {
    std::cerr << "Invalid object '" << id.toStdString() << "'\n";
    return false;
  }

  bool ok;

  if (by != "") {
    double a = M_PI*by.toDouble(&ok)/180.0;

     if      (at != "") {
      QStringList strs;

      CQTclUtil::splitList(at, strs);

      if (strs.size() == 2) {
        double x = strs[0].toDouble(&ok);
        double y = strs[1].toDouble(&ok);

        for (const auto &obj : objects)
          obj->rotateAt(a, CPoint2D(x, y));
      }
      else {
        std::cerr << "Invalid point '" << at.toStdString() << "'\n";
        return false;
      }
    }
    else {
      for (const auto &obj : objects)
        obj->rotateBy(a);
    }
  }

  area()->console()->window()->redraw(/*update*/true);

  return true;
}

//---

void
WriteSVGFileTclCmd::
addArgs(CQTclCmd::CmdArgs &argv)
{
  addArg(argv, "-file", ArgType::String, "filename");
}

QStringList
WriteSVGFileTclCmd::
getArgValues(const QString &, const NameValueMap &)
{
  return QStringList();
}

bool
WriteSVGFileTclCmd::
exec(CQTclCmd::CmdArgs &argv)
{
  addArgs(argv);

  bool rc;

  if (! argv.parse(rc))
    return rc;

  auto file = argv.getParseStr("file");

  auto *svg = area()->console()->window()->svg();

  std::ofstream os(file.toStdString());

  svg->print(os, /*hier*/true);

  return true;
}

//---

TclCmdProc::
TclCmdProc(ConsoleScrollArea *area) :
 CQTclCommand::TclCmdProc(area)
{
}

}

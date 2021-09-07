#include <CQInkscapeWindow.h>

#include <CQApp.h>
#include <CQStyle.h>
#include <CQUtil.h>

#include <CSVG.h>
#include <CScreenUnits.h>
#include <CUtil.h>

#include <iostream>

using namespace CQInkscape;

int
main(int argc, char **argv)
{
  bool        debug   = false;
  int         dpi     = -1;
  int         fontDpi = -1;
  std::string tcl;

  CUtil::setOStream(std::cerr);

  QStringList files;

  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      auto arg = std::string(&argv[i][1]);

      if      (arg == "debug")
        debug = true;
      else if (arg == "dpi") {
        ++i;

        if (i < argc)
          dpi = atoi(argv[i]);
      }
      else if (arg == "font_dpi") {
        ++i;

        if (i < argc)
          fontDpi = atoi(argv[i]);
      }
      else if (arg == "tcl") {
        ++i;

        if (i < argc)
          tcl = argv[i];
      }
      else
        std::cerr << "Invalid option: " << argv[i] << "\n";
    }
    else
      files.push_back(argv[i]);
  }

  //---

  CQApp app(argc, argv);

  app.setStyle(new CQStyle);

  if (dpi > 0)
    CScreenUnitsMgrInst->setDpi(dpi);
  else
    CScreenUnitsMgrInst->setDpi(72.0);

  CQUtil::initProperties();

  //---

  auto *window = new Window;

  if (fontDpi > 0)
    window->svg()->setFontDpi(fontDpi);

  window->svg()->setDebug(debug);

  window->addFiles(files);

  window->show();

  if (tcl != "")
    window->loadTclFile(QString::fromStdString(tcl));

  //---

  app.exec();

  return 0;
}

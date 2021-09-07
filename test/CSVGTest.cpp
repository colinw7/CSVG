#include <CSVGLib.h>
#include <CSVGImageRenderer.h>
#include <CSVGLogRenderer.h>
#include <CSVGImageData.h>
#include <CSVGWrite.h>

#include <CImageRenderer2D.h>
#include <CImageLib.h>

int
main(int argc, char **argv)
{
  bool debug         = false;
  bool quiet         = false;
  bool check_viewbox = false;
  bool print         = false;
  bool flat          = false;
  bool log           = false;
  bool write         = false;
  bool colors        = false;

  std::vector<std::string> files;

  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      auto arg = std::string(&argv[i][1]);

      if      (arg == "debug")
        debug = true;
      else if (arg == "quiet")
        quiet = true;
      else if (arg == "check_viewbox")
        check_viewbox = true;
      else if (arg == "print")
        print = true;
      else if (arg == "flat")
        flat = true;
      else if (arg == "log")
        log = true;
      else if (arg == "write")
        write = true;
      else if (arg == "colors")
        colors = true;
      else
        std::cerr << "Invalid option: " << argv[i] << std::endl;
    }
    else
      files.push_back(argv[i]);
  }

  if (files.empty()) {
    std::cerr << "No files specified" << std::endl;
    exit(1);
  }

  //---

  for (const auto &file : files) {
    CSVG svg;

    svg.setDebug(debug);

    svg.setQuiet(quiet);

    svg.setCheckViewBox(check_viewbox);

    svg.init();

    if (! svg.read(file)) {
      std::cerr << "Failed to read '" << file << "'" << std::endl;
      continue;
    }

    if      (print) {
      CSVGLogRenderer lrenderer;

      lrenderer.setSilent(true);

      svg.setRenderer(&lrenderer);

      svg.print(std::cout, true);
    }
    else if (flat) {
      CSVGLogRenderer lrenderer;

      lrenderer.setSilent(true);

      svg.setRenderer(&lrenderer);

      svg.printFlat(std::cout);
    }
    else if (log) {
      CSVGLogRenderer lrenderer;

      if (quiet)
        lrenderer.setSilent(true);

      svg.setRenderer(&lrenderer);

      svg.draw();
    }
    else if (write) {
      CSVGWriteVisitor writer(std::cout);

      auto *obj = svg.getRootBlock();

      writer.exec(obj);
    }
    else if (colors) {
      const auto &colors1 = svg.getColors();

      for (const auto &c : colors1)
        std::cerr << c.first << " : " << c.second << "\n";
    }
    else {
      auto *irenderer = new CSVGImageRenderer(svg.getIWidth(), svg.getIHeight());

      svg.setRenderer(irenderer);

      svg.draw();

      CFile fp(file);

      auto base = fp.getBase();

      auto name = CStrUtil::strprintf("svg_%s.png", base.c_str());

      irenderer->getImagePtr()->write(name, CFILE_TYPE_IMAGE_PNG);

      delete irenderer;
    }
  }

  return 0;
}

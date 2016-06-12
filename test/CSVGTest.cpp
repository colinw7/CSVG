#include <CSVGLib.h>
#include <CSVGImageRenderer.h>
#include <CSVGLogRenderer.h>

int
main(int argc, char **argv)
{
  bool debug  = false;
  bool print  = false;
  bool log    = false;
  bool colors = false;

  std::vector<std::string> files;

  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      if      (strcmp(&argv[i][1], "debug") == 0)
        debug = true;
      else if (strcmp(&argv[i][1], "print") == 0)
        print = true;
      else if (strcmp(&argv[i][1], "log") == 0)
        log = true;
      else if (strcmp(&argv[i][1], "colors") == 0)
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

  uint num_files = files.size();

  for (uint i = 0; i < num_files; ++i) {
    CImageNoSrc src;

    CImagePtr image = CImageMgrInst->createImage(src);

    CSVG svg;

    svg.setDebug(debug);

    svg.init();

    if (! svg.read(files[i])) {
      std::cerr << "Failed to read '" << files[i] << "'" << std::endl;
      continue;
    }

    if      (print) {
      svg.print(std::cout, true);
    }
    else if (log) {
      CSVGLogRenderer lrenderer;

      svg.setRenderer(&lrenderer);

      svg.draw();
    }
    else if (colors) {
      const CSVG::Colors &colors = svg.getColors();

      for (const auto &c : colors) {
        std::cerr << c.first << " : " << c.second << std::endl;
      }
    }
    else {
      image->setDataSize(svg.getIWidth(), svg.getIHeight());

      CImageRenderer2D renderer(image);

      CSVGImageRenderer irenderer(&renderer);

      svg.setRenderer(&irenderer);

      svg.draw();

      CFile file(files[i]);

      std::string base = file.getBase();

      std::string name = CStrUtil::strprintf("svg_%s.png", base.c_str());

      renderer.getImage()->write(name, CFILE_TYPE_IMAGE_PNG);
    }
  }

  return 0;
}

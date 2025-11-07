#ifndef CSVGAudio_H
#define CSVGAudio_H

#include <CSVGObject.h>

class CSVGAudio : public CSVGObject, public CSVGPrintBase<CSVGAudio> {
 public:
  CSVG_OBJECT_DEF("audio", CSVGObjTypeId::TBREAK)

  CSVGAudio(CSVG &svg);
  CSVGAudio(const CSVGAudio &audio);

  CSVGAudio *dup() const override;

  bool processOption(const std::string &name, const std::string &value) override;

  std::optional<std::string> getNameValue(const std::string &name) const override;

  bool draw() override;

  void print(std::ostream &os, bool hier=false) const override;

  void printValues(std::ostream &os, bool flat=false) const override;

  void accept(CSVGVisitor *visitor) override { visitor->visit(this); }
};

#endif

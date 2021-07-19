#include <CSVGClip.h>
#include <CSVG.h>

void
CSVGClip::
setRule(const std::string &rule_def)
{
  auto rule = svg_.decodeFillRuleString(rule_def);

  setRule(rule);
}

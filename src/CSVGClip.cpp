#include "CSVGI.h"

void
CSVGClip::
setRule(const string &rule_def)
{
  CFillType rule = svg_.decodeFillRuleString(rule_def);

  setRule(rule);
}

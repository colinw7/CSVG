#include <CSVGAudio.h>
#include <CSVG.h>
#include <CSVGLog.h>

/* Attributes:
    TODO
*/
CSVGAudio::
CSVGAudio(CSVG &svg) :
 CSVGObject(svg)
{
}

CSVGAudio::
CSVGAudio(const CSVGAudio &audio) :
 CSVGObject(audio)
{
}

CSVGAudio *
CSVGAudio::
dup() const
{
  return new CSVGAudio(*this);
}

bool
CSVGAudio::
processOption(const std::string &, const std::string &)
{
  // TODO
  return true;
}

COptString
CSVGAudio::
getNameValue(const std::string &name) const
{
  return CSVGObject::getNameValue(name);
}

bool
CSVGAudio::
draw()
{
  return true;
}

void
CSVGAudio::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<audio";

    printValues(os);

    os << ">";

    if (hasChildren()) {
      os << std::endl;

      printChildren(os, hier);
    }
    else {
      os << getText();
    }

    os << "</audio>" << std::endl;
  }
  else
    os << "audio";
}

void
CSVGAudio::
printValues(std::ostream &os, bool flat) const
{
  CSVGObject::printValues(os, flat);
}

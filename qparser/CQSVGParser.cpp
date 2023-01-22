#include <CQSVGParser.h>

#include <QFile>
#include <QXmlStreamReader>
#include <set>
#include <iostream>

class CQSVGParserHandler {
 public:
  using NameValue  = std::pair<QString, QString>;
  using NameValues = std::vector<NameValue>;

 public:
  CQSVGParserHandler(CQSVGParser *parser) :
   parser_(parser) {
  }

  bool parse(QXmlStreamReader &xml) {
    while (! xml.atEnd()) {
      xml.readNext();

      switch (xml.tokenType()) {
        case QXmlStreamReader::StartDocument: {
          if (parser_->isEncode()) {
            outputStr("<!DOCTYPE svg [\n");
            outputStr("]\n");
          }

          if (parser_->isXmlHeader()) {
            auto header = QString("<?xml version=\"%1\" encoding=\"%2\" standalone=\"no\"?>\n").
                  arg(xml.documentVersion().toString()).arg(xml.documentEncoding().toString());
            outputStr(header);

#if 0
            const auto &nsArray = xml.namespaceDeclarations();

            for (const auto &ns : nsArray) {
              auto uri    = ns.namespaceUri();
              auto prefix = ns.prefix();

              outputStr(uri + ":" + prefix + "\n");
            }
#endif
          }

          break;
        }
        case QXmlStreamReader::EndDocument: {
          break;
        }
        case QXmlStreamReader::StartElement: {
          if (! startElement(xml.prefix().toString(), xml.qualifiedName().toString(),
                             xml.attributes()))
            return false;

          break;
        }
        case QXmlStreamReader::EndElement: {
          if (! endElement(xml.prefix().toString(), xml.qualifiedName().toString()))
            return false;

          break;
        }
        case QXmlStreamReader::Comment: {
          // ignore comments
          break;
        }
        case QXmlStreamReader::Characters: {
          // ignore whitespace
          if (xml.isWhitespace())
            continue;

          if (! characters(xml.text().toString()))
            return false;

          break;
        }
        default: {
          //std::cerr << "Token " << int(xml.tokenType()) << " not handled\n";
          break;
        }
      }
    }

    return xml.hasError();
  }

 private:
  // CDATA
  bool characters(const QString &ch) {
    auto ch1 = ch.trimmed();

    if (ch1.length()) {
      if (parser_->isDump() || parser_->isEncode() || parser_->isTransform()) {
        if (depth_ > 0 && ! closed_) {
          outputStr(">");

          closed_ = true;
        }

        outputStr(ch);

        text_ = true;
      }
    }

    return true;
  }

  bool startElement(const QString &prefix, const QString &qName,
                    const QXmlStreamAttributes &attributes) {
    prefix_ = prefix;

    bool                 newAttributesSet { false };
    QXmlStreamAttributes newAttributes;

    if (parser_->isPath()) {
      // save bounding box
      if      (qName == "svg") {
        for (int i = 0; i < attributes.length(); ++i) {
          auto &attr = attributes[i];

          auto name  = attr.qualifiedName();
          auto value = attr.value().toString();

          if      (name == "viewBox") {
            auto strs = value.split(" ", Qt::SkipEmptyParts);

            if (strs.size() == 4) {
              bool ok { false }, ok1 { true };
              int x = strs[0].toInt(&ok); if (! ok) ok1 = false;
              int y = strs[1].toInt(&ok); if (! ok) ok1 = false;
              int w = strs[2].toInt(&ok); if (! ok) ok1 = false;
              int h = strs[3].toInt(&ok); if (! ok) ok1 = false;

              if (ok1)
                outputStr(QString("%1 %2 %3 %4\n").arg(x).arg(y).arg(w).arg(h));
            }
          }
        }
      }
      // save path
      else if (qName == "path") {
        for (int i = 0; i < attributes.length(); ++i) {
          auto &attr = attributes[i];

          auto name  = attr.qualifiedName();
          auto value = attr.value().toString();

          if      (name == "d")
            outputStr(value + "\n");
          else if (name == "style")
            outputStr(value + "\n");
        }
      }
    }

    const auto &transformData = parser_->transformData();

    if (transformData.enabled) {
      // check group
      if (qName == "g") {
        bool idFound = false;

        for (int i = 0; i < attributes.length(); ++i) {
          auto &attr = attributes[i];

          auto name  = attr.qualifiedName();
          auto value = attr.value().toString();

          if (name == "id" && transformData.id == value) {
            idFound = true;
            break;
          }
        }

        if (idFound) {
          newAttributesSet = true;

          bool transformFound = false;

          for (int i = 0; i < attributes.length(); ++i) {
            auto &attr = attributes[i];

            auto uri   = attr.namespaceUri();
            auto name  = attr.qualifiedName();
            auto value = attr.value().toString();

            if (name == "transform") {
              transformFound = true;

              newAttributes.append(uri.toString(), name.toString(), transformData.angle);
            }
            else
              newAttributes.append(uri.toString(), name.toString(), value);
          }

          if (! transformFound)
            newAttributes.append("transform", transformData.angle);
        }
      }
    }

    //---

    // dump element
    bool dump = false;

    if (parser_->isDump() || parser_->isEncode() || parser_->isTransform())
      dump = true;

    if (prefix != "")
      dump = false;

    if (dump) {
      if (! newAttributesSet)
        newAttributes = attributes;

      nobreak_ = isNoBreak(qName);

      if (depth_ > 0 && ! closed_) {
        if (! nobreak_)
          outputStr(">\n");
        else
          outputStr(">");

        closed_ = true;
      }

      if (! nobreak_)
        outputStr(depthStr_ + "<" + qName);
      else
        outputStr("<" + qName);

      for (int i = 0; i < newAttributes.length(); ++i) {
        auto &attr = newAttributes[i];

      //auto uri    = attr.namespaceUri();
        auto prefix = attr.prefix();
        auto name   = attr.qualifiedName();
        auto value  = attr.value().toString();

        if (prefix == "") {
          outputStr("\n" + depthStr_ + "   ");
          outputStr(name + "=\"" + value + "\"");
        }
#if 0
        else {
          outputStr("\n" + depthStr_ + "   ");
          outputStr(prefix + ":" + name + "=\"" + value + "\"");
        }
#endif
      }

      closed_ = false;
      text_   = false;
    }

    //---

    incDepth();

    return true;
  }

  bool endElement(const QString & /*prefix*/, const QString &qName) {
    decDepth();

    //---

    bool dump = false;

    if (parser_->isDump() || parser_->isEncode() || parser_->isTransform())
      dump = true;

    if (prefix_ != "")
      dump = false;

    if (dump) {
      if (! closed_) {
        outputStr("/>\n");

        closed_ = true;
      }
      else {
        if (! text_)
          outputStr(depthStr_ + "</" + qName + ">\n");
        else
          outputStr("</" + qName + ">\n");
      }

      text_ = false;
    }

    return true;
  }

  void outputStr(const QString &str) {
    std::cout << str.toStdString();
  }

  bool isNoBreak(const QString &name) const {
    if (name == "tspan") return true;

    return false;
  }

  void incDepth() {
    ++depth_;

    depthStr_ += "  ";

    prefixStack_.push_back(prefix_);

    prefix_ = "";
  }

  void decDepth() {
    assert(! prefixStack_.empty());

    prefix_ = prefixStack_.back();

    prefixStack_.pop_back();

    --depth_;

    depthStr_ = depthStr_.mid(0, depthStr_.length() - 2);
  }

 private:
  CQSVGParser* parser_ { nullptr };

  int     depth_   { 0 };
  QString depthStr_;

  QString              prefix_;
  std::vector<QString> prefixStack_;

  mutable bool closed_  { false };
  mutable bool text_    { false };
  mutable bool nobreak_ { false };
};

//---

CQSVGParser::
CQSVGParser()
{
}

bool
CQSVGParser::
parse(const QString &filename)
{
  filename_ = filename;

  QFile file(filename_);

  if (! file.open(QIODevice::ReadOnly))
    return false;

  CQSVGParserHandler handler(this);

  QXmlStreamReader xml(&file);

  xml.setNamespaceProcessing(false);

  handler.parse(xml);

  return true;
}

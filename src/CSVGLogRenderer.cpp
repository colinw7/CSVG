#include <CSVGLogRenderer.h>

CSVGLogRenderer::
CSVGLogRenderer() :
 os_(&std::cout)
{
}

CSVGLogRenderer::
CSVGLogRenderer(const CSVGLogRenderer &r) :
 os_(r.os_), silent_(r.silent_)
{
}

CSVGLogRenderer *
CSVGLogRenderer::
dup() const
{
  return new CSVGLogRenderer(*this);
}

void
CSVGLogRenderer::
logNL(const std::string &str) const
{
  if (! isSilent())
    os() << str << std::endl;
}

void
CSVGLogRenderer::
log(const std::string &str) const
{
  if (! isSilent())
    os() << str;
}

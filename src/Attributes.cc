#include "Attributes.hh"
#include <list>
#include <sstream>
#include <string>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/json_parser.hpp>


namespace dadi {

Attributes::Attributes() {}

Attributes::Attributes(const Attributes& other) : pt(other.pt) {}

void
Attributes::loadAttr(const std::string& data, int format) {
  using boost::property_tree::ptree;
  std::istringstream ss(data);

  switch(format) {
  case FORMAT_JSON:
    read_json(ss, pt);
    break;
  case FORMAT_INI:
    read_ini(ss, pt);
    break;
  case FORMAT_XML:
  default:
    read_xml(ss, pt);
  }
}

std::string
Attributes::saveAttr(int format) const {
  std::ostringstream ss;

  switch(format) {
  case FORMAT_JSON:
    write_json(ss, pt);
    break;
  case FORMAT_INI:
    write_ini(ss, pt);
    break;
  case FORMAT_XML:
  default:
    write_xml(ss, pt);
  }

  return ss.str();
}

} /* namespace dadi */
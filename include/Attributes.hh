#ifndef _ATTRIBUTES_HH_
#define _ATTRIBUTES_HH_
/**
 * @file   Attributes.hh
 * @author Haïkel Guémar <haikel.guemar@sysfera.com>
 * @brief  defines attributes management
 *
 */

#include <list>
#include <string>
#include <boost/any.hpp>
#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include "Exception/Attributes.hh"

namespace dadi {

/**
 * @class Attributes
 * @brief base class with attributes
 *
 * stupid class that wraps a boost::property_tree::ptree
 * and (de)serialize it in xml format (easy to parse)
 */
class Attributes {
public:
  enum Format {
    FORMAT_XML = 0,
    FORMAT_INI,
    FORMAT_JSON
  };

  /**
   * @brief default constructor
   */
  Attributes();
  /**
   * @brief copy constructor
   */
  Attributes(const Attributes& other);
  /**
   * @brief constructor from serialized data
   * @param data serialized data
   * @param format XML by default
   */
  Attributes(const std::string& data, int format = FORMAT_XML);

  /* accessors */
  /**
   * @brief get value associated to path
   * @param path path to attribute
   * @return expected value
   * @throw dadi::UnknownAttributeError
   * @throw dadi::InvalidAttributeError
   */
  template<typename T> T
  getAttr(const std::string& path) const {
    try {
      return pt.get<T>(path);
    } catch (const boost::property_tree::ptree_bad_path& e) {
      BOOST_THROW_EXCEPTION(UnknownAttributeError() << errinfo_msg(e.what()));
    } catch (const boost::property_tree::ptree_bad_data& e) {
      BOOST_THROW_EXCEPTION(InvalidAttributeError() << errinfo_msg(e.what()));
    }
  }

  /**
   * @brief get value associated to path
   * @param path path to attribute
   * @return expected list of values
   * @throw dadi::UnknownAttributeError
   * @throw dadi::InvalidAttributeError
   */
  template<typename T> std::list<T>
  getAttrList(const std::string& path) const {
    std::list<T> myList;
    try {
      BOOST_FOREACH(boost::property_tree::ptree::value_type& v,
                    pt.get_child(path)) {
        myList.push_back(v.second.get_value<T>());
      }
    } catch (const boost::property_tree::ptree_bad_path& e) {
      BOOST_THROW_EXCEPTION(UnknownAttributeError() << errinfo_msg(e.what()));
    } catch (const boost::property_tree::ptree_bad_data& e) {
      BOOST_THROW_EXCEPTION(InvalidAttributeError() << errinfo_msg(e.what()));
    }

    return myList;
  }

  /**
   * @brief get value associated to path or send default value
   *        does not throw any exception
   * @param path path to attribute
   * @param default_value default value to return
   * @return expected value or default
   */
  template<typename T> T
  getAttr(const std::string& path, T default_value) const {
    return pt.get(path, default_value);
  }

  /* modifiers */
  /**
   * @brief add value to attributes
   * @param path path to attribute
   * @param value
   */
  template<typename T> void
  putAttr(const std::string& path, T value) {
    pt.put(path, value);
  }

  /**
   * @brief deserialize attributes
   * @param[in] data serialized attributes
   * @param format XML by default
   */
  void loadAttr(const std::string& data, int format = FORMAT_XML);

  /**
   * @brief serialize attributes
   * @param format XML by default
   * @return serialized attribute
   */
  std::string saveAttr(int format = 0) const;

  /* operators */
  Attributes& operator=(const Attributes& other);
  bool operator==(const Attributes& other);
  bool operator!=(const Attributes& other);

private:
  boost::property_tree::ptree pt; /**< property tree holding attributes */
};

} /* namespace dadi */

#endif /* _ATTRIBUTES_HH_ */

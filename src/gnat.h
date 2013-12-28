#ifndef gnat_gnat_H
#define gnat_gnat_H

#include <map>
#include <vector>
#include <string>
#include <list>

namespace gnat {

// Some common typedefs
typedef std::string String;

// TODO: enable C++11 and use template aliases
template<typename T> class Vector : public std::vector<T> {};
template<typename T> class List : public std::list<T> {};
template<typename K, typename V> class Map : public std::map<K,V> {};

}  // namespace gnat

#endif


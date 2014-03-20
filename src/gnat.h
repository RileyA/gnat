#ifndef gnat_gnat_H
#define gnat_gnat_H

#include <map>
#include <set>
#include <vector>
#include <string>
#include <list>
#include <cstdio>
#include <cstdlib>
#include <cmath>

// TODO: <cstdint>?
#include <inttypes.h>

namespace gnat {

// Some common typedefs
typedef std::string String;
typedef float Real;  // So we can swap to double precision somewhat painlessly.
typedef unsigned char Byte;

// TODO: enable C++11 and use template aliases
template<typename T> class Vector : public std::vector<T> {};
template<typename T> class List : public std::list<T> {};
template<typename K, typename V> class Map : public std::map<K,V> {};
template<typename K> class Set : public std::set<K> {};

// TODO: Set up debug vs release build configs, and disable this in release.
#define DCHECK(check) if (!(check)) \
  {\
    printf("Check fail: %s:%d\n", __FILE__, __LINE__); \
    exit(1);                                         \
  }

#define NOTREACHED() {\
  printf("NOTREACHED: %s:%d\n", __FILE__, __LINE__); \
  exit(1);                                         \
  }

#define APPLICATION_NAME "GNAT_DEBUG"

}  // namespace gnat

#endif


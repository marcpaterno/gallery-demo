#ifndef FOR_EACH_ENTRY_HH
#define FOR_EACH_ENTRY_HH

#include <cstddef>

// for_each_entry(fm, f) encapsulates a loop over all entries in the
// primary product of an association, and calls the provided function
// once for each collection of associated objects.
// 'fm' must be a FindMany object, relating the entries in the product
// whose handle was used to create the FindMany object to some objects
// (of type B) in a second product in the event. For each element in
// the first product, for_each_entry will call the callable object
// 'f', (typically a lambda expression), passing to it a argument of
// type 'std::vector<B const*> const&'.

template <class FM, class FCN>
void for_each_entry(FM const& fm, FCN f) {
  for (std::size_t i = 0, sz = fm.size(); i != sz; ++i) {
    f(fm.at(i));
  }
}

#endif

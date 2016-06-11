#ifndef FOR_EACH_ASSOCIATED_HH
#define FOR_EACH_ASSOCIATED_HH

template <class FM, class FCN>
void for_each_associated(FM const& fm, FCN f) {
  for (std::size_t i = 0, sz = fm.size(); i != sz; ++i) {
    for (auto const& x : fm.at(i)) {
      f(x);
    }
  }
}

#endif

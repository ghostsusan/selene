// This file is part of the `Selene` library.
// Copyright 2017 Michael Hofmann (https://github.com/kmhofmann).
// Distributed under MIT license. See accompanying LICENSE file in the top-level directory.

#ifndef SELENE_TEST_UTILS_HPP
#define SELENE_TEST_UTILS_HPP

#include <selene/base/Promote.hpp>

#include <boost/filesystem.hpp>

#include <random>
#include <type_traits>

namespace sln_test {

inline boost::filesystem::path get_tmp_path()
{
  namespace fs = boost::filesystem;
  const auto tmp_path = fs::temp_directory_path() / "selene_test";

  if (!fs::exists(tmp_path))
  {
    fs::create_directories(tmp_path);
  }

  return tmp_path;
}

template <typename T>
inline auto uniform_distribution(T lb, T ub)
{
  // http://en.cppreference.com/w/cpp/numeric/random/uniform_int_distribution:
  // The result type generated by the generator. The effect is undefined if this is not one of short, int, long,
  // long long, unsigned short, unsigned int, unsigned long, or unsigned long long.
  using DT = std::conditional_t<(sizeof(T) < sizeof(short)), sln::promote_t<T>, T>;

  constexpr auto is_int = std::is_integral<T>::value;
  constexpr auto is_fp = std::is_floating_point<T>::value;

  using DieType = std::conditional_t<is_int, std::uniform_int_distribution<DT>,
      std::conditional_t<is_fp, std::uniform_real_distribution<T>, void>>;
  return DieType(lb, ub);
}

}  // namespace sln_test

#endif  // SELENE_TEST_UTILS_HPP

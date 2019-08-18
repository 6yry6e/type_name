#ifndef _TYPE_NAME_H_
#define _TYPE_NAME_H_

#include <array>

namespace detail {

  template <size_t ... Ns> struct range {};

  template <size_t ... Ns> struct range_gen;

  template <size_t B, size_t E, size_t ... Ns>
  struct range_gen<B, E, Ns...> {
    using type = typename range_gen<B, E - 1, E - 1, Ns...>::type;
  };

  template <size_t B, size_t ... Ns>
  struct range_gen<B, B, Ns...> {
    using type = range<Ns...>;
  };

  template <size_t B, size_t E>
  using range_t = typename range_gen<B, E + 1>::type;

  template <size_t N>
  using sequence_t = typename range_gen<0, N>::type;

  template <typename C, std::size_t N, std::size_t...Is>
  constexpr std::array<C, sizeof...(Is) + 1> substr(const C(&s)[N], range<Is...>)
  {
    return {((Is < N) ? s[Is] : 0)..., 0};
  }

  template <std::size_t L, typename C, std::size_t N>
  constexpr std::array<C, L + 1> truncate(const C(&s)[N])
  {
    return substr(s, 0, L);
  }

  template <std::size_t B, std::size_t E, typename C, std::size_t N>
  constexpr std::array<C, E - B + 2> substr(const C(&s)[N])
  {
    return substr(s, range_t<B, E>{});
  }

}


template <typename T>
constexpr auto get_type_name ()
{
#if defined(__GNUC__)
  constexpr decltype(__PRETTY_FUNCTION__) func_name = __PRETTY_FUNCTION__;
  return detail::substr<41, std::size(func_name) - 3>(func_name);
#elif defined(_MSC_VER)
  constexpr decltype(__FUNCSIG__) func_name = __FUNCSIG__;
  return detail::substr<27, std::size(func_name) - 9>(func_name);
#else
  static_assert(false, "Unsupported compiler");
#endif
}

#endif
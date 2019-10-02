/*
Copyright 2019 Glen Joseph Fernandes
(glenjofe@gmail.com)

Distributed under the Boost Software License, Version 1.0.
(http://www.boost.org/LICENSE_1_0.txt)
*/
#ifndef BOOST_SPAN_SPAN_HPP
#define BOOST_SPAN_SPAN_HPP

#include <boost/config.hpp>
#if !defined(BOOST_NO_CXX11_HDR_ARRAY)
#include <array>
#endif
#if !defined(BOOST_NO_CXX11_HDR_INITIALIZER_LIST)
#include <initializer_list>
#endif
#include <iterator>
#if !defined(BOOST_NO_CXX11_HDR_TUPLE)
#include <tuple>
#endif
#if !defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
#include <type_traits>
#endif

namespace boost
{
namespace spans
{
BOOST_STATIC_CONSTEXPR std::size_t dynamic_extent = -1;

template <class T, std::size_t E = dynamic_extent>
class span;

namespace detail
{
#if !defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
using std::remove_cv;
#else
template <class T>
struct remove_cv
{
  typedef T type;
};

template <class T>
struct remove_cv<const T>
{
  typedef T type;
};

template <class T>
struct remove_cv<volatile T>
{
  typedef T type;
};

template <class T>
struct remove_cv<const volatile T>
{
  typedef T type;
};
#endif

#if !defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
using std::enable_if;
#else
template <bool, class = void>
struct enable_if
{
};

template <class R>
struct enable_if<true, R>
{
  typedef R type;
};
#endif

template <std::size_t E, std::size_t N>
struct capacity
{
  BOOST_STATIC_CONSTEXPR bool value = E == boost::spans::dynamic_extent || E == N;
};

struct none
{
};

#if !defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
using std::is_convertible;
#else
template <class, class>
struct is_convertible
{
  BOOST_STATIC_CONSTEXPR bool value = true;
};
#endif

template <class>
struct is_span
{
  BOOST_STATIC_CONSTEXPR bool value = false;
};

template <class T, std::size_t E>
struct is_span<boost::spans::span<T, E>>
{
  BOOST_STATIC_CONSTEXPR bool value = true;
};

template <class>
struct is_std_array
{
  BOOST_STATIC_CONSTEXPR bool value = false;
};

#if !defined(BOOST_NO_CXX11_HDR_ARRAY)
template <class T, std::size_t N>
struct is_std_array<std::array<T, N>>
{
  BOOST_STATIC_CONSTEXPR bool value = true;
};
#endif

#if !defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
using std::is_array;
#else
template <class>
struct is_array
{
  BOOST_STATIC_CONSTEXPR bool value = false;
};

template <class T, std::size_t N>
struct is_array<T[N]>
{
  BOOST_STATIC_CONSTEXPR bool value = true;
};
#endif

template <class>
struct is_list
{
  BOOST_STATIC_CONSTEXPR bool value = false;
};

#if !defined(BOOST_NO_CXX11_HDR_INITIALIZER_LIST)
template <class T>
struct is_list<std::initializer_list<T>>
{
  BOOST_STATIC_CONSTEXPR bool value = true;
};
#endif

#if !defined(BOOST_NO_CXX11_SFINAE_EXPR) && !defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
template <class, class>
struct valid
{
  typedef void type;
};

template <class, class, class = void>
struct is_compatible
{
  BOOST_STATIC_CONSTEXPR bool value = false;
};

template <class T, class C>
struct is_compatible<
  T,
  C,
  typename valid<decltype(std::declval<C&>().size()),
                 typename std::enable_if<std::is_convertible<
                   typename std::remove_pointer<decltype(std::declval<C&>().data())>::type (*)[],
                   T (*)[]>::value>::type>::type>
{
  BOOST_STATIC_CONSTEXPR bool value = true;
};
#else
template <class, class>
struct is_compatible
{
  BOOST_STATIC_CONSTEXPR bool value = true;
};
#endif

template <class T, std::size_t E, class C>
struct container
{
  BOOST_STATIC_CONSTEXPR bool value =
    E == boost::spans::dynamic_extent && !is_span<typename remove_cv<C>::type>::value &&
    !is_std_array<typename remove_cv<C>::type>::value &&
    !is_array<typename remove_cv<C>::type>::value && !is_list<typename remove_cv<C>::type>::value &&
    is_compatible<T, C>::value;
};

template <std::size_t E, std::size_t O, std::size_t C = boost::spans::dynamic_extent>
struct sub
{
  BOOST_STATIC_CONSTEXPR std::size_t value =
    C == boost::spans::dynamic_extent
      ? (E == boost::spans::dynamic_extent ? boost::spans::dynamic_extent : E - O)
      : C;
};

template <class T, std::size_t E>
struct store
{
  BOOST_CONSTEXPR
  store(T* p_, std::size_t) BOOST_NOEXCEPT : p(p_)
  {
  }
  BOOST_STATIC_CONSTEXPR std::size_t n = E;
  T*                                 p;
};

template <class T>
struct store<T, boost::spans::dynamic_extent>
{
  BOOST_CONSTEXPR
  store(T* p_, std::size_t n_) BOOST_NOEXCEPT : p(p_),
                                                n(n_)
  {
  }
  T*          p;
  std::size_t n;
};

template <class T, std::size_t E>
struct views
{
  BOOST_STATIC_CONSTEXPR std::size_t value =
    E == boost::spans::dynamic_extent ? boost::spans::dynamic_extent : sizeof(T) * E;
};

#if !defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
using std::is_const;
#else
template <class>
struct is_const
{
  BOOST_STATIC_CONSTEXPR bool value = false;
};

template <class T>
struct is_const<const T>
{
  BOOST_STATIC_CONSTEXPR bool value = true;
};
#endif

#if !defined(BOOST_NO_CXX11_HDR_TYPE_TRAITS)
using std::integral_constant;
#else
template <class T, T V>
struct integral_constant
{
  typedef T                 value_type;
  typedef integral_constant type;

  BOOST_STATIC_CONSTEXPR T value = V;

  BOOST_CONSTEXPR operator value_type() const BOOST_NOEXCEPT { return V; }

  BOOST_CONSTEXPR value_type
                  operator()() const BOOST_NOEXCEPT
  {
    return V;
  }
};

template <class T, T V>
BOOST_CONSTEXPR_OR_CONST T integral_constant<T, V>::value;
#endif

} // namespace detail

template <class T, std::size_t E>
class span {
public:
  typedef T                                   element_type;
  typedef typename detail::remove_cv<T>::type value_type;
  typedef std::size_t                         size_type;
  typedef std::ptrdiff_t                      difference_type;
  typedef T*                                  pointer;
  typedef const T*                            const_pointer;
  typedef T&                                  reference;
  typedef const T&                            const_reference;
  typedef T*                                  iterator;
  typedef const T*                            const_iterator;
  typedef std::reverse_iterator<T*>           reverse_iterator;
  typedef std::reverse_iterator<const T*>     const_reverse_iterator;

  BOOST_STATIC_CONSTEXPR std::size_t extent = E;

#if !defined(BOOST_NO_CXX11_FUNCTION_TEMPLATE_DEFAULT_ARGS)
  template <std::size_t N = E,
            class         = typename detail::enable_if<N == dynamic_extent || N == 0>::type>
  BOOST_CONSTEXPR
  span() BOOST_NOEXCEPT : s_(0, 0)
  {
  }
#else
  BOOST_CONSTEXPR
  span() BOOST_NOEXCEPT : s_(0, 0)
  {
  }
#endif

  BOOST_CONSTEXPR
  span(pointer p, size_type c)
    : s_(p, c)
  {
  }

  BOOST_CONSTEXPR
  span(pointer f, pointer l)
    : s_(f, l - f)
  {
  }

  template <std::size_t N>
  BOOST_CONSTEXPR
    span(T (&a)[N],
         typename detail::enable_if<detail::capacity<E, N>::value, detail::none>::type =
           detail::none()) BOOST_NOEXCEPT : s_(a, N)
  {
  }

#if !defined(BOOST_NO_CXX11_HDR_ARRAY)
  template <std::size_t N>
  BOOST_CONSTEXPR
  span(std::array<value_type, N>& a,
       typename detail::enable_if<detail::capacity<E, N>::value, detail::none>::type =
         detail::none()) BOOST_NOEXCEPT : s_(a.data(), N)
  {
  }

  template <std::size_t N>
  BOOST_CONSTEXPR
  span(const std::array<value_type, N>& a,
       typename detail::enable_if<detail::capacity<E, N>::value, detail::none>::type =
         detail::none()) BOOST_NOEXCEPT : s_(a.data(), N)
  {
  }
#endif

#if !defined(BOOST_NO_CXX11_HDR_INITIALIZER_LIST)
  template <class U>
  BOOST_CONSTEXPR
  span(std::initializer_list<U> l,
       typename detail::enable_if<E == dynamic_extent &&
                                    detail::is_convertible<const U (*)[], T (*)[]>::value,
                                  detail::none>::type = detail::none()) BOOST_NOEXCEPT
    : s_(l.begin(), l.size())
  {
  }
#endif

  template <class C>
  BOOST_CONSTEXPR
  span(C& c,
       typename detail::enable_if<detail::container<T, E, C>::value, detail::none>::type =
         detail::none())
    : s_(c.data(), c.size())
  {
  }

  template <class C>
  BOOST_CONSTEXPR
  span(const C& c,
       typename detail::enable_if<detail::container<T, E, C>::value, detail::none>::type =
         detail::none())
    : s_(c.data(), c.size())
  {
  }

  template <class U, std::size_t N>
  BOOST_CONSTEXPR
  span(const span<U, N>& s,
       typename detail::enable_if<detail::capacity<E, N>::value &&
                                    detail::is_convertible<U (*)[], T (*)[]>::value,
                                  detail::none>::type = detail::none()) BOOST_NOEXCEPT
    : s_(s.data(), s.size())
  {
  }

  template <std::size_t C>
  BOOST_CONSTEXPR span<T, C>
                  first() const
  {
    return span<T, C>(s_.p, C);
  }

  template <std::size_t C>
  BOOST_CONSTEXPR span<T, C>
                  last() const
  {
    return span<T, C>(s_.p + (s_.n - C), C);
  }

  template <std::size_t O>
  BOOST_CONSTEXPR span<T, detail::sub<E, O>::value>
                  subspan() const
  {
    return span<T, detail::sub<E, O>::value>(s_.p + O, s_.n - O);
  }

  template <std::size_t O, std::size_t C>
  BOOST_CONSTEXPR
    typename detail::enable_if<C == dynamic_extent, span<T, detail::sub<E, O, C>::value>>::type
    subspan() const
  {
    return span<T, detail::sub<E, O, C>::value>(s_.p + O, s_.n - O);
  }

  template <std::size_t O, std::size_t C>
  BOOST_CONSTEXPR
    typename detail::enable_if<C != dynamic_extent, span<T, detail::sub<E, O, C>::value>>::type
    subspan() const
  {
    return span<T, detail::sub<E, O, C>::value>(s_.p + O, C);
  }

  BOOST_CONSTEXPR span<T, dynamic_extent>
                  first(size_type c) const
  {
    return span<T, dynamic_extent>(s_.p, c);
  }

  BOOST_CONSTEXPR span<T, dynamic_extent>
                  last(size_type c) const
  {
    return span<T, dynamic_extent>(s_.p + (s_.n - c), c);
  }

  BOOST_CONSTEXPR span<T, dynamic_extent>
                  subspan(size_type o, size_type c = dynamic_extent) const
  {
    return span<T, dynamic_extent>(s_.p + o, c == dynamic_extent ? s_.n - o : c);
  }

  BOOST_CONSTEXPR size_type
                  size() const BOOST_NOEXCEPT
  {
    return s_.n;
  }

  BOOST_CONSTEXPR size_type
                  size_bytes() const BOOST_NOEXCEPT
  {
    return s_.n * sizeof(T);
  }

  BOOST_CONSTEXPR bool
  empty() const BOOST_NOEXCEPT
  {
    return s_.n == 0;
  }

  BOOST_CONSTEXPR reference operator[](size_type i) const { return s_.p[i]; }

  BOOST_CONSTEXPR reference
                  front() const
  {
    return *s_.p;
  }

  BOOST_CONSTEXPR reference
                  back() const
  {
    return s_.p[s_.n - 1];
  }

  BOOST_CONSTEXPR pointer
                  data() const BOOST_NOEXCEPT
  {
    return s_.p;
  }

  BOOST_CONSTEXPR iterator
                  begin() const BOOST_NOEXCEPT
  {
    return s_.p;
  }

  BOOST_CONSTEXPR iterator
                  end() const BOOST_NOEXCEPT
  {
    return s_.p + s_.n;
  }

  BOOST_CONSTEXPR reverse_iterator
                  rbegin() const BOOST_NOEXCEPT
  {
    return reverse_iterator(s_.p + s_.n);
  }

  BOOST_CONSTEXPR reverse_iterator
                  rend() const BOOST_NOEXCEPT
  {
    return reverse_iterator(s_.p);
  }

  BOOST_CONSTEXPR const_iterator
                  cbegin() const BOOST_NOEXCEPT
  {
    return s_.p;
  }

  BOOST_CONSTEXPR const_iterator
                  cend() const BOOST_NOEXCEPT
  {
    return s_.p + s_.n;
  }

  BOOST_CONSTEXPR const_reverse_iterator
                  crbegin() const BOOST_NOEXCEPT
  {
    return const_reverse_iterator(s_.p + s_.n);
  }

  BOOST_CONSTEXPR const_reverse_iterator
                  crend() const BOOST_NOEXCEPT
  {
    return const_reverse_iterator(s_.p);
  }

  friend BOOST_CONSTEXPR iterator
                         begin(span s) BOOST_NOEXCEPT
  {
    return s.begin();
  }

  friend BOOST_CONSTEXPR iterator
                         end(span s) BOOST_NOEXCEPT
  {
    return s.end();
  }

private:
  detail::store<T, E> s_;
};

template <class T, std::size_t E>
BOOST_CONSTEXPR_OR_CONST std::size_t span<T, E>::extent;

#if __cpp_deduction_guides >= 201703
template <class T, size_t N>
span(T (&)[N])->span<T, N>;

template <class T, size_t N>
span(std::array<T, N>&)->span<T, N>;

template <class T, size_t N>
span(const std::array<T, N>&)->span<const T, N>;

template <class C>
span(C&)->span<typename C::value_type>;

template <class C>
span(const C&)->span<const typename C::value_type>;
#endif

#if __cpp_lib_byte >= 201603
template <class T, std::size_t E>
inline span<const std::byte, detail::views<T, E>::value>
as_bytes(span<T, E> s) BOOST_NOEXCEPT
{
  return span<const std::byte, detail::views<T, E>::value>(
    reinterpret_cast<const std::byte*>(s.data()), s.size_bytes());
}
#endif

template <class T, std::size_t E>
inline span<const char, detail::views<T, E>::value>
as_chars(span<T, E> s) BOOST_NOEXCEPT
{
  return span<const char, detail::views<T, E>::value>(reinterpret_cast<const char*>(s.data()),
                                                      s.size_bytes());
}

#if __cpp_lib_byte >= 201603
template <class T, std::size_t E>
inline typename detail::enable_if<!detail::is_const<T>::value,
                                  span<std::byte, detail::views<T, E>::value>>::type
as_writable_bytes(span<T, E> s) BOOST_NOEXCEPT
{
  return span<std::byte, detail::views<T, E>::value>(reinterpret_cast<std::byte*>(s.data()),
                                                     s.size_bytes());
}
#endif

template <class T, std::size_t E>
inline typename detail::enable_if<!detail::is_const<T>::value,
                                  span<char, detail::views<T, E>::value>>::type
as_writable_chars(span<T, E> s) BOOST_NOEXCEPT
{
  return span<char, detail::views<T, E>::value>(reinterpret_cast<char*>(s.data()), s.size_bytes());
}

template <std::size_t I, class T, std::size_t E>
BOOST_CONSTEXPR inline T&
get(span<T, E> s) BOOST_NOEXCEPT
{
#if !defined(BOOST_NO_CXX11_STATIC_ASSERT)
  static_assert(E != dynamic_extent && I < E, "E != dynamic_extent && I < E");
#endif
  return s[I];
}

template <class T>
BOOST_CONSTEXPR inline span<T>
make_span(T* p, std::size_t n) BOOST_NOEXCEPT
{
  return span<T>(p, n);
}

template <class T>
BOOST_CONSTEXPR inline span<T>
make_span(T* f, T* l) BOOST_NOEXCEPT
{
  return span<T>(f, l);
}

template <class T, std::size_t N>
BOOST_CONSTEXPR inline span<T, N> make_span(T (&a)[N]) BOOST_NOEXCEPT
{
  return span<T, N>(a);
}

#if !defined(BOOST_NO_CXX11_HDR_ARRAY)
template <class T, std::size_t N>
BOOST_CONSTEXPR inline span<T, N>
make_span(std::array<T, N>& a) BOOST_NOEXCEPT
{
  return span<T, N>(a);
}

template <class T, std::size_t N>
BOOST_CONSTEXPR inline span<const T, N>
make_span(const std::array<T, N>& a) BOOST_NOEXCEPT
{
  return span<const T, N>(a);
}
#endif

template <class C>
BOOST_CONSTEXPR inline span<typename C::value_type>
make_span(C& c)
{
  return span<typename C::value_type>(c);
}

template <class C>
BOOST_CONSTEXPR inline span<const typename C::value_type>
make_span(const C& c)
{
  return span<const typename C::value_type>(c);
}

} // namespace spans
} // namespace boost

#if !defined(BOOST_NO_CXX11_HDR_TUPLE)
namespace std
{
template <class T, size_t E>
struct tuple_size<boost::spans::span<T, E>> : boost::spans::detail::integral_constant<size_t, E>
{
};

template <class T>
struct tuple_size<boost::spans::span<T, boost::spans::dynamic_extent>>
{
};

template <size_t I, class T, std::size_t E>
struct tuple_element<I, boost::spans::span<T, E>>
{
#if !defined(BOOST_NO_CXX11_STATIC_ASSERT)
  static_assert(E != boost::spans::dynamic_extent && I < E, "E != dynamic_extent && I < E");
#endif
  typedef T type;
};

} // namespace std

#endif
#endif

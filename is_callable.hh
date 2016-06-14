#ifndef IS_CALLABLE_HH
#define IS_CALLABLE_HH
// trait to detect callable types. This will be standardized in C++17.
// This isn't quite working yet...
namespace detail {
  template <class ... > using void_t  = void;
 
  template <typename Expr, typename Enable = void>
  struct is_callable_impl : std::false_type
  { };
 
  template <typename F, typename ...Args>
  struct is_callable_impl<F(Args...), void_t<std::result_of_t<F(Args...)>>> : std::true_type
  { };
}
 
template <typename Expr>
struct is_callable : detail::is_callable_impl<Expr>
{ };

#endif

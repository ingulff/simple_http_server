#ifndef TT_UTILS_ALGO_HPP
#define TT_UTILS_ALGO_HPP

namespace tt_program::utils
{

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };

template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

}

#endif // TT_UTILS_ALGO_HPP

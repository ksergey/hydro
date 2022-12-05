// ------------------------------------------------------------
// Copyright 2022-present Sergey Kovalevich <inndie@gmail.com>
// ------------------------------------------------------------

#pragma once

#include <tuple>
#include <type_traits>
#include <utility>

namespace mymk {
namespace detail {

template<typename... States>
constexpr std::size_t mp_count() {
  return sizeof...(States);
}

template<typename State, typename... States, std::size_t... I>
constexpr std::size_t findImpl(std::integer_sequence<std::size_t, I...>) {
  using Tuple = std::tuple<States...>;
  std::size_t const index = ((std::is_same_v<State, std::tuple_element_t<I, Tuple>> ? (I + 1) : 0) + ...);
  return index == 0 ? (1 + mp_count<States...>()) : (index - 1);
}

template<typename State, typename... States>
constexpr std::size_t mp_find() {
  return findImpl<State, States...>(std::make_index_sequence<mp_count<States...>()>());
}

template<std::size_t... I, typename Fn>
constexpr decltype(auto) mp_with_index(std::size_t index, Fn&& fn, std::integer_sequence<std::size_t, I...>) {
  auto invoke = [&](auto N) {
    if (index == N) {
      std::invoke(std::forward<Fn>(fn), N);
    }
  };

  ((invoke(std::integral_constant<std::size_t, I>())), ...);
}

template<std::size_t Size, typename Fn>
constexpr decltype(auto) mp_with_index(std::size_t index, Fn&& fn) {
  return mp_with_index(index, std::forward<Fn>(fn), std::make_index_sequence<Size>());
}

} // namespace detail

template<typename... States>
class FSM {
private:
  using Storage = std::tuple<States...>;

  static constexpr auto kNone = std::size_t(-1);

  Storage storage_;

  std::size_t current_ = kNone;
  std::size_t next_ = kNone;

public:
  FSM() = default;

  template<typename State>
  bool isCurrentState() const noexcept {
    constexpr auto index = detail::mp_find<State, States...>();
    static_assert(index < detail::mp_count<States...>(), "state is not a part of FSM");
    return index == current_;
  }

  template<class State>
  void setNextState() noexcept {
    constexpr auto index = detail::mp_find<State, States...>();
    static_assert(index < detail::mp_count<States...>(), "state is not a part of FSM");
    next_ = index;
  }

  template<typename... Args>
  void run(Args&&... args) {
    do {
      if (next_ != kNone) {
        // leave current state
        detail::mp_with_index<sizeof...(States)>(current_, [&](auto I) {
          std::get<I>(storage_).leave(*this, std::forward<Args>(args)...);
        });

        // change current state to next
        current_ = next_;

        // enter current state
        detail::mp_with_index<sizeof...(States)>(current_, [&](auto I) {
          std::get<I>(storage_).enter(*this, std::forward<Args>(args)...);
        });

        next_ = kNone;
      }

      // run current state
      detail::mp_with_index<sizeof...(States)>(current_, [&](auto I) {
        std::get<I>(storage_).run(*this, std::forward<Args>(args)...);
      });

    } while (next_ != kNone);
  }

  template<typename Fn>
  void observe(Fn&& fn) {
    detail::mp_with_index<sizeof...(States)>(current_, [&](auto I) {
      std::invoke(std::forward<Fn>(fn), std::get<I>(storage_));
    });
  }
};

} // namespace mymk

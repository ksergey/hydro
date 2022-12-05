// ------------------------------------------------------------
// Copyright 2022-present Sergey Kovalevich <inndie@gmail.com>
// ------------------------------------------------------------

#pragma once

#include <functional>

#include <Arduino.h>

namespace mymk {

template<int Pin, unsigned long DebounceTimeMs = 50>
class Debounce {
private:
  int lastReadState_ = LOW;
  int state_ = LOW;

  unsigned long debounceDeadline_ = 0;

  struct Empty {
    constexpr void operator()() noexcept {}
  };

public:
  Debounce(int currentState = LOW) noexcept : state_(currentState) {}

  void setup() noexcept {
    pinMode(Pin, INPUT);
  }

  [[nodiscard]] int state() const noexcept {
    return state_;
  }

  template<typename Fn = Empty>
  void update(Fn&& fn = {}) noexcept {
    int const value = digitalRead(Pin);
    if (value != lastReadState_) {
      debounceDeadline_ = millis() + DebounceTimeMs;
    }
    if (debounceDeadline_ < millis()) {
      state_ = value;
      std::invoke(std::forward<Fn>(fn));
    }
    lastReadState_ = value;
  }
};

} // namespace mymk

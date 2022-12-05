// ------------------------------------------------------------
// Copyright 2021-present Sergey Kovalevich <inndie@gmail.com>
// ------------------------------------------------------------

#pragma once

#include <functional>

#include <Arduino.h>

namespace mymk {

enum class ButtonState { Press, Release };

template<int Pin, unsigned long DebounceTimeMs = 50>
class Button {
private:
  int lastButtonState_ = LOW;
  int buttonState_ = LOW;

  unsigned long debounceDeadline_ = 0;

  struct Empty {
    constexpr void operator()() noexcept {}
  };

public:
  Button() noexcept {}

  void setup() noexcept {
    pinMode(Pin, INPUT);
  }

  template<typename Fn = Empty>
  void update(Fn&& fn = {}) noexcept {
    int const value = digitalRead(Pin);
    if (value != lastButtonState_) {
      debounceDeadline_ = millis() + DebounceTimeMs;
    }
    if (debounceDeadline_ < millis()) {
      if (value != buttonState_) {
        buttonState_ = value;
        std::invoke(std::forward<Fn>(fn), (buttonState_ == HIGH) ? ButtonState::Press : ButtonState::Release);
      }
    }
    lastButtonState_ = value;
  }
};

} // namespace mymk

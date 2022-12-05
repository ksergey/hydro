// ------------------------------------------------------------
// Copyright 2021-present Sergey Kovalevich <inndie@gmail.com>
// ------------------------------------------------------------

#pragma once

#include <Arduino.h>

namespace mymk {

template<int Pin>
struct Signal {
  void setup() noexcept {
    pinMode(Pin, OUTPUT);
  }

  void operator()(int value) noexcept {
    ::digitalWrite(Pin, value);
  }
};

} // namespace mymk

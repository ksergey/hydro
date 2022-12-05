// ------------------------------------------------------------
// Copyright 2021-present Sergey Kovalevich <inndie@gmail.com>
// ------------------------------------------------------------

#pragma once

#include <functional>

#include <Arduino.h>

namespace mymk {

template<unsigned long IntervalMs = 1000>
class PeriodicTask {
private:
  unsigned long nextFireTask_ = 0;

public:
  PeriodicTask() = default;

  template<class Fn>
  void update(Fn&& fn) {
    if (nextFireTask_ < millis()) {
      nextFireTask_ = millis() + IntervalMs;
      std::invoke(std::forward<Fn>(fn));
    }
  }
};

} // namespace mymk

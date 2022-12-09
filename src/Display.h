// ------------------------------------------------------------
// Copyright 2022-present Sergey Kovalevich <inndie@gmail.com>
// ------------------------------------------------------------

#pragma once

#include <array>
#include <string>

#include <fmt/format.h>

#include <SSD1306Wire.h>

namespace app {

class Display {
private:
  static constexpr auto kMaxDisplayLines = std::size_t(6);

  using DisplayBuffer = std::array<std::string, kMaxDisplayLines>;

  SSD1306Wire wire_;
  DisplayBuffer buffer_;

public:
  Display(Display const&) = delete;
  Display& operator=(Display const&) = delete;

  template<typename... Ts>
  Display(Ts&&... args) : wire_(std::forward<Ts>(args)...) {}

  template<std::size_t N, typename... Ts>
  void formatLine(Ts&&... args) {
    buffer_[N] = fmt::format(std::forward<Ts>(args)...);
  }

  void clear() {
    for (auto& line : buffer_) {
      line = std::string();
    }
  }

  void setup();
  void refresh();
};

} // namespace app

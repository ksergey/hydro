// ------------------------------------------------------------
// Copyright 2022-present Sergey Kovalevich <inndie@gmail.com>
// ------------------------------------------------------------

#include "Display.h"

#include <cstring>

namespace app {

void Display::setup() {
  wire_.init();
  wire_.flipScreenVertically();
  wire_.setFont(ArialMT_Plain_10);
}

void Display::refresh() {
  wire_.clear();

  wire_.setTextAlignment(TEXT_ALIGN_LEFT);
  wire_.setFont(ArialMT_Plain_10);

  std::size_t xOffset = 0;
  std::size_t yOffset = 0;
  for (auto const& line : buffer_) {
    wire_.drawString(xOffset, yOffset, line.c_str());
    yOffset += 10; // font size
  }

  wire_.display();
}

} // namespace app

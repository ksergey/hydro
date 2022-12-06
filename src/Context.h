// ------------------------------------------------------------
// Copyright 2022-present Sergey Kovalevich <inndie@gmail.com>
// ------------------------------------------------------------

#pragma once

#include <cstdint>

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiManager.h>
#include <WiFiUdp.h>

#include "Button.h"
#include "Debounce.h"
#include "Display.h"
#include "Signal.h"

namespace app {
namespace addr {

constexpr auto kDisplayI2C = int(0x3c);

} // namespace addr

namespace pin {

constexpr auto kButton = int(4);      // GPIO4
constexpr auto kPump = int(5);        // GPIO5
constexpr auto kSensor0 = int(13);    // GPIO13
constexpr auto kSensor1 = int(15);    // GPIO15
constexpr auto kDisplaySDA = int(14); // GPIO14
constexpr auto kDisplaySCK = int(12); // GPIO12

} // namespace pin

struct Context {
  mymk::Button<pin::kButton> button;
  mymk::Debounce<pin::kSensor0> sensor0;
  mymk::Debounce<pin::kSensor1> sensor1;
  mymk::Signal<pin::kPump> pump;

  Display display = Display(addr::kDisplayI2C, pin::kDisplaySDA, pin::kDisplaySCK);

  WiFiManager wifiManager;
  WiFiUDP ntpUDP = WiFiUDP();
  NTPClient timeClient = NTPClient(ntpUDP);

  uint64_t nextFloodingTime = std::uint64_t(0);
};

} // namespace app

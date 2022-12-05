// ------------------------------------------------------------
// Copyright 2022-present Sergey Kovalevich <inndie@gmail.com>
// ------------------------------------------------------------

#include "Hydro.h"

namespace app {
namespace {

static constexpr auto kTimezone = int(3 * 3600);
static constexpr auto kTimeUpdateIntervalMs = int(60 * 1000);

} // namespace

void Hydro::setup() {
  Serial.begin(115200);
  Serial.println();

  // setup ports
  context_.button.setup();
  context_.sensor0.setup();
  context_.sensor1.setup();
  context_.pump.setup();

  // setup attached display
  context_.display.setup();

  // NTP client
  context_.timeClient.begin();
  context_.timeClient.setUpdateInterval(kTimeUpdateIntervalMs);
  context_.timeClient.setTimeOffset(kTimezone);

  // wireless connection
  context_.wifiManager.setConfigPortalBlocking(false);
  context_.wifiManager.setConfigPortalTimeout(60);
  context_.wifiManager.autoConnect("HYDROPONICS_starts_here");

  fsm_.setNextState<State_Init>();
}

void Hydro::loop() {
  // handle wifi manager
  context_.wifiManager.process();
  // update time from ntp
  context_.timeClient.update();
  // Run fsm state
  fsm_.run(context_);
  // refresh display
  context_.display.refresh();
}

} // namespace app

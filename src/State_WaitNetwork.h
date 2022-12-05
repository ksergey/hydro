// ------------------------------------------------------------
// Copyright 2022-present Sergey Kovalevich <inndie@gmail.com>
// ------------------------------------------------------------

#pragma once

#include "Context.h"

namespace app {

class State_WaitNetwork {
private:
public:
  State_WaitNetwork() = default;

  template<typename FSM>
  void enter([[maybe_unused]] FSM& fsm, Context& ctx) {
    Serial.println("State_WaitNetwork::enter");
    ctx.display.formatLine<0>("state: wait network");
    ctx.display.formatLine<1>("ip: N/A");
  }

  template<typename FSM>
  void leave([[maybe_unused]] FSM& fsm, Context& ctx) {
    Serial.println("State_WaitNetwork::leave");
    ctx.display.formatLine<0>("state: ***");
    ctx.display.formatLine<1>("ip: {}", WiFi.localIP().toString().c_str());
  }

  template<typename FSM>
  void run([[maybe_unused]] FSM& fsm, [[maybe_unused]] Context& ctx) {
    if (WiFi.status() == WL_CONNECTED) {
      return fsm.template setNextState<State_Idle>();
    }
  }
};

} // namespace app

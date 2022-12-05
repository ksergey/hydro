// ------------------------------------------------------------
// Copyright 2022-present Sergey Kovalevich <inndie@gmail.com>
// ------------------------------------------------------------

#pragma once

#include "Context.h"

namespace app {

struct State_WaitNetwork;

class State_Init {
private:
public:
  State_Init() = default;

  template<typename FSM>
  void enter([[maybe_unused]] FSM& fsm, Context& ctx) {
    Serial.println("State_Init::enter");
    ctx.display.formatLine<0>("state: init");
  }

  template<typename FSM>
  void leave([[maybe_unused]] FSM& fsm, Context& ctx) {
    Serial.println("State_Init::leave");
    ctx.display.formatLine<0>("state: ***");
  }

  template<typename FSM>
  void run(FSM& fsm, [[maybe_unused]] Context& ctx) {
    Serial.println("Let's go boys. Doing my job.");
    fsm.template setNextState<State_WaitNetwork>();
  }
};

} // namespace app

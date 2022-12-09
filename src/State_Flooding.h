// ------------------------------------------------------------
// Copyright 2022-present Sergey Kovalevich <inndie@gmail.com>
// ------------------------------------------------------------

#pragma once

#include "Context.h"

namespace app {

class State_Idle;

class State_Flooding {
private:
public:
  State_Flooding() = default;

  template<typename FSM>
  void enter([[maybe_unused]] FSM& fsm, Context& ctx) {
    Serial.println("State_Flooding::enter");
    ctx.display.clear();
    ctx.display.formatLine<0>("state: flooding");
  }

  template<typename FSM>
  void leave([[maybe_unused]] FSM& fsm, Context& ctx) {
    Serial.println("State_Flooding::leave");
    ctx.pump(0);
  }

  template<typename FSM>
  void run([[maybe_unused]] FSM& fsm, [[maybe_unused]] Context& ctx) {
    ctx.sensor0.update([&] {
      ctx.display.formatLine<1>("sensor0 {}", ctx.sensor0.state() == LOW ? "-" : "triggered");
    });
    ctx.sensor1.update([&] {
      ctx.display.formatLine<2>("sensor1 {}", ctx.sensor1.state() == LOW ? "-" : "triggered");
    });

    if (ctx.sensor0.state() == LOW) {
      if (ctx.sensor1.state() == LOW) {
        // looks like not enought liquid
        return fsm.template setNextState<State_Error>();
      }
    }
    if (ctx.sensor1.state() == HIGH) {
      // looks like we done
      return fsm.template setNextState<State_Idle>();
    }

    // TODO check too long in state

    // run pump, run
    ctx.pump(map(10, 0, 100, 0, 255));
  }
};

} // namespace app

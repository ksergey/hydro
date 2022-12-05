// ------------------------------------------------------------
// Copyright 2022-present Sergey Kovalevich <inndie@gmail.com>
// ------------------------------------------------------------

#pragma once

#include "Context.h"

namespace app {

class State_WaitNetwork;

class State_Idle {
private:
  int clicksCount_ = 0;

public:
  State_Idle() = default;

  template<typename FSM>
  void enter([[maybe_unused]] FSM& fsm, Context& ctx) {
    Serial.println("State_Idle::enter");

    clicksCount_ = 0;

    ctx.display.formatLine<0>("state: idle");
  }

  template<typename FSM>
  void leave([[maybe_unused]] FSM& fsm, Context& ctx) {
    Serial.println("State_Idle::leave");

    ctx.display.formatLine<0>("state: ***");
    ctx.display.formatLine<2>("time: N/A");
    ctx.display.formatLine<3>("sensor0 -");
    ctx.display.formatLine<4>("sensor1 -");
    ctx.display.formatLine<5>("button clicks: 0");

    ctx.pump(0);
  }

  template<typename FSM>
  void run([[maybe_unused]] FSM& fsm, Context& ctx) {
    if (WiFi.status() != WL_CONNECTED) {
      return fsm.template setNextState<State_WaitNetwork>();
    }

    ctx.display.formatLine<2>("time: {}", ctx.timeClient.getFormattedTime().c_str());

    ctx.sensor0.update([&] {
      ctx.display.formatLine<3>("sensor0 {}", ctx.sensor0.state() == LOW ? "-" : "triggered");
    });

    ctx.sensor1.update([&] {
      ctx.display.formatLine<4>("sensor1 {}", ctx.sensor1.state() == LOW ? "-" : "triggered");
    });

    ctx.button.update([&](mymk::ButtonState state) {
      if (state == mymk::ButtonState::Press) {
        clicksCount_++;
        ctx.display.formatLine<5>("button clicks: {}", clicksCount_);
      }
    });

    ctx.pump(map(clicksCount_ % 4, 0, 3, 0, 250));
  }
};

} // namespace app

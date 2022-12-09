// ------------------------------------------------------------
// Copyright 2022-present Sergey Kovalevich <inndie@gmail.com>
// ------------------------------------------------------------

#pragma once

#include "Context.h"

namespace app {

class State_Error {
private:
public:
  State_Error() = default;

  template<typename FSM>
  void enter([[maybe_unused]] FSM& fsm, Context& ctx) {
    Serial.println("State_Error::enter");
    ctx.display.clear();
    ctx.display.formatLine<0>("state: ERROR");
  }

  template<typename FSM>
  void leave([[maybe_unused]] FSM& fsm, Context& ctx) {
    Serial.println("State_Error::leave");
  }

  template<typename FSM>
  void run([[maybe_unused]] FSM& fsm, [[maybe_unused]] Context& ctx) {}
};

} // namespace app

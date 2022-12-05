// ------------------------------------------------------------
// Copyright 2022-present Sergey Kovalevich <inndie@gmail.com>
// ------------------------------------------------------------

#pragma once

#include <fmt/format.h> // should be first header, because Arduino.h

#include "Context.h"
#include "FSM.h"
#include "State_Idle.h"
#include "State_Init.h"
#include "State_WaitNetwork.h"

namespace app {

class Hydro {
private:
  using FSM = mymk::FSM<State_Init, State_WaitNetwork, State_Idle>;

  Context context_;
  FSM fsm_;

private:
  Hydro(Hydro const&) = delete;
  Hydro& operator=(Hydro const&) = delete;
  Hydro() = default;

public:
  static Hydro& instance() {
    static Hydro hydro;
    return hydro;
  }

  void setup();
  void loop();

private:
};

} // namespace app

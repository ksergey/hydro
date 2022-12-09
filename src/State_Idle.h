// ------------------------------------------------------------
// Copyright 2022-present Sergey Kovalevich <inndie@gmail.com>
// ------------------------------------------------------------

#pragma once

#include <optional>

#include "Context.h"

namespace app {

class State_WaitNetwork;
class State_Flooding;

class State_Idle {
private:
  static constexpr unsigned long kSecondsInDay = 86400ul;
  static constexpr unsigned long kFloodingInterval = 2 * 3600ul; // 2 hours

  std::optional<unsigned long> floodingTime_;

public:
  State_Idle() = default;

  template<typename FSM>
  void enter([[maybe_unused]] FSM& fsm, Context& ctx) {
    Serial.println("State_Idle::enter");

    floodingTime_ = std::nullopt;

    ctx.display.clear();
    ctx.display.formatLine<0>("state: idle");
    ctx.display.formatLine<1>("time: N/A");
    ctx.display.formatLine<2>("fl: N/A");
  }

  template<typename FSM>
  void leave([[maybe_unused]] FSM& fsm, Context& ctx) {
    Serial.println("State_Idle::leave");
  }

  template<typename FSM>
  void run([[maybe_unused]] FSM& fsm, Context& ctx) {
    if (WiFi.status() != WL_CONNECTED) {
      return fsm.template setNextState<State_WaitNetwork>();
    }

    // update time from ntp
    ctx.timeClient.update();

    if (!floodingTime_) {
      if (ctx.timeClient.isTimeSet()) {
        floodingTime_ = (ctx.timeClient.getEpochTime() / kFloodingInterval + 1) * kFloodingInterval;
        ctx.display.formatLine<2>("fl: {}", formatTimeOfDay(*floodingTime_).c_str());
      }
    }

    if (floodingTime_ <= ctx.timeClient.getEpochTime()) {
      return fsm.template setNextState<State_Flooding>();
    }

    ctx.display.formatLine<1>("time: {}", ctx.timeClient.getFormattedTime().c_str());

    ctx.sensor0.update([&] {
      ctx.display.formatLine<3>("sensor0 {}", ctx.sensor0.state() == LOW ? "-" : "triggered");
    });
    ctx.sensor1.update([&] {
      ctx.display.formatLine<4>("sensor1 {}", ctx.sensor1.state() == LOW ? "-" : "triggered");
    });

    ctx.button.update([&](mymk::ButtonState state) {
      if (state == mymk::ButtonState::Press) {
        return fsm.template setNextState<State_Flooding>();
      }
    });
  }

private:
  static String formatTimeOfDay(unsigned long rawTime) {
    unsigned long hours = (rawTime % 86400L) / 3600;
    String hoursStr = hours < 10 ? "0" + String(hours) : String(hours);

    unsigned long minutes = (rawTime % 3600) / 60;
    String minuteStr = minutes < 10 ? "0" + String(minutes) : String(minutes);

    unsigned long seconds = rawTime % 60;
    String secondStr = seconds < 10 ? "0" + String(seconds) : String(seconds);

    return hoursStr + ":" + minuteStr + ":" + secondStr;
  }
};

} // namespace app

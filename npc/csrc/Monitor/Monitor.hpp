#pragma once

// template <bool ITrace>
class Monitor {
public:
  Monitor()
      : state(State::STOP) {}

  enum class State {
    RUNNING,
    STOP,
    END,
    ABORT,
    QUIT,
  };

  State state;
  int ret; // valid when state == END
};

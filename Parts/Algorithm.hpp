#pragma once

#include "Parts/Screen.hpp"

class Algorithm {
public:
    virtual void init(Screen*) = 0;
    virtual bool step(Screen*) = 0;
    virtual ~Algorithm(){};
    virtual int stepDelay() {
      return 25;
    }
};

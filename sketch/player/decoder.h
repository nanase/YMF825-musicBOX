#pragma once
#include "definitions.h"

class Decoder {
public:
  virtual bool progress() = 0;

  virtual ~Decoder() {
  }
};

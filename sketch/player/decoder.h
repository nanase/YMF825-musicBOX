#pragma once
#include "definitions.h"

class Decoder {
public:
  virtual bool initialize() = 0;
  virtual bool progress()   = 0;

  virtual ~Decoder() {
  }
};

class M25Decoder : public Decoder {
public:
  virtual bool initialize();
  virtual bool progress();
  static bool IsM25File(const char *filename);
};

class JpmDecoder : public Decoder {
public:
  virtual bool initialize();
  virtual bool progress();
  static bool IsJpmFile(const char *filename);
};

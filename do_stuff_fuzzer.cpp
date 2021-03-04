// Copyright 2017 Google Inc. All Rights Reserved.
// Licensed under the Apache License, Version 2.0 (the "License");
#include "my_api.h"

#include <string>
#include <stdlib.h>
#include <pthread.h>

int Global;
void *Thread1(void *x) {
  Global = 42;
  return x;
}

// Simple fuzz target for DoStuff().
// See http://libfuzzer.info for details.
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  std::string str(reinterpret_cast<const char *>(data), size);
  DoStuff(str);  // Disregard the output.
  if (data[0] == 'a') {
    pthread_t t;
    pthread_create(&t, NULL, Thread1, NULL);
    Global = 43;
    pthread_join(t, NULL);
    return 0;
  }
  return 0;
}

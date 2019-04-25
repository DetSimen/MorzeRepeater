#pragma once
#include "Arduino.h"
#include "Types.h"

const bool ACTIVE_LOW = true;
const bool ACTIVE_HIGH = false;

enum class TActiveLevel :bool { High = false, Low = true };

const THandle INVALID_HANDLE PROGMEM = -1;
const int16_t INVALID_INDEX = -1;

const addr_t  NULL_ID = 0;

const char eoln PROGMEM = '\n';
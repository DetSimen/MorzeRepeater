#pragma once
#include "Arduino.h"


#define UInt16Array(...) [](void)->uint16_t *{ static uint16_t _[] = {__VA_ARGS__}; return _; }()
#define IntArray(...) [](void)->int *{ static int _[] = {__VA_ARGS__}; return _; }()



// нестандартные названия стандартных типов данных, 
// для унутреннего удобства пользования (мне) 


using byte = uint8_t;
using sbyte = int8_t;

using byteptr = uint8_t *;
using sbyteptr = int8_t *;

using THandle = int8_t;				// тип аналог Handle Windows только для AVR
using THandle16 = int16_t;

using bsize_t = uint8_t;			// аналог size_t, только длиной в 1 байт

using addr_t = uint16_t;			// тип адрес AVR

using pin_t = uint8_t;				// тип "номер пина", беззнаковый, размером 1 байт 


using void_p	= void *;			// указатель на ничто (нетипизированный указатель)
using pointer	= void *;			// указатель на ничто (нетипизированный указатель)

using char_p	= char *;			// указатель на однобайтовый char
using pchar_t	= char *;


using PErrorFunc = void(*)(int);    // указатель на функцию обрабоччика ошибок, принимает int код ошибки
using pvfCallback  = void(*)(void);      // указатель на функцию без параметров, ничего не возвращающую: void AnyFunc(void) 
using PFuncValueChanged = void(*)(uint16_t);




using  PFlashString = const __FlashStringHelper *;  // указатель на строку в PROGMEM





// 
// 
// 

#include "MorzeTable.h"

uint8_t getMorzeCode(const char ASymbol)
{
	char symbol = isAlpha(ASymbol) ? toUpperCase(ASymbol) : ASymbol;

	uint8_t datasize = sizeof(MorzeArray[0]);       // размер структуры TMorze, в данном случае - 2
	uint8_t length = sizeof(MorzeArray) / datasize; // длина массива всех символов 

	uint16_t address = (uint16_t)(&MorzeArray[0]);	// адрес начала массива в PROGMEM

//сканируем весь массив на совпадение ASymbol с TMorze.EngChar

	for (uint8_t i = 0; i < length; address+=datasize, i++) {  
		
		char ch = pgm_read_byte(address);
		
		if (ch == symbol) return pgm_read_byte(address + 1); // если нашли, возвращаем TMorze.Data
	}

	return 0;  //  не нашли.  
}

uint8_t getSymbolMask(const char ASymbolData)
{
	if (ASymbolData == 0) return 0;

	uint8_t mask = 0x80;  // выставляем старший бит маски в единицу

	// и сдвигая его вправо ищем первый ненулевой бит
	// пропускаем его и возвращаем mask с единицей в первом значащем разряде знака
	for (; mask != 0; mask >>= 1) { 
		if (ASymbolData & mask) return mask >> 1;
	}

	return mask;  // или 0
}

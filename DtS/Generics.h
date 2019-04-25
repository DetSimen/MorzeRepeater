#pragma once
#include "Arduino.h"
#include "Types.h"
#include "Consts.h"



template <typename T> 
Print &operator << (Print &stream, T out)
	{
		stream.print(out);
		return stream;
	}


// обменивает между собой значения переменных a и b.  
// аналог инструкции xchg x86

template <typename T>
void swap(T &a, T &b) {
	T c(a);
	a = b;
	b = c;
}





// отдает true, если значение переменной aValue
// лежит в интервале [aMin..aMax] включая границы

template <typename T>
bool Between(T aValue, T aMin, T aMax) {
	if (aMax < aMin) swap(aMin, aMax);
	return ((aValue >= aMin) && (aValue <= aMax));
}


// 
//  ограничивает значение переменной aValue заданным интервалом [aMinValue..aMaxValue]
// 
template <typename T>
void adjustValue(T &aValue, T aMinValue, T aMaxValue) {
	if (aMinValue > aMaxValue) swap(aMinValue, aMaxValue);
	if (aValue < aMinValue) aValue = aMinValue;
	if (aValue > aMaxValue) aValue = aMaxValue;
}





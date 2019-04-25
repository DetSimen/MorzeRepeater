/*
    Name:       Morze.ino
    Created:	21.04.2019 4:16:26
    Author:     DetSimen
*/

#include "Arduino.h"
#include "MorzeTable.h"
#include "TimerList.h"
#include "Messages.h"


static uint8_t TXPin = 10;  // пин передаччика

extern TTimerList TimerList;	// списой таймеров (до 10) паумолчанью

TMessageList MessageList(12);  // очереть на 12 сапщений

static const int msg_ReadSerial = 0x100;   // сообщение "Читать Сериал"
static const int msg_SendChar = 0x101;		// сообщение "Передать символ (букву)"
static const int msg_SendNextBit = 0x102;	// сообщение "Передать следующий бит в букве"

static const uint32_t DOT_TIME = 75;  // основное время, длительность точки 75 мс, остальные производные
static const uint32_t DASH_TIME = 3 * DOT_TIME; // длительность тире

static const uint16_t REPEAT_TIME = 5000;  // Задержка в мс между повторениями текста, по умолчанию 5 сек. 
static const uint16_t SERIAL_TIMEOUT = 2000; // если в течение 2000 мс не было данных с сериала, значит приняли всю строку

static const uint8_t MAX_STRING_LENGTH = 128; // макс длина передаваемой строки

enum class enumTXState : bool { Pause = false, Bit = true };  

enumTXState TXState = enumTXState::Pause;  // что передаем в данный момент: или бит (точка/тире) или паузу

THandle hTXTimer = INVALID_HANDLE;       // таймер передаччика
THandle hSerialTimer = INVALID_HANDLE;	// таймер таймаута приема из Serial
THandle hRepeatTimer = INVALID_HANDLE;	// таймер повтора фразы

uint8_t TXCurrentMask = 0;   // маска текущего символа
uint8_t TXCurrentCode = 0;   // битовый код Морзе текущего символа
bool	TXBusy = false;		//  идет передача бита или паузы
bool	TXStopped = false;  //  передаччик остановлен нахрен. ничего не передаеца


String StringToTransmit;            // строка, которую будем слать 
uint16_t TransmitCharIndex = 0;		// индекс текущего символа, в этой строке


void TXOnOff(const bool On) {  // передаччик вкл/выкл
	digitalWrite(TXPin, On);
}

void tmrTXTimer(void) {     // здесь кончился таймер передачи бита/паузы
	SendMessage(msg_TimerEnd, hTXTimer);
	TimerList.Stop(hTXTimer); // остановим этот таймер 
}

void tmrSerialTimer(void) {  // строку приняли до конца
	SendMessage(msg_TimerEnd, hSerialTimer);
	TimerList.Stop(hSerialTimer); // таймер больше не нужен
}

void tmrRepeat(void) {     // таймер повтора кончился, начинаем передавать сначала
	TransmitCharIndex = 0;
	TXStopped = false;          
	TimerList.Stop(hRepeatTimer);
}

void sendBit(const bool aBit) {  // передать один бит (точку == false или тире == true)

	TXState = enumTXState::Bit;  // признак: передаем бит

	TXOnOff(true);				// ключ на старт!  

	TXBusy = true;				// передаччик теперя занят

	TimerList.setNewInterval(hTXTimer, aBit ? DASH_TIME : DOT_TIME);  // длительность таймера разная для точки и тире
	TimerList.Reset(hTXTimer);  // перезапустим его сначала интервала

}

void sendPause(const uint8_t kf = 1) {  // передаем паузу длиной в 1 точку по умолчанию
	TXState = enumTXState::Pause;
	
	TXBusy = true; // передаччик занят
	
	TimerList.setNewInterval(hTXTimer, kf*DOT_TIME);  // либо длительность паузы кратна точке в kf раз
	TimerList.Reset(hTXTimer);  // перезапустим таймер сначала
}

; void setup()
{
	Serial.begin(115200);

	delay(1000); // чтоб всё устаканилось к старту

	pinMode(TXPin, OUTPUT);

	TXOnOff(false);	// ключ выключен

	// все таймеры создаюца по умолчанию остановленными
	//
	hTXTimer = TimerList.AddStopped(DOT_TIME, tmrTXTimer);  // таймер передаччика бит 
	hSerialTimer = TimerList.AddStopped(SERIAL_TIMEOUT, tmrSerialTimer);  // таймер таймаута чтения из Serial  
	hRepeatTimer = TimerList.AddStopped(REPEAT_TIME, tmrRepeat);  // таймер повтора передачи строки 

	StringToTransmit.reserve(MAX_STRING_LENGTH); // хапнем сразу место под строку, чтоб не перераспределять потом
	StringToTransmit = "CQ DX UA6EM CQ DX UA6EM QSA? 73! 73! 73! K";  // строка для передачи по умолчанию
	TransmitCharIndex = 0;  
}

void loop()
{
	if (Serial.available()) {        // если в сериал чота припрыгало
		SendMessage(msg_ReadSerial);	// пошлем команду прочесть сериал
		TXStopped = true;				// передачу остановим
		TransmitCharIndex = 0;		// и все индексы и маски обнулим, 
		TXCurrentCode = 0;			// так как после приема будем передавать уже новую строку
		TXCurrentMask = 0;
	}

	if ((not TXBusy) and (not TXStopped)) {  // если передаччик не остановлен и не занят передачей бита

		if (TXCurrentMask > 0)  // если маска еще до конца не сдвинулась
		{
			SendMessage(msg_SendNextBit); // передать следующий бит знака
		}
		else 
		{
			SendMessage(msg_SendChar); // знак кончился, начать передавать след. символ
		}
	}


	if ( not MessageList.Available()) return;   // если сапщений в очереди нет, выходим

	TMessage msg = MessageList.GetMessage();

	switch (msg.Message)
	{

	case msg_TimerEnd: {
		if (msg.LoParam == hSerialTimer) {  // кончился таймер приёма по сериал
			TXStopped = false;		// можно стартовать передаччик, есличо
			break;
		}
		if (msg.LoParam == hTXTimer) { // кончился таймер передачи бита/паузы
			TXBusy = false;			// можно передавать следующий

			if (TXState == enumTXState::Bit) { // если передавали бит, передадим 1 паузу
				TXOnOff(false);					// выключим ключ
				TXCurrentMask >>= 1;
				if (TXCurrentMask>0) sendPause(); else sendPause(3); // а если знак кончился, то 3 паузы
			}

			break;
		}

		break;
	}

	case msg_SendNextBit: {   // передаем след. бит  (точка/тире) 
		sendBit(TXCurrentCode & TXCurrentMask);
		break;
	}

	case msg_ReadSerial: {   // чтение из сериала
		if (!TimerList.isActive(hSerialTimer)) {  // если таймер таймаута остановлен 
			StringToTransmit = "";					// значить читаем новую строку
			TransmitCharIndex = 0;
		}
		StringToTransmit += char(Serial.read()); // берем символ из сериал и вклеиваем в строку
		TimerList.Reset(hSerialTimer);			// перезапускаем таймер таймаута (помнишь? 2 секунды после ПОСЛЕДНЕГО символа) 
		break;
	}

 	case msg_SendChar: {    // послать знак
		if (TransmitCharIndex < StringToTransmit.length()) { // если строка еще не кончиилась
			char c = StringToTransmit[TransmitCharIndex++]; // взять символ 
			if (c != ' ') {  // если не пробел, то
				TXCurrentCode = getMorzeCode(c);				// по нему взять код Морзя  см. MorzeTable.cpp
				TXCurrentMask = getSymbolMask(getMorzeCode(c)); // и маску    см. MorzeTable.cpp
			}
			else sendPause(7);  // пробел между словами - передаем паузу в 7 точек
		}
		else {    // если строка кончилась
			TXStopped = true;			// остановим передачу
			TransmitCharIndex = 0;			// вернем указатель на 0 символ строки
			TimerList.Reset(hRepeatTimer);	// и запустим таймер для REPEAT
		}
		break;
	}
		Serial.print("Unknown message code: 0x"); Serial.println(msg.Message, HEX);
		break;
	}

}

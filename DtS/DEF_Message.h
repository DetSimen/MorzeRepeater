#pragma once

#define DEF_MESSAGE_UNIT

// system messages

#define  msg_Empty				0x00
#define  msg_Error				0x01
#define  msg_CriticalError		0x02
#define  msg_ClassError			0x03
#define  msg_Paint				0x04
#define  msg_UnhandledMessage	0x05
#define  msg_ChangeAppState		0x06		// сменить состояние приложения
#define  msg_AppStateChanged	0x07		// состояние приложения изменено

// user messages

#define msg_HalfSecondTick		0x30	// тик раз в полсекунды
#define msg_SecondTick			0x31	// тик раз в секунду
#define msg_TimeChanged			0x32    // часы/минуты изменились
#define msg_TimerEnd			0x33    // таймер дощитал до 0
#define msg_AlarmOn				0x34
#define msg_TimeSyncro			0x35
#define msg_CountDown			0x36
#define msg_CountEnd			0x37
#define msg_Timeout				0x38    // заданный интервал времени истёк
#define msg_UpdateEEPROM		0x39
#define msg_AlarmOff			0x3A    // выключить будильнег

#define msg_ReadKeyboard		0x40	// опросить клаву
#define msg_KeyDown				0x41
#define msg_Key_Up				0x42
#define msg_KeyPress			0x43
#define msg_LongKey				0x44	// долгое нажатие

#define msg_MenuChoice			0x50    // меню выбрано

#define msg_SensorValueChanged	0x60
#define msg_SensorStateChanged	0x61
#define msg_ReadSensor			0x62
#define msg_BtnStateChange		0x63    // Кнопка поменяла состояние нажалась/отпустилась
#define msg_SensorAlarm			0x64    // значение измеренной сенсором величины вышло за заданные пределы

// encoder 
#define msg_EncoderChanged		0x70
#define msg_EncoderLeft			0x71
#define msg_EncoderRight		0x72
#define msg_EncoderButton		0x73

#define msg_ModuleStateChanged  0x80


#define msg_DeviceStateChanged  	0xA0    // состояние исполнительного устройства изменилось



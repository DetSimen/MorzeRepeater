#include <Types.h>
#include "Messages.h"


extern TMessageList MessageList;


void SendClassMessage(int16_t AMsg, uint16_t AClassID, int16_t ALoParam, int16_t AHiParam) {
	MessageList.Add(AMsg, ALoParam, AHiParam, AClassID);
}


void SendMessage(int16_t AMsg, int16_t ALoParam, int16_t AHiParam) {
	MessageList.Add(AMsg, ALoParam, AHiParam, 0);
}


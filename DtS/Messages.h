#pragma  once
#include "arduino.h"
#include "Types.h"
#include "DEF_Message.h"


extern void SendClassMessage(int16_t AMsg, uint16_t AClassID, int16_t ALoParam = 0, int16_t AHiParam = 0);
extern void SendMessage(int16_t aMsg, int16_t aLoParam = 0, int16_t AHiParam = 0);



struct TMessage;
using  PMessage = TMessage *;

struct TMessage
{
public:
	int16_t   Message;
	int16_t   LoParam;
	int16_t   HiParam;
	uint16_t  ClassID;

	TMessage() :TMessage(msg_Empty) {};
	TMessage(int16_t msg, int16_t loparam = 0, int16_t hiparam = 0, uint16_t aclassid = 0) {
		Message = msg;
		LoParam = loparam;
		HiParam = hiparam;
		ClassID = aclassid;
	}

};

class TMessageList
{
protected:
	PMessage	*Items;

	bsize_t		fsize;
	bsize_t		fcount;
	
	void     DeleteFirst(void){
		uint8_t _sreg = SREG;
		cli();
		for (bsize_t i = 1; i < fsize; i++) Items[i - 1] = Items[i];
		Items[fsize - 1] = NULL;
		if (fcount>0) fcount--;
		SREG = _sreg;
	}

	TMessageList() {};

public:

	TMessageList(bsize_t asize) {
		Items = new PMessage[fsize = asize];

		for (bsize_t i = 0; i < asize; i++) Items[i] = NULL;
		fcount = 0;
	}

#ifdef USE_DESTRUCTOR 

	~TMessageList() {
		while (fcount) GetMessage();
		delete[] Items;
	}

#endif // USE_DESTRUCTOR 

	bool Available() const
	{
		return (fcount > 0);
	}

	TMessage GetMessage()
	{
		TMessage msg;
		PMessage pmsg = Items[0];

		if (pmsg != NULL)
		{
			msg = *pmsg;
			free(pmsg);
			DeleteFirst();
		}
		//	else Serial << "pmsg = NULL\n";
		return msg;
	}

	void Add(const PMessage msg){
		if ((msg != NULL) && (fcount < fsize)) {
			Items[fcount++] = msg;
		}
		else free(msg);
	}

	void Add(int16_t amsg, int16_t alo = 0, int16_t ahigh = 0, uint16_t aclassid = 0) {
		Add(new TMessage(amsg, alo, ahigh, aclassid));
	}

	bsize_t  getCount() const {
		return fcount;
	}

};


#include "StdAfx.h"

initialiseSingleton(WorldSocket);

void WorldSocket::Update()
{
	/*PackToGet *ptg=NULL;
	while ((ptg = _packets.Pop()))
	{
		ASSERT(ptg);
		(ptg->com->*Handlers[ptg->opcode])(ptg->packet);

		delete ptg->packet;
		delete ptg;
	};*/

	PackToSend *pts=NULL;
	while ((pts = _tosend.Pop()))
	{
		ASSERT(pts);

		pts->com->writePacket(pts->packet,pts->dispose);

		pts->packet = NULL;
		delete pts;
	};
}
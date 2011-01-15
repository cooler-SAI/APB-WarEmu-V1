#include "StdAfx.h"

void PQScript::Update(uint32 tdiff)
{
	onUpdate(tdiff);

	if(m_nextevent && m_eventtime)
	{
		if(m_nextevent > tdiff)
			m_nextevent-=tdiff;
		else
		{
			m_nextevent = m_eventtime;
			onEvent();
		}
	}
	
}
void PQScript::ModifyEventTime(uint32 time)
{
	m_eventtime = time;
	m_nextevent = m_eventtime;
}
void PQScript::StopEvent()
{
	m_eventtime=0;
	m_nextevent=0;
}
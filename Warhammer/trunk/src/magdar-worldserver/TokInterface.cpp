#include "StdAfx.h"

void TokInterface::LoadToks()
{
	if(m_toks.size()) return; // Téléportation pas de reload;

	QueryResult * Result = CharacterDatabase.Query("SELECT toktype,tokid,checked FROM characters_tok WHERE character_id=%u",m_player->GetPlayerID());
	if(Result)
	{
		do{

			TokPlayerInfo * Info = new TokPlayerInfo;	
			Info->toktype=Result->Fetch()[0].GetUInt32();
			Info->tokid = Result->Fetch()[1].GetUInt32();
			Info->checked=true;
			m_toks[Info->tokid]=Info;

			sendTok(Info);

		}while(Result->NextRow());
	}
	delete Result;
}

bool TokInterface::HasTok(uint16 entry) { return m_toks[entry] != NULL; };
void TokInterface::AddTok(uint16 entry)
{
	if(!entry) return;

	Log.Debug("Tok","Adding Tok %u to %u",entry,m_player->GetPlayerID());

	if(HasTok(entry)) return;



	TokPlayerInfo * Info = new TokPlayerInfo;
	Info->tokid = entry;
	Info->checked=0;
	Info->toktype=1;
	m_toks[entry]=Info;

	CharacterDatabase.Execute("INSERT INTO characters_tok (character_id,toktype,tokid,checked) VALUE (%u,%u,%u,0)",m_player->GetPlayerID(),Info->toktype,Info->tokid);

	sendTok(Info);

	const TokInfo * Tok = objmgr.GetTokInfo(entry);
	if(Tok)
		if(Tok->exp)
			m_player->GiveXp(Tok->exp);
}

void TokInterface::sendTok(TokPlayerInfo* Info)
{
	if(!Info) return;
	if(!m_player) return;

	Log.Debug("TokInterface","Sending Tok %u",Info->tokid);
	Buffer *b = new Buffer(9+3);
	b->write<uint16>(9);
	b->write<uint8>(F_TOK_ENTRY_UPDATE);
	b->write<uint32>(Info->toktype);
	b->write<uint16>(Info->tokid);
	b->write<uint8>(0x01);
	b->write<uint8>(!Info->checked);
	b->write<uint8>(1);
	m_player->sendPacket(b);
}
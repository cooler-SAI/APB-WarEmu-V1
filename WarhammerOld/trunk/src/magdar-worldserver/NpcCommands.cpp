#include "StdAfx.h"

void ChatHandler::onNpcMod(Player * Plr,string command,	vector<string> m_values)
{
	if( Plr == NULL ) return;
	if(m_values.size() <= 2) return;

	Unit * obj = Plr->GetTarget();
	if(!obj)
	{
		Plr->sendStringMessage(0,"Serveur","Aucune Selection",CHATLOGFILTERS_SHOUT);
		return;
	}
	if(!obj->IsCreature())
	{
		Plr->sendStringMessage(0,"Serveur","La selection n'est pas une creature",CHATLOGFILTERS_SHOUT);
		return;
	}
	Creature * Crea = obj->GetCreature();
	if(Crea == NULL) return;

	string colone = m_values[1];
	string value = m_values[2];
	Log.Debug("ChatHandler","colone=%s ; value=%s",colone.c_str(),value.c_str());
}
void ChatHandler::onNpcInfo(Player * Plr,string command,	vector<string> m_values)
{
	if( Plr == NULL ) return;

	Unit * obj = Plr->GetTarget();
	if(!obj)
	{
		Plr->sendStringMessage(0,"Serveur","Aucune Selection",CHATLOGFILTERS_SHOUT);
		return;
	}
	if(!obj->IsCreature())
	{
		Plr->sendStringMessage(0,"Serveur","La selection n'est pas une creature",CHATLOGFILTERS_SHOUT);
		return;
	}
	Creature * Crea = obj->GetCreature();
	if(Crea == NULL) return;

	stringstream ss;
	ss << " X="<<Crea->GetX()<<" Y="<<Crea->GetY();
	ss << " Entry="<<Crea->GetEntry()<<" Guid="<<Crea->GetGuid()<<" Level="<<uint32(Crea->GetLevel())<<" Model="<<uint32(Crea->GetModel());
	ss << " Ico="<<uint32(Crea->GetCreatureProto()->icone);

	Plr->sendStringMessage(Crea->GetOid(),Crea->GetName().c_str(),ss.str().c_str(),CHATLOGFILTERS_MONSTER_SAY);

}
void ChatHandler::onNpcDelete(Player * Plr,string command,	vector<string> m_values)
{
	if( Plr == NULL ) return;

	Unit * obj = Plr->GetTarget();
	if(!obj)
	{
		Plr->sendStringMessage(0,"Serveur","Aucune Selection",CHATLOGFILTERS_SHOUT);
		return;
	}
	if(!obj->IsCreature())
	{
		Plr->sendStringMessage(0,"Serveur","La selection n'est pas une creature",CHATLOGFILTERS_SHOUT);
		return;
	}
	Creature * Crea = obj->GetCreature();
	if(Crea == NULL) return;

	WorldDatabase.Execute("DELETE FROM creature_spawns WHERE guid=%u",Crea->GetGuid());
	stringstream ss;
	ss << "Npc Spawn["<<Crea->GetGuid()<<"] Deleted From DB";
	Plr->sendStringMessage(0,Plr->GetName(),ss.str());

	Crea->RemoveFromWorld();
}
void ChatHandler::onNpcTurn(Player * Plr,string command,	vector<string> m_values)
{
	if(Plr == NULL) return;

	Unit * target = Plr->GetTarget();
	if(target == NULL) return;
	if(!target->IsCreature()) return;
	Creature * Crea = target->GetCreature();

	uint16 rotation = atoi(m_values[1].c_str());
	if(!rotation) return;

	Crea->SetPosition(Crea->GetX(),Crea->GetY(),Crea->GetZ(),rotation);
}
void ChatHandler::onNpcSpawn(Player * Plr,string command,	vector<string> m_values)
{
	if(Plr == NULL) return;

	if(!Plr->IsInWorld()) return;

	uint32 entry = atoi(m_values[1].c_str());
	uint32 save = atoi(m_values[2].c_str());

	if(!entry) return;

	Creature * Crea = Plr->GetZoneMgr()->SpawnCreature(entry,Plr->GetWorldX(),Plr->GetWorldY(),Plr->GetWorldZ(),Plr->GetWorldO(),true);

	if(save && Crea)
		Crea->Save();

}
void ChatHandler::onNpcQuests(Player * Plr,string command,	vector<string> m_values)
{
	if(Plr == NULL) return;

	if(!Plr->GetTarget() || (Plr->GetTarget() && !Plr->GetTarget()->IsCreature()))
	{
		Plr->sendLocalizedString("",TEXT_INVALID_GIVE_TARGET);
		return;
	}

	Creature * Crea = Plr->GetTarget()->GetCreature();

	if(Crea->HasQuests() && Crea->NumOfQuests())
	{
		for(list<QuestInfo*>::iterator itr=Crea->QuestsBegin();itr!=Crea->QuestsEnd();++itr)
			if((*itr))
				Plr->sendStringMessage(0,Crea->GetName().c_str(),(*itr)->name.c_str());
	}
	else Plr->sendStringMessage(0,"Server","Aucune Quete !");

}
void ChatHandler::onNpcBytes(Player * Plr,string command,	vector<string> m_values)
{
	Log.Debug("ChatHandler","Modify Bytes");
	if( m_values.size() < 2) return;

	Log.Debug("ChatHandler","Modify Bytes 2");
	Creature * Target = NULL;
	Target = Plr->GetTarget() ? (Plr->GetTarget()->IsCreature() ? Plr->GetTarget()->GetCreature() : NULL) : NULL;

	if(Target == NULL) return;

	Log.Debug("ChatHandler","Modify Bytes 3");
	string sub = m_values[0];
	int value = atoi(m_values[1].c_str());

	Log.Debug("ChatHandler","Modify Bytes %s %u",sub.c_str(),value);
	Target->RemoveFromWorld();
	
	if(sub == "add")
		Target->AddByte(uint8(value));
	else if(sub == "remove")
		Target->RemoveByte(uint8(value));
	else if(sub == "clean")
		Target->ClearByte();

	Target->AddToWorld();

	Plr->sendStringMessage(0,"Server","Command Appliqued");
}
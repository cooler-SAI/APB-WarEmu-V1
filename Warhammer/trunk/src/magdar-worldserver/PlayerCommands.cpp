#include "StdAfx.h"

void ChatHandler::onGps(Player * Plr,string command,	vector<string> m_values)
{
	if( Plr == NULL ) return;

	stringstream ss,dd,ee;
	ss << "X:"<<Plr->GetX()<<" Y:"<<Plr->GetY()<<" Z:"<<Plr->GetZ()<< " O:"<<Plr->GetO();
	Plr->sendStringMessage(Plr->GetOid(),Plr->GetName(),ss.str());

	dd << "PX:"<<Plr->CalcPinX()<<" PY:"<<Plr->CalcPinY();
	Plr->sendStringMessage(Plr->GetOid(),Plr->GetName(),dd.str());

	ee << "OffX="<<Plr->GetOffX() << " OffY="+Plr->GetOffY();
	Plr->sendStringMessage(Plr->GetOid(),Plr->GetName(),ee.str());


	if(Plr->GetTarget())
	{
		stringstream zz;
		zz << "Distance de "<<Plr->GetTarget()->GetName().c_str()<< " = " << Plr->GetDistance(Plr->GetTarget());
		Plr->sendStringMessage(Plr->GetOid(),Plr->GetName(),zz.str());
	}
}
void ChatHandler::onSpeed(Player * Plr,string command,	vector<string> m_values)
{
	if( Plr == NULL ) return;
	if(m_values.size() <= 1) return;

	uint16 speed = atoi(m_values[1].c_str());

	if(speed < 50 || speed > 5000)
	{
		Plr->sendStringMessage(Plr->GetOid(),Plr->GetName(),string("Parametre invalide !"));
		return;
	}

	Plr->SetSpeed(speed);
}
void ChatHandler::onMoney(Player * Plr,string command,	vector<string> m_values)
{
	if( Plr == NULL ) return;
	if(m_values.size() <= 1) return;

	uint32 money = atoi(m_values[1].c_str());

	Plr->SetWealth(money);
}
void ChatHandler::onItem(Player * Plr,string command,	vector<string> m_values)
{
	if(Plr == NULL) return;

	uint32 entry= atoi(m_values[1].c_str());
	uint32 count=1;

	const ItemProto * Proto = objmgr.GetItemProto(entry);

	if(!Proto)
	{
		Plr->sendStringMessage(0,Plr->GetName(),"Entry invalide !");
		return;
	}

	Player * Target=NULL;
	if(Plr->GetTarget() && Plr->GetTarget()->IsPlayer())
		Target= Plr->GetTarget()->GetPlayer();
	else Target = Plr;

	if(!Target->GetItemInterface()) return;

	Target->GetItemInterface()->CreateItem(entry,count);
	stringstream ss;
	ss << "Vous ajoutez l'item [" << Proto->name << "] a ["<<Target->GetName().c_str()<<"].";
	Plr->sendStringMessage(0,Plr->GetName(),ss.str(),CHATLOGFILTERS_SHOUT);
}
void ChatHandler::onKill(Player * Plr,string command,	vector<string> m_values)
{
	if(Plr == NULL) return;

	Unit * target = Plr->GetTarget();
	if(target == NULL) return;
	if(target->IsPlayer()) return;

	Unit * unit = target->GetUnit();

	Plr->DealDamage(unit,unit->GetHealth());
}
void ChatHandler::onLevel(Player * Plr,string command,	vector<string> m_values)
{
	if(Plr == NULL) return;

	uint8 level = atoi(m_values[1].c_str());

	if(!level) return;

	Log.Debug("ChatHandler","Modify level %u",level);

	Player * Target = Plr->GetTarget() ? (Plr->GetTarget()->IsPlayer() ? Plr->GetTarget()->GetPlayer() : NULL) : NULL;
	if(Target == NULL) Target = Plr;

	LevelInfo * info = objmgr.GetLevelInfo(Target->GetCareerLine(),level);
	if(!info) return;
	Target->ApplyLevelInfo(info);
	Target->sendXP();
}
void ChatHandler::onGiveXp(Player * Plr,string command,	vector<string> m_values)
{
	if(Plr == NULL) return;

	uint32 xp = atoi(m_values[0].c_str());

	Player * target = NULL;
	if(Plr->GetTarget() && Plr->GetTarget()->IsPlayer())
		target = Plr->GetTarget()->GetPlayer();
	else target = Plr;

	target->GiveXp(xp);
}
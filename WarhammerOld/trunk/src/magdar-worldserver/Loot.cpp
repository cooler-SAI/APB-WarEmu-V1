#include "StdAfx.h"


bool Loot::GenerateLoot()
{
	if(!m_unit) return false;

	if(m_unit->IsCreature())
	{
		Creature * Crea = m_unit->GetCreature();

		CreatureLoot * Cloot =  objmgr.GetCreatureLoot(Crea->GetEntry());
		if(!Cloot) return false;

		M_loot.Acquire();

		vector<const ItemProto*> m_items = Cloot->GenerateLoot();
		if(m_items.size())
		{
			for(vector<const ItemProto*>::iterator itr=m_items.begin();itr!=m_items.end();++itr)
			{
				if(!(*itr)) continue;

				LootInfo * Info = new LootInfo;
				Info->num = m_loot.size();
				Info->entry = (*itr)->entry;
				Info->Looter = NULL;
				Info->Proto = (*itr);

				m_loot.push_back(Info);
			}
		}

		M_loot.Release();

		m_money = (Crea->GetLevel()*7)+(Crea->m_Ranklevel*30);
		m_money += (Crea->m_Ranklevel*20);

		uint32 tmp = RandomUInt(m_money+1);

		if(tmp >= m_money)	// On supprime un random d'argent
			m_money = 0;
		else						
			m_money -= tmp;

		m_money*=objmgr.gold_rate; // On multipli le reste par le gold rate

		if(m_money || m_loot.size()) // Si il y a de l'argent ou des items on return true
			return true;
	}
	return false;
}
LootInfo * Loot::GetItemLoot(uint8 num)
{
	if(num >= m_loot.size())
		return NULL;

	LootInfo * Info = NULL;

	M_loot.Acquire();
	Info = m_loot[num];
	M_loot.Release();

	return Info;
}
void Loot::RemoveItemLoot(uint8 num)
{
	if(num >= m_loot.size())
		return;

	M_loot.Acquire();
	m_loot[num] = NULL;
	M_loot.Release();
}
uint8 Loot::GetLootCount()
{
	uint8 counts=0;

	M_loot.Acquire();
	for(int size = 0;size < m_loot.size(); ++size)
	{
		if(m_loot[size] != NULL)
			++counts;
	}
	M_loot.Release();

	return counts;
}
bool Loot::IsValid()
{
	if(GetLootCount() || m_money)
		return true;
	else return false;
}
void Loot::AddPlayerToLoot(uint32 entry)
{
	if(!m_unit) return;
	if(!m_unit->IsInWorld()) return;

	M_looters.Acquire();

	if(m_looters[entry] == NULL)
		m_looters[entry] = m_unit->GetZoneMgr()->GetPlayer(entry);

	M_looters.Release();
}
void Loot::RemovePlayerToLoot(uint32 entry)
{
	M_looters.Acquire();
	m_looters.erase(entry);
	M_looters.Release();
}
bool Loot::HasPlayerToLoot(uint32 entry)
{
	bool ok = false;

	M_looters.Acquire();
	ok = (m_looters[entry] != NULL);
	M_looters.Release();

	return ok;
}
void Loot::HandleInteract(Player * Plr,Struct_f_interract &interract)
{
	if(!Plr) return;

	Log.Debug("loot","HandleInteract");

	if(m_looter && m_looter != Plr)
		return; // Le joueur ne peut pas loot car ce n'est pas a lui

	if(m_group && Plr->GetGroup() != m_group)
		return; // Le groupe pouvant looter n'est pas celui du joueur

	Log.Debug("loot","HandleInteract menu = %u",interract.menu);

	AddPlayerToLoot(Plr->GetPlayerID());

	switch(interract.menu)
	{
	case 15: // Fermeture du loot
		RemovePlayerToLoot(Plr->GetPlayerID());
		return;

		break;
	case 13: // Récupération de tous les items
		{
			bool error=false;
			
			M_loot.Acquire();

			LootInfo * Info = NULL;
			for(int size = 0;size < m_loot.size(); ++size)
			{
				Info = m_loot[size];
				if(!Info) continue;

				if(Info->Looter == NULL || Info->Looter == Plr)
				{
					if(Plr->GetItemInterface()->CreateItem(Info->Proto))
					{
						m_loot[size] = NULL;
						delete Info;
					} else error = true;
				}
			}

			if(error)
				Plr->sendLocalizedString("",TEXT_OVERAGE_CANT_LOOT);

			M_loot.Release();
			RefreshLoot();
		}break;

	case 12: // Récupération d'un item
		{
			Log.Debug("loot","HandleInteract menu = 12 , num = %u", interract.itemnum );
			M_loot.Acquire();
			LootInfo * Info = NULL;
			if(interract.itemnum < m_loot.size())
			{
				Info = m_loot[interract.itemnum];
				if(Info)
				{
					if(!Plr->GetItemInterface()->CreateItem(Info->Proto))
						Plr->sendLocalizedString("",TEXT_OVERAGE_CANT_LOOT);
					else
					{
						m_loot[interract.itemnum] = NULL;
						delete Info;

						RefreshLoot();
					}
				}
			}
			M_loot.Release();
		}break;
	default:
		{
			Log.Debug("loot","HandleInteract default");
		M_loot.Acquire();

		if(m_money)
		{
			if(Plr->GetGroup())
			{
				uint32 members = 0;

				for(GroupMembersSet::iterator itr = Plr->GetGroup()->GetGroupBegin();itr!=Plr->GetGroup()->GetGroupEnd();++itr)
				{
					if((*itr) && (*itr)->GetZoneMgr() == Plr->GetZoneMgr())
						++members;
				}

				if(members)
				{
					m_money/=members;

					for(GroupMembersSet::iterator itr = Plr->GetGroup()->GetGroupBegin();itr!=Plr->GetGroup()->GetGroupEnd();++itr)
					{
						if((*itr) && (*itr)->GetZoneMgr() == Plr->GetZoneMgr())
							(*itr)->AddWealth(m_money);
					}
				}
			}
			else
				Plr->AddWealth(m_money);

			m_money = 0;
		}
		SendLoot(Plr);
		M_loot.Release();
		}break;
	};
}
void Loot::RefreshLoot()
{
	M_looters.Acquire();
	for(tr1::unordered_map<uint32,Player*>::iterator itr=m_looters.begin();itr!=m_looters.end();++itr)
	{
		if(itr->second && itr->second->IsInWorld())
			SendLoot(itr->second);
	}
	M_looters.Release();
}
void Loot::SendLoot(Player * Plr)
{
	if(!Plr) return;

	M_loot.Acquire();
	M_looters.Acquire();

	Buffer *b = new Buffer(0);

	uint8 counts = 0;
	LootInfo * Info = NULL;
	for(uint8 size = 0; size < m_loot.size();++size)
	{
		Info = m_loot[size];
		if(!Info) continue;
		if(!Info->Proto) continue;


		b->write<uint8>(size);
		sChat.BuildItemInfo(b,Info->Proto);
		++counts;
	}

	uint16 responsize = b->size()+2;

	Buffer *d = new Buffer(responsize+3);
	d->write<uint16>(responsize);
	d->write<uint8>(F_INTERACT_RESPONSE);
	d->write<uint8>(4);
	d->write<uint8>(counts);
	d->write(b,true);
	d->Trace();
	Plr->sendPacket(d);

	M_looters.Release();
	M_loot.Release();
}
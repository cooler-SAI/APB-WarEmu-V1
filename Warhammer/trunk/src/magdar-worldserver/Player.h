#ifndef PLAYER_H
#define PLAYER_H

class ClientCom;
class ItemsInterface;
class Group;
class Item;
class Player;
class InfluenceInterface;
class QuestInterface;
class AbilityInterface;
class Ability;
class TokInterface;
class MailInterface;


struct GroupInvitation;
struct ZoneInfo;
struct LevelInfo;
struct InfluenceInfo;
struct ItemInfluence;

#define	OWNER 0
#define INVITED 1
#define MAXTRADESLOT 9
#define MAX_BUY_BACK_SIZE 20

struct TradeInfo
{
	Mutex m_lock;
	void Lock() { m_lock.Acquire(); };
	void UnLock() { m_lock.Release(); };

	Player * Owner;
	Player * Invited;

	Item * m_itemstrade[2][9];
	uint32 m_money[2];
	uint8 m_accept[2];
	uint8 m_update[2];

	bool oktotrade[2];

	bool changed;
	bool close;

	TradeInfo(Player * owner,Player * invited);
	void SetAccept(Player * Plr,uint8 accept);
	void SetMoney(Player * Plr,uint32 money);
	bool HasItemFromSlot(Player * Plr,uint16 playerslot);
	void AddItemToSlot(Player * Plr,Item * itm,uint16 tradeslot);
	void RemoveItemFromSlot(Player * Plr,uint16 tradeslot);
	void TradeItems();

	void CheckDistance();
	static void SendDistanceMessage(Player * Plr);
	static void SendTradeClose(Player * Plr,uint16 oid);
	static void SendAlreadyTrade(Player * Plr);
	
	void Update();

	void SendTradeInfo(Player * Plr);
	void SendItemInfo();

	uint32 GetPlayerSlot(Player * Plr)
	{
		uint32 slot=2;

		if(Plr == Owner)		slot=OWNER;
		else if(Plr == Invited)	slot=INVITED;

		return slot;
	};

	uint8 GetPlayerStatus(Player *Plr)
	{
		uint32 slot=GetPlayerSlot(Plr);
		if(slot>=2) return 3;

		return m_accept[slot];
	}
	void SetUpdate(Player * Plr,uint8 update)
	{
		uint32 slot=GetPlayerSlot(Plr);
		if(slot>=2) return;

		m_update[slot]=update;
	}

	void Destroy();
};

class SERVER_DECL Player: public Unit
{
public:
	Player(ClientCom * Com);
	~Player();
	void Update( uint32 time );
	bool Load(uint32 character_id);
	void Save();

	void SetWealth(uint32 Money)	{ m_wealth = Money; sendWealth(); sEventHook.ExecuteHook(EVENTS_PLAYER_MONEY_UPDATED,1,this); }
	bool RemoveWealth(uint32 count) 
	{ 
		if(m_wealth < count) return false;
		else SetWealth(m_wealth-count);
		return true; 
	}
	bool AddWealth(uint32 count)	{ SetWealth(m_wealth+count); return true; }

	/************************************************************************/
	/* CHARACTER															*/
	/************************************************************************/
	uint16	GetCareerLine() { return m_careerline; }
	uint16	GetCareer()		{ return m_career; }
	uint8	GetRace()		{ return m_race; }
	uint32	GetWealth()		{ return m_wealth; }
	uint32	GetPlayedTime() { return (uint32(UNIXTIME)-m_start_time)+m_playedTime; }
	uint32	GetPlayerID()	{ return m_character_id; }
	uint8	GetRealm()		{ return m_realm; }
	uint32	GetAccountID()	{ return m_account; };
	ClientCom * GetCom()	{ return m_Com; }
	ItemsInterface * GetItemInterface() { return m_iteminterface; }

	void ApplyLevelInfo(LevelInfo * info);
	void sendLevel(bool up=false);
	void ApplyStatBonus();


	void SetSpeed(uint16 Speed) { m_speed = Speed; sendSpeed(); }
	bool CanSee(Object * obj) { return true; }
	__forceinline bool IsVisible(Object * pObj) { return !(m_visibleObjects.find(pObj) == m_visibleObjects.end()); }
	// Visible objects
	unordered_set<Object* > m_visibleObjects;
	__forceinline void AddVisibleObject(Object * pObj) { m_visibleObjects.insert(pObj); }
	__forceinline void RemoveVisibleObject(Object * pObj) { m_visibleObjects.erase(pObj); }
	__forceinline void RemoveVisibleObject(InRangeSet::iterator itr) { m_visibleObjects.erase(itr); }
	__forceinline InRangeSet::iterator FindVisible(Object * obj) { return m_visibleObjects.find(obj); }
	__forceinline void RemoveIfVisible(Object * obj)
	{
		InRangeSet::iterator itr = m_visibleObjects.find(obj);
		if(itr == m_visibleObjects.end())
			return;

		m_visibleObjects.erase(obj);
		//PushOutOfRange(obj->GetNewGUID());
	}

	__forceinline bool GetVisibility(Object * obj, InRangeSet::iterator *itr)
	{
		*itr = m_visibleObjects.find(obj);
		return ((*itr) != m_visibleObjects.end());
	}

	__forceinline InRangeSet::iterator GetVisibleSetBegin() { return m_visibleObjects.begin(); }
	__forceinline InRangeSet::iterator GetVisibleSetEnd() { return m_visibleObjects.end(); }

	/************************************************************************/
	/* SKILL																*/
	/************************************************************************/
	void RemoveSkill(uint32 SkillType) { if(HasSkill(SkillType)) m_skills-=SkillType; }
	void AddSkill(uint32 SkillType) { if(!HasSkill(SkillType)) m_skills+=SkillType; }
	bool HasSkill(uint32 SkillType) { return (m_skills &= SkillType) == SkillType; }
	uint32 GetSkills() { return m_skills;};

	/************************************************************************/
	/* SENDER																*/
	/************************************************************************/
	void sendLoad();
	void sendLeave();
	void sendWealth();
    void sendStats();
    void sendSpeed();
	void sendPlayerInitted();
    void sendQuestList();
    void sendRenown();
    void sendAbilities();
    void sendCharacterPrefs();
    void sendTOK();
    void sendRVRStats();

    //void sendInfluenceInfo(InfluenceData * Data);
	//void sendInfluenceInfo(uint32 chapterid);
	//void sendInfluenceDetail(InfluenceData * Data);

    void sendCounterInfo();
    void sendPlayedTime();
    void sendHealth();
    void sendPlayerLoaded();
    void sendTactics();
    void sendMoraleList();
    void loadEffects();
	void sendItems();
    void sendGFXMod();
    void sendPlayerInitComplete();
	void onRequestWorldLarge();
	void sendCareerCategory();
	void sendCareerPackage();


	/************************************************************************/
	/* PACKET																*/
	/************************************************************************/
	Buffer* Allocate(size_t size);	// Alloue le Buffer dans le bufferpool
	void DisposeBuffer(Buffer *b);	// Supprime le buffer dans le bufferpool
	void sendPacket(Buffer * b,bool del=true);	// Envoi du packet , del pour supprimé dans le buffer pool
	void dispachPacket(Buffer *b);	// Copie le Buffer et l'envoi au joueurs
	void delayedPacket(Buffer *b);	// Ajoute le packet dans une liste d'envoi (packet non prioritaire , F_CREATE_PLAYER,F_PLAYER_INVENTORY,...)

	/************************************************************************/
	/* INTERRACT															*/
	/************************************************************************/
	void SendEquiped(Player * Plr);
	void SendMeTo(Player * Plr);
	void SendInterractTo(Player * Plr,Struct_f_interract &interract);

	/************************************************************************/
	/* CHAT																	*/
	/************************************************************************/
	void sendStringMessage(uint16 oid,string name,string text,ChatLogFilters type=CHATLOGFILTERS_SAY);
	void sendWipeMessage(Player * Receiver,string text);
	void sendGuildMessage(Player * Sender,string text);
	void sendLocalizedString(string text,uint16 type,bool deux=false);
	void sendShowDialog(string text,uint16 type);
	void sendShowDialog(uint16 value,uint16 type);

	/************************************************************************/
	/* MOVEMENT                                                             */
	/************************************************************************/
	void onMovement(uint16 X,uint16 Y,uint16 Z,uint16 O);		//Losque le joueur se déplace
	void onPlayerMovementOffset(uint16 offsetX,uint16 offsetY);	//Lorsque le joueur change d'offset 
	void ConvertNormalPos();	// Converti les position joueurs en World position
	bool m_acceptmonster;		// True lorsque le joueur est bien en jeu (premier mouvement)

	/************************************************************************/
	/* GROUPE																*/
	/************************************************************************/
	Group * m_Group;
	GroupInvitation * Invitation;
	Group * GetGroup() { return m_Group; }
	void SetGroup(Group * group);


	uint8 GetPermission();

	/************************************************************************/
	/* TRADE																*/
	/************************************************************************/
	TradeInfo * Trade;

	/************************************************************************/
	/* COMBAT																*/
	/************************************************************************/
	void EventAttackStart();
	void EventAttackStop();
	void _EventAttack( bool offhand );
	void setAttackTimer(int32 time, bool offhand);
	uint32 m_AttackMsgTimer;	// "too far away" and "wrong facing" timer
	uint32 m_attackTimer;
	uint32 m_attackTimer_1;
	bool m_duelWield;
	bool m_attacking;

	uint16 CalculDamage();
	uint16 CalculSpeed();

	Unit * GetTarget() { return m_Target; }


	/************************************************************************/
	/* Xp                                                           */
	/************************************************************************/
	void GiveXp(uint32 value);						// Ajoute de l'xp et vérifi si lvl up
	void SetXp(uint32 xp) { m_xp=xp;sendXP(); };	// Applique l'xp , aucune vérification
    void sendXP();									// Envoi l'xp au joueur

	/************************************************************************/
	/* SOCIAL                                                               */
	/************************************************************************/
	Mutex M_social;
	set<uint32> m_friend;
	set<uint32> m_ignores;
	set<uint32> m_hasfriends;

	// Envoi la liste des amis
	void Social_SendFriendList();

	// Ajoute/Suppr un ami
	void Social_AddFriend(string name);
	void Social_RemoveFriend(string name);

	// Ajoute/suppr un ignore
	void Social_AddIgnore(string name);
	void Social_RemoveIgnore(string name);

	// Quelqu'un m'ajoute/me suppr de ces amis
	void Social_RemoveHasFriend(Player * Plr);
	void Social_AddHasFriend(Player * Plr);

	// envoi un message aux gens qui sont mes amis que je suis co/deco
	void Social_SetOnline();
	void Social_SetOffline();
	void Social_SendFriends();
	void SaveSocial();


	/************************************************************************/
	/* INTERRACT                                                            */
	/************************************************************************/
	Creature * m_interract;

	Mutex M_buyback;
	list<Item*> m_buyback;
	void SendBuyBack();
	void AddBuyBack(Item * itm);
	void RemoveBuyBack(Item * itm);


	MailInterface * m_MailInterface;
	MailInterface * GetMailInterface() { return m_MailInterface; }

	/************************************************************************/
	/* CONNECTION                                                           */
	/************************************************************************/

	int m_decostage;
	bool m_leaved;
	bool m_disconecting;
	int GetDecotime() { return m_decostage; };
	void SetDecotime(uint32 v) { m_decostage-=v; };
	bool IsDisconnecting() { return m_disconecting; };
	void SetDisctonnecting(bool v) { m_disconecting=v; };


	/************************************************************************/
	/* TELEPORT																*/
	/************************************************************************/
	bool m_teleporting;
	void SafeTeleport(ZoneInfo *Info);
	void SafeTeleport(uint32 x,uint32 y,uint16 z,uint16 o);
	void SafeTeleportToPlayer(Player * Plr) { if(Plr == NULL) return; SafeTeleport(Plr->GetZoneId(),Plr->GetRegion(),Plr->GetWorldX(),Plr->GetWorldY(),Plr->GetWorldZ(),Plr->GetWorldO()); };
	void SafeTeleport(uint16 zone,uint8 region,uint32 x,uint32 y,uint16 z,uint16 o);
	void sendPacketSwitchRegion();

	// liste des zone découvertes
	FastMutex M_zoneacces;
	map<uint16,ZoneInfo*> m_zoneacces;

	void SaveAccesZone(uint16 zone);
	void CheckZoneAcces(Creature * Crea);
	void BuildZoneAcces(Buffer *b);
	uint32 GetZoneAccesCount() 
	{ 
		uint32 counts=0;
		M_zoneacces.Acquire(); 
		counts+=m_zoneacces.size();
		M_zoneacces.Release(); 
		return counts;
	};

	// Résurection du joueurs
	void RezPlayer();

	/************************************************************************/
	/* QUEST																*/
	/************************************************************************/
	QuestInterface * m_QuestInferface;
	QuestInterface * GetQuestInterface() { return m_QuestInferface; };

	bool m_firstmovement;
	bool m_loading;

	InfluenceInterface * m_InfluenceInterface;
	TokInterface * m_TokInterface;


	/************************************************************************/
	/* ABILITY																*/
	/************************************************************************/
	AbilityInterface * GetAbilityInterface() { return m_AbilityInterface; };

private:

	uint32	m_character_id;	// Character id du personnage
	uint32	m_account;		// Account id du personnage
	uint8	m_slot;			// Slot du personnage 0-9
	uint8	m_career;		// Carière du personnage
	uint8	m_careerline;	// Carière line du personnage
	uint8	m_realm;		// Realm : 1 Ordre , 2 Destruction
	uint32	m_heldleft;		//
	uint8	m_race;			// Race du personnage
	uint32	m_last_time_played;	// Dernier temps de jeux
	uint32	m_wealth;		// Argent du personnage
	uint32	m_xp;			// Xp actuel du personnage
	uint32	m_restxp;		//
	uint32	m_renown;		// Renow  du personnage
	uint8	m_renownrank;	// Renow Rank du personnage
	uint16	m_rallyPoint;	// Point de ralliement pour téléportation
	uint32	m_playedTime;	// Temps de jeu actuel
	uint32	m_skills;		// Skill flags
	uint32	m_startPos_x;	// Position de départ
	uint32	m_startPos_y;	// Position de départ
	ClientCom *		m_Com;	// Interface de connexion
	ItemsInterface * m_iteminterface;	// Item interface, inventaire , équipement
	uint32 m_start_time;	// Heure de connexion

	uint32 m_nextsave;		// UNIXTIME de la prochaine sauvegarde du personnage
	uint32 m_nextinsertcheck; // UNIXTIME de la prochaine vérification d'insertion d'item

	AbilityInterface * m_AbilityInterface;
};

#endif
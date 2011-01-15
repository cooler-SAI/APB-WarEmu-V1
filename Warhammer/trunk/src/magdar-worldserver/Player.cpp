#include "StdAfx.h"

using namespace SystemData;

Player::Player(ClientCom *Com) : Unit()
{
	m_type = TYPEID_PLAYER; 
	m_Com = Com;
	m_decostage=20000;
	m_disconecting=false;
	m_leaved=false; 
	m_Target=NULL;
	m_TargetType = GameData::TARGETTYPES_TARGET_NONE;

	m_firstmovement=true;
	m_character_id=0;
	m_account=0;
	m_slot=0;
	m_career=0;
	m_careerline=0;
	m_realm=0;
	m_heldleft=0;
	m_race=0;
	m_last_time_played=0;
	m_wealth=0;
	m_xp=0;
	m_restxp=0;
	m_renown=0;
	m_renownrank=0;
	m_rallyPoint=0;
	m_playedTime=0;
	m_actionpoints=0;
	m_maxactionpoints=0;
	m_skills=0;

	m_startPos_x=0;
	m_startPos_y=0;
	m_iteminterface=NULL;
	m_start_time=0;
	m_Group=NULL;
	Invitation=NULL;
	m_nextsave=0;
	m_nextinsertcheck = 0;
	m_bonushealth=0;
	Trade=NULL;

	if(objmgr.m_savetime)
		m_nextsave=UNIXTIME+objmgr.m_savetime;

	m_interract=NULL;
}
Player::~Player()
{
	m_Com = NULL;
	if(IsInWorld())
		RemoveFromWorld();

	if(m_interract)
		m_interract = NULL;
}
Buffer* Player::Allocate(size_t size) 
{ 
	return m_Com->m_bufferPool->allocateBuffer(size); 
}
void Player::DisposeBuffer(Buffer *b) 
{ 
	m_Com->m_bufferPool->disposeBuffer(b); 
}

bool Player::Load(uint32 character_id)
{
	PlayerInfo * Info = objmgr.GetPlayerInfo(character_id,true);
	m_loading=true;
	if(!Info) 
	{
		Log.Error("Player","Can not load Player [%u]",character_id);
		return false;
	}
	m_character_id = Info->character_id;
	m_name = Info->name;
	//m_subname = "The Forces of Destruction";

	/////////////////////////
	// POSITION SYSTEM
	/////////////////////////

	m_firstmovement=true;
	m_teleporting=false;
	m_acceptmonster=false;

	m_zone = Info->zone;
	m_region = Info->region;
	m_worldposition = LocationVector(Info->Position.x,Info->Position.y,Info->Position.z,Info->Position.o);
	m_startPos_x =	uint32(m_worldposition.x);
	m_startPos_y =	uint32(m_worldposition.y);
	m_OffSetX=		mapmgr.CalculOffset(m_startPos_x);
	m_OffSetY=		mapmgr.CalculOffset(m_startPos_y);
	m_zone_x =		m_OffSetX << 12;
	m_zone_y =		m_OffSetY << 12;


	/////////////////////////
	// STATS SYSTEM
	/////////////////////////
	m_speed =		Info->speed;
	m_level =		Info->level;

	for(uint32 i=0;i<8;++i)
		m_trait[i] = Info->traits[i];

	m_model =		Info->model;
	m_scale =		100;
	m_sex =			Info->sex;

	m_account =		Info->account;
	m_slot =		Info->slot;
	m_career =		Info->career;
	m_careerline =	Info->careerline;
	m_realm =		Info->realm;
	m_heldleft =	Info->heldleft;
	m_race =		Info->race;

	m_last_time_played = Info->last_time_played;

	m_wealth =		Info->wealth;
	m_speed =		Info->speed;
	m_xp =			Info->xp;
	m_restxp =		Info->restxp;
	m_renown =		Info->renown;
	m_renownrank =	Info->renownrank;
	m_rallyPoint =	Info->rallyPoint;
	m_playedTime =	Info->playedTime;
	m_actionpoints = Info->actionpoints;
	m_maxactionpoints = Info->maxactionpoints;

	if(m_actionpoints > m_maxactionpoints)
		m_actionpoints = m_maxactionpoints;

	M_social.Acquire(); //<<----
	m_friend = Info->friends;
	m_hasfriends = Info->hasfriends;
	M_social.Release(); //---->>

	LevelInfo * LvLInfo = objmgr.GetLevelInfo(m_careerline,m_level);
	if(LvLInfo)
		ApplyLevelInfo(LvLInfo);
	else
	{
		for(uint8 i=1;i<GameData::STATS_COUNT;++i)
		{
			if( Info->m_stats[i] )	SetBaseStat(i,Info->m_stats[i]);
			else					SetBaseStat(i,0);
		}
	}

	m_skills = Info->m_skills;

	M_zoneacces.Acquire();	//<<----

	for(vector<ZoneInfo*>::iterator itr=Info->m_zonesacces.begin();itr!=Info->m_zonesacces.end();++itr)
		m_zoneacces[(*itr)->zoneid] = (*itr);

	M_zoneacces.Release();	//---->>

	m_iteminterface = new ItemsInterface(this);
	m_iteminterface->Load();

	m_start_time = uint32(UNIXTIME);
	ApplyStatBonus();
	m_health = m_maxhealth;
	m_QuestInferface = new QuestInterface();
	m_QuestInferface->Init(this);

	m_AbilityInterface = new AbilityInterface();
	m_AbilityInterface->Init(this);

	m_InfluenceInterface = new InfluenceInterface(this);
	m_TokInterface = new TokInterface(this);

	m_MailInterface = new MailInterface(this);
	m_MailInterface->Load();
	
	m_loading=false;
	return true;
}
void Player::sendLoad()
{
	m_firstmovement=true;
	m_acceptmonster=false;
	m_attackTimer_1=0;
	m_attackTimer=0;
	m_duelWield=false;

	sendWealth();
    sendStats();
    sendSpeed();

	AddToWorld(true);

    sendPlayerInitted();

	//sendCareerCategory();
	//sendCareerPackage();

	m_Com->sendKeepStatus();
	m_Com->sendCampaignStatus();
	m_Com->sendGuildData();
	m_Com->sendChannels();
	m_Com->sendObjectiveStatus();
	m_Com->sendCityCapture();
	m_MailInterface->sendMailCounts();
	m_Com->sendExperienceTable();

    ///\todo here I miss all F_CAREER_CATEGORY and F_CAREER_PACKAGE_INFO packets (constDB?)
    sendXP();
    sendRenown();
    sendAbilities();
    sendCharacterPrefs();
	m_InfluenceInterface->LoadInfluences();
    m_TokInterface->LoadToks();
    sendLevel();
    sendRVRStats();

    sendCounterInfo();
    sendPlayedTime();

    sendHealth();

	sendTactics();
	sendMoraleList();
	loadEffects();
	sendGFXMod();

	GetItemInterface()->SendAllItems();

	sendQuestList();
    sendPlayerInitComplete();

	Log.Success("Player","Loading Complete : [%u]-[%u][%s]",m_oid,m_character_id,GetName().c_str());

	CharacterDatabase.Execute("UPDATE characters SET online=1 WHERE character_id=%u",m_character_id);
}
void Player::sendWealth()
{
    //humpf. do not find it anymore in the short 1.2 dump I have...
    uint16 responseSize = opcodeSize[F_PLAYER_WEALTH][SC];
    Buffer* b = Allocate(responseSize+3);
    b->write<uint16>(responseSize);
    b->write<uint8>(F_PLAYER_WEALTH);
    b->write<uint32>(0);    //Tried other values without visible effects
    b->write<uint32>(m_wealth);
    sendPacket(b);
}

void Player::sendStats()
{

    uint16 responseSize = 7+GameData::STATS_COUNT*3;
    Buffer* b = Allocate(responseSize+3);
    b->write<uint16>(responseSize);
    b->write<uint8>(F_PLAYER_STATS);

    b->write<uint8>(GameData::STATS_COUNT);
	b->write<uint8>(m_iteminterface->GetAttackSpeed()); //attack speed //\todo fix attack speed
	b->write<uint16>(m_iteminterface->GetTotalDamage());  //DPS //\todo fix DPS
	b->write<uint16>(m_iteminterface->GetTotalArmor()); //Armor
    b->write<uint8>(0);
    for (int j=1; j<=GameData::STATS_COUNT;j++)
    {
        b->write<uint8>(j);
        b->write<uint16>(m_basestats[j]);
    }
    sendPacket(b);
}

void Player::sendSpeed()
{
    uint16 responseSize = opcodeSize[F_MAX_VELOCITY][SC];
    Buffer* b = Allocate(responseSize+3);
    b->write<uint16>(responseSize);
    b->write<uint8>(F_MAX_VELOCITY);
    b->write<uint16>(m_speed);
    b->write<uint8>(1); // ability to rotate
    b->write<uint8>(100); // Do not know
    sendPacket(b);
}

void Player::sendPlayerInitted()
{
	uint16 responseSize = 44+objmgr.m_realm->Name.length();
    Buffer* b = Allocate(responseSize+3);
    b->write<uint16>(responseSize);
    b->write<uint8>(S_PLAYER_INITTED);
    b->write<uint16>(m_oid);
    b->write<uint16>(0); //??
    b->write<uint32>(m_character_id); //??
	b->write<uint16>( uint16(m_worldposition.z) );
    b->write<uint16>(0); //??
    b->write<uint32>(uint32(m_worldposition.x));
    b->write<uint32>(uint32(m_worldposition.y));
	b->write<uint16>(uint16(m_worldposition.o));
    b->write<uint8>(0); //??
	b->write<uint8>(GetRealm());
    b->write<uint32>(0); //??
    b->write<uint8>(0); //??
    b->write<uint8>(m_region); 
    b->write<uint16>(1); //??
    b->write<uint8>(0); //??
    b->write<uint8>(m_career);
    b->fill(0,6);
    b->writeBytePascalString(objmgr.m_realm->Name);
    b->fill(0,3);
    sendPacket(b);

	{
/*
{SERVER} Packet: (0x00D6) F_SET_TIME PacketSize = 8
|------------------------------------------------|----------------|
|00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F |0123456789ABCDEF|
|------------------------------------------------|----------------|
|00 DA 65 44 00 00 00 07                         |..eD....        |
-------------------------------------------------------------------
*/
		Buffer *d = new Buffer(8+3);
		d->write<uint16>(8);
		d->write<uint8>(F_SET_TIME);
		d->write<uint64>(0x00DA654400000007);
		sendPacket(d);
		/*
{SERVER} Packet: (0x0083) S_WORLD_SENT PacketSize = 1
|------------------------------------------------|----------------|
|00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F |0123456789ABCDEF|
|------------------------------------------------|----------------|
|00                                              |.               |
-------------------------------------------------------------------
*/
		Buffer *x = new Buffer(1+3);
		x->write<uint16>(1);
		x->write<uint8>(S_WORLD_SENT);
		x->write<uint8>(0x00);
		sendPacket(x);
	}
}

void Player::sendQuestList()
{
    /// \todo Quest List: Code Me!
    Log.Debug("Player","sendQuestList()");

	if(m_QuestInferface)
		m_QuestInferface->SendAllQuest();
}
void Player::sendCareerCategory()
{
	Log.Debug("Player","Sending CareerCategory");

	Characterinfo * Info = objmgr.GetCharacterInfo( m_career );
	if( !Info ) return;
	stringstream name;
	name << Info->career_name << " Abilities";

	uint16 responseSize = 58+name.str().length();
    Buffer* b = Allocate(responseSize+3);
    b->write<uint16>(responseSize);
    b->write<uint8>(F_CAREER_CATEGORY);
	b->write<uint16>(0x0001);
	b->fill(0,14);
	b->writeBytePascalString(name.str());
	b->write<uint16>(0x0012);

	for( uint8 i=1;i<19;++i)
	{
		b->write<uint16>(i);
	}
	b->fill(0,3);

	sendPacket(b);
}
void Player::sendCareerPackage()
{




}
void Player::sendXP()
{
	const XpInfo * Info = NULL;
	Info = objmgr.GetXpInfo(m_level);

    uint16 responseSize = 16;
	Log.Debug("Player","Sending xp");
    Buffer* b = Allocate(responseSize+3);
    b->write<uint16>(responseSize);
    b->write<uint8>(F_PLAYER_EXPERIENCE);
    b->write<uint32>(m_xp);
	b->write<uint32>( Info != NULL ? Info->xp : uint32(1));
    b->write<uint32>(m_restxp);
	b->write<uint8>(m_level); // // level
	b->write<uint8>(0); //
    b->write<uint16>(0); //
	b->Trace();
    sendPacket(b);
}

void Player::sendRenown()
{
    uint16 responseSize = opcodeSize[F_PLAYER_RENOWN][SC];
    Buffer* b = Allocate(responseSize+3);
    b->write<uint16>(responseSize);
    b->write<uint8>(F_PLAYER_RENOWN);
    b->write<uint32>(m_renown);
	b->write<uint32>( objmgr.GetRenownToLevel(m_renownrank+1) );
    b->write<uint8>(m_renownrank);
    b->fill(0,3);

    sendPacket(b);
}

void Player::sendAbilities()
{
    /// \todo Completely reverse these packets
    /// \todo Should we store abilities or have a constdb somewhere? Depends on level+career?
	if(m_AbilityInterface)
		m_AbilityInterface->SendAllAbility();

	uint32 swappedSkills= m_skills; 
	SwapByte::Swap(swappedSkills);

	Buffer* b = Allocate(12+3);
	b->write<uint16>(12); // take care of this hard-coded value!
	b->write<uint8>(F_CHARACTER_INFO);
	b->write<uint8>(3); //skills
	b->fill(0,3);//No idea
	b->write<uint8>(m_careerline);
	b->write<uint8>(m_race);
	b->write<uint32>(swappedSkills ); // General skills (masked. 1 bit = 1 ability)
	b->write<uint16>(m_rallyPoint); //rally point ID
	sendPacket(b);
}

void Player::sendCharacterPrefs()
{
    uint16 responseSize = opcodeSize[F_CHARACTER_PREFS][SC];
    Buffer* b = Allocate(responseSize+3);
    b->write<uint16>(responseSize);
    b->write<uint8>(F_CHARACTER_PREFS);
    b->write<uint8>(1); //No idea

    sendPacket(b);
}

void Player::sendTOK()
{
    /// \todo Reverse this packet
    /// \todo Store the TOK entries in the db

   /* uint16 responseSize = 0x05E2; //Constant from 1.0.4 to 1.2 ...
    uint8 TOKContent[0x05E2]=
    {
        0x01,0x00,0x05,0xDC,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00
    };


    Buffer* b = Allocate(responseSize+3);
    b->write<uint16>(responseSize);
    b->write<uint8>(F_TOK_ENTRY_UPDATE);
    b->write(TOKContent,responseSize);
    sendPacket(b);*/
/*
{SERVER} Packet: (0x00F8) F_TOK_ENTRY_UPDATE PacketSize = 9
|------------------------------------------------|----------------|
|00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F |0123456789ABCDEF|
|------------------------------------------------|----------------|
|00 00 00 01 00 83 01 01 00                      |.........       |
-------------------------------------------------------------------
*/
	{
		Buffer *b = new Buffer(9+3);
		b->write<uint16>(9);
		b->write<uint8>(F_TOK_ENTRY_UPDATE);
		b->write<uint32>(1);
		b->write<uint16>(0x83);
		b->write<uint16>(0x0101);
		b->write<uint8>(0);
		sendPacket(b);
	}
	/*
{SERVER} Packet: (0x00F8) F_TOK_ENTRY_UPDATE PacketSize = 9
|------------------------------------------------|----------------|
|00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F |0123456789ABCDEF|
|------------------------------------------------|----------------|
|00 00 00 01 00 85 01 01 00                      |.........       |
-------------------------------------------------------------------
*/
	/*	{
		Buffer *b = new Buffer(9+3);
		b->write<uint16>(9);
		b->write<uint8>(F_TOK_ENTRY_UPDATE);
		b->write<uint32>(1);
		b->write<uint16>(0x85);
		b->write<uint16>(0x0101);
		b->write<uint8>(0);
		sendPacket(b);
	}*/
/*
{SERVER} Packet: (0x00F8) F_TOK_ENTRY_UPDATE PacketSize = 9
|------------------------------------------------|----------------|
|00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F |0123456789ABCDEF|
|------------------------------------------------|----------------|
|00 00 00 01 18 34 01 00 00                      |.....4...       |
-------------------------------------------------------------------
*/

	/*{
		Buffer *b = new Buffer(9+3);
		b->write<uint16>(9);
		b->write<uint8>(F_TOK_ENTRY_UPDATE);
		b->write<uint32>(1);
		b->write<uint16>(0x1834);
		b->write<uint16>(0x0100);
		b->write<uint8>(0);
		sendPacket(b);
	}*/
}

void Player::sendRVRStats()
{
    /// \todo Reverse this packet
    // No visual effect detected by changing values...
    // Depending on what it means, it could be moved to the game server later on.
    uint16 responseSize = opcodeSize[F_RVR_STATS][SC];
    Buffer* b = Allocate(responseSize+3);
    b->write<uint16>(responseSize);
    b->write<uint8>(F_RVR_STATS);
    b->write<uint32>(0); //From the way it varies, this really seems to be uint32s
    b->write<uint32>(0);
    b->write<uint32>(0);
    b->write<uint32>(0);
    b->write<uint32>(0);
    b->write<uint32>(0);
    b->write<uint32>(0);
    b->write<uint32>(0);
    b->write<uint32>(0);

    sendPacket(b);

}

void Player::sendCounterInfo()
{
    /// \todo Find out what these counters are, stored them in db and load them back.
    uint16 responseSize = 2+2+4+2; // take care of this hard-coded value!
    Buffer* b = Allocate(responseSize+3);
    b->write<uint16>(responseSize); // take care of this hard-coded value!
    b->write<uint8>(F_ACTION_COUNTER_INFO);
    b->write<uint16>(1); //number of counters
    b->write<uint16>(7); //counterID
    b->write<uint32>(1); //counter value
    b->write<uint16>(0); //end bytes

    sendPacket(b);
}

void Player::sendPlayedTime()
{
    //Note. Here we only use minutes. That should give 8.000+ years of playtime....
    //What a waste of bytes...
    uint16 responseSize = opcodeSize[F_PLAY_TIME_STATS][SC];
    Buffer* b = Allocate(responseSize+3);
    b->write<uint16>(responseSize);
    b->write<uint8>(F_PLAY_TIME_STATS);

	int seconds = 	GetPlayedTime();
	int mins=0;
	int hours=0;
	int days=0;
	if(seconds >= 60)
	{
		mins = seconds / 60;
		if(mins)
		{
			seconds -= mins*60;
			if(mins >= 60)
			{
				hours = mins / 60;
				if(hours)
				{
					mins -= hours*60;
					if(hours >= 24)
					{
						days = hours/24;
						if(days)
							hours -= days*24;
					}
				}
			}
		}
	}


    b->write<uint32>(days); //Days
    b->write<uint32>(hours); //Hours
    b->write<uint32>(mins); //Minutes

   sendPacket(b);

}

void Player::sendHealth()
{
    uint16 responseSize = opcodeSize[F_PLAYER_HEALTH][SC];
    Buffer* b = Allocate(responseSize+3);
    b->write<uint16>(responseSize);
    b->write<uint8>(F_PLAYER_HEALTH);
    b->write<uint32>(m_health);
	b->write<uint32>(GetTotalHealth());
    b->write<uint16>(m_actionpoints);
    b->write<uint16>(m_maxactionpoints);
    b->write<uint16>(0); //? Controls a blue circle in the same bar
    b->write<uint16>(0x0E10); // idem
	sendPacket(b);
}

void Player::sendPlayerLoaded()
{
    uint16 responseSize = opcodeSize[S_PLAYER_LOADED][SC];
    Buffer* b = Allocate(responseSize+3);
    b->write<uint16>(responseSize);
    b->write<uint8>(S_PLAYER_LOADED);
    b->write<uint16>(m_oid);
    sendPacket(b);
}

void Player::sendTactics()
{
    uint16 responseSize = opcodeSize[F_TACTICS][SC];
    Buffer* b = Allocate(responseSize+3);
    b->write<uint16>(responseSize);
    b->write<uint8>(F_TACTICS);
    b->write<uint16>(0x0300);  // do not know.
    sendPacket(b);
}

void Player::sendMoraleList()
{
    uint16 responseSize = opcodeSize[F_MORALE_LIST][SC];
    Buffer* b = Allocate(responseSize+3);
    b->write<uint16>(responseSize);
    b->write<uint8>(F_MORALE_LIST);
    b->fill(0,responseSize);  // Quite easy for the moment
    sendPacket(b);
}

void Player::loadEffects()
{
    /// \todo load the character effects
    // Load persistent effect list from the db
    // Do a sendEffect for each
}

void Player::sendGFXMod()
{
    /// \todo finish reversing this packet

    uint16 responseSize = 6; // packet can take different length
    Buffer* b = Allocate(responseSize+3);
    b->write<uint16>(responseSize);
    b->write<uint8>(F_GFX_MOD);
    b->write<uint16>(0);
	b->write<uint16>( m_oid );
    b->write<uint16>(0);

    sendPacket(b);
}

void Player::sendPlayerInitComplete()
{
    uint16 responseSize = opcodeSize[F_PLAYER_INIT_COMPLETE][SC];
    Buffer* b = Allocate(responseSize+3);
    b->write<uint16>(responseSize);
    b->write<uint8>(F_PLAYER_INIT_COMPLETE);
    b->write<uint8>(m_oid & 0x00FF); //For some reason the oid is sent here in reverse order...
    b->write<uint8>(m_oid & 0xFF00);

    sendPacket(b);
}

void Player::onRequestWorldLarge()
{

    /// \todo send the F_CREATE_MONSTER, F_ACTIVE_EFFECTS and F_PLAYER_INVENTORY for NPC around
    ///- Set time
    /// \todo Move this at the game server level

    time_t rawtime;
    tm * ptm;
    time(&rawtime);
    ptm = gmtime(&rawtime);
    uint32 currentTime = (((ptm->tm_hour*60)+ptm->tm_min)*60+ptm->tm_sec)*1000;


	uint16 responseSize=0;
	{
		responseSize = opcodeSize[F_SET_TIME][SC];
		Buffer* b = Allocate(responseSize+3);
		b->write<uint16>(responseSize);
		b->write<uint8>(F_SET_TIME);
		b->write<uint32>(currentTime);
		b->write<uint32>(7); // time acceleration
		sendPacket(b);
	}

	{
		///- Send World Sent
		responseSize = opcodeSize[S_WORLD_SENT][SC];
		Buffer* b = Allocate(responseSize+3);
		b->write<uint16>(responseSize);
		b->write<uint8>(S_WORLD_SENT);
		b->write<uint8>(0);
		sendPacket(b);
	}

    /// \todo send F_CREATE_PLAYER, F_ACTIVE_EFFECTS, F_PLAYER_INVENTORY for players around
    // And maybe a F_UPDATE_HOT_SPOT

	sendLocalizedString(objmgr.m_Welcome,TEXT_SERVER_MOTD);

	sEventHook.ExecuteHook(EVENTS_ENTER_WORLD,1,this);
}
void Player::sendPacket(Buffer * b,bool del)
{
	if( m_Com  ) m_Com->writePacket(b,del);
}
void Player::delayedPacket(Buffer *b)
{
	PackToSend * pts = new PackToSend;
	pts->com = GetCom();
	pts->packet = b;
	pts->dispose = true;

	sWorldSocket.QueuePacketToSend(pts);
}
void Player::dispachPacket(Buffer *b)
{
	Buffer* d = Allocate(b->size());

	b->pushReaderPos();
	d->write(b,true);
	b->popReaderPos();

	m_Com->writePacket(d);
}
void Player::sendStringMessage(uint16 oid,string name,string text,ChatLogFilters type)
{
		uint16 responseSize = 7+1+name.length()+1+1+1+1+text.length()+1;
		Buffer* b = Allocate(responseSize+3);
		b->write<uint16>(responseSize);
		b->write<uint8>(F_CHAT);
		b->write<uint16>( oid );

		b->write<uint8>(type); // Type
		b->write<uint8>(0); // zone
		b->write<uint8>(0); // unk2
		b->write<uint8>(0); // unk3
		b->write<uint8>(0); // unk4

		b->write<uint8>(name.length()+1);
		b->write(reinterpret_cast<const uint8*>(name.c_str()),name.length());
		b->write<uint8>(0); // unk5
		b->write<uint8>(0); // unk6
		b->write<uint8>(text.length()+1);
		b->write(reinterpret_cast<const uint8*>(text.c_str()),text.length());
		b->write<uint8>(0); // unk6
		b->write<uint8>(0); // unk7

		sendPacket(b);
}
void Player::sendLocalizedString(string text,uint16 type,bool deux)
{
	uint16 responseSize = 14+text.length();
	Buffer* b = Allocate(responseSize+3);
	b->write<uint16>(responseSize);
	b->write<uint8>(F_LOCALIZED_STRING);

	b->write<uint8>(deux ? 2 : 0);

	b->write<uint8>(0);
	b->write<uint16>(0);
	b->write<uint16>(type);
	b->write<uint16>(0);
	b->write<uint8>(0);
	b->write<uint8>(deux ? 0 : 1);
	b->write<uint8>(deux ? 0 : 1);

	b->write<uint8>(0);

	b->write<uint8>(text.length()+1);
	b->write(reinterpret_cast<const uint8*>(text.c_str()),text.length());
	b->write<uint8>(0);
	sendPacket(b);
}
void Player::sendShowDialog(string text,uint16 type)
{
	uint16 responseSize = 5+text.length();
	Buffer* b = Allocate(responseSize+3);
	b->write<uint16>(responseSize);
	b->write<uint8>(F_SHOW_DIALOG);
	b->write<uint16>(type);
	b->write<uint8>(0);
	b->write<uint8>(text.length()+1);
	b->write(reinterpret_cast<const uint8*>(text.c_str()),text.length());
	b->write<uint8>(0);
	sendPacket(b);
}
void Player::sendShowDialog(uint16 value,uint16 type)
{
	uint16 responseSize = 6;
	Buffer* b = Allocate(responseSize+3);
	b->write<uint16>(responseSize);
	b->write<uint8>(F_SHOW_DIALOG);
	b->write<uint16>(type);
	b->write<uint16>(0);
	b->write<uint16>(value);
	sendPacket(b);
}
void Player::sendWipeMessage(Player * Receiver,string text)
{
	if( !Receiver ) return;

	//Donc , j'envoi le message au Receiver
	Receiver->sendStringMessage(GetOid(),GetName(),text,CHATLOGFILTERS_TELL_RECEIVE);

	// Ensuite je m'envoi un message a moi même pour me dire ce que j'ai envoyé
	sendStringMessage(GetOid(),Receiver->GetName(),text,CHATLOGFILTERS_TELL_SEND);
}
void Player::sendGuildMessage(Player * Sender,string text)
{

}
void Player::Save()
{
	Log.Debug("Player","Saving Player [%s].",GetName().c_str());

    std::stringstream ssSQL;
    ssSQL	<< "UPDATE `characters` SET "
       << "zone='"				<< (uint32)m_zone		<< "',"
       << "level='"			<< (uint32)m_level		<< "',"
       << "wealth='"			<< m_wealth				<< "',"
       << "speed='"			<< (uint32)m_speed		<< "',"
       << "region='"			<< (uint32)m_region		<< "',"
       << "posX='"				<< (uint32)(GetWorldX()) << "',"
       << "posY='"				<< (uint32)(GetWorldY()) << "',"
       << "posZ='"				<< (uint32)(GetWorldZ()) << "',"
       << "posHeading='"		<< (uint32)(GetWorldO()) << "',"
       << "XP='"				<< m_xp		<< "',"
       << "restXP='"			<< m_restxp << "',"
       << "renown='"			<< m_renown << "',"
       << "renownRank='"		<< (uint32)m_renownrank << "',"
       << "rallyPoint='"		<< (uint32)m_rallyPoint << "',"
       << "playedTime='"		<< GetPlayedTime()		<< "',"
       << "health='"			<< (uint32)m_health		<< "',"
    	<< "maxHealth='"		<< (uint32)m_maxhealth	<< "',"
    	<< "actionPoints='"		<< (uint32)m_actionpoints	<< "',"
		<< "maxActionPoints='"	<< (uint32)m_maxactionpoints << "' WHERE character_id='" << m_character_id<<"'";

	CharacterDatabase.Execute( ssSQL.str().c_str() );

	if(m_AbilityInterface)
		m_AbilityInterface->Save();

	M_stats.Acquire();
	for( map<uint8,uint16>::iterator itr=m_basestats.begin(); itr != m_basestats.end();++itr)
		CharacterDatabase.Execute("REPLACE INTO characters_stats (character_id,type_id,type_lv) VALUES (%u,%u,%u)",m_character_id,itr->first,itr->second);
	M_stats.Release();

	if(m_iteminterface)
		m_iteminterface->Save();

	if(m_QuestInferface)
		m_QuestInferface->Save();


	SaveSocial();
}
void Player::SaveSocial()
{
	M_social.Acquire();
	CharacterDatabase.Execute("DELETE FROM characters_friends WHERE character_id=%u",m_character_id);
	for( set<uint32>::iterator itr=m_friend.begin();itr!=m_friend.end();++itr)
		CharacterDatabase.Execute("INSERT INTO characters_friends VALUES ('%u','%u','')",m_character_id,(*itr));
	M_social.Release();
}
void Player::sendLeave()
{
	if(m_leaved) return;

	Log.Debug("Player","Leaving Player [%s].",GetName().c_str());

	if(m_decostage && (GetPermission() > 0 && objmgr.m_discotimeforgm) )
	{
		if(IsMoving())
			sendLocalizedString("",TEXT_MUST_NOT_MOVE_TO_QUIT,true);
		else
		{
			sEventMgr.AddEvent(this, &Player::sendLeave,EVENTS_EXIT_GAME,5000,1,1);

			stringstream ss; ss << m_decostage/1000;
			sendLocalizedString(ss.str(),TEXT_YOU_WILL_LOG_OUT_IN_X_SECONDS);
			m_decostage-=5000;
			if(m_decostage<0) m_decostage=0;
			m_disconecting=true;
		}
	}
	else
	{	

		if(IsInWorld())
			RemoveFromWorld();

		Buffer *b =  new Buffer(2+3);
		b->write<uint16>(2);
		b->write<uint8>(F_PLAYER_QUIT);
		b->write<uint8>(0);
		b->write<uint8>(1);
		sendPacket(b,false);
		m_leaved=true;
		m_disconecting=false;
		sEventHook.ExecuteHook(EVENTS_LOG_OUT,1,this);
		delete b;
	}
}
void Player::SendEquiped(Player * Plr)
{
	if(GetItemInterface())
		GetItemInterface()->SendAllEquiped(Plr);
}
void Player::SendMeTo(Player * Plr)
{
		if( Plr == NULL ) return;
			
		uint16 responseSize = 63+GetName().length()+GetSubName().length();
		Buffer* b = Plr->Allocate(responseSize+3);
		b->write<uint16>(responseSize);
		b->write<uint8>(F_CREATE_PLAYER);
		b->write<uint16>( GetCom()->getConnectionID() );
		b->write<uint16>( GetOid() );
		b->write<uint16>( GetModel() );
		b->write<uint16>( GetCareerLine() );
		b->write<uint16>( (uint16)(GetZ()/4) );
		b->write<uint16>( GetZoneId() );
		//12
		b->write<uint16>( CalcPinX() );
		b->write<uint16>( CalcPinY() );
		b->write<uint16>( (uint16)(GetO()/8) );
		b->write<uint8>( GetLevel() ); // Level
		b->write<uint8>( 0x00 ); // 00 unk

		b->write<uint8>( Plr->GetRealm() == 2 ? 0x0A : 0x46 );
		b->fill(0,3);

		b->write<uint8>( GetTrait(0) );
		b->write<uint8>( GetTrait(1) );
		b->write<uint8>( GetTrait(2) );
		b->write<uint8>( GetTrait(3) );
		//b->fill(0,3);
		// 16
		b->write<uint8>( GetTrait(4) );
		b->write<uint8>( GetTrait(5) );
		b->write<uint8>( GetTrait(6) );
		b->write<uint8>( GetTrait(7) );
		b->fill(0,12);
		b->write<uint8>( GetRace() );
		b->fill(0,11);
		//20
		b->write<uint8>(GetName().length());
		b->write(reinterpret_cast<const uint8*>(GetName().c_str()),GetName().length());
		b->write<uint8>(GetSubName().length());
		b->write(reinterpret_cast<const uint8*>(GetSubName().c_str()),GetSubName().length());
		b->write<uint8>(0x00); // Guild Name lenght
		b->fill(0,4);
		//7
		Plr->delayedPacket(b);
}
uint8 Player::GetPermission() 
{ 
	if(m_Com) 
		return m_Com->m_permission;
	else return 0; 
}
void Player::SetGroup(Group * group)
{ 
	Log.Debug("Player","SetGroup");

	m_Group = group;
	if(group)
		group->AddMember(this);
}
void Player::RezPlayer()
{
	if(!IsDead())
		return;

	m_health = m_maxhealth;
	sendHealth();

	uint16 responseSize = 4;
	Buffer* b = Allocate(responseSize+3);
	b->write<uint16>(responseSize);
	b->write<uint8>(F_PLAYER_CLEAR_DEATH);
	b->write<uint16>(GetOid());
	b->write<uint16>(0);
	dispachPacketInRange(b,true);

	sEventHook.ExecuteHook(EVENTS_PLAYER_DEATH_CLEARED,1,this);
	//sEventHook.ExecuteHook(EVENTS_RELEASE_CORPSE,1,this);
}
void Player::Update( uint32 p_time )
{
	if(!IsInWorld())
		return;

	Unit::Update( p_time );
	uint32 mstime = getMSTime();

	if(m_attacking && !GetCurrentAbility())
	{
		// Check attack timer.
		if(mstime >= m_attackTimer)
			_EventAttack(false);

		if(m_duelWield && mstime >= m_attackTimer_1)
			_EventAttack(true);
	}

	if(m_AbilityInterface)
		m_AbilityInterface->Update(p_time);

	if(Trade != NULL)
	{
		Trade->Lock();
		if(Trade->close)
		{
			Trade->Destroy();
			Trade = NULL;
		}
		else if(Trade->changed)
		{
			Trade->Update();
		}

		if(Trade != NULL)
			Trade->UnLock();
	}
	
	if(m_nextinsertcheck && m_nextinsertcheck <= UNIXTIME)
	{
		// Fields : guid, character_id, type (Item : 0, Spell : 1, etc ...), item_or_spell_id
		QueryResult * Result = CharacterDatabase.Query("SELECT * FROM item_insert_queue WHERE character_id = %u", m_character_id);
		if(Result)
		{
			do{			
			    Field* fields = Result->Fetch();

				uint8 type = fields[2].GetUInt8();
				
				switch(type)
				{
				case 0:
					{
					const ItemProto * Proto = objmgr.GetItemProto(fields[3].GetUInt32());

					if(!Proto)
						continue;
					else
						if(GetItemInterface()->CreateItem(Proto))
							CharacterDatabase.Query("DELETE FROM item_insert_queue WHERE guid = %u", fields[0].GetUInt32());
					}break;
				case 1:
					// TODO
					break;
				}
			}while(Result->NextRow());
		}
		delete Result;
		m_nextinsertcheck = UNIXTIME + 30000;
	}

	if(Invitation)
		if(Invitation->expire <= UNIXTIME)
			Invitation->DeclineInvitation();

	if(m_nextsave && m_nextsave <= UNIXTIME)
	{
		Save();

		if(objmgr.m_savetime)
			m_nextsave = UNIXTIME+objmgr.m_savetime;
		else m_nextsave=0;
	}

	if(!m_Com || !m_Com->m_Plr || m_Com->m_Plr != this)
	{
		Log.Error("Player","Deleting Player %s from UPDATE",GetName().c_str());
		objmgr.DeletePlayer(this);
	}
}
void Player::EventAttackStart()
{
	m_attacking = true;
}

void Player::EventAttackStop()
{
	m_attacking = false;
}
void Player::_EventAttack( bool offhand )
{
	Unit * target = GetTarget();
	if( target == NULL || (target!=NULL && target==this) || (target!=NULL &&  !target->IsCreature() && !target->IsPlayer() ) )
	{
		Log.Error("Player","NO TARGET");
		EventAttackStop();
		return;
	}

	if(IsDead())
	{
		Log.Error("Player","I'm DEAD");
		EventAttackStop();
		return;
	}

	Unit * uTarget = NULL;
	if(target->IsCreature())
		uTarget = target->GetCreature();
	else if(target->IsPlayer())
		uTarget = target->GetPlayer();

	if(!uTarget)
	{
		Log.Error("Player","Target is INVALID");
		EventAttackStop();
		return;
	}

	if(uTarget->IsDead())
	{
		Log.Error("Player","Target is DEAD");
		EventAttackStop();
		return;
	}

	if (!canReachWithAttack(uTarget))
	{
		if(m_AttackMsgTimer != 1)
		{
			m_AttackMsgTimer = 1;
		}
		setAttackTimer(300, offhand);
	}
	else if(!isInFront(uTarget))
	{
		// We still have to do this one.
		if(m_AttackMsgTimer != 2)
		{
			m_AttackMsgTimer = 2;
		}
		setAttackTimer(300, offhand);
	}
	else
	{
		m_AttackMsgTimer = 0;
		
		// Set to weapon time.
		setAttackTimer(0, offhand);		

		Strike( uTarget );
	}
}
void Player::setAttackTimer(int32 time, bool offhand)
{
	if(!time)
	{
		const ItemProto * proto=NULL;
		if(offhand)
		{
			Item * itm = GetItemInterface()->GetItemInSlot(MAIN_GAUCHE);
			if(itm) proto = itm->GetProto();
		}
		else
		{
			Item * itm = GetItemInterface()->GetItemInSlot(MAIN_DROITE);
			if(itm) proto = itm->GetProto();
		}
		if(proto)	time = proto->speed*10;
		else		time = 2000;
	}

	if(offhand) m_attackTimer_1 = getMSTime() + time;
	else		m_attackTimer = getMSTime() + time;
}
void Player::SendBuyBack()
{
	M_buyback.Acquire();

	Log.Debug("Player","SendBuyBack size=%u",m_buyback.size());

	uint16 responseSize=2;

	uint8 count=0;
	Buffer *b = new Buffer(0);
	for(list<Item*>::iterator itr= m_buyback.begin();itr!=m_buyback.end();++itr)
	{
		if(!(*itr)) continue;

		b->write<uint32>( (*itr)->CalculBuyBackPrice(1) );
		ItemsInterface::BuildItemInfo(b,(*itr)->GetProto(),(*itr)->GetCount(),(*itr),false);
		count++;
	}


	Buffer* d = Allocate(b->size()+2+3);
	d->write<uint16>(b->size()+2);
	d->write<uint8>(F_STORE_BUY_BACK);
	d->write<uint8>(count); // size buyback
	d->write(b);
	d->write<uint8>(0x00);
	sendPacket(d);
	delete b;

	M_buyback.Release();

}
void Player::AddBuyBack(Item * itm)
{
	if(!itm) return;

	Log.Debug("Player","AddBuyBack entry=%u",itm->GetProto()->entry);

	M_buyback.Acquire();
	if(m_buyback.size() >= MAX_BUY_BACK_SIZE)
	{
		Item * del = m_buyback.back();
		if( del ) del->Delete(); // L'item est perdu définitivement, supprimé de la DB et de la mémoire
		m_buyback.pop_back();
	}
	m_buyback.push_front( itm );
	itm->Delete(false); // On supprime l'item de la DB mais one le garde en mémoire , si le joueur le rachète , alors lorsque le joueur quitera il sera REPLACE,sinon perdu
	M_buyback.Release();

	if(itm->GetScript())
		itm->GetScript()->onSellItem();

	SendBuyBack();
}
void Player::RemoveBuyBack(Item * itm)
{
	if(!itm) return;

	Log.Debug("Player","RemoveBuyBack entry=%u",itm->GetProto()->entry);

	M_buyback.Acquire();
	m_buyback.remove(itm);
	M_buyback.Release();
	SendBuyBack();
}
void Player::SafeTeleport(uint32 x,uint32 y,uint16 z,uint16 o)
{
	if(!x || !y || !z) return;

	Log.Debug("Player","Safeteleport : JUMP %u,%u,%u,%u",x,y,z,o);

	uint16 responseSize = 20;
	Buffer* b = Allocate(responseSize+3);
	b->write<uint16>(responseSize);
	b->write<uint8>(F_PLAYER_JUMP);
	b->write<uint32>(x); // wx
	b->write<uint32>(y); // wy
	b->write<uint16>(GetOid());
	b->write<uint16>(z); // z/4
	b->write<uint16>(o); // orientation
	b->fill(0,5);
	b->write<uint8>(0x01);
	sendPacket(b);
}
void Player::CheckZoneAcces(Creature * Crea)
{
	if(Crea == NULL) return;
	if(!Crea->IsInWorld()) return;

	ZoneInfo * Info = Crea->GetZoneMgr()->GetZoneInfo();
	if(!Info) return;

	M_zoneacces.Acquire();

	if(m_zoneacces[Info->zoneid] == NULL)
	{
		m_zoneacces[Info->zoneid] = Info;
		SaveAccesZone(Info->zoneid);
	}

	M_zoneacces.Release();
}
void Player::BuildZoneAcces(Buffer *b)
{
	M_zoneacces.Acquire();

	uint16 counts=1;
	for(map<uint16,ZoneInfo*>::iterator itr=m_zoneacces.begin();itr!=m_zoneacces.end();++itr)
	{
		if(!itr->second) continue;

		b->write<uint16>(counts);
		b->write<uint8>(0x02);
		b->write<uint16>( itr->second->price );
		b->write<uint16>( itr->second->zoneid );
		b->write<uint8>(0x01);
		counts++;
	}
	M_zoneacces.Release();
}
void Player::SaveAccesZone(uint16 zone) 
{ 
	CharacterDatabase.Execute("INSERT INTO characters_zonesacces VALUES ('%u','%u')",m_character_id,zone); 
}
void Player::SafeTeleport(ZoneInfo *Info)
{
	if(!Info) return;

	uint8 realmid = GetRealm();
	if(realmid > 2) return;

	SafeTeleport(Info->zoneid,Info->region,Info->m_tp[realmid].x,Info->m_tp[realmid].y,Info->m_tp[realmid].z,Info->m_tp[realmid].o);
}
void Player::SafeTeleport(uint16 zone,uint8 region,uint32 x,uint32 y,uint16 z,uint16 o)
{
	Log.Debug("Player","Teleporting player on zone=%u %u,%u,%u,%u",zone,x,y,z,o);
	if(zone == GetZoneId())
	{
		SafeTeleport(x,y,z,o);
		return;
	}

	m_teleporting=true;
	if(IsInWorld()) RemoveFromWorld();

	SetZoneId(zone);
	SetRegion(region);



	m_worldposition.ChangeCoords(x,y,z,o);
	m_OffSetX = mapmgr.CalculOffset(x);
	m_OffSetY = mapmgr.CalculOffset(y);
	m_zone_x = m_OffSetX << 12;
	m_zone_y = m_OffSetY << 12;

	Save();

	sendPacketSwitchRegion();
}
void Player::sendPacketSwitchRegion()
{
	uint16 responseSize = 20;
	Buffer* b = Allocate(responseSize+3);
	b->write<uint16>(responseSize);
	b->write<uint8>(F_SWITCH_REGION);
	b->write<uint16>(m_zone);
	b->fill(0,5);
	b->write<uint8>(0x01);
	b->write<uint8>(0x01);
	b->fill(0,11);
	sendPacket(b);
}
uint16 Player::CalculDamage()
{
	float dmg = 1;
	float Str = GetStat( GameData::STATS_STRENGTH );
	float Tou = GetStat( GameData::STATS_TOUGHNESS );
	float Wdps = GetItemInterface()->GetWdps(MAIN_DROITE);
	float Wspeed = GetItemInterface()->GetWspeed(MAIN_DROITE);
	Wspeed/=100;
	dmg = ( (Str/10) + Wdps) * Wspeed;

	return uint16(dmg);
}
uint16 Player::CalculSpeed()
{
	if(GetItemInterface())
	{
		Item * itm = GetItemInterface()->GetItemInSlot(MAIN_DROITE);
		if(!itm) return 1000;

		return itm->GetProto()->speed;
	}
	else return 1000;
}
void Player::GiveXp(uint32 value)
{
	Log.Debug("Player","%s GiveXp %u",GetName().c_str(),value);

	if(!value) return;
	if(m_level==40) return;

	const XpInfo * Info = objmgr.GetXpInfo(m_level);

	if(!Info) return;

	m_xp+=value;

	if(m_xp >= Info->xp)
	{
		uint32 rest = m_xp-Info->xp;

		m_xp=0;

		LevelInfo * info = objmgr.GetLevelInfo(GetCareerLine(),m_level+1);
		if(!info) return;

		ApplyLevelInfo(info);

		if(m_level==40)
		{
			sendXP();
			return;
		}

		if(rest)	GiveXp(rest);
		else		sendXP();
		
	} 
	else sendXP();
}
void Player::ApplyLevelInfo(LevelInfo * info)
{
	if(!info) return;

	Log.Debug("Player","%u ApplyLevelInfo %u",m_character_id,info->level);
	bool updatelvl=false;

	if(m_level != info->level)
		updatelvl=true;

	map<uint8,uint16> m_diff;
	for(uint8 i=1;i<GameData::STATS_COUNT;++i)
	{
		if(info->stats[i] > GetBaseStat(i))
			m_diff[i] = info->stats[i]-GetBaseStat(i);

		SetBaseStat(i,info->stats[i]);
	}

	ApplyStatBonus();

	m_level=info->level;

	if(updatelvl)
		sendLevel(true);

	if(updatelvl)
		sendStats();

	if(updatelvl)
	{	
		Buffer *d = Allocate(5+(m_diff.size()*3)+3);
		d->write<uint16>(5+(m_diff.size()*3));
		d->write<uint8>(F_PLAYER_LEVEL_UP);
		d->write<uint32>(0);
		d->write<uint8>(m_diff.size());
		for(map<uint8,uint16>::iterator itr=m_diff.begin();itr!=m_diff.end();++itr)
		{
			d->write<uint8>(itr->first);
			d->write<uint16>(itr->second);
		}
		sendPacket(d);
	}

	if(updatelvl && m_AbilityInterface)
		m_AbilityInterface->UpdateLevel(m_level);

	if(updatelvl && GetGroup())
		GetGroup()->Update();
}
void Player::sendLevel(bool up)
{
	Buffer *d = Allocate(4+3);
	d->write<uint16>(4);
	d->write<uint8>(F_PLAYER_RANK_UPDATE);
	d->write<uint8>(m_level-1);
	d->write<uint8>(0x20);
	d->write<uint16>(GetOid());
	if(up)
	{
		dispachPacketInRange(d,true);
		DisposeBuffer(d);
	}
	else sendPacket(d);
}
void Player::ApplyStatBonus()
{
	uint32 ancienht = m_maxhealth;
	m_maxhealth = GetStat(GameData::STATS_WOUNDS)*10;

	if(m_health > m_maxhealth)
		m_health = m_maxhealth;

	if(!m_loading && ancienht != m_maxhealth) // Si le perso n'est pas en train de charger , alors on envoi les info
		sendHealth();
}
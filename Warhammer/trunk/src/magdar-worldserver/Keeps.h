#ifndef KEEPS_H
#define KEEPS_H

#include "GameData.h"
#include "SystemData.h"

class Keep
{
public:
    GameData::Realm keepRealm;
    GameData::KeepStatus keepStatus;
    std::string guildName;
    uint8 keepRank;
    uint16 keepTactics[3];
    Keep()
            : keepRealm(GameData::REALM_NONE)
            , keepStatus(GameData::KEEPSTATUS_SAFE)
            , guildName("")
            , keepRank(0)
    {
        for (int i=0;i<3;++i)
            keepTactics[i]=0;
    };
};

class Objective
{
public:
    GameData::Realm objectiveRealm;
    bool hasName;
    Objective()
            : objectiveRealm(GameData::REALM_NONE)
            , hasName(false)
    {
    }
};

class City
{
public:
    GameData::CityId cityID;
    SystemData::CityStates cityState;
    GameData::Realm cityRealm;
    City(GameData::CityId Id, GameData::Realm realm)
            : cityState(SystemData::CITYSTATES_SAFE)
            , cityRealm(realm)
    {
    }
};
#endif
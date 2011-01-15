#ifndef __STDAFX_H
#define __STDAFX_H

#pragma warning(disable : 4244  )
#pragma warning(disable : 4081  )

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

#define DEF_VALUE_NOT_SET 0xDEADBEEF

// Active les scripts AI commenter pour les désactiver
#define __WITH_SCRIPTS

#define _GAME

#include <list>
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
//#include <fstream>

#include "Common.h"
#include <rpc.h>
#include <rpcdce.h>

#include "../magdar-shared/Config/ConfigEnv.h"

#include <sfml/Graphics.hpp>
#include <sfml/System.hpp>

#include <boost/asio/io_service.hpp>
#include <boost/pointer_cast.hpp>
#include <boost/thread.hpp> // for the mutex
#include <boost/version.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/progress.hpp>
#include <openssl/sha.h>
#include <openssl/evp.h>

using namespace boost;

#include "../magdar-shared/Timer.h"
#include "../magdar-shared/Util.h"
#include <zlib/zlib.h>

#include "../magdar-shared/Database/DatabaseEnv.h"
#include "../magdar-shared/CrashHandler.h"

#include "../magdar-shared/FastQueue.h"
#include "../magdar-shared/Network/Network.h"
#include "../magdar-shared/Network/Acceptor.h"
#include "../magdar-shared/Network/Buffer.h"
#include "../magdar-shared/Network/BufferPool.h"
#include "../magdar-shared/Network/Connection.h"
#include "../magdar-shared/Network/ConnectionAcceptor.h"
#include "../magdar-shared/Network/ConnectionRequestor.h"
#include "../magdar-shared/Network/Requestor.h"
#include "../magdar-shared/LocationVector.h"


#include "../magdar-shared/InterOpcode.h"
#include "../magdar-shared/GameData.h"
#include "../magdar-worldserver/ProgressBar.h"
#include "../magdar-worldserver/SystemData.h"
#include "../magdar-worldserver/Keeps.h"
#include "../magdar-worldserver/EventMgr.h"
#include "../magdar-worldserver/EventableObject.h"
#include "../magdar-worldserver/Storage.h"
#include "../magdar-worldserver/EventHook.h"
#include "../magdar-worldserver/WorldSocket.h"


// Object
#include "../magdar-worldserver/Object.h"
#include "../magdar-worldserver/Unit.h"
#include "../magdar-worldserver/GameObject.h"
#include "../magdar-worldserver/Creature.h"
#include "../magdar-worldserver/Player.h"
#include "../magdar-worldserver/Item.h"
#include "../magdar-worldserver/Group.h"

// Map
#include "../magdar-worldserver/CellMgr.h"
#include "../magdar-worldserver/HeightField.h"
#include "../magdar-worldserver/ZoneMgr.h"
#include "../magdar-worldserver/MapMgr.h"

// Interface
#include "../magdar-worldserver/CombatManager.h"
#include "../magdar-worldserver/ItemsInterface.h"
#include "../magdar-worldserver/QuestInterface.h"
#include "../magdar-worldserver/InfluenceInterface.h"
#include "../magdar-worldserver/TokInterface.h"
#include "../magdar-worldserver/MailInterface.h"
#include "../magdar-worldserver/PQuest.h"
#include "../magdar-worldserver/Loot.h"


//AI && Script
#include "../magdar-worldserver/AiInterface.h"
#include "../magdar-worldserver/ScriptedAI.h"
#include "../magdar-worldserver/AI_SimpleDB.h"
#include "../magdar-worldserver/ScriptedQuest.h"
#include "../magdar-worldserver/ScriptedItem.h"
#include "../magdar-worldserver/ScriptMgr.h"
#include "../magdar-worldserver/PQScript.h"

// Spells
#include "../magdar-worldserver/Ability.h"
#include "../magdar-worldserver/AbilityEffect.h"
#include "../magdar-worldserver/AbilityTarget.h"
#include "../magdar-worldserver/AbilityInterface.h"

#include "../magdar-worldserver/StatsMgr.h"
#include "../magdar-worldserver/MainServerDefines.h"
#include "../magdar-worldserver/WorldServer.h"
#include "../magdar-worldserver/AuthLogon.h"
#include "../magdar-worldserver/ObjMgr.h"
#include "../magdar-worldserver/QuestManager.h"
#include "../magdar-worldserver/WorldRunnable.h"
#include "../magdar-worldserver/ChatHandler.h"
#include "../magdar-worldserver/ClientCom.h"

#endif
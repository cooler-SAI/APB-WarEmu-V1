#ifndef _STDAFX_H
#define _STDAFX_H

#define DEF_VALUE_NOT_SET 0xDEADBEEF

#include <list>
#include <vector>
#include <map>
#include <sstream>
#include <string>
//#include <fstream>

#include "Common.h"
#include "../magdar-shared/Config/ConfigEnv.h"

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
#include <openssl/sha.h>
#include <openssl/evp.h>

#include "../magdar-shared/Timer.h"
#include "../magdar-shared/Util.h"
#include "../magdar-shared/ByteBuffer.h"
#include <zlib/zlib.h>

#include "../magdar-shared/Database/DatabaseEnv.h"
#include "../magdar-shared/CrashHandler.h"

#include "../magdar-shared/Network/Network.h"
#include "../magdar-shared/Network/Acceptor.h"
#include "../magdar-shared/Network/Buffer.h"
#include "../magdar-shared/Network/BufferPool.h"
#include "../magdar-shared/Network/Connection.h"
#include "../magdar-shared/Network/ConnectionAcceptor.h"
#include "../magdar-shared/Network/ConnectionRequestor.h"
#include "../magdar-shared/Network/Requestor.h"

#include "../magdar-logonserver/PeriodicFunctionCall_Thread.h"

#include "../magdar-shared/InterOpcode.h"
#include "../magdar-logonserver/InformationCore.h"
#include "../magdar-logonserver/DatabaseMgr.h"
#include "../magdar-logonserver/LogonServer.h"
#include "../magdar-logonserver/AccountMgr.h"
#include "../magdar-logonserver/AuthClient.h"
#include "../magdar-logonserver/AuthWorld.h"

#endif
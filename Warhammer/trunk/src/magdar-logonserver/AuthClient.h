#ifndef AUTHCLIENT_H
#define AUTHCLIENT_H

#include <Common.h>

#include <boost/asio/io_service.hpp>
#include "../magdar-shared/Network/Connection.h"
#include <list>

///Authentication opcodes
enum AuthOpcodes
{
    CMSG_CONNECT                                    = 0x01,
    SMSG_CONNECT_RESPONSE                           = 0x02,
    CMSG_USER_LOGIN                                 = 0x03,
    SMSG_USER_LOGIN_RESPONSE                        = 0x04,
    CMSG_AUTHENTICATE                               = 0x05,
    SMSG_AUTHENTICATE_RESPONSE                      = 0x06,
    CMSG_GET_SERVER_LIST                            = 0x07,
    SMSG_GET_SERVER_LIST_RESPONSE                   = 0x08,
    CMSG_GET_CHARACTER_SUMMARIES                    = 0x09,
    SMSG_GET_CHARACTER_SUMMARIES_RESPONSE           = 0x0a,

    CMSG_REFRESH_AUTHENTICATION                     = 0x0d,
    SMSG_REFRESH_AUTHENTICATION_RESPONSE            = 0x0e,
    CMSG_LOGIN_CLEARANCE                            = 0x0f, //supposed, never seen
    SMSG_LOGIN_CLEARANCE_RESPONSE                   = 0x10, //supposed, never seen
    CMSG_GET_PRODUCT_ACCOUNT_PROPERTY_LIST          = 0x11,
    SMSG_GET_PRODUCT_ACCOUNT_PROPERTY_LIST_RESPONSE = 0x12,
    MAX_OPCODE                                      = 0x12
};

/// Packet size without header
static const uint16 PacketSize[MAX_OPCODE+1] =
{
    0x00, ///< no packet
    0x08, ///< CMSG_CONNECT
    0x0a, ///< SMSG_CONNECT_RESPONSE
    UNKNOWN_SIZE, ///< CMSG_USER_LOGIN
    0x22, ///< SMSG_USER_LOGIN_RESPONSE
    0x48, ///< CMSG_AUTHENTICATE
    0x07, ///< SMSG_AUTHENTICATE_RESPONSE
    0x04, ///< CMSG_GET_SERVER_LIST
    UNKNOWN_SIZE, ///< SMSG_GET_SERVER_LIST_RESPONSE
    0x48, ///< CMSG_TOKEN_AUTHENTICATE
    0x07, ///< SMSG_TOKEN_AUTHENTICATE_RESPONSE
    0x04, ///< CMSG_GET_CHARACTER_SUMMARIES
    UNKNOWN_SIZE, ///< SMSG_GET_CHARACTER_SUMMARIES_RESPONSE
    0x04, ///< CMSG_REFRESH_AUTHENTICATION
    0x05, ///< SMSG_REFRESH_AUTHENTICATION_RESPONSE
    UNKNOWN_SIZE, ///< CMSG_LOGIN_CLEARANCE ///\todo check this packet
    UNKNOWN_SIZE, ///< SMSG_LOGIN_CLEARANCE_RESPONSE ///\todo check this packet
    0x04, ///< CMSG_GET_PRODUCT_ACCOUNT_PROPERTY_LIST
    UNKNOWN_SIZE ///< SMSG_GET_PRODUCT_ACCOUNT_PROPERTY_LIST_RESPONSE
};

/// Authentication phase
enum AuthStatus
{
    UN_AUTHED				= 1,
    WAITING_PASSWORD		= 2,
    SUCCESSFULLY_AUTHED		= 3
};

/// Authentication results
enum AuthResult
{
    AUTH_SUCCESS                    = 0x00,
    AUTH_ACCT_EXPIRED               = 0x07,
    AUTH_ACCT_BAD_USERNAME_PASSWORD = 0x09,
    AUTH_ACCT_TERMINATED            = 0x0D,
    AUTH_ACCT_SUSPENDED             = 0x0E
};

/// Token Authentication result
enum TokenAuthResult
{
    // USER_NOT_FOUND = default message for all other errors (1,2,5,6,7,0xA,0xB,0x10-0x1A)
    TOK_SUCCESS             = 0x00,
    TOK_USER_NOT_FOUND      = 0x01,         ///< User not found
    TOK_RESP_TIMEOUT        = 0x03,         ///< Response timeout
    TOK_RESP_TIMEOUT2       = 0x04,         ///< Response timeout
    TOK_AUTH_FAILED         = 0x07,         ///< Authentication failed
    TOK_UNABLE_SERVER_LIST  = 0x09,         ///< Unable to retrieve server list
    TOK_ACCT_SUSPENDED      = 0x0D,         ///< Account suspended
    TOK_ACCT_NOT_ACTIVE     = 0x0E          ///< Account not active
};

class Buffer;
class BufferPool;

/// Authentication connection management
class AuthClient : public Connection
{
public:

    AuthClient(boost::asio::io_service& IOService, BufferPool* hp);
    void start(void* param=NULL);
	void writePacket(Buffer* b, int opcode);

	uint32 GetAccountID() { return m_accountID; };

    ~AuthClient();

private:

	void onDisconnect();
    void onRead(const boost::system::error_code& e);
    void onWrite(const boost::system::error_code& e);
    uint32 readSize(uint32* bytesLeft);

public:
    //Responders
    void onConnect();
    void onUserLogin();
	void onLoginClearence();
    void onAuthenticate();
    void onTokenAuthenticate();
    void onGetServerList();
    void onGetCharacterSummaries();
    void onGetProductAccountPropertyList();
    void onRefreshAuthentication();

    void notImplemented();

	std::string m_username;
    uint32 m_accountID;
	uint8 m_permission;

	Account * m_acct;

private:
    std::string generateKeyBits();
    void sendCharacterList(uint32 sequence);

    bool m_expectData;
    uint32 m_expectedSize;
    uint8 m_opcode;
    boost::asio::io_service& m_IOService;
    uint32 m_charRequestNumber;
    bool m_charRequested;

    std::string m_strX1; ///< Bits used by Authentication process calculation
    std::string m_strX2;
    std::string m_strK3;

    static uint32 connectionCount;  ///< number of accepted connections
};

#endif
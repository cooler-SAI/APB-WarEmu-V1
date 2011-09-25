#ifndef CONNECTION_H_
#define CONNECTION_H_

#include <Common.h>

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>
#include <boost/system/error_code.hpp>
#include "Buffer.h"

class BufferPool;

/**
 * Abstract connection class
 * @author Albator, Doron
 */
class Connection : private boost::noncopyable,  public boost::enable_shared_from_this<Connection>
{
public:

    Connection(boost::asio::io_service& IOService, BufferPool* bp);
    ~Connection();
    boost::asio::ip::tcp::socket& socket();

    /**
     * First operation executed after establishing a connection
     */
    virtual void start(void* startParam=NULL) = 0;
public:

    void preRead(const boost::system::error_code& e,
                 std::size_t bytesTransferred);

    virtual void onRead(const boost::system::error_code& e) = 0;

    virtual void onWrite(const boost::system::error_code& e) = 0;

	virtual void onDisconnect() = 0;
    void disconnect();

    void AsyncRead();

    void AsyncWrite(Buffer* b);

    void AsyncWrite(Buffer* b, uint32 size);

    boost::asio::ip::tcp::socket m_socket;
    BufferPool* m_bufferPool;
    Buffer m_readBuffer;
    uint16 m_connectionID;
    bool b_connected;

	std::string GetStringIp() { return m_socket.remote_endpoint().address().to_string(); }
};
#endif /*CONNECTION_H_*/

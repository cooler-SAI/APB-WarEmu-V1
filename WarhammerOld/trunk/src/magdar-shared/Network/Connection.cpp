#include "Connection.h"

#include <boost/asio.hpp>
#include <boost/bind.hpp>

Connection::Connection(boost::asio::io_service& IOService, BufferPool* bp)
        : m_socket(IOService)
        , m_bufferPool(bp)
        , m_readBuffer(60000)
        , b_connected(false)
{
}
Connection::~Connection()
{
//	delete m_bufferPool;
}
boost::asio::ip::tcp::socket& Connection::socket()
{
    return m_socket;
}

void Connection::disconnect()
{	
	if( b_connected )
		onDisconnect();

    b_connected = false;
    m_socket.shutdown(boost::asio::socket_base::shutdown_both);
}

void Connection::AsyncRead()
{
    m_readBuffer.reset();

    m_socket.async_read_some(boost::asio::buffer(m_readBuffer.mutBuffer()),
                             boost::bind(&Connection::preRead, shared_from_this(),
                                         boost::asio::placeholders::error,
                                         boost::asio::placeholders::bytes_transferred));
}

void Connection::AsyncWrite(Buffer* b)
{
	if(!b) return;

    boost::asio::async_write(m_socket, boost::asio::buffer(b->constBuffer(), b->size()),
                             boost::bind(&Connection::onWrite, shared_from_this(),
                                         boost::asio::placeholders::error));
}

void Connection::AsyncWrite(Buffer* b, uint32 size)
{
	if(!b) return;

    boost::asio::async_write(m_socket, boost::asio::buffer(b->constBuffer(),size),
                             boost::bind(&Connection::onWrite, shared_from_this(),
                                         boost::asio::placeholders::error));
}

void Connection::preRead(const boost::system::error_code& e, std::size_t bytesTransferred)
{
    m_readBuffer.m_size = bytesTransferred;
	THREAD_TRY_EXECUTION2
    onRead(e);
	THREAD_HANDLE_CRASH2
}
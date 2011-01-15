#ifndef SINGLETHREADCONNECTIONACCEPTOR_H_
#define SINGLETHREADCONNECTIONACCEPTOR_H_

#include <Common.h>

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include <vector>
#include "Acceptor.h"
#include "BufferPool.h"

/* Acceptor using only one thread (a single io_service)
* The io_service can be provided in the constructor to reuse an existing one.
* For instance: a single io_service can listen on different address and port and handle
* different type of Connection.
*/
template<typename T>
class SingleThreadConnectionAcceptor : public Acceptor
{
public:

    /*
     * Create a new io_service and attach the acceptor
     *
     */
    SingleThreadConnectionAcceptor(const std::string& address, const std::string& port)
            : m_connection(),
            m_reuseIOService(false)
    {

        m_IOService = new boost::asio::io_service();
        startAcceptor(address, port);

    }

    /*
     * Reuse an IOService and attach a new acceptor
     */
    SingleThreadConnectionAcceptor(const std::string& address, const std::string& port, boost::asio::io_service* IOService)
            : m_connection(),
            m_IOService(IOService),
            m_reuseIOService(true)
    {
        startAcceptor(address, port);
    }


    /**
    * Run until stop is called or io_service return
    *
    */
    void run()
    {

        if (!m_reuseIOService)
        {
            m_IOService->run();
        }

    }


    /**
    * stop all the running io_services
    */
    void stop()
    {
        m_IOService->stop();
    }


    ~SingleThreadConnectionAcceptor()
    {
        delete m_work;
        delete m_bufferPool;
        delete m_acceptor;
    }

    boost::asio::io_service* IOService()
    {

        return m_IOService;
    }

private:

    void startAcceptor(const std::string& address, const std::string& port)
    {
        m_work = new boost::asio::io_service::work(*m_IOService);
        m_bufferPool = new BufferPool(20,1000);

        m_acceptor = new boost::asio::ip::tcp::acceptor(*m_IOService);
        boost::asio::ip::tcp::resolver resolver(m_acceptor->io_service());
        boost::asio::ip::tcp::resolver::query query(address, port);
        boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);
        m_acceptor->open(endpoint.protocol());
        m_acceptor->set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
        m_acceptor->bind(endpoint);
        m_acceptor->listen();
        m_connection.reset(new T(*m_IOService,m_bufferPool));
        m_acceptor->async_accept(m_connection->socket(), boost::bind(
                                     &SingleThreadConnectionAcceptor::onAccept, this,
                                     boost::asio::placeholders::error));

		Log.Info("SingleThreadCoAcceptor","Server listening on port: %s",port.c_str());
    }

    /**
    * event call when there is a new connection attempt
    */
    void onAccept(const boost::system::error_code& e)
    {
        if (!e)
        {
            m_connection->start();
            m_connection.reset(new T(*m_IOService,m_bufferPool));
            m_acceptor->async_accept(m_connection->socket(), boost::bind(
                                         &SingleThreadConnectionAcceptor::onAccept, this,
                                         boost::asio::placeholders::error));
        }
    }


    boost::asio::ip::tcp::acceptor* m_acceptor;
    boost::asio::io_service* m_IOService;
    BufferPool* m_bufferPool;
    boost::asio::io_service::work* m_work;

    boost::shared_ptr<T> m_connection;
    bool m_reuseIOService;
};

#endif /*SINGLETHREADCONNECTIONACCEPTOR_H_*/

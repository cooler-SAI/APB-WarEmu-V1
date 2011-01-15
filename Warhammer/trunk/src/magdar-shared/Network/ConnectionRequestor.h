#ifndef CONNECTIONREQUESTOR_H
#define CONNECTIONREQUESTOR_H

#include <Common.h>

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/exception.hpp>
#include <boost/thread.hpp>
#include <boost/thread/condition.hpp>

#include "BufferPool.h"
#include "Requestor.h"

typedef boost::error_info<struct tag_errno,std::string> err_info;
class ConnectionRequestorError : public boost::exception, public std::exception { };


template<typename T>
class ConnectionRequestor : public Requestor
{
public:

    ConnectionRequestor(const std::string& address, const std::string& port, void* startParam=NULL)
            : m_connection()
            , m_bufferPool(20,1000)
            , m_connectAttempt(5)
            , m_connected(false)
            , m_address(address)
            , m_port(port)
            , m_startParam(startParam)
    {
        m_IOService = new boost::asio::io_service();
        m_Resolver = new boost::asio::ip::tcp::resolver(*(m_IOService));
        m_connection.reset(new T(*(m_IOService), &m_bufferPool));
    }


    /**
    * Run until stop is called or io_service return
    *
    */
    void run()
    {
		THREAD_TRY_EXECUTION2
        m_IOService->run();
		THREAD_HANDLE_CRASH2
    }


    /**
    * stop all the running io_services
    */
    void stop()
    {
        m_IOService->stop();
    }


    ~ConnectionRequestor()
    {
		m_connection.reset();
        delete m_IOService;
        delete m_Resolver;
    }

    void connect()
    {
        boost::asio::ip::tcp::resolver::query query(m_address,m_port);


        while (m_connectAttempt && !m_connected)
        {
            boost::asio::ip::tcp::resolver::iterator endpoint_iterator = m_Resolver->resolve(query);
            boost::asio::ip::tcp::resolver::iterator end;

            boost::system::error_code error = boost::asio::error::host_not_found;
            while (error && endpoint_iterator != end)
            {
                m_connection->socket().close();
                m_connection->socket().connect(*endpoint_iterator++, error);
            }


            if (!error)
            {
                m_connected = true;
				m_connection->b_connected = true;
                m_connection->start(m_startParam);
            }
            else
				Log.Info("ConnectionRequestor","Connection fail (retry: %u )",m_connectAttempt);

            m_connectAttempt--;

        }

        if (!m_connected)
            Log.Error("ConnectionRequestor","Can't connect to [%s]",m_address.c_str());

		m_connectAttempt=5;
    }

    boost::shared_ptr<T> getConnection()
    {
        return m_connection;
    }
	   
	bool m_connected;
private:



    BufferPool m_bufferPool;
    boost::asio::io_service* m_IOService;
    boost::asio::ip::tcp::resolver* m_Resolver;
    boost::shared_ptr<T> m_connection;
    uint32 m_connectAttempt;
    std::string m_address;
    std::string m_port;
    void* m_startParam;                ///< Parameter (used to hook back to the object requesting the connection)


};
#endif /*CONNECTIONACCEPTOR_H_*/

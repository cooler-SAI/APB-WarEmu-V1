#ifndef NETWORK_H_
#define NETWORK_H_

#include <Common.h>

#include <boost/asio/io_service.hpp>
#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>
#include <boost/thread.hpp>
#include <vector>
#include "ConnectionAcceptor.h"
#include "ConnectionRequestor.h"
#include "SingleThreadConnectionAcceptor.h"

class Requestor;
class Acceptor;

/**
 * Network class
 * @author Albator, Doron
 */
class Network : private boost::noncopyable
{
public:
    Network();

    /**
     * Create a new connection acceptor and run it in a separate thread
     */
    template<typename AcceptorType>
    void createConnectionAcceptor(const std::string& address, const std::string& port,
                                  std::size_t poolSize)
    {
        ConnectionAcceptor<AcceptorType>* ca = new ConnectionAcceptor<AcceptorType>(address, port, poolSize);
        m_acceptors.push_back(ca);
        m_threadGroup.create_thread(boost::bind(&ConnectionAcceptor<AcceptorType>::run, ca));

    }

    template<typename AcceptorType>
    void createSingleThreadConnectionAcceptor(const std::string& address, const std::string& port)
    {
        SingleThreadConnectionAcceptor<AcceptorType>* ca;

        if (m_singleThreadIOService)
            ca = new SingleThreadConnectionAcceptor<AcceptorType>(address, port,m_singleThreadIOService);
        else
        {
            ca = new SingleThreadConnectionAcceptor<AcceptorType>(address, port);
            m_singleThreadIOService = ca->IOService();
        }

        m_acceptors.push_back(ca);
        m_threadGroup.create_thread(boost::bind(&SingleThreadConnectionAcceptor<AcceptorType>::run, ca));

    }

    /**
     * Create a new connection requestor and run it in a separate thread
     */
    template<typename RequestorType>
    ConnectionRequestor<RequestorType>* createConnectionRequestor(const std::string& address, const std::string& port, void* startParam=NULL)
    {
        ConnectionRequestor<RequestorType>* cr = new ConnectionRequestor<RequestorType>(address, port, startParam);
        cr->connect();
        m_requestors.push_back(cr);
        m_threadGroup.create_thread(boost::bind(&ConnectionRequestor<RequestorType>::run, cr));
        return cr;
    }

    /**
     * shut down all the running acceptor
     */
    void stop();

    /**
     * Wait acceptors until they stop
     */
    void wait();

    ~Network();

private:
    std::vector<Acceptor*> m_acceptors;
    std::vector<Requestor*> m_requestors;
    boost::thread_group m_threadGroup;
    boost::asio::io_service* m_singleThreadIOService;
};
#endif /*NETWORK_H_*/

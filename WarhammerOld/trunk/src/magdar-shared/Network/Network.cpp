#include "Network.h"
#include "Acceptor.h"

#include <boost/foreach.hpp>

Network::Network() : m_singleThreadIOService(NULL)
{

}


void Network::stop()
{
    BOOST_FOREACH(Acceptor* ca, m_acceptors)
    {
        //std::cout << "Stopping ca!" << std::endl;
        if(ca) ca->stop();
    }
    BOOST_FOREACH(Requestor* cr, m_requestors)
    {
        //std::cout << "Stopping cr!" << std::endl;
        if(cr) cr->stop();
    }

}

Network::~Network()
{
    BOOST_FOREACH(Acceptor* ca, m_acceptors)
    {
        //std::cout << "Deleting ca!" << std::endl;
		if(ca) delete ca;
        //std::cout << "ca Deleted!" << std::endl;
    }
    BOOST_FOREACH(Requestor* cr, m_requestors)
    {
        //std::cout << "Deleting cr!" << std::endl;
		if(cr) delete cr;
        //std::cout << "cr Deleted!" << std::endl;
    }

    if (m_singleThreadIOService)
    {
		if(m_singleThreadIOService) delete m_singleThreadIOService;
    }
}

void Network::wait()
{
    m_threadGroup.join_all();
}


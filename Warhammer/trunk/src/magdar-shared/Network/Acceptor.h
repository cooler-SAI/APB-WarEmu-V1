#ifndef ACCEPTOR_H_
#define ACCEPTOR_H_

#include <Common.h>

#include <boost/asio/io_service.hpp>
#include <boost/noncopyable.hpp>

class BufferPool;

class Acceptor : private boost::noncopyable
{
public:


    /**
     * Run until stop is called or all the io_service return
     *
     */
    virtual void run()=0;

    /**
     * stop all the running io_services
     */
    virtual void stop()=0;

    virtual ~Acceptor()
    {
    }

private:


    /**
     * event call when there is a new connection attempt
     */
    virtual void onAccept(const boost::system::error_code& e)=0;


};
#endif

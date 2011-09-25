#ifndef REQUESTOR_H_
#define REQUESTOR_H_

#include <Common.h>

#include <boost/asio/ip/tcp.hpp>
#include <boost/noncopyable.hpp>
#include <boost/system/error_code.hpp>

class Requestor : private boost::noncopyable
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

    virtual ~Requestor()
    {
    }





};
#endif

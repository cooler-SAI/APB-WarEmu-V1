#include "BufferPool.h"
#include "Buffer.h"

//////////////////////////////////////////////////////////////////
//
// Buffer Pool class definition
//
/////////////////////////////////////////////////////////////////
BufferPool::BufferPool(int chunkNumber, std::size_t chunkSize) : m_bufferStack()
{
    /*for (int i=0; i< chunkNumber; ++i)
    {
        m_bufferStack.push(new Buffer(chunkSize));
    }*/
}

Buffer* BufferPool::allocateBuffer(std::size_t size)
{
	return new Buffer(size);
    /*if (m_bufferStack.size()==0) // no more buffer available in the pool
    {
        Buffer* b = new Buffer(size);
        m_bufferStack.push(b);
        return b;
    }
    else
    {
        Buffer* buf = m_bufferStack.top();
        if (buf->capacity() < size)
        {
            buf->resize(size);
        }

        m_bufferStack.pop();
        return buf;
    }*/
}

void BufferPool::disposeBuffer(Buffer* buf)
{
	delete buf;
   /* buf->reset();
    m_bufferStack.push(buf);*/
}


BufferPool::~BufferPool()
{

    /*for (int i=m_bufferStack.size(); i <= 0; i--)
    {
        Buffer* b = m_bufferStack.top();
        delete b;
        m_bufferStack.pop();
    }*/

}
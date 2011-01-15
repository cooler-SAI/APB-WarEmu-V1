#ifndef BUFFERPOOL_H_
#define BUFFERPOOL_H_

#include <Common.h>

#include <stack>
class Buffer;

/**
 * BufferPool class to read and write data on raw buffer.
 * @author Albator
 *
 */
class BufferPool
{

public:

    BufferPool(int chunkNumber, std::size_t chunkSize);

    /**
     * Request a buffer from the pool.
     * This system avoid memory allocation for each packets created and focus on memory reuse
     * These buffers have to be manually released by releaseBuffer.
     * @param size size of the requested buffer in byte.
     */
    Buffer* allocateBuffer(std::size_t size);

    /**
     * release a previously requested buffer.
     * This method signal to BufferPool that the space allocated in previously is free again.
     * The buffer cursors are reseted.
     * @param buffer the allocated space to release
     */
    void disposeBuffer(Buffer* buf);

    /**
     * Delete all buffer in the pool for clean shutdown.
     */
    ~BufferPool();

private:
    std::stack<Buffer*> m_bufferStack;

};



#endif /*BUFFERPOOL_H_*/


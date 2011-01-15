#ifndef BUFFER_H_
#define BUFFER_H_

#include <Common.h>

#include <boost/assert.hpp>
#include <string>
#include "SwapByte.h"
#include <vector>
static const uint16 UNKNOWN_SIZE = 0xffff;

class Connection;

/**
 *  A simple byte buffer with C++ convenient methods
 *  Support asio const buffer by using boost::asio::buffer(buf.rawBuffer(), buf.size());
 *  @author Albator
 */
class Buffer
{
    // Allows fixing the buffer length after boost asio 'read_some'
    friend class Connection;
    //void Connection::preRead(const boost::system::error_code& e,
    //	std::size_t bytesTransferred);

public:

	void Trace(int32 offset=0);

    Buffer(std::size_t chunkSize);

    /**
     *  Used size
     */
    std::size_t size() const;

    /**
    *  Remaining size to be read
    */
    std::size_t remains() const;

    /**
     * Capacity of the buffer
     */
    std::size_t capacity() const;

    /**
     * Resize the buffer to the given size
     */
    void resize(std::size_t size);

    /**
     * Reset buffer to re-use
     */
    void reset();

    ////////////////////////////////////////////////
    // Write op
    ////////////////////////////////////////////////
    template <typename T> Buffer& operator<<(const T data)
    {
        write(data);
        return *this;
    }

    /**
     *  Write data to the buffer
     */
    void write(const uint8* data, std::size_t length);

    template <typename T> void write(T data)
    {
        if ((m_writeCursor+sizeof(T)) > m_rawBuffer.size())
			m_rawBuffer.resize(m_writeCursor+sizeof(T));
        SwapByte::Swap<T>(data);
        write(reinterpret_cast<const uint8*>(&data), sizeof(data));
    }

    /**
     * Write a string <uint32 size><string>
     */
    void write(const std::string& data);

    /// Write a string <uint8 size><string>
    void writeBytePascalString(const std::string& data);

    /**
     * Write a buffer into another one
     */
    void write(Buffer* data, bool fromStart=false);
    void write(Buffer* data, std::size_t length);
    /**
     * Fill the buffer with the given byte
     */
    void fill(const uint8 fillByte, std::size_t length);

    /////////////////////////////////////////////////
    // Read op
    //////////////////////////////////////////////////
    template <typename T> Buffer& operator>>(T& data)
    {
        data = read<T>();
        return *this;
    }

    Buffer& operator>>(std::string& data)
    {
        data = read();
        return *this;
    }

    ///Read data from the buffer
    bool read(uint8* data, std::size_t length);

    template <typename T> T read()
    {
        if ((m_readCursor+sizeof(T)) > m_rawBuffer.size())
            return NULL;
        T ret = *(reinterpret_cast<T*>(&m_rawBuffer[m_readCursor]));
        m_readCursor+=sizeof(T);
        SwapByte::Swap<T>(ret);
        return ret;
    }

    /**
     * Read a native type
     * This doesn't change the position of the read cursor.
     */
    template <typename T> T peek()
    {
        return peek<T>(0);
    }

    /**
     * Read a native type at a position from the readCursor
     * This doesn't change the position of the read cursor.
     */
    template <typename T> T peek(uint32 pos)
    {
        if ((m_readCursor+pos+sizeof(T)) > m_rawBuffer.size())
            return NULL;
        T ret = *(reinterpret_cast<T*>(&m_rawBuffer[m_readCursor+pos]));
        SwapByte::Swap<T>(ret);
        return ret;
    }

    /**
     * Read string <uint32,size><string>
     */
    std::string read();


    /**
     * Skip n bytes with the readCursor
     */
    void skip(uint32 bytesToSkip);


    /**
     * Remember current reader position
     */
    void pushReaderPos();


    /**
     * Set current reader to last pushed position
     */
    void popReaderPos();

    void tracePrint();

    void tracePrint(int32 offset);

    ///////////////////////////////////////////////////
    // Asio const buffer impl
    ///////////////////////////////////////////////////
    const uint8* constBuffer() const;
    std::vector<uint8>& mutBuffer();

    /// Decrypt the rest of the packet (form the position of the readCursor)
    void decryptWARRC4Packet(const uint8 size, const uint8* RC4Key, const int RC4KeyLen);

    /// encrypt the rest of the packet (form the position of the readCursor)
    void encryptWARRC4Packet(const uint8* RC4Key, const int RC4KeyLen);

protected:

    std::vector<uint8> m_rawBuffer;
    uint32 m_writeCursor;
    uint32 m_readCursor;
    std::size_t m_size;
    std::vector<uint32> m_storedPositions;
};


#endif /*BUFFER_H_*/

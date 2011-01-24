#include "Buffer.h"

//#include <iomanip>
#include <Util.h>
#include "../crypt/Crypt.h"

////////////////////////////////////////////////////////
//
// Buffer methods implementation
//
////////////////////////////////////////////////////////
Buffer::Buffer(std::size_t chunkSize) :
m_writeCursor(0), m_readCursor(0), m_size(0)
{
	if(!chunkSize)
		chunkSize = 20000;

	m_rawBuffer.resize(chunkSize);
}

void Buffer::write(const uint8* data, std::size_t length)
{
	if(!length) return;

    if ((m_writeCursor+length) > m_rawBuffer.size())
		resize(m_writeCursor+length);

	if((m_writeCursor+length) > m_rawBuffer.size())
		return;

    memcpy(&(m_rawBuffer[m_writeCursor]), data, length);
    m_writeCursor+=length;
    m_size+= length;
}

bool Buffer::read(uint8* data, std::size_t length)
{
	if(!length) return false;

    if ((m_readCursor+length) > m_rawBuffer.size())
        return false;

    memcpy(data, &(m_rawBuffer[m_readCursor]), length);
    m_readCursor+=length;
	return true;
}

void Buffer::resize(std::size_t size)
{
	if(size >= 0xFFFF)	return;

    m_rawBuffer.resize(size);
}

std::size_t Buffer::capacity() const
{
    return m_rawBuffer.size();
}

void Buffer::reset()
{
    m_size=0;
    m_writeCursor=0;
    m_readCursor=0;
}

std::size_t Buffer::size() const
{
    return m_size;
}

std::size_t Buffer::remains() const
{
    return m_size-m_readCursor;
}

const uint8* Buffer::constBuffer() const
{
    return &(m_rawBuffer[m_readCursor]);
}

std::vector<uint8>& Buffer::mutBuffer()
{
    return m_rawBuffer;
}

std::string Buffer::read()
{

    // Will throw if not enough data
    uint32 stringSize = read<uint32>();

    if ((m_readCursor+stringSize) > m_rawBuffer.size())
        return "";
    std::string ret(&(m_rawBuffer[m_readCursor]),
                    &(m_rawBuffer[m_readCursor+stringSize]));

    m_readCursor+=stringSize;
    return ret;

}

void Buffer::write(const std::string& data)
{
    /// will throw if buffer too small
    write<uint32>(data.size());
    write((const uint8*) &(data[0]),data.size());
}

void Buffer::writeBytePascalString(const std::string& data)
{
    /// will throw if buffer too small
    write<uint8>(data.size());
    write((const uint8*) &(data[0]),data.size());
}

void Buffer::write(Buffer* data, bool fromStart)
{
    if (fromStart)
        data->m_readCursor=0;
    /// will throw if buffer too small
    write(data, data->remains());
}

void Buffer::write(Buffer* data, std::size_t length)
{
    /// will throw if buffer too small
	if(!data) return;

	for(std::size_t i=0;i<length;++i)
        write<uint8>(data->read<uint8>());
}

void Buffer::tracePrint()
{
    tracePrint(0);
}

void Buffer::tracePrint(int32 offset)
{
    std::ostringstream stm;
    stm << std::endl;

    //LOG_TRACE(classLogger, "buffer size: " << m_size);

    if ((m_readCursor+offset >= 0) && (m_readCursor+offset < m_size))
    {
        Utils::hexdump(&(m_rawBuffer[m_readCursor+offset]), m_size-(m_readCursor+offset));
    }
    /*
    	for(uint32 i=offset; i < m_size; ++i)
    	{
    		stm << std::hex << std::setfill('0') << std::setw(2);
    		stm << static_cast<int>(m_rawBuffer[i]);
    		if (((i+1) % 8) == 0)
    			stm << ' ';
    		if (((i+1) % 16) == 0)
    			stm << std::endl;
    	}
    	LOG_DEBUG(logger, "" << stm.str());
    */
}


/**
 * Skip n bytes.
 */
void Buffer::skip(uint32 bytesToSkip)
{
    m_readCursor += bytesToSkip;
}


/**
 * Remember current reader position
 */
void Buffer::pushReaderPos()
{
    m_storedPositions.push_back(m_readCursor);
}

/**
 * Set current reader to last pushed position
 */
void Buffer::popReaderPos()
{
    m_readCursor = m_storedPositions.back();
    m_storedPositions.pop_back();
}
/**
 * Fill the buffer with the given byte
 */
void Buffer::fill(const uint8 fillByte, std::size_t length)
{
	for(std::size_t i=0;i<length;++i)
        write<uint8>(fillByte);
}

void Buffer::decryptWARRC4Packet(const uint8 size, const uint8 *RC4Key, const int RC4KeyLen)
{
    DORCrypt::decryptWARRC4Packet(&m_rawBuffer[m_readCursor],size, RC4Key, RC4KeyLen);
}

void Buffer::encryptWARRC4Packet(const uint8 *RC4Key, const int RC4KeyLen)
{
    DORCrypt::encryptWARRC4Packet(&m_rawBuffer[m_readCursor],m_size-m_readCursor, RC4Key, RC4KeyLen);
}
void Buffer::Trace(int32 offset)
{
		std::ostringstream stm;
		stm << std::endl;

		//LOG_WARN(classLogger, "buffer size: " << m_size);

		if ((m_readCursor+offset >= 0) && (m_readCursor+offset < m_size))
		{
			//Utils::hexdump(&(m_rawBuffer[m_readCursor+offset]), m_size-(m_readCursor+offset));
		}
}
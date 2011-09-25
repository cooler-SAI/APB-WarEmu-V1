#include <vector>
#include <list>
#include <map>
#include <stdio.h>
#include <stdlib.h>

typedef __int64            int64;
typedef long               int32;
typedef short              int16;
typedef signed char        int8;
typedef unsigned __int64   uint64;
typedef unsigned long      uint32;
typedef unsigned short     uint16;
typedef unsigned char      uint8;
typedef uint64      OBJECT_HANDLE;

class ByteBuffer
{
    public:
        const static size_t DEFAULT_SIZE = 0xFFFF;

        ByteBuffer(): _rpos(0), _wpos(0)
        {
            _storage.reserve(DEFAULT_SIZE);
        }

        ByteBuffer(size_t res): _rpos(0), _wpos(0)
        {
            _storage.reserve(res);
        }

        ByteBuffer(const ByteBuffer &buf): _rpos(buf._rpos), _wpos(buf._wpos), _storage(buf._storage) { }

        void clear()
        {
            _storage.clear();
            _rpos = _wpos = 0;
        }

        template <typename T> void append(T value)
        {
            append((uint8 *)&value, sizeof(value));
        }

        template <typename T> void put(size_t pos,T value)
        {
            put(pos,(uint8 *)&value,sizeof(value));
        }

        ByteBuffer &operator<<(bool value)
        {
            append<char>((char)value);
            return *this;
        }

        ByteBuffer &operator<<(uint8 value)
        {
            append<uint8>(value);
            return *this;
        }

        ByteBuffer &operator<<(uint16 value)
        {
            append<uint16>(value);
            return *this;
        }

        ByteBuffer &operator<<(uint32 value)
        {
            append<uint32>(value);
            return *this;
        }

        ByteBuffer &operator<<(uint64 value)
        {
            append<uint64>(value);
            return *this;
        }

        ByteBuffer &operator<<(int8 value)
        {
            append<int8>(value);
            return *this;
        }

        ByteBuffer &operator<<(int16 value)
        {
            append<int16>(value);
            return *this;
        }

        ByteBuffer &operator<<(int32 value)
        {
            append<int32>(value);
            return *this;
        }

        ByteBuffer &operator<<(int64 value)
        {
            append<int64>(value);
            return *this;
        }

        ByteBuffer &operator<<(float value)
        {
            append<float>(value);
            return *this;
        }

        ByteBuffer &operator<<(double value)
        {
            append<double>(value);
            return *this;
        }

        ByteBuffer &operator<<(const std::string &value)
        {
            append((uint8 const *)value.c_str(), value.length());
            append((uint8)0);
            return *this;
        }

        ByteBuffer &operator<<(const char *str)
        {
            append((uint8 const *)str, str ? strlen(str) : 0);
            append((uint8)0);
            return *this;
        }

        ByteBuffer &operator>>(bool &value)
        {
            value = read<char>() > 0 ? true : false;
            return *this;
        }

        ByteBuffer &operator>>(uint8 &value)
        {
            value = read<uint8>();
            return *this;
        }

        ByteBuffer &operator>>(uint16 &value)
        {
            value = read<uint16>();
            return *this;
        }

        ByteBuffer &operator>>(uint32 &value)
        {
            value = read<uint32>();
            return *this;
        }

        ByteBuffer &operator>>(uint64 &value)
        {
            value = read<uint64>();
            return *this;
        }

        ByteBuffer &operator>>(int8 &value)
        {
            value = read<int8>();
            return *this;
        }

        ByteBuffer &operator>>(int16 &value)
        {
            value = read<int16>();
            return *this;
        }

        ByteBuffer &operator>>(int32 &value)
        {
            value = read<int32>();
            return *this;
        }

        ByteBuffer &operator>>(int64 &value)
        {
            value = read<int64>();
            return *this;
        }

        ByteBuffer &operator>>(float &value)
        {
            value = read<float>();
            return *this;
        }

        ByteBuffer &operator>>(double &value)
        {
            value = read<double>();
            return *this;
        }

        ByteBuffer &operator>>(std::string& value)
        {
            value.clear();
            while (rpos() < size())
            {
                char c=read<char>();
                if (c==0)
                    break;
                value+=c;
            }
            return *this;
        }

        uint8 operator[](size_t pos)
        {
            return read<uint8>(pos);
        }

        size_t rpos()
        {
            return _rpos;
        };

        size_t rpos(size_t rpos_)
        {
            _rpos = rpos_;
            return _rpos;
        };

        size_t wpos()
        {
            return _wpos;
        }

        size_t wpos(size_t wpos_)
        {
            _wpos = wpos_;
            return _wpos;
        }

        template <typename T> T read()
        {
            T r=read<T>(_rpos);
            _rpos += sizeof(T);
            return r;
        };
        template <typename T> T read(size_t pos) const
        {
            return *((T const*)&_storage[pos]);
        }

        void read(uint8 *dest, size_t len)
        {
            memcpy(dest, &_storage[_rpos], len);
            _rpos += len;
        }

        const uint8 *contents() const { return &_storage[0]; }

        size_t size() const { return _storage.size(); }
        bool empty() const { return _storage.empty(); }

        void resize(size_t newsize)
        {
            _storage.resize(newsize);
            _rpos = 0;
            _wpos = size();
        };
        void reserve(size_t ressize)
        {
            if (ressize > size()) _storage.reserve(ressize);
        };

        void append(const std::string& str)
        {
            append((uint8 const*)str.c_str(),str.size() + 1);
        }
        void append(const char *src, size_t cnt)
        {
            return append((const uint8 *)src, cnt);
        }
        void append(const uint8 *src, size_t cnt)
        {
            if (!cnt) return;

            if (_storage.size() < _wpos + cnt)
                _storage.resize(_wpos + cnt);
            memcpy(&_storage[_wpos], src, cnt);
            _wpos += cnt;
        }
        void append(const ByteBuffer& buffer)
        {
            if(buffer.size()) append(buffer.contents(),buffer.size());
        }

        void appendPackGUID(uint64 guid)
        {
            size_t mask_position = wpos();
            *this << uint8(0);
            for(uint8 i = 0; i < 8; i++)
            {
                if(guid & 0xFF)
                {
                    _storage[mask_position] |= (1<<i);
                    *this << ((uint8)(guid & 0xFF));
                }

                guid >>= 8;
            }
        }

        void put(size_t pos, const uint8 *src, size_t cnt)
        {
            memcpy(&_storage[pos], src, cnt);
        }
        void print_storage()
        {
                return;
        }

        void textlike()
        {
                return;
        }

        void hexlike()
        {
                return;
		}

        size_t _rpos, _wpos;
    protected:
        bool PrintPosError(bool add, size_t pos, size_t esize) const
        {
            return false;
        }
        std::vector<uint8> _storage;
};

template <typename T> ByteBuffer &operator<<(ByteBuffer &b, std::vector<T> v)
{
    b << (uint32)v.size();
    for (typename std::vector<T>::iterator i = v.begin(); i != v.end(); i++)
    {
        b << *i;
    }
    return b;
}

template <typename T> ByteBuffer &operator>>(ByteBuffer &b, std::vector<T> &v)
{
    uint32 vsize;
    b >> vsize;
    v.clear();
    while(vsize--)
    {
        T t;
        b >> t;
        v.push_back(t);
    }
    return b;
}

template <typename T> ByteBuffer &operator<<(ByteBuffer &b, std::list<T> v)
{
    b << (uint32)v.size();
    for (typename std::list<T>::iterator i = v.begin(); i != v.end(); i++)
    {
        b << *i;
    }
    return b;
}

template <typename T> ByteBuffer &operator>>(ByteBuffer &b, std::list<T> &v)
{
    uint32 vsize;
    b >> vsize;
    v.clear();
    while(vsize--)
    {
        T t;
        b >> t;
        v.push_back(t);
    }
    return b;
}

template <typename K, typename V> ByteBuffer &operator<<(ByteBuffer &b, std::map<K, V> &m)
{
    b << (uint32)m.size();
    for (typename std::map<K, V>::iterator i = m.begin(); i != m.end(); i++)
    {
        b << i->first << i->second;
    }
    return b;
}

template <typename K, typename V> ByteBuffer &operator>>(ByteBuffer &b, std::map<K, V> &m)
{
    uint32 msize;
    b >> msize;
    m.clear();
    while(msize--)
    {
        K k;
        V v;
        b >> k >> v;
        m.insert(make_pair(k, v));
    }
    return b;
}
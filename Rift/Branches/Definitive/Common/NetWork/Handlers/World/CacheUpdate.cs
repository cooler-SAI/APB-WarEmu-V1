using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Common;
using FrameWork;

namespace Common
{
    [Serializable]
    [ISerializableAttribute((long)Opcodes.CacheUpdate)]
    public class CacheUpdate : ISerializablePacket
    {
        [Unsigned7Bit(0)]
        public long CacheType;

        [Raw4Bit(1)]
        public uint CacheID;

        [ListBit(2)]
        public List<ISerializablePacket> CacheDatas;

        public override void OnRead(RiftClient From)
        {

        }
    }
}

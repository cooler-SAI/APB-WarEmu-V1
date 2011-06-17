using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork;

namespace Common
{
    [ISerializableAttribute((long)Opcodes.ProtocolHandshakeServerKey)]
    public class HandshakeServerKey : ISerializablePacket
    {
        [Unsigned7BitAttribute(0)]
        public long Nid;

        [Encoded7BitAttribute(1)]
        public long Unk;

        [ArrayBit(2)]
        public byte[] ServerKey;

        public override void OnRead(RiftClient From)
        {

        }
    }
}

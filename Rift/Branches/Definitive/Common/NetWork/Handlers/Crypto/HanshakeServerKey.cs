using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork;

namespace Common
{
    [Serializable]
    [ISerializableAttribute((long)Opcodes.ProtocolHandshakeServerKey)]
    public class HandshakeServerKey : ISerializablePacket
    {
        #region 1.1 Client

        [Unsigned7BitAttribute(0)]
        public long Nid;

        [ArrayBit(1)]
        public byte[] ServerKey;

        #endregion

        #region 1.2 Client

        //[Unsigned7BitAttribute(0)]
        //public long Nid;

        //[Encoded7BitAttribute(1)] 1.2 Client
        //public long Unk;

        //[ArrayBit(2)]
        //public byte[] ServerKey;

        #endregion

        public override void OnRead(RiftClient From)
        {

        }
    }
}

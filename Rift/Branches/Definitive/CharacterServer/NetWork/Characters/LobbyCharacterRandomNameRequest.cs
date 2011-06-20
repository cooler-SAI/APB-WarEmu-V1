using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Common;
using FrameWork;

namespace CharacterServer
{
    [ISerializableAttribute((long)Opcodes.LobbyCharacterRandomNameRequest)]
    public class LobbyCharacterRandomNameRequest : ISerializablePacket
    {
        [Unsigned7Bit(0)]
        public long Field0;

        [Unsigned7Bit(2)]
        public long Field2;

        [Unsigned7Bit(3)]
        public long Field3;

        [Unsigned7Bit(4)]
        public long Field4;

        public override void OnRead(RiftClient From)
        {
            Log.Success("RandomName", "0=" + Field0 + ",2=" + Field2 + ",3=" + Field3 + ",4=" + Field4);
        }
    }
}

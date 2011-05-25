using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Common;
using FrameWork;

namespace CharacterServer
{
    [ISerializableAttribute((long)Opcodes.LobbyWorldListResponse)]
    public class LobbyWorldListResponse : ISerializablePacket
    {
        [ListBit(1)]
        public List<ISerializablePacket> Realms = new List<ISerializablePacket>();

        public override void OnRead(RiftClient From)
        {

        }
    }
}

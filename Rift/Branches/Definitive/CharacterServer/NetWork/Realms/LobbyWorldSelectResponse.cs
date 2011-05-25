using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Common;

namespace CharacterServer
{
    [ISerializableAttribute((long)Opcodes.LobbyWorldSelectResponse)]
    public class LobbyWorldSelectResponse : ISerializablePacket
    {
        public override void OnRead(RiftClient From)
        {

        }
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Common;
using FrameWork;

namespace Common
{
    [Serializable]
    [ISerializableAttribute((long)Opcodes.LobbyCharacterListResponse)]
    public class LobbyCharacterListResponse : ISerializablePacket
    {
        [ListBit(1)]
        public List<ISerializablePacket> Characters = new List<ISerializablePacket>();
    }
}

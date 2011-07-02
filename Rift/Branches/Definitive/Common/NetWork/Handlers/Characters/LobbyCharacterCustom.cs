using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Common
{
    [Serializable]
    [ISerializableAttribute((long)Opcodes.LobbyCharacterCustom)]
    public class LobbyCharacterCustom : ISerializablePacket
    {

    }
}

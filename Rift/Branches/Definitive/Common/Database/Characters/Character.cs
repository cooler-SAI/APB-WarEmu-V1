using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork;

namespace Common
{
    [Serializable]
    [DataTable(DatabaseName = "Characters", TableName = "Characters", PreCache = false)]
    [ISerializableAttribute((long)Opcodes.LobbyCharacterEntry)]
    public class Character : ISerializablePacket
    {
        [DataElement()]
        [Unsigned7Bit(0)]
        public long AccountId;

        [DataElement()]
        [ArrayBit(1)]
        public string Email;

        [DataElement()]
        [Raw8Bit(2)]
        public long CharacterId;

        [DataElement()]
        [ArrayBit(3)]
        public string CharacterName;

        [DataElement()]
        [Unsigned7Bit(4)]
        public long Field4 = 2;

        [Relation(LocalField = "CharacterId", RemoteField = "CharacterId", AutoDelete = true, AutoLoad = true)]
        [PacketBit(5)]
        public CharacterInfo Info;

        [DataElement()]
        [BoolBit(6)]
        public bool Field6 = true;

        [DataElement()]
        [Raw8Bit(7)]
        public long Field7 = 129483019433300000;
    }
}
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
        public long Race;

        [BoolBit(0)]
        public bool Sex;

        [Unsigned7Bit(2)]
        public long Faction;

        [Unsigned7Bit(3)]
        public long Field3;

        [Unsigned7Bit(4)]
        public long Class;

        public override void OnRead(RiftClient From)
        {
            if(From.Acct == null || From.Rm == null)
                return;

            string Name = From.Rm.GetObject<CharactersMgr>().GetRandomName();

            ISerializablePacket Packet = new ISerializablePacket();
            Packet.Opcode = (int)Opcodes.LobbyCharacterRandomNameResponse;
            Packet.AddField(1, EPacketFieldType.ByteArray, Name);
            From.SendSerialized(Packet);
        }
    }
}

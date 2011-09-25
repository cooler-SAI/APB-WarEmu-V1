using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork;

namespace Common
{
    [Serializable]
    [ISerializableAttribute((long)Opcodes.LobbyCharacterCreateRequest)]
    public class LobbyCharacterCreateRequest : ISerializablePacket
    {
        [ArrayBit(0)]
        public string Name;

        [Unsigned7Bit(1)]
        public long Race = 1;
        // Race + Sex

        [Unsigned7Bit(2)]
        public long Sex = 0;

        [Raw4Bit(4)]
        public uint HeadID;

        [Raw4Bit(5)]
        public uint HairID;

        [BoolBit(6)]
        public bool Field6;

        [Raw4Bit(10)]
        public uint Field10;

        [Unsigned7Bit(13)]
        public long Class = 1; // Classe

        [PacketBit(28)]
        public LobbyCharacterCustom CharacterCustom;

        public override void OnRead(RiftClient From)
        {
            if (From.Acct == null || From.Rm == null)
                return;

            CharactersMgr Mgr = From.Rm.GetObject<CharactersMgr>();
            if (Mgr.GetCharactersCount(From.Acct.Id) >= 6)
            {
                From.Disconnect();
                return;
            }

            ISerializablePacket Response = new ISerializablePacket();
            Response.Opcode = (int)Opcodes.LobbyCharacterCreateResponse;
            if (Mgr.CharacterExist(Name))
                Response.AddField(0, EPacketFieldType.Unsigned7BitEncoded, (long)6);
            else
            {
                Response.AddField(0, EPacketFieldType.Unsigned7BitEncoded, (long)6);


                Character Char = new Character();
                Char.AccountId = From.Acct.Id;
                Char.CharacterName = Name;
                Char.Email = From.Acct.Email;

                Char.Info = new CharacterInfo();
                Char.Info.Class = Class;
                Char.Info.CustomPacket = this.CharacterCustom;
                Char.Info.Level = Mgr.StartingLevel;
                Char.Info.Race = Race;
                Char.Info.Sex = Sex;

                Mgr.AddCharacter(Char);
            }

            From.SendSerialized(Response);
        }
    }
}

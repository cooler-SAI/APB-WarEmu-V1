using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Common;
using FrameWork.Logger;
using FrameWork.NetWork;

namespace WorldServer
{
    [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.F_CREATE_CHARACTER, "onCreateCharacter")]
    public class F_CREATE_CHARACTER : IPacketHandler
    {
        public struct CreateInfo
        {
            public byte slot, race, career, sex, model;
            public UInt16 NameSize;
        }
        public void HandlePacket(BaseClient client, PacketIn packet)
        {
            GameClient cclient = client as GameClient;
            CreateInfo Info;

            Info.slot = packet.GetUint8();
            Info.race = packet.GetUint8();
            Info.career = packet.GetUint8();
            Info.sex = packet.GetUint8();
            Info.model = packet.GetUint8();
            Info.NameSize = packet.GetUint16();
            packet.Skip(2);

            byte[] Traits = new byte[8];
            packet.Read(Traits, 0, Traits.Length);
            packet.Skip(7);

            string Name = packet.GetString(Info.NameSize);

            if (!CharMgr.NameIsUsed(Name))
            {

                CharacterInfo CharInfo = CharMgr.GetCharacterInfo(Info.career);
                if (CharInfo == null)
                {
                    Log.Error("ON_CREATE", "Impossible de trouver la carrière :" + Info.career);
                    return;
                }

                Log.Success("OnCreate", "Création d'un nouveau personnage : " + Name);

                Character Char = new Character();
                Char.AccountId = cclient._Account.AccountId;
                Char.bTraits = Traits;
                Char.Career = Info.career;
                Char.CareerLine = CharInfo.CareerLine;
                Char.ModelId = Info.model;
                Char.Name = Name;
                Char.Race = Info.race;
                Char.Realm = CharInfo.Realm;
                Char.RealmId = Program.Rm.RealmId;
                Char.Sex = Info.sex;

                if (!CharMgr.CreateChar(Char))
                {
                    Log.Error("CreateCharacter", "Hack : création de + de 10 characters!");
                    return;
                }

                Character_items Citm = null;
                CharacterInfo_item[] Items = CharMgr.GetCharacterInfoItem(Char.CareerLine);

                for (int i = 0; i < Items.Length; ++i)
                {
                    if (Items[i] == null)
                        continue;

                    Citm = new Character_items();
                    Citm.Counts = Items[i].Count;
                    Citm.CharacterId = Char.CharacterId;
                    Citm.Entry = Items[i].Entry;
                    Citm.ModelId = Items[i].ModelId;
                    Citm.SlotId = Items[i].SlotId;
                    CharMgr.CreateItem(Citm);
                }

                Character_value CInfo = new Character_value();
                CInfo.CharacterId = Char.CharacterId;
                CInfo.Level = 1;
                CInfo.Money = 0;
                CInfo.Online = false;
                CInfo.RallyPoint = CharInfo.RallyPt;
                CInfo.RegionId = CharInfo.Region;
                CInfo.Renown = 0;
                CInfo.RenownRank = 1;
                CInfo.RestXp = 0;
                CInfo.Skills = CharInfo.Skills;
                CInfo.Speed = 100;
                CInfo.WorldO = CharInfo.WorldO;
                CInfo.WorldX = CharInfo.WorldX;
                CInfo.WorldY = CharInfo.WorldY;
                CInfo.WorldZ = CharInfo.WorldZ;
                CInfo.Xp = 0;
                CInfo.ZoneId = CharInfo.ZoneId;

                CharMgr.Database.AddObject(CInfo);

                Char.Value = new Character_value[1] { CInfo };
            }
            PacketOut Out = new PacketOut((byte)Opcodes.F_SEND_CHARACTER_RESPONSE);
            Out.WriteString(cclient._Account.Username,24);
            cclient.SendTCP(Out);
        }
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Common;
using FrameWork.Logger;
using FrameWork.NetWork;

namespace WorldServer
{
    [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.F_QUEST, "onQuest")]
    public class F_QUEST : IPacketHandler
    {
        public void HandlePacket(BaseClient client, PacketIn packet)
        {
            GameClient cclient = client as GameClient;

            UInt16 QuestID = packet.GetUint16();
            UInt16 State = packet.GetUint16();
            UInt16 Unk1 = packet.GetUint16();
            byte Unk2 = packet.GetUint8();
            byte Unk3 = packet.GetUint8();
            UInt16 Unk4 = packet.GetUint16();
            UInt16 CreatureOID = packet.GetUint16();

            switch (State)
            {
                case 1: // Show Quest
                    {
                        Creature Crea = cclient.Plr.Region.GetObject(CreatureOID) as Creature;
                        if (Crea != null)
                            Crea.QtsInterface.BuildQuest(QuestID, cclient.Plr);

                    }break;

            };


        }
    }
}

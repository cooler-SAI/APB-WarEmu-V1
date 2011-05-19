using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Common;
using FrameWork.Logger;
using FrameWork.NetWork;

namespace WorldServer
{
    [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.F_RANDOM_NAME_LIST_INFO, "onRandomNameListInfo")]
    public class F_RANDOM_NAME_LIST_INFO : IPacketHandler
    {
        struct RandomNameInfo
        {
            public byte Race, Unk, Slot;
        }

        public void HandlePacket(BaseClient client, PacketIn packet)
        {
            GameClient cclient = client as GameClient;
            RandomNameInfo Info = BaseClient.ByteToType<RandomNameInfo>(packet);

            Random_name[] Names = CharMgr.GetRandomNames();

            PacketOut Out = new PacketOut((byte)Opcodes.F_RANDOM_NAME_LIST_INFO);
            Out.WriteByte(0);
            Out.WriteByte(Info.Unk);
            Out.WriteByte(Info.Slot);
            Out.WriteUInt16(0);
            Out.WriteByte((byte)Names.Length);

            for (int i = Names.Length - 1; i >= 0; --i)
                Out.FillString(Names[i].Name, Names[i].Name.Length + 1);

            cclient.SendTCP(Out);
        }
    }
}

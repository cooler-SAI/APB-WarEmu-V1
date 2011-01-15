using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork.Logger;
using FrameWork.NetWork;

using Common;

namespace LobbyServer
{
    [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.ASK_WORLD_ENTER, "onAskWorldEnter")]
    public class ASK_WORLD_ENTER : IPacketHandler
    {
        public int HandlePacket(BaseClient client, PacketIn packet)
        {
            LobbyClient cclient = (LobbyClient)client;

            byte sloid = packet.GetUint8();
            Program.CharMgr.SetEnter(cclient.Account.Id, sloid);

            WorldInfo Info = Program.CharMgr.GetWorldInfo(cclient.Account.WorldId);

            PacketOut Out = new PacketOut((UInt32)Opcodes.ANS_WORLD_ENTER);

            if (Info == null)
                Out.WriteUInt32Reverse(1);
            else
            {
                Out.WriteUInt32Reverse(0);

                Out.WriteInt32Reverse(Info.Ip); // WorldServerIp
                Out.WriteUInt16Reverse((UInt16)Info.Port); // Port
                Out.WriteInt64Reverse(TCPManager.GetTimeStamp());
            }

            cclient.SendTCP(Out);

            return 0;
        }
    }
}

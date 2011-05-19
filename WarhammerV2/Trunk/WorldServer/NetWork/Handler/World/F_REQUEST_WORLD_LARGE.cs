using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Common;
using FrameWork.Logger;
using FrameWork.NetWork;

namespace WorldServer
{
    [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.F_REQUEST_WORLD_LARGE, "onRequestWorldLarge")]
    public class F_REQUEST_WORLD_LARGE : IPacketHandler
    {
        public void HandlePacket(BaseClient client, PacketIn packet)
        {
            GameClient cclient = client as GameClient;

            if (cclient.Plr == null)
                return;

            if (!cclient.Plr.IsLoad())
                return;

            PacketOut Out = new PacketOut((byte)Opcodes.F_SET_TIME);
            Out.WriteUInt32((uint)TCPServer.GetTimeStamp());
            Out.WriteUInt32(7);
            cclient.Plr.SendPacket(Out);

            Out = new PacketOut((byte)Opcodes.S_WORLD_SENT);
            Out.WriteByte(0);
            cclient.Plr.SendPacket(Out);
        }
    }
}

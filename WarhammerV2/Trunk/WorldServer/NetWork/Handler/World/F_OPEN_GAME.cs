using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Common;
using FrameWork.Logger;
using FrameWork.NetWork;

namespace WorldServer
{
    [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.F_OPEN_GAME, "onOpenGame")]
    public class F_OPEN_GAME : IPacketHandler
    {
        public void HandlePacket(BaseClient client, PacketIn packet)
        {
            GameClient cclient = client as GameClient;

            PacketOut Out = new PacketOut((byte)Opcodes.S_GAME_OPENED);

            if (cclient.Plr == null)
                Out.WriteByte(1);
            else
                Out.WriteByte(0);

            cclient.SendTCP(Out);
        }
    }
}

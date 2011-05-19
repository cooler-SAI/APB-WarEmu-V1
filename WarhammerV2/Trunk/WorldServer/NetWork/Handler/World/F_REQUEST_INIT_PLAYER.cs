using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Common;
using FrameWork.Logger;
using FrameWork.NetWork;

namespace WorldServer
{
    [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.F_REQUEST_INIT_PLAYER, "onRequestInitPlayer")]
    public class F_REQUEST_INIT_PLAYER : IPacketHandler
    {
        public void HandlePacket(BaseClient client, PacketIn packet)
        {
            GameClient cclient = client as GameClient;

            Log.Dump("F_REQUEST_INIT_PLAYER", packet.ToArray(), 0, packet.ToArray().Length);

            if (cclient.Plr == null)
                return;

            foreach (Player Obj in cclient.Plr._PlayerRanged)
                Obj.SendMeTo(cclient.Plr);
        }
    }
}

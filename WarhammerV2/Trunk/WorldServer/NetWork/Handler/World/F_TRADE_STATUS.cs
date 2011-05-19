using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Common;
using FrameWork.Logger;
using FrameWork.NetWork;

namespace WorldServer
{
    [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.F_TRADE_STATUS, "onTradeStatus")]
    public class F_TRADE_STATUS : IPacketHandler
    {
        public void HandlePacket(BaseClient client, PacketIn packet)
        {
            GameClient cclient = client as GameClient;
            if (!cclient.IsPlaying())
                return;

            cclient.Plr.ItmInterface.HandleTrade(packet);
        }
    }
}

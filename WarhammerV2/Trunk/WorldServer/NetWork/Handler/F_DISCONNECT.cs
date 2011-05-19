using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Common;
using FrameWork.Logger;
using FrameWork.NetWork;

namespace WorldServer
{
    [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.F_DISCONNECT, "onDisconnect")]
    public class F_DISCONNECT : IPacketHandler
    {
        public void HandlePacket(BaseClient client, PacketIn packet)
        {
            GameClient cclient = client as GameClient;

            if (cclient._Account == null)
                return;

            Log.Succes("F_DISCONNECT", "MeId=" + cclient.Id);

            GameClient OtherClient = (client.Server as TCPServer).GetClientByAccount(cclient,cclient._Account.AccountId);
            if (OtherClient != null)
                OtherClient.Disconnect();
        }
    }
}

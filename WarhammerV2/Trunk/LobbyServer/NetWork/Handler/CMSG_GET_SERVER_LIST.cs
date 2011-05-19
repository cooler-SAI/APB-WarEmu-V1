using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Common;

using FrameWork.NetWork;

namespace LobbyServer.NetWork.Handler
{
    [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.CMSG_GET_SERVER_LIST, "onGetServerList")]
    public class CMSG_GET_SERVER_LIST : IPacketHandler
    {
        public void HandlePacket(BaseClient client, PacketIn packet)
        {
            Client cclient = client as Client;

            uint sequence = packet.GetUint32();

            byte[] Res = Program.AcctMgr.BuildRealms(sequence);

            PacketOut Out = new PacketOut((byte)Opcodes.SMSG_GET_SERVER_LIST_RESPONSE);
            Out.Write(Res, 0, Res.Length);

            cclient.SendTCPCuted(Out);
        }
    }
}

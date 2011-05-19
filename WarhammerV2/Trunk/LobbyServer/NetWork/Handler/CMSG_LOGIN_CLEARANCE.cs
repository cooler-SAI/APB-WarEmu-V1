using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork.NetWork;

namespace LobbyServer.NetWork.Handler
{
    [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.CMSG_LOGIN_CLEARANCE, "onLoginClearance")]
    public class CMSG_LOGIN_CLEARANCE : IPacketHandler
    {
        public void HandlePacket(BaseClient client, PacketIn packet)
        {
            Client cclient = client as Client;

            UInt32 sequence = packet.GetUint32();

            PacketOut Out = new PacketOut((byte)Opcodes.SMSG_LOGIN_CLEARANCE_RESPONSE);
            Out.WriteUInt32(sequence);
            Out.WriteUInt32(0);
            Out.WriteUInt16(1);
            Out.WriteUInt16(1);
            Out.WriteUInt32(1);

            cclient.SendTCPCuted(Out);
        }
    }
}

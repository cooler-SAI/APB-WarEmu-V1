using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork.NetWork;

namespace LobbyServer.NetWork.Handler
{
    [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.CMSG_CONNECT, "onConnect")]
    public class CMSG_CONNECT : IPacketHandler
    {
        public void HandlePacket(BaseClient client, PacketIn packet)
        {
            Client cclient = client as Client;

            UInt32 sequence = packet.GetUint32();
            UInt32 unk = packet.GetUint32();

            PacketOut Out = new PacketOut((byte)Opcodes.SMSG_CONNECT_RESPONSE);
            Out.WriteUInt32(sequence);
            Out.WriteUInt16(0);
            Out.WriteUInt32(unk);

            cclient.SendTCPCuted(Out);
        }
    }
}

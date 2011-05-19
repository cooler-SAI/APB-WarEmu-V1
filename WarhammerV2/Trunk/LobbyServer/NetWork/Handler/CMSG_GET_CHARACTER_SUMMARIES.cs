using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork.NetWork;

namespace LobbyServer.NetWork.Handler
{
    [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.CMSG_GET_CHARACTER_SUMMARIES, "onGetCharacterSummaries")]
    public class CMSG_GET_CHARACTER_SUMMARIES : IPacketHandler
    {
        public void HandlePacket(BaseClient client, PacketIn packet)
        {
            Client cclient = client as Client;

            uint sequence = packet.GetUint32();

            PacketOut Out = new PacketOut((byte)Opcodes.SMSG_GET_CHARACTER_SUMMARIES_RESPONSE);
            Out.WriteUInt32(sequence);
            Out.WriteUInt16(0);
            Out.WriteUInt16(0); // Character count

            cclient.SendTCPCuted(Out);
        }
    }
}

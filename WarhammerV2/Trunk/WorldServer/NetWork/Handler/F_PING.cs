using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Common;
using FrameWork.Logger;
using FrameWork.NetWork;

namespace WorldServer
{
    [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.F_PING, "onPing")]
    public class F_PING : IPacketHandler
    {
        public void HandlePacket(BaseClient client, PacketIn packet)
        {
            GameClient cclient = client as GameClient;

            uint Timestamp = packet.GetUint32();

            PacketOut Out = new PacketOut((byte)Opcodes.S_PONG);
            Out.WriteUInt32(Timestamp);
            Out.WriteUInt64((UInt64)TCPManager.GetTimeStamp());
            Out.WriteUInt32((UInt32)(cclient.SequenceID+1));
            Out.WriteUInt32(0);
            cclient.SendTCP(Out);
        }
    }
}

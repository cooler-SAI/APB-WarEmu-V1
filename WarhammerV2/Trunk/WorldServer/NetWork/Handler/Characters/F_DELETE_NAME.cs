using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Common;
using FrameWork.Logger;
using FrameWork.NetWork;

namespace WorldServer
{
    [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.F_DELETE_NAME, "onDeleteName")]
    public class F_DELETE_NAME : IPacketHandler
    {
        public void HandlePacket(BaseClient client, PacketIn packet)
        {
            GameClient cclient = client as GameClient;

            string CharName = packet.GetString(30);
            string UserName = packet.GetString(20);

            byte Bad = 0;

            if (CharName.Length <= 0 || CharMgr.NameIsUsed(CharName))
                Bad = 1;

            Log.Debug("F_DELETE_NAME", "Bad=" + Bad + ",Name=" + CharName);

            PacketOut Out = new PacketOut((byte)Opcodes.F_CHECK_NAME);
            Out.WriteString(CharName, 30);
            Out.WriteString(UserName, 20);
            Out.WriteByte(Bad);
            Out.WriteByte(0);
            Out.WriteByte(0);
            Out.WriteByte(0);
            cclient.SendTCP(Out);
        }
    }
}

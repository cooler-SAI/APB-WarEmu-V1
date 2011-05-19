using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Common;
using FrameWork.Logger;
using FrameWork.NetWork;

namespace WorldServer
{
    [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.F_REQUEST_CHAR_TEMPLATES, "onRequestCharTemplates")]
    public class F_REQUEST_CHAR_TEMPLATES : IPacketHandler
    {
        public void HandlePacket(BaseClient client, PacketIn packet)
        {
            GameClient cclient = client as GameClient;

            PacketOut Out = new PacketOut((byte)Opcodes.F_REQUEST_CHAR_TEMPLATES);
            Out.Write(new byte[0x11], 0, 0x11);
            cclient.SendTCP(Out);
        }
    }
}

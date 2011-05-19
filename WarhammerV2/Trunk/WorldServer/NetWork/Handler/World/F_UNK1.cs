using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Common;
using FrameWork.Logger;
using FrameWork.NetWork;

namespace WorldServer
{
    [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.F_UNK1, "onUnk1")]
    public class F_UNK1 : IPacketHandler
    {
        public void HandlePacket(BaseClient client, PacketIn packet)
        {

        }
    }
}

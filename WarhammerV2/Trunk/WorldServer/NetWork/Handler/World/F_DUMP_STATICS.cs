using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Common;
using FrameWork.Logger;
using FrameWork.NetWork;

namespace WorldServer
{
    [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.F_DUMP_STATICS, "onDumpStatics")]
    public class F_DUMP_STATICS : IPacketHandler
    {
        public void HandlePacket(BaseClient client, PacketIn packet)
        {
            GameClient cclient = client as GameClient;

            if (cclient.Plr == null || !cclient.Plr.IsInWorld())
                return;

            UInt32 Unk1 = packet.GetUint32();
            UInt16 Unk2 = packet.GetUint16();
            UInt16 OffX = packet.GetUint16();
            UInt16 Unk3 = packet.GetUint16();
            UInt16 OffY = packet.GetUint16();

            Log.Succes("F_DUMP_STATIC", "X=" + OffX + ",Y=" + OffY);

            cclient.Plr.SetOffset(OffX, OffY);
        }
    }
}

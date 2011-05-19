using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Common;
using FrameWork.Logger;
using FrameWork.NetWork;

namespace WorldServer
{
    [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.F_TRANSFER_ITEM, "onTransferItem")]
    public class F_TRANSFER_ITEM : IPacketHandler
    {
        public void HandlePacket(BaseClient client, PacketIn packet)
        {
            GameClient cclient = client as GameClient;

            if (cclient.Plr == null || !cclient.Plr.IsInWorld())
                return;

            ushort Oid = packet.GetUint16();
            ushort To = packet.GetUint16();
            ushort From = packet.GetUint16();
            ushort Count = packet.GetUint16();

            cclient.Plr.ItmInterface.MoveSlot(From, To, Count);
        }
    }
}

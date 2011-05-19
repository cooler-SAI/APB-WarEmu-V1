using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Common;
using FrameWork.Logger;
using FrameWork.NetWork;

namespace WorldServer
{
    public struct InteractMenu
    {
        public UInt16 Oid;
        public UInt16 Menu;
        public byte Page;
        public byte Num;
        public UInt16 Unk;
        public UInt16 Count;
        public UInt16 SellCount;
    }

    [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.F_INTERACT, "onInteract")]
    public class F_INTERACT : IPacketHandler
    {
        public void HandlePacket(BaseClient client, PacketIn packet)
        {
            GameClient cclient = client as GameClient;

            if (cclient.Plr == null || !cclient.Plr.IsInWorld())
                return;

            Log.Dump("F_INTERACT", packet.ToArray(), 0, packet.ToArray().Length);

            InteractMenu Menu = new InteractMenu();
            Menu.Oid = packet.GetUint16();
            Menu.Menu = packet.GetUint16();
            Menu.Page = packet.GetUint8();
            Menu.Num = packet.GetUint8();
            Menu.Unk = packet.GetUint16();
            Menu.SellCount = packet.GetUint16();
            Menu.Count = packet.GetUint16();

            Object Obj = cclient.Plr.Region.GetObject(Menu.Oid);
            if (Obj == null)
                return;

            if (Obj.GetDistanceTo(cclient.Plr) > 15)
                return;

            Obj.SendInteract(cclient.Plr,Menu);
        }
    }
}

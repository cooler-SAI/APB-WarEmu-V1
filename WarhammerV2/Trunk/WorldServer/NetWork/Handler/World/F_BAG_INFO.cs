using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Common;
using FrameWork.Logger;
using FrameWork.NetWork;

namespace WorldServer
{
    [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.F_BAG_INFO, "onBagInfo")]
    public class F_BAG_INFO : IPacketHandler
    {
        public void HandlePacket(BaseClient client, PacketIn packet)
        {
            GameClient cclient = client as GameClient;

            if (!cclient.IsPlaying())
                return;

            byte SlotCount = packet.GetUint8();
            byte Price = packet.GetUint8();

            Player Plr = cclient.Plr;

            if(!Plr.ItmInterface.HasMaxBag())
                if(Plr.HaveMoney(Plr.ItmInterface.GetBagPrice()))
                {
                    if (Plr.RemoveMoney(Plr.ItmInterface.GetBagPrice()))
                    {
                        ++Plr.ItmInterface.BagBuy;
                        Plr.ItmInterface.SendMaxInventory(Plr);
                    }
                }
        }
    }
}

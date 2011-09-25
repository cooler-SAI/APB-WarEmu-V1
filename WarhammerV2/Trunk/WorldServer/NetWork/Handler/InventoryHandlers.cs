/*
 * Copyright (C) 2011 APS
 *	http://AllPrivateServer.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
 
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Common;
using FrameWork;

namespace WorldServer
{
    public class InventoryHandlers : IPacketHandler
    {
        [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.F_BAG_INFO, "onBagInfo")]
        static public void F_BAG_INFO(BaseClient client, PacketIn packet)
        {
            GameClient cclient = client as GameClient;

            if (!cclient.IsPlaying())
                return;

            byte SlotCount = packet.GetUint8();
            byte Price = packet.GetUint8();

            Player Plr = cclient.Plr;

            if (!Plr.ItmInterface.HasMaxBag())
                if (Plr.HaveMoney(Plr.ItmInterface.GetBagPrice()))
                {
                    if (Plr.RemoveMoney(Plr.ItmInterface.GetBagPrice()))
                    {
                        ++Plr.ItmInterface.BagBuy;
                        Plr.ItmInterface.SendMaxInventory(Plr);
                    }
                }
        }

        [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.F_TRANSFER_ITEM, "onTransferItem")]
        static public void F_TRANSFER_ITEM(BaseClient client, PacketIn packet)
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

        [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.F_TRADE_STATUS, "onTradeStatus")]
        static public void F_TRADE_STATUS(BaseClient client, PacketIn packet)
        {
            GameClient cclient = client as GameClient;
            if (!cclient.IsPlaying())
                return;

            cclient.Plr.ItmInterface.HandleTrade(packet);
        }
    }
}

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

using FrameWork;

using Common;

namespace LobbyServer
{
    public class ASK_CHARACTER_CREATE : IPacketHandler
    {
        [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.ASK_CHARACTER_CREATE, "onAskCharacterCreate")]
        static public void HandlePacket(BaseClient client, PacketIn packet)
        {
            LobbyClient cclient = client as LobbyClient;

            byte freeslot = Program.CharMgr.GetFreeSlot(cclient.Account.Id);

            if (freeslot == 0 || cclient.CreateChar == null)
                ANS_CHARACTER_CREATE.Send(cclient);
            else
            {
                cclient.CreateChar.SlotId = freeslot;
                cclient.CreateChar.Faction = packet.GetUint8();
                cclient.CreateChar.Gender = packet.GetUint8();
                cclient.CreateChar.Version = (int)packet.GetUint32R();
                cclient.CreateChar.Seconds = (int)packet.GetUint32R();

                byte[] Custom = new byte[packet.Length - packet.Position];
                packet.Read(Custom, 0, Custom.Length);

                cclient.CreateChar.Custom = BitConverter.ToString(Custom);

                Program.CharMgr.CreateCharacter(cclient.CreateChar);
                ANS_CHARACTER_CREATE.Send(cclient);
            }
        }
    }
}

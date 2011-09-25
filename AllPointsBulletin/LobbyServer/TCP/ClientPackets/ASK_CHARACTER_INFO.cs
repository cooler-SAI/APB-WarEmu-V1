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

using FrameWork.Logger;
using FrameWork.NetWork;

using Common;

namespace LobbyServer
{
    [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.ASK_CHARACTER_INFO, "onAskCharacterInfo")]
    public class ASK_CHARACTER_INFO : IPacketHandler
    {
        public int HandlePacket(BaseClient client, PacketIn packet)
        {
            LobbyClient cclient = client as LobbyClient;

            byte slotid = packet.GetUint8();

            CharacterInfo Info = Program.CharMgr.GetInfoBySlotId(cclient.Account.Id, slotid);

            PacketOut Out = new PacketOut((UInt32)Opcodes.ANS_CHARACTER_INFO);

            if (Info == null || (Info != null & Info.Character == null))
                Out.WriteUInt32Reverse(1);
            else
            {
                DBCharacter Char = Info.Character;

                Out.WriteUInt32Reverse(0); // Return code

                Out.WriteByte(Char.SlotId); // Slot
                Out.WriteByte(Char.Gender); // Gender
                Out.WriteUInt32Reverse(1); // PlayTime
                Out.WriteUInt32Reverse((UInt32)Char.Rank); // Ranking
                Out.WriteByte(Char.Threat); // Threat

                Out.WriteUInt32Reverse((UInt32)(40000));

                Out.WriteParsedString("APB-EMU", 60); // Clan Name

                byte[] Custom = Char.GetaCustom();
                Out.Write(Custom, 0, Custom.Length);
            }

            cclient.SendTCP(Out);

            return 0;
        }
    }
}

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

using FrameWork.NetWork;

using Common;

namespace LobbyServer
{
    static public class ANS_CHARACTER_INFO
    {
        static public void Send(LobbyClient client)
        {
            Program.CharMgr.LoadCharacters(client.Account.Id);

            CharacterInfo[] _Chars = Program.CharMgr.GetInfos(client.Account.Id);
            WorldInfo Info = Program.CharMgr.GetWorldInfo(client.Account.WorldId);

            PacketOut Out = new PacketOut((UInt32)Opcodes.CHARACTER_LIST);
            Out.WriteByte((byte)_Chars.Length);
            foreach (CharacterInfo Char in _Chars)
            {
                Out.WriteByte(Char.Character.SlotId);
                Out.WriteByte(Char.Character.Faction);
                Out.WriteByte((byte)(Info.RpcID == 0 ? 0 : 1));
                Out.WriteUInt32Reverse((UInt32)Info._Info.Id);
                Out.WriteParsedString(Info._Info.Name, 32);
                Out.WriteParsedString(Char.Character.Name, 32);
            }

            client.SendTCP(Out);

            if (_Chars.Length <= 0)
                WORLD_LIST.Send(client);
        }
    }
}

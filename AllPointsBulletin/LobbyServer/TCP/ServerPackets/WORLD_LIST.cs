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
    static public class WORLD_LIST
    {
        static public void Send(LobbyClient client)
        {
            List<WorldInfo> _Worlds = Program.CharMgr.GetWorlds();

            PacketOut Out = new PacketOut((UInt32)Opcodes.WORLD_LIST);
            Out.WriteInt32R(0); // Returned Code 

            Out.WriteUInt16R((UInt16)_Worlds.Count); // World Count

            foreach (WorldInfo Info in _Worlds)
            {
                Out.WriteUInt32R((UInt32)Info._Info.Id); // WorldUid

                Out.WriteUnicodeString(Info._Info.Name, 32); // WorldName

                Out.WriteByte((byte)(Info.RpcID == 0 ? 0 : 1)); // Status
                Out.WriteByte(Info._Info.Population); // Pop  0:Low 1:Medium 2:High
                Out.WriteByte(Info._Info.Enf); // enf recommended , true or false
                Out.WriteByte(Info._Info.Crim); // crim recommended , true or false
            }

            client.SendTCP(Out);
        }
    }
}

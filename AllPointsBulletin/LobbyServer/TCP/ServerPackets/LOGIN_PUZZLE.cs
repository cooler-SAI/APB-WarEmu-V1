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

namespace LobbyServer
{
    static public class LOGIN_PUZZLE
    {
        static public void Send(LobbyClient client)
        {
            PacketOut Out = new PacketOut((UInt32)Opcodes.LOGIN_PUZZLE);
            Out.WriteInt32R(Program.Version[0]);
            Out.WriteInt32R(Program.Version[1]);
            Out.WriteInt32R(Program.Version[2]);
            Out.WriteInt32R(Program.Build);

            Out.WriteByte(0x05);

            Out.WriteInt64R(-313054979819954861);

            Out.WriteUInt32R(0);
            Out.WriteUInt32R(0);
            Out.WriteUInt32R(0);
            client.SendTCP(Out);
        }
    }
}

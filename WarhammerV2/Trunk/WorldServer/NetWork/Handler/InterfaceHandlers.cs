/*
 * Copyright (C) 2011 APS
 *   http://AllPrivateServer.com
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
    public class InterfaceHandlers : IPacketHandler
    {
        [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.F_INTERFACE_COMMAND, (int)eClientState.WorldEnter, "onInterfaceCommand")]
        static public void F_INTERFACE_COMMAND(BaseClient client, PacketIn packet)
        {
            GameClient cclient = client as GameClient;

            if (cclient.Plr == null)
                return;

            byte CommandId = packet.GetUint8();

            switch (CommandId)
            {
                case 0: // ????
                    {
                    } break;

                case 1: // ????
                    {
                    } break;

                case 2: // Resurrect Button
                    {
                        cclient.Plr.PreRespawnPlayer();
                    } break;

            };

        }
    }
}
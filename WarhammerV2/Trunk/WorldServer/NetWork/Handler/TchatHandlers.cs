/*
 * Copyright (C) 2013 APS
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
    public class TchatHandlers : IPacketHandler
    {
        [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.F_TEXT, (int)eClientState.Playing, "onText")]
        static public void F_TEXT(BaseClient client, PacketIn packet)
        {
            GameClient cclient = client as GameClient;

            if (cclient.Plr == null)
                return;

            byte Unk = packet.GetUint8();
            string Text = packet.GetString((int)(packet.Length - packet.Position));

            CommandMgr.HandleText(cclient.Plr, Text);
        }

        [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.F_EMOTE, (int)eClientState.Playing, "onEmote")]
        static public void F_EMOTE(BaseClient client, PacketIn packet)
        {
            GameClient cclient = client as GameClient;

            if (cclient.Plr == null)
                return;

            UInt32 emote = packet.GetUint32();

            PacketOut Out = new PacketOut((byte)Opcodes.F_EMOTE);
            Out.WriteUInt16(cclient.Plr.Oid);
            Out.WriteUInt16((UInt16)emote);
            if (cclient.Plr.CbtInterface.HasTarget(GameData.TargetTypes.TARGETTYPES_TARGET_ALLY))
                Out.WriteUInt16(cclient.Plr.CbtInterface.Targets[(int)GameData.TargetTypes.TARGETTYPES_TARGET_ALLY]);
            cclient.Plr.DispatchPacket(Out, false);
            cclient.Plr.SendPacket(Out);

        }
    }
}

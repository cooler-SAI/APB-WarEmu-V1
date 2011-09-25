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
using FrameWork.Database;

using Common;

namespace WorldServer
{
    [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.ASK_WORLD_ENTER, "onAskWorldEnter")]
    public class ASK_WORLD_ENTER : IPacketHandler
    {
        public int HandlePacket(BaseClient client, PacketIn packet)
        {
            WorldClient cclient = (WorldClient)client;
            UInt32 AcctId = packet.GetUint32Reversed();

            Log.Debug("AskWorldEnter", "New client, Account = " + AcctId);

            cclient.Account = Program.CharMgr.GetAccount((int)AcctId);
            cclient.Character = Program.CharMgr.GetInfoForEnter((int)AcctId);

            PacketOut Out = new PacketOut((UInt32)Opcodes.ANS_WORLD_ENTER);

            if (cclient.Account == null || cclient.Character == null)
                Out.WriteInt32Reverse(1);
            else
            {
                Out.WriteInt32Reverse(0);
                Out.WriteInt32Reverse(cclient.Character.Character.Id);
                Out.WriteInt32Reverse(cclient.Account.Points); // Points
                Out.WriteByte(1);
                Out.WriteInt64Reverse(TCPManager.GetTimeStamp());
                Out.WriteFloat(5.00f); // fMarketPlaceMinimumBidPct
                Out.WriteByte(0); // GroupPublic
                Out.WriteByte(0); // GroupInvite

                Out.WriteInt32Reverse(Program.FileMgr.GetFileVersion(cclient.Account.Id, 1, false, Program.WorldName, cclient.Character.Character.Name)); // ConfigFileVersion
                Out.WriteInt32Reverse(Program.FileMgr.GetFileVersion(cclient.Account.Id, 2, false, Program.WorldName, cclient.Character.Character.Name));
                Out.WriteInt32Reverse(Program.FileMgr.GetFileVersion(cclient.Account.Id, 3, false, Program.WorldName, cclient.Character.Character.Name));
                Out.WriteInt32Reverse(Program.FileMgr.GetFileVersion(cclient.Account.Id, 4, false, Program.WorldName, cclient.Character.Character.Name));
                Out.WriteInt32Reverse(Program.FileMgr.GetFileVersion(cclient.Account.Id, 5, false, Program.WorldName, cclient.Character.Character.Name));

                Out.WriteByte(1); // Tutorial Complete
                Out.WriteByte(0); // lookInForGroup
            }

            cclient.SendTCP(Out);

            return 0;
        }
    }
}

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
    public class ASK_CONFIGFILE_SAVE : IPacketHandler
    {
        [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.ASK_CONFIGFILE_SAVE, "onAskConfigfileSave")]
        static public void HandlePacket(BaseClient client, PacketIn packet)
        {
            LobbyClient cclient = (LobbyClient)client;

            byte FileId = packet.GetUint8();
            UInt32 Version = packet.GetUint32R();

            byte[] File = new byte[packet.Length - packet.Position];
            packet.Read(File, 0, File.Length);

            File = ZlibMgr.Decompress(File);

            Log.Debug("ConfigSave", "Config saved : fileid=" + FileId + ", Version=" + Version + ",Size=" + File.Length);

            Program.FileMgr.SaveInfo(cclient.Account.Id, FileId, File);

            PacketOut Out = new PacketOut((UInt32)Opcodes.ANS_CONFIGFILE_SAVE);
            Out.WriteUInt32R(0);
            Out.WriteByte(FileId);
            Out.Write(File, 0, File.Length);
            cclient.SendTCP(Out);
        }
    }
}

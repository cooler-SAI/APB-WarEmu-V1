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

namespace LauncherServer
{
    public class Client : BaseClient
    {
        public long LastInfoRequest = 0;

        public Client(TCPManager srv)
            : base(srv)
        {

        }

        public override void OnConnect()
        {
            Log.Debug("Client", "Connexion " + GetIp);
        }

        public override void OnDisconnect()
        {
            Log.Debug("Client", "Deconnexion " + GetIp);
        }

        protected override void OnReceive(byte[] Packet)
        {
            lock (this)
            {
                PacketIn pack = new PacketIn(Packet, 0, Packet.Length);
                pack.Size = pack.GetUint32();
                pack.Opcode = pack.GetUint8();

                if (!Enum.IsDefined(typeof(Opcodes), (byte)pack.Opcode))
                {
                    Log.Error("OnReceive", "Opcode invalide : " + pack.Opcode);
                    return;
                }

                Server.HandlePacket((BaseClient)this, pack);
            }
        }
    }
}

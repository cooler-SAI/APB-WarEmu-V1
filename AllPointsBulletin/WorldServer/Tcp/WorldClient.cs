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

namespace WorldServer
{
    public class WorldClient : BaseClient
    {
        public DBAccount Account = null;
        public CharacterInfo Character = null;

        #region Base

        public WorldClient(TCPManager srv)
            : base(srv)
        {
        }

        public override void OnConnect()
        {

        }

        public override void OnDisconnect()
        {

        }

        #endregion

        #region TCP

        protected override void OnReceive(PacketIn packet)
        {
            lock (this)
            {
                packet.Size = packet.GetUint32Reversed();
                packet = DeCrypt(packet);
                packet.Opcode = packet.GetUint32Reversed();

                Server.HandlePacket(this, packet);
            }
        }

        #endregion
    }
}

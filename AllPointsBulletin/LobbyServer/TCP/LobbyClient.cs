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
using FrameWork.NetWork.Crypt.Crypto;

using Common;

namespace LobbyServer
{
    public class LobbyClient : BaseClient
    {
        public DBAccount Account = null;
        public DBCharacter CreateChar = null;

        #region SRP

        public SecureRandom random = new SecureRandom();
        public Srp6Server serv = new Srp6Server();

        public static BigInteger FromHex(string hex)
        {
            return new BigInteger(1, Hex.Decode(hex));
        }

        public readonly BigInteger N = FromHex("D4C7F8A2B32C11B8FBA9581EC4BA4F1B04215642EF7355E37C0FC0443EF756EA2C6B8EEB755A1C723027663CAA265EF785B8FF6A9B35227A52D86633DBDFCA43");
        public readonly BigInteger g = BigInteger.Two;

        // BYTES FROM PACKET
        public byte[] A;
        public byte[] Proof;

        // BYTES FOR SEND
        public byte[] B;
        public byte[] Salt;

        #endregion

        #region Base

        public LobbyClient(TCPManager srv)
            : base(srv)
        {
        }

        public override void OnConnect()
        {
            LOGIN_PUZZLE.Send(this);
        }

        public override void OnDisconnect()
        {

        }

        #endregion

        #region TCP

        protected override void OnReceive(byte[] Packet)
        {
            lock (this)
            {
                PacketIn packet = new PacketIn(Packet, 0, Packet.Length);
                packet.Size = packet.GetUint32R();
                packet = DeCrypt(packet);
                packet.Opcode = packet.GetUint32R();

                Server.HandlePacket(this, packet);
            }
        }

        #endregion
    }
}

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

namespace WorldServer
{
    [Crypt("RC4Crypto")]
    public class RC4Crypto : ICryptHandler
    {
        public byte[] Crypt(CryptKey Key, byte[] packet)
        {
            packet = PacketOut.EncryptMythicRC4(Key.GetbKey(), packet);

            return packet;
        }

        public PacketIn Decrypt(CryptKey Key, byte[] packet)
        {
            PacketIn Packet = new PacketIn(packet, 0, packet.Length);
            Packet = Packet.DecryptMythicRC4(Key.GetbKey());
            return Packet;
        }

        public CryptKey GenerateKey(BaseClient client)
        {
            return new CryptKey(new byte[0]);
        }
    }
}

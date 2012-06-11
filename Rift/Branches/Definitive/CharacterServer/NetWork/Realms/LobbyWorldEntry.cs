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

using Common;

namespace CharacterServer
{
    [ISerializableAttribute((long)Opcodes.LobbyWorldEntry)]
    public class LobbyWorldEntry : ISerializablePacket
    {
        [Unsigned7Bit(0)]
        public long RealmID;

        [Unsigned7Bit(1)]
        public long Version;

        [BoolBit(3)]
        public bool PVP;

        [Unsigned7Bit(6)]
        public long CharactersCount;

        [BoolBit(10)]
        public bool RP;

        [Unsigned7Bit(11)]
        public long Population;

        /*[BoolBit(12)]
        public bool Field12 = true;

        [BoolBit(13)]
        public bool Field13 = true;

        [BoolBit(14)]
        public bool Field14 = true;*/

        [Unsigned7Bit(15)]
        public long Language;

        [BoolBit(16)]
        public bool Online = true;

        [BoolBit(17)]
        public bool Recommended;

        /*[BoolBit(18)]
        public bool Field18 = true;

        [BoolBit(19)]
        public bool Field19 = true;

        [BoolBit(20)]
        public bool Field20 = true;

        [BoolBit(21)]
        public bool Field21 = true;*/

        public override void OnRead(RiftClient From)
        {

        }
    }
}

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
using FrameWork;

namespace CharacterServer
{
    [ISerializableAttribute((long)Opcodes.LobbyCharacterDeleteResponse)]
    public class LobbyCharacterDeleteResponse : ISerializablePacket
    {
        [Unsigned7Bit(0)]
        public long Result;
    }

    [ISerializableAttribute((long)Opcodes.LobbyCharacterDeleteRequest)]
    public class LobbyCharacterDeleteRequest : ISerializablePacket
    {
        [ArrayBit(0)]
        public string UnkText;

        [Raw8Bit(1)]
        public long GUID;

        public override void OnRead(RiftClient From)
        {
            Log.Success("LobbyCharacterDeleteRequest", "Deleting Character : " + GUID);

            if (From.Acct == null || From.Rm == null)
                return;

            CharactersMgr Mgr = From.Rm.GetObject<CharactersMgr>();
            bool Result = Mgr.DeleteCharacter(GUID, From.Acct.Id);

            // TODO : Check in game player
            LobbyCharacterDeleteResponse DeleteResult = new LobbyCharacterDeleteResponse();
            DeleteResult.Result = Convert.ToInt64(!Result); // Result, 15 Error must wait logout, 0 OK
            From.SendSerialized(DeleteResult);
        }
    }
}

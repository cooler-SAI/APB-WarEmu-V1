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

namespace WorldServer
{
    public class TCPServer : TCPManager
    {
        public TCPServer()
            : base()
        {
            PacketOut.SizeLen       = sizeof(UInt16);
            PacketOut.OpcodeInLen   = false;
            PacketOut.SizeInLen     = false;
            PacketOut.OpcodeReverse = false;
            PacketOut.SizeReverse   = false;
            PacketOut.Struct        = PackStruct.SizeAndOpcode;
        }

        protected override BaseClient GetNewClient()
        {
            GameClient client = new GameClient(this);

            return client;
        }

        public GameClient GetClientByAccount(GameClient Me,int AccountId)
        {
            lock(Clients)
                for(int i=0;i<Clients.Length;++i)
                    if (Clients[i] != null && Clients[i] != Me)
                    {
                        GameClient Client = Clients[i] as GameClient;
                        if (Client.HasAccount() && Client._Account.AccountId == AccountId)
                            return Client;
                    }

            return null;
        }

        public Player GetPlayerByName(string Name)
        {
            lock(Clients)
                for(int i=0;i<Clients.Length;++i)
                    if (Clients[i] != null)
                    {
                        GameClient Client = (Clients[i] as GameClient);
                        if (Client.IsPlaying() && Client.HasPlayer() && Client.Plr.Name.ToLower() == Name.ToLower())
                            return Client.Plr;
                    }

            return null;
        }
    }
}

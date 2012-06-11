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

namespace LobbyServer
{
    public class ASK_LOGIN : IPacketHandler
    {
        [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.ASK_LOGIN, "onAskLogin")]
        static public void HandlePacket(BaseClient client, PacketIn packet)
        {
            LobbyClient cclient = client as LobbyClient;
            packet.Skip(24);
            string Email = packet.GetUnicodeString().ToUpper();

            Log.Notice("ASK_LOGIN", "Authentification de : " + Email);

            SendLoginResult(cclient, Email,Program.CharMgr.LoadAccount(Email));
        }

        static public void SendLoginResult(LobbyClient client,string Email,bool result)
        {
            if(!result)
            {
                Log.Error("ASK_LOGN","Compte introuvable !");
                ANS_LOGIN_FAILED.Send(client, 10010);
                client.Disconnect();
            }
            else
            {
                Log.Success("ASK_LOGN","Authentification en cours.");
                client.Account = Program.CharMgr.GetAccount(Email);
                if (client.Account == null)
                    SendLoginResult(client, Email, false);
                else
                    LOGIN_SALT.Send(client);
            }
        }
    }
}

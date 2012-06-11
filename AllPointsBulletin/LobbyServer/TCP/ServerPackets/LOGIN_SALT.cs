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

namespace LobbyServer
{
    static public class LOGIN_SALT
    {
        static public void Send(LobbyClient client)
        {
            Log.Debug("LOGIN_SALT", "Send to " + client.Account.Email);
            MakeSalt(client);

            PacketOut Out = new PacketOut((UInt32)Opcodes.LOGIN_SALT);
            Out.WriteInt32R(client.Account.Id);
            Out.Write(client.B, 0, 64);
            Out.WriteByte(0x40);
            Out.WriteByte(0);
            Out.Write(client.Salt, 0, 10);
            client.SendTCP(Out);
        }

        static public void MakeSalt(LobbyClient client)
        {
            string Id = Convert.ToString(client.Account.Id);
            string Email = client.Account.Email;
            string Pass = client.Account.Password;

            byte[] I = Encoding.ASCII.GetBytes(Id);
            byte[] P = Encoding.ASCII.GetBytes(Pass);
            byte[] s = new byte[10];
            client.random.NextBytes(s);

            Srp6VerifierGenerator gen = new Srp6VerifierGenerator();
            gen.Init(client.N, client.g, new Sha1Digest());
            BigInteger v = gen.GenerateVerifier(s, I, P);

            client.serv.Init(client.N, client.g, v, new Sha1Digest(), client.random);

            BigInteger B_s = client.serv.GenerateServerCredentials();
            client.B = B_s.ToByteArray();
            client.Salt = s;
        }
    }
}

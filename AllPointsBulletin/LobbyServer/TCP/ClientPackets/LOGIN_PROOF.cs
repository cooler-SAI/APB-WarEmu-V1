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
    public class LOGIN_PROOF : IPacketHandler
    {
        [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.LOGIN_PROOF, "onLoginProof")]
        static public void HandlePacket(BaseClient client, PacketIn packet)
        {
            LobbyClient cclient = client as LobbyClient;

            byte[] PublicKey = new byte[64];
            for (int i = 0; i < 64; ++i)
                PublicKey[i] = packet.GetUint8();
            Reverse(PublicKey, 64);
            cclient.A = PublicKey;

            UInt16 unk = packet.GetUint16();

            byte[] Proof = new byte[20];
            for (int i = 0; i < 20; ++i)
                Proof[i] = packet.GetUint8();
            Reverse(Proof, 20);
            cclient.Proof = Proof;

            if (IsValid(cclient))
                ANS_LOGIN_SUCCES.Send(cclient);
            else
                cclient.Disconnect();
        }

        static public void Reverse<T>(T[] buffer, int length)
        {
            for (int i = 0; i < length / 2; i++)
            {
                T temp = buffer[i];
                buffer[i] = buffer[length - i - 1];
                buffer[length - i - 1] = temp;
            }
        }

        static public bool IsValid(LobbyClient client)
        {
            if (client.Account == null)
                return false;

            return true;

            client.serv.CalculateSecret(new BigInteger(client.A));
            BigInteger Pro = new BigInteger(client.Proof);
            BigInteger check = Srp6Utilities.ValidatePublicValue(client.N, Pro);

            return check.IntValue > 0;
        }
    }
}

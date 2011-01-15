using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork.Logger;
using FrameWork.NetWork;

namespace LobbyServer
{
    static public class LOGIN_SALT
    {
        static public void Send(LobbyClient client)
        {
            Log.Debug("LOGIN_SALT", "Send to " + client.Account.Email);
            MakeSalt(client);

            PacketOut Out = new PacketOut((UInt32)Opcodes.LOGIN_SALT);
            Out.WriteInt32Reverse(client.Account.Id);
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
            FrameWork.NetWork.Crypto.BigInteger v = gen.GenerateVerifier(s, I, P);

            client.serv.Init(client.N, client.g, v, new Sha1Digest(), client.random);

            FrameWork.NetWork.Crypto.BigInteger B_s = client.serv.GenerateServerCredentials();
            client.B = B_s.ToByteArray();
            client.Salt = s;
        }
    }
}

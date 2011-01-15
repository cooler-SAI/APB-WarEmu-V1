using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork.Logger;
using FrameWork.NetWork;

using Common;

namespace LobbyServer
{
    [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.LOGIN_PROOF, "onLoginProof")]
    public class LOGIN_PROOF : IPacketHandler
    {
        public int HandlePacket(BaseClient client, PacketIn packet)
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

            return 0;
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

            client.serv.CalculateSecret(new FrameWork.NetWork.Crypto.BigInteger(client.A));
            FrameWork.NetWork.Crypto.BigInteger Pro = new FrameWork.NetWork.Crypto.BigInteger(client.Proof);
            FrameWork.NetWork.Crypto.BigInteger check = Srp6Utilities.ValidatePublicValue(client.N, Pro);

            return check.IntValue > 0;
        }
    }
}

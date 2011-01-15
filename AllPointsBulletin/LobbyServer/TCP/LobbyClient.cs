using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork.Logger;
using FrameWork.NetWork;

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

        public static FrameWork.NetWork.Crypto.BigInteger FromHex(string hex)
        {
            return new FrameWork.NetWork.Crypto.BigInteger(1, Hex.Decode(hex));
        }

        public readonly FrameWork.NetWork.Crypto.BigInteger N = FromHex("D4C7F8A2B32C11B8FBA9581EC4BA4F1B04215642EF7355E37C0FC0443EF756EA2C6B8EEB755A1C723027663CAA265EF785B8FF6A9B35227A52D86633DBDFCA43");
        public readonly FrameWork.NetWork.Crypto.BigInteger g = FrameWork.NetWork.Crypto.BigInteger.Two;

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

        protected override void OnReceive(PacketIn packet)
        {
            lock (this)
            {
                packet.Size = packet.GetUint32Reversed();
                packet = DeCrypt(packet);
                packet.Opcode = packet.GetUint32Reversed();

                Server.HandlePacket(this, packet);
            }
        }

        #endregion
    }
}

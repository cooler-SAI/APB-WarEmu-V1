using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Common;
using FrameWork.Logger;
using FrameWork.NetWork;

namespace WorldServer
{
    [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.F_ENCRYPTKEY, "onEncryptKey")]
    public class F_ENCRYPTKEY : IPacketHandler
    {
        public struct sEncrypt
        {
            public byte cipher, application, major, minor, revision, unk1;
        };

        public void HandlePacket(BaseClient client, PacketIn packet)
        {
            GameClient cclient = client as GameClient;

            sEncrypt Result = BaseClient.ByteToType<sEncrypt>(packet);

            string Version = Result.major + "." + Result.minor + "." + Result.revision;

            Log.Debug("F_ENCRYPTKEY", "Version = " + Version);

            if (Result.cipher == 0)
            {
                PacketOut Out = new PacketOut((byte)Opcodes.F_RECEIVE_ENCRYPTKEY);
                Out.WriteByte(1);
                cclient.SendTCP(Out);
            }
            else if (Result.cipher == 1)
            {
                byte[] EncryptKey = new byte[256];
                packet.Read(EncryptKey, 0, EncryptKey.Length);
                cclient.AddCrypt("RC4Crypto", new CryptKey(EncryptKey), new CryptKey(EncryptKey));
            }
        }
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Common;
using FrameWork.Logger;
using FrameWork.NetWork;

namespace WorldServer
{
    [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.F_REQUEST_CHAR, "onRequestChar")]
    public class F_REQUEST_CHAR : IPacketHandler
    {
        public void HandlePacket(BaseClient client, PacketIn packet)
        {
            GameClient cclient = client as GameClient;

            cclient.State = eClientState.CharScreen;

            UInt16 Operation = packet.GetUint16();

            if (Operation == 0x2D58)
            {
                PacketOut Out = new PacketOut((byte)Opcodes.F_REQUEST_CHAR_ERROR);
                Out.WriteByte((byte)CharMgr.GetAccountRealm(cclient._Account.AccountId));
                cclient.SendTCP(Out);
            }
            else
            {
                PacketOut Out = new PacketOut((byte)Opcodes.F_REQUEST_CHAR_RESPONSE);
                Out.FillString(cclient._Account.Username, 21);
                Out.WriteByte((byte)CharMgr.GetAccountRealm(cclient._Account.AccountId));
                byte[] Chars = CharMgr.BuildCharacters(cclient._Account.AccountId);
                Out.Write(Chars, 0, Chars.Length);
                cclient.SendTCP(Out);
            }
        }
    }
}

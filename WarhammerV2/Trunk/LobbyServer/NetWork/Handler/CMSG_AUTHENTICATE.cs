using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Common;
using FrameWork.NetWork;

namespace LobbyServer.NetWork.Handler
{
    [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.CMSG_AUTHENTICATE, "onAuthenticate")]
    public class CMSG_AUTHENTICATE : IPacketHandler
    {
        public void HandlePacket(BaseClient client, PacketIn packet)
        {
            Client cclient = client as Client;

            UInt32 Sequence = packet.GetUint32();
            string Username = packet.GetString();
            string Token = packet.GetString();

            AuthResult Result = Program.AcctMgr.CheckToken(Username, Token);

            PacketOut Out = new PacketOut((byte)Opcodes.SMSG_AUTHENTICATE_RESPONSE);
            Out.WriteUInt32(Sequence);
            Out.WriteUInt16((UInt16)Result);
            Out.WriteByte(0);

            cclient.SendTCPCuted(Out);

            if (Result != AuthResult.AUTH_SUCCESS)
                cclient.Disconnect();
            else
            {
                cclient.Username = Username;
                cclient.Token = Token;
            }
        }
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Common;
using FrameWork;

namespace LobbyServer.NetWork.Handler
{
    public class AuthentificationHandlers : IPacketHandler
    {
        [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.CMSG_CONNECT, 0, "onConnect")]
        static public void CMSG_CONNECT(BaseClient client, PacketIn packet)
        {
            Client cclient = client as Client;

            UInt32 sequence = packet.GetUint32();
            UInt32 unk = packet.GetUint32();

            PacketOut Out = new PacketOut((byte)Opcodes.SMSG_CONNECT_RESPONSE);
            Out.WriteUInt32(sequence);
            Out.WriteUInt16(0);
            Out.WriteUInt32(unk);

            cclient.SendTCPCuted(Out);
        }

        [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.CMSG_AUTHENTICATE, 0, "onAuthenticate")]
        static public void CMSG_AUTHENTICATE(BaseClient client, PacketIn packet)
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

        [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.CMSG_LOGIN_CLEARANCE, 0, "onLoginClearance")]
        static public void CMSG_LOGIN_CLEARANCE(BaseClient client, PacketIn packet)
        {
            Client cclient = client as Client;

            UInt32 sequence = packet.GetUint32();

            PacketOut Out = new PacketOut((byte)Opcodes.SMSG_LOGIN_CLEARANCE_RESPONSE);
            Out.WriteUInt32(sequence);
            Out.WriteUInt32(0);
            Out.WriteUInt16(1);
            Out.WriteUInt16(1);
            Out.WriteUInt32(1);

            cclient.SendTCPCuted(Out);
        }

        [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.CMSG_GET_SERVER_LIST, 0, "onGetServerList")]
        static public void CMSG_GET_SERVER_LIST(BaseClient client, PacketIn packet)
        {
            Client cclient = client as Client;

            uint sequence = packet.GetUint32();

            byte[] Res = Program.AcctMgr.BuildRealms(sequence);

            PacketOut Out = new PacketOut((byte)Opcodes.SMSG_GET_SERVER_LIST_RESPONSE);
            Out.Write(Res, 0, Res.Length);

            cclient.SendTCPCuted(Out);
        }

        [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.CMSG_GET_CHARACTER_SUMMARIES, 0, "onGetCharacterSummaries")]
        static public void CMSG_GET_CHARACTER_SUMMARIES(BaseClient client, PacketIn packet)
        {
            Client cclient = client as Client;

            uint sequence = packet.GetUint32();

            PacketOut Out = new PacketOut((byte)Opcodes.SMSG_GET_CHARACTER_SUMMARIES_RESPONSE);
            Out.WriteUInt32(sequence);
            Out.WriteUInt16(0);
            Out.WriteUInt16(0); // Character count

            cclient.SendTCPCuted(Out);
        }
    }
}

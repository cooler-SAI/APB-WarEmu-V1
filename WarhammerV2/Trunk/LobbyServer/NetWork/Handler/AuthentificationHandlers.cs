/*
 * Copyright (C) 2013 APS
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
using Google.ProtocolBuffers;
using WarProtocol;

using Common;
using FrameWork;

namespace LobbyServer.NetWork.Handler
{
    public class AuthentificationHandlers : IPacketHandler
    {
        [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.CMSG_VerifyProtocolReq, 0, "CMSG_VerifyProtocolReq")]
        static public void CMSG_VerifyProtocolReq(BaseClient client, PacketIn packet)
        {
            PacketOut Out = new PacketOut((byte)Opcodes.SMSG_VerifyProtocolReply);
            Client LobbyClient = (client as Client);

            byte[] IV_HASH1 = { 0x01, 0x53, 0x21, 0x4d, 0x4a, 0x04, 0x27, 0xb7, 0xb4, 0x59, 0x0f, 0x3e, 0xa7, 0x9d, 0x29, 0xe9 };
            byte[] IV_HASH2 = { 0x49, 0x18, 0xa1, 0x2a, 0x64, 0xe1, 0xda, 0xbd, 0x84, 0xd9, 0xf4, 0x8a, 0x8b, 0x3c, 0x27, 0x20 };

            ByteString iv1 = ByteString.CopyFrom(IV_HASH1);
            ByteString iv2 = ByteString.CopyFrom(IV_HASH2);
            VerifyProtocolReply.Builder verify = VerifyProtocolReply.CreateBuilder();
            verify.SetResultCode(VerifyProtocolReply.Types.ResultCode.RES_SUCCESS);

            verify.SetIv1(ByteString.CopyFrom(IV_HASH1));
            verify.SetIv2(ByteString.CopyFrom(IV_HASH2));
            Out.Write(verify.Build().ToByteArray());

            LobbyClient.SendTCPCuted(Out);
        }

        [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.CMSG_AuthSessionTokenReq, 0, "CMSG_AuthSessionTokenReq")]
        static public void CMSG_AuthSessionTokenReq(BaseClient client, PacketIn packet)
        {
            PacketOut Out = new PacketOut((byte)Opcodes.SMSG_AuthSessionTokenReply);
            Client LobbyClient = (client as Client);

            AuthSessionTokenReq.Builder authReq = AuthSessionTokenReq.CreateBuilder();
            authReq.MergeFrom(packet.ToArray());

            string session = Encoding.ASCII.GetString(authReq.SessionToken.ToByteArray());

            AuthSessionTokenReply.Builder authReply = AuthSessionTokenReply.CreateBuilder();
            authReply.SetResultCode(AuthSessionTokenReply.Types.ResultCode.RES_SUCCESS);

            Out.Write(authReply.Build().ToByteArray());
            LobbyClient.SendTCPCuted(Out);
        }

        [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.CMSG_GetCharSummaryListReq, 0, "CMSG_GetCharSummaryListReq")]
        static public void CMSG_GetCharSummaryListReq(BaseClient client, PacketIn packet)
        {
            Client LobbyClient = (client as Client);

            PacketOut Out = new PacketOut((byte)Opcodes.SMSG_GetCharSummaryListReply);
            Out.Write(new byte[2] { 0x08, 00 });
            LobbyClient.SendTCPCuted(Out);
        }

        [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.CMSG_GetClusterListReq, 0, "CMSG_GetClusterListReq")]
        static public void CMSG_GetClusterListReq(BaseClient client, PacketIn packet)
        {
            Client LobbyClient = (client as Client);
            byte[] cluster = null;

            PacketOut Out = new PacketOut((byte)Opcodes.SMSG_GetClusterListReply);
            if (LobbyClient.GetIp.Contains("127.0.0.1"))
                cluster = Program.AcctMgr.BuildRealms("127.0.0.1");
            else if (LobbyClient.GetIp.Contains("192.168"))
                cluster = Program.AcctMgr.BuildRealms("192.168.1.14");
            else
                cluster = Program.AcctMgr.BuildRealms(null);

            Out.Write(cluster);
            LobbyClient.SendTCPCuted(Out);
        }

        [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.CMSG_GetAcctPropListReq, 0, "CMSG_GetAcctPropListReq")]
        static public void CMSG_GetAcctPropListReq(BaseClient client, PacketIn packet)
        {
            Client LobbyClient = (client as Client);

            PacketOut Out = new PacketOut((byte)Opcodes.SMSG_GetAcctPropListReply);
            byte[] val = { 0x08, 0x00 };
            Out.Write(val);
            LobbyClient.SendTCPCuted(Out);
        }

        [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.CMSG_MetricEventNotify, 0, "CMSG_MetricEventNotify")]
        static public void CMSG_MetricEventNotify(BaseClient client, PacketIn packet)
        {

        }


        /*[PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.CMSG_CONNECT, 0, "onConnect")]
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
        }*/
    }
}

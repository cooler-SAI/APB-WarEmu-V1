using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork.NetWork;

namespace LobbyServer
{
    static public class ANS_LOGIN_SUCCES
    {
        static public void Send(LobbyClient client)
        {
            PacketOut Out = new PacketOut((UInt32)Opcodes.ANS_LOGIN_SUCCESS);
            Out.WriteParsedString(client.Account.RealTag,48);

            Out.WriteUInt32Reverse(21); // Account Type

            Out.WriteInt64Reverse(TCPManager.GetTimeStamp());

            // Temps avant expiration du compte
            Out.WriteByte(0x13); // ms
            Out.WriteByte(0x29); // sec
            Out.WriteUInt16Reverse(0x12); // Hour

            // Date de création
            Out.WriteByte(0x0E); // day
            Out.WriteByte(0x07); // month
            Out.WriteUInt16Reverse(2010); // Year

            Out.WriteInt32Reverse(1450); // Temps de jeu (secondes)
            Out.WriteInt32Reverse(client.Account.Points); // Points APB

            for(int i=1;i<6;++i)
                Out.WriteInt32Reverse(
                    Program.FileMgr.GetFileVersion(client.Account.Id, i, true, "", "")); // Config file, Server Version

            Out.WriteInt32Reverse(0);

            Out.WriteUInt16(0x957D);
            Out.WriteUInt16(0x0400);

            Out.WriteUInt16(0x5052);
            Out.WriteUInt16(0x4F45);
            Out.WriteUInt16(0x552E);
            Out.WriteUInt16(0x3232);
            Out.WriteUInt16(0x3738);
            Out.WriteUInt16(0x3031);
            Out.WriteUInt16(0);

            Out.WriteUInt16(0x0067);

            Out.WriteUInt64(0x526C624331313256);
            Out.WriteUInt64(0x486E314100000000);
            Out.WriteUInt16(0);
            Out.WriteStringBytes("");
            Out.WriteByte(0);

            client.SendTCP(Out);

            ANS_CHARACTER_INFO.Send(client);
        }
    }
}

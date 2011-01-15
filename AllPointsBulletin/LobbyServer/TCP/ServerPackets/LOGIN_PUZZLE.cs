using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork.NetWork;

namespace LobbyServer
{
    static public class LOGIN_PUZZLE
    {
        static public void Send(LobbyClient client)
        {
            PacketOut Out = new PacketOut((UInt32)Opcodes.LOGIN_PUZZLE);
            Out.WriteInt32Reverse(Program.Version[0]);
            Out.WriteInt32Reverse(Program.Version[1]);
            Out.WriteInt32Reverse(Program.Version[2]);
            Out.WriteInt32Reverse(Program.Build);

            Out.WriteByte(0x05);

            Out.WriteInt64Reverse(-313054979819954861);

            Out.WriteUInt32Reverse(0);
            Out.WriteUInt32Reverse(0);
            Out.WriteUInt32Reverse(0);
            client.SendTCP(Out);
        }
    }
}

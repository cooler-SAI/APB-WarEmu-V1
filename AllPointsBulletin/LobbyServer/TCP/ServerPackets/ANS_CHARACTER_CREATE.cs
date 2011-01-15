using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork.NetWork;

using Common;

namespace LobbyServer
{
    static class ANS_CHARACTER_CREATE
    {
        static public void Send(LobbyClient client)
        {
            PacketOut Out = new PacketOut((UInt32)Opcodes.ANS_CHARACTER_CREATE);
            if (client.CreateChar == null)
                Out.WriteInt32Reverse(1);
            else
            {
                Out.WriteInt32Reverse(0);
                Out.WriteInt32Reverse(client.CreateChar.SlotId);
            }
            client.CreateChar = null;
            client.SendTCP(Out);
        }
    }
}

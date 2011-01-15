using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork.NetWork;

using Common;

namespace LobbyServer
{
    static public class WORLD_LIST
    {
        static public void Send(LobbyClient client)
        {
            List<WorldInfo> _Worlds = Program.CharMgr.GetWorlds();

            PacketOut Out = new PacketOut((UInt32)Opcodes.WORLD_LIST);
            Out.WriteInt32Reverse(0); // Returned Code 

            Out.WriteUInt16Reverse((UInt16)_Worlds.Count); // World Count

            foreach (WorldInfo Info in _Worlds)
            {
                Out.WriteUInt32Reverse((UInt32)Info._Info.Id); // WorldUid

                Out.WriteParsedString(Info._Info.Name,32); // WorldName

                Out.WriteByte((byte)(Info.RpcID == 0 ? 0 : 1)); // Status
                Out.WriteByte(Info._Info.Population); // Pop  0:Low 1:Medium 2:High
                Out.WriteByte(Info._Info.Enf); // enf recommended , true or false
                Out.WriteByte(Info._Info.Crim); // crim recommended , true or false
            }

            client.SendTCP(Out);
        }
    }
}

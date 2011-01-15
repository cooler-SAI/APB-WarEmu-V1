using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork.NetWork;

using Common;

namespace LobbyServer
{
    static public class ANS_CHARACTER_INFO
    {
        static public void Send(LobbyClient client)
        {
            Program.CharMgr.LoadCharacters(client.Account.Id);

            CharacterInfo[] _Chars = Program.CharMgr.GetInfos(client.Account.Id);
            WorldInfo Info = Program.CharMgr.GetWorldInfo(client.Account.WorldId);

            PacketOut Out = new PacketOut((UInt32)Opcodes.CHARACTER_LIST);
            Out.WriteByte((byte)_Chars.Length);
            foreach (CharacterInfo Char in _Chars)
            {
                Out.WriteByte(Char.Character.SlotId);
                Out.WriteByte(Char.Character.Faction);
                Out.WriteByte((byte)(Info.RpcID == 0 ? 0 : 1));
                Out.WriteUInt32Reverse((UInt32)Info._Info.Id);
                Out.WriteParsedString(Info._Info.Name, 32);
                Out.WriteParsedString(Char.Character.Name, 32);
            }

            client.SendTCP(Out);

            if (_Chars.Length <= 0)
                WORLD_LIST.Send(client);
        }
    }
}

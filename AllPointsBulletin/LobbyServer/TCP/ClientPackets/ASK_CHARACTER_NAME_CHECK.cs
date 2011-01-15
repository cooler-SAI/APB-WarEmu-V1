using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;

using FrameWork.Logger;
using FrameWork.NetWork;

namespace LobbyServer
{
    [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.ASK_CHARACTER_NAME_CHECK, "onAskCharacterNameCheck")]
    public class ASK_CHARACTER_NAME_CHECK : IPacketHandler
    {
        public int HandlePacket(BaseClient client, PacketIn packet)
        {
            LobbyClient cclient = (LobbyClient)client;

            UInt32 WorldUid = packet.GetUint32Reversed();
            string Name = packet.GetParsedString();

            PacketOut Out = new PacketOut((UInt32)Opcodes.ANS_CHARACTER_NAME_CHECK);

            if (CheckName(Name) == 0)
            {            
                cclient.CreateChar = new DBCharacter();
                cclient.CreateChar.AcctId = cclient.Account.Id;
                cclient.CreateChar.Name = Name;

                if (cclient.Account.WorldId != WorldUid)
                    Program.CharMgr.SetAccountWorld(cclient.Account.Id, (int)WorldUid);

                Out.WriteUInt32Reverse(0);
            }
            else
            {
                cclient.CreateChar = null;
                Out.WriteUInt32Reverse(1);
            }

            cclient.SendTCP(Out);

            return 0;
        }

        static public UInt32 CheckName(string name)
        {
            Regex objAlphaNumericPattern = new Regex("[^a-zA-Z0-9]");
            return (UInt32)(objAlphaNumericPattern.IsMatch(name) ? 1 : 0);
        }
    }
}

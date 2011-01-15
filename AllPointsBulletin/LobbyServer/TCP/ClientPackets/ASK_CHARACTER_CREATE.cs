using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork.Logger;
using FrameWork.NetWork;

using Common;

namespace LobbyServer
{
    [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.ASK_CHARACTER_CREATE, "onAskCharacterCreate")]
    public class ASK_CHARACTER_CREATE : IPacketHandler
    {
        public int HandlePacket(BaseClient client, PacketIn packet)
        {
            LobbyClient cclient = client as LobbyClient;

            byte freeslot = Program.CharMgr.GetFreeSlot(cclient.Account.Id);

            if (freeslot == 0 || cclient.CreateChar == null)
                ANS_CHARACTER_CREATE.Send(cclient);
            else
            {
                cclient.CreateChar.SlotId = freeslot;
                cclient.CreateChar.Faction = packet.GetUint8();
                cclient.CreateChar.Gender = packet.GetUint8();
                cclient.CreateChar.Version = (int)packet.GetUint32Reversed();
                cclient.CreateChar.Seconds = (int)packet.GetUint32Reversed();

                byte[] Custom = new byte[packet.Length - packet.Position];
                packet.Read(Custom, 0, Custom.Length);

                cclient.CreateChar.Custom = BitConverter.ToString(Custom);

                Program.CharMgr.CreateCharacter(cclient.CreateChar);
                ANS_CHARACTER_CREATE.Send(cclient);
            }

            return 0;
        }
    }
}

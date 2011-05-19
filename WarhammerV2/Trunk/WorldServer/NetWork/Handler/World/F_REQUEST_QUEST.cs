using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Common;
using FrameWork.Logger;
using FrameWork.NetWork;

namespace WorldServer
{
    [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.F_REQUEST_QUEST, "onRequestQuest")]
    public class F_REQUEST_QUEST : IPacketHandler
    {
        public void HandlePacket(BaseClient client, PacketIn packet)
        {
            GameClient cclient = client as GameClient;

            Log.Succes("HandlePacket", "Handle F_REQUEST_QUEST");

            UInt16 QuestID = packet.GetUint16();
            byte State = packet.GetUint8();

            switch (State)
            {
                case 0: // Accept Quest
                    {

                    }break;

                case 1: // Decline Quest
                    {

                    }
                    break;

                case 2: // Send To Group
                    {

                    }break;
            };
        }
    }
}

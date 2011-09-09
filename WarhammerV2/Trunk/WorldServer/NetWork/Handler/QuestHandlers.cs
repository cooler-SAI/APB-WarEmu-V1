using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Common;
using FrameWork;

namespace WorldServer
{
    public class QuestHandlers : IPacketHandler
    {
        [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.F_QUEST, "onQuest")]
        static public void F_QUEST(BaseClient client, PacketIn packet)
        {
            GameClient cclient = client as GameClient;

            UInt16 QuestID = packet.GetUint16();
            UInt16 State = packet.GetUint16();
            UInt16 Unk1 = packet.GetUint16();
            byte Unk2 = packet.GetUint8();
            byte Unk3 = packet.GetUint8();
            UInt16 Unk4 = packet.GetUint16();
            UInt16 CreatureOID = packet.GetUint16();

            switch (State)
            {
                case 1: // Show Quest
                    {
                        Log.Info("F_QUEST", "Show Quest : " + QuestID);

                        Creature Crea = cclient.Plr.Region.GetObject(CreatureOID) as Creature;

                        if (Crea != null && Crea.QtsInterface.HasQuestStarter(QuestID))
                            Crea.QtsInterface.BuildQuest(QuestID, cclient.Plr);

                    } break;

                case 2: // Accept Quest
                    {
                        Log.Info("F_QUEST", "Accept Quest : " + QuestID);

                        Creature Crea = cclient.Plr.Region.GetObject(CreatureOID) as Creature;

                        if (Crea != null && Crea.QtsInterface.HasQuestStarter(QuestID))
                        {
                            cclient.Plr.QtsInterface.AcceptQuest(QuestID);

                            if (!Crea.QtsInterface.CreatureHasStartQuest(cclient.Plr))
                            {
                                Crea.SendRemove(cclient.Plr);
                                Crea.SendMeTo(cclient.Plr);
                            }
                        }

                    }break;

                case 3: // Quest Done
                    {
                        Log.Info("F_QUEST", "Done Quest : " + QuestID);

                        Creature Crea = cclient.Plr.Region.GetObject(CreatureOID) as Creature;

                        if (Crea != null && Crea.QtsInterface.hasQuestFinisher(QuestID))
                        {
                            cclient.Plr.QtsInterface.DoneQuest(QuestID);
                            Crea.SendRemove(cclient.Plr);
                            Crea.SendMeTo(cclient.Plr);
                        }

                    }break;

                case 4: // Quest Done Info
                    {
                        Log.Info("F_QUEST", "Done Quest Info: " + QuestID);
                        Creature Crea = cclient.Plr.Region.GetObject(CreatureOID) as Creature;

                        if (Crea != null && Crea.QtsInterface.hasQuestFinisher(QuestID))
                            Crea.QtsInterface.SendQuestDoneInfo(cclient.Plr, QuestID);
                    } break;

                case 5: // Select Quest Reward
                    {
                        Log.Info("F_QUEST", "Select Quest Reward: " + QuestID);
                        Creature Crea = cclient.Plr.Region.GetObject(CreatureOID) as Creature;

                        if (Crea != null && Crea.QtsInterface.hasQuestFinisher(QuestID))
                            cclient.Plr.QtsInterface.SelectRewards(QuestID, Unk3);

                    } break;

            };
        }

        [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.F_REQUEST_QUEST, "onRequestQuest")]
        static public void F_REQUEST_QUEST(BaseClient client, PacketIn packet)
        {
            GameClient cclient = client as GameClient;

            Log.Success("HandlePacket", "Handle F_REQUEST_QUEST");

            UInt16 QuestID = packet.GetUint16();
            byte State = packet.GetUint8();

            switch (State)
            {
                case 0: // Show Quest
                    {
                        Log.Info("F_REQUEST_QUEST", "Show Quest : " + QuestID);
                        cclient.Plr.QtsInterface.SendQuest(QuestID);

                    } break;

                case 1: // Decline Quest
                    {

                    }
                    break;

                case 2: // Send To Group
                    {

                    } break;
            };
        }
    }
}

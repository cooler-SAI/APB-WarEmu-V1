/*
 * Copyright (C) 2011 APS
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

            Creature Crea = cclient.Plr.Region.GetObject(CreatureOID) as Creature;

            if (Crea == null)
                return;

            switch (State)
            {
                case 1: // Show Quest
                    {
                        Log.Info("F_QUEST", "Show Quest : " + QuestID);

                        if (Crea.QtsInterface.HasQuestStarter(QuestID))
                            Crea.QtsInterface.BuildQuest(QuestID, cclient.Plr);

                    } break;

                case 2: // Accept Quest
                    {
                        Log.Info("F_QUEST", "Accept Quest : " + QuestID);

                        if (Crea.QtsInterface.HasQuestStarter(QuestID))
                        {
                            if (cclient.Plr.QtsInterface.AcceptQuest(QuestID))
                            {
                                if (!Crea.QtsInterface.CreatureHasStartQuest(cclient.Plr))
                                {
                                    Crea.SendRemove(cclient.Plr);
                                    Crea.SendMeTo(cclient.Plr);
                                }
                            }
                        }

                    }break;

                case 3: // Quest Done
                    {
                        if (Crea.QtsInterface.hasQuestFinisher(QuestID))
                        {
                            Log.Info("F_QUEST", "Done Quest : " + QuestID);

                            if (cclient.Plr.QtsInterface.DoneQuest(QuestID))
                            {
                                Crea.SendRemove(cclient.Plr);
                                Crea.SendMeTo(cclient.Plr);
                            }
                            else
                            {
                                Crea.QtsInterface.BuildQuest(QuestID, cclient.Plr);
                            }
                        }

                    } break;

                case 4: // Quest Done Info
                    {

                        if (Crea.QtsInterface.hasQuestFinisher(QuestID))
                            Crea.QtsInterface.SendQuestDoneInfo(cclient.Plr, QuestID);
                        else if (Crea.QtsInterface.HasQuestStarter(QuestID))
                        {
                            Log.Info("F_QUEST", "InProgress Quest : " + QuestID);
                            Crea.QtsInterface.SendQuestInProgressInfo(cclient.Plr, QuestID);
                        }

                    } break;

                case 5: // Select Quest Reward
                    {
                        Log.Info("F_QUEST", "Select Quest Reward: " + QuestID);

                        if (Crea.QtsInterface.hasQuestFinisher(QuestID))
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

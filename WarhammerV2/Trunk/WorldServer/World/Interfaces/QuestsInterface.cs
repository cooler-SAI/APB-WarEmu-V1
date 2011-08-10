﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Common;
using FrameWork;
using GameData;

namespace WorldServer
{
    public class QuestsInterface : BaseInterface
    {
        public uint Entry
        {
            get
            {
                if (Obj == null)
                    return 0;

                if (Obj.IsCreature())
                    return Obj.GetCreature().Entry;
               
                return 0;
            }
        }

        public QuestsInterface(Object Obj)
            : base(Obj)
        {

        }

        #region Npc

        public bool CreatureHasRunningQuest(Player Plr)
        {
            return false;
        }
        public bool CreatureHasFinisherQuest(Player Plr)
        {
            return false;
        }
        public bool CreatureHasStartQuest(Player Plr)
        {
            if (Entry == 0)
                return false;

            List<Quest> Quests = WorldMgr.GetStartQuests(Entry);

            if (Quests == null)
                return false;

            return Quests.Find(Q =>
                {
                    return Q.Level <= Plr.Level 
                        && Plr.QtsInterface.HasDoneQuest(Q.PrevQuest) 
                        && !Plr.QtsInterface.HasQuest(Q.Entry)
                        && !Plr.QtsInterface.HasDoneQuest(Q.Entry);
                }) != null;
        }

        public void HandleInteract(Player Plr, InteractMenu Menu)
        {
            if(Entry == 0)
                return;

            List<Quest> Starter = WorldMgr.GetStartQuests(Entry);
            List<Quest> Finisher = WorldMgr.GetFinishersQuests(Entry);

            if (Starter == null && Finisher == null)
                return;

            string Text = "";

            PacketOut Out = new PacketOut((byte)Opcodes.F_INTERACT_RESPONSE);
            Out.WriteByte(0);
            Out.WriteUInt16(Obj.Oid);
            Out.Fill(0, 3);
            Out.WriteByte(0x60);
            Out.WriteUInt32(0);
            Out.WriteUInt16(Plr.Oid);

            if (Starter != null)
            {
                List<Quest> Starts = Starter.FindAll(q => Plr.QtsInterface.CanStartQuest(q) );

                Log.Success("QuestInterface", "Handle Interact : Starts=" + Starts.Count);

                Out.WriteByte((byte)Starts.Count);
                foreach (Quest Q in Starts)
                {
                    if (Text.Length <= 0)
                        Text = Q.Particular;

                    Out.WriteByte(0);
                    Out.WriteUInt16(Q.Entry);
                    Out.WriteUInt16(0);
                    Out.WritePascalString(Q.Name);
                }
            }
            else
                Out.WriteByte(0);

            if (Finisher != null)
            {
                List<Quest> Finishs = Finisher.FindAll(q => Plr.QtsInterface.CanEndQuest(q));

                Log.Success("QuestInterface", "Handle Interact : Finishs=" + Finishs.Count);

                Out.WriteByte((byte)Finishs.Count);
                foreach (Quest Q in Finishs)
                {
                    if (Text.Length <= 0)
                        Text = Q.Particular;

                    Out.WriteByte(0);
                    Out.WriteUInt16(Q.Entry);
                    Out.WritePascalString(Q.Name);
                }
            }
            else
                Out.WriteByte(0);

            Out.WriteByte(0);
            Out.WritePascalString(Text);
            Out.WriteByte(0);

            Plr.SendPacket(Out);
        }

        #endregion

        #region Players

        public Dictionary<int, Character_quest> _Quests = new Dictionary<int, Character_quest>();

        public void Load(Character_quest[] Quests)
        {
            if (Quests == null)
                return;

            foreach (Character_quest Quest in Quests)
            {
                Quest.Quest = WorldMgr.GetQuest(Quest.QuestID);
                if (Quest.Quest == null)
                    continue;

                foreach (Character_Objectives Obj in Quest._Objectives)
                    Obj.Objective = WorldMgr.GetQuestObjective(Obj.ObjectiveID);

                if (!_Quests.ContainsKey(Quest.QuestID))
                    _Quests.Add(Quest.QuestID, Quest);
            }
        }

        public bool HasQuest(UInt16 QuestID)
        {
            if (QuestID == 0)
                return true;

            return _Quests.ContainsKey(QuestID);
        }
        public bool HasFinishQuest(UInt16 QuestID)
        {
            if (QuestID == 0)
                return true;

            if (!HasQuest(QuestID))
                return false;

            return GetQuest(QuestID).IsDone();
        }
        public bool HasDoneQuest(UInt16 QuestID)
        {
            if (QuestID == 0)
                return true;

            if (!HasQuest(QuestID))
                return false;

            return GetQuest(QuestID).Done;
        }

        public Character_quest GetQuest(UInt16 QuestID)
        {
            Character_quest Quest;
            _Quests.TryGetValue(QuestID, out Quest);
            return Quest;
        }

        public bool CanStartQuest(Quest Quest)
        {
            if(GetPlayer() == null)
                return false;

            if (Quest == null)
                return false;

            if (HasQuest(Quest.Entry) || Quest.Level > GetPlayer().Level)
                return false;

            return true;
        }
        public bool CanEndQuest(Quest Quest)
        {
            if (GetPlayer() == null)
                return false;

            if (Quest == null)
                return false;

            if (!HasQuest(Quest.Entry) || !HasFinishQuest(Quest.Entry))
                return false;

            return true;
        }

        public void AcceptQuest(UInt16 QuestID)
        {
            AcceptQuest(WorldMgr.GetQuest(QuestID));
        }

        public void AcceptQuest(Quest Quest)
        {
            if (Quest == null)
                return;

            if (!CanStartQuest(Quest))
                return;

            Character_quest CQuest = new Character_quest();
            CQuest.QuestID = Quest.Entry;
            CQuest.Done = false;
            CQuest.CharacterID = GetPlayer().CharacterId;
            CQuest.Quest = Quest;

            foreach(Quest_Objectives QObj in Quest.Objectives)
            {
                Character_Objectives CObj = new Character_Objectives();
                CObj.Count = 0;
                CObj.Objective = QObj;
                CObj.ObjectiveID = QObj.Guid;
                CQuest._Objectives.Add(CObj);
            }

            _Quests.Add(Quest.Entry, CQuest);
            CharMgr.Database.AddObject(CQuest);

            SendQuestState(Quest, QuestCompletion.QUESTCOMPLETION_OFFER);
        }

        public void DeclineQuest(Quest Quest)
        {
            if (Quest == null)
                return;

            if (!HasQuest(Quest.Entry))
                return;
        }

        public void FinishQuest(Quest Quest)
        {
            if (Quest == null)
                return;

            if (!HasFinishQuest(Quest.Entry))
                return;
        }

        #endregion

        static public void BuildQuestInfo(PacketOut Out,Player Plr, Quest Q)
        {
            BuildQuestHeader(Out, Q, true);

            Dictionary<Item_Info,uint> Choices = GenerateRewards(Q, Plr);

            Out.WriteByte(Q.ChoiceCount);
            Out.WriteByte(0);
            Out.WriteByte((byte)Choices.Count);

            foreach (KeyValuePair<Item_Info, uint> Kp in Choices)
                Item.BuildItem(ref Out, null, Kp.Key, 0, (ushort)Kp.Value);

            BuildObjectives(Out, Q.Objectives);

            Out.WriteByte(0);
        }
        static public void BuildQuestHeader(PacketOut Out, Quest Q, bool Particular)
        {
            Out.WritePascalString(Q.Name);
            Out.WriteUInt16((UInt16)Q.Description.Length);
            Out.WriteStringBytes(Q.Description);
            if (Particular)
            {
                Out.WriteUInt16((UInt16)Q.Particular.Length);
                Out.WriteStringBytes(Q.Particular);
            }
            Out.WriteByte(1);
            Out.WriteUInt32(Q.Gold);
            Out.WriteUInt32(Q.Xp);
        }
        public void BuildQuest(UInt16 QuestID, Player Plr)
        {
            Quest Q = WorldMgr.GetQuest(QuestID);
            if (Q == null)
                return;

            PacketOut Out = new PacketOut((byte)Opcodes.F_INTERACT_RESPONSE);
            Out.WriteByte(1);
            Out.WriteByte(1);
            Out.WriteUInt16(Q.Entry);
            Out.WriteUInt16(0);

            Out.WriteUInt16(Obj.Oid);
            Out.WriteUInt16(0);

            Out.WriteUInt16(Plr.Oid);
            Out.WriteUInt16(0);

            BuildQuestInfo(Out, Plr, Q);

            Plr.SendPacket(Out);
        }
        public void BuildQuest(PacketOut Out, Quest Q)
        {
            Out.WriteByte(Q.ChoiceCount);
            Out.WriteByte(0);

            
        }

        static public void BuildObjectives(PacketOut Out, List<Quest_Objectives> Objs)
        {
            Out.WriteByte((byte)Objs.Count);

            foreach (Quest_Objectives Objective in Objs)
            {
                Out.WriteByte((byte)Objective.ObjCount);
                Out.WritePascalString(Objective.Description);
            }
        }

        static public void BuildObjectives(PacketOut Out, List<Character_Objectives> Objs)
        {
            Out.WriteByte((byte)Objs.Count);

            foreach (Character_Objectives Objective in Objs)
            {
                Out.WriteByte((byte)Objective.Count);
                Out.WriteByte((byte)Objective.Objective.ObjCount);
                Out.WriteUInt16(0);
                Out.WritePascalString(Objective.Objective.Description);
            }
        }

        public void SendQuest(ushort QuestID)
        {
            Character_quest CQuest = GetQuest(QuestID);
            SendQuest(CQuest);
        }

        public void SendQuests()
        {
            PacketOut Out = new PacketOut((byte)Opcodes.F_QUEST_LIST);
            Out.WriteByte((byte)_Quests.Count);
            foreach (Character_quest Quest in _Quests.Values)
            {
                Out.WriteUInt16(Quest.QuestID);
                Out.WriteByte(0);
                Out.WritePascalString(Quest.Quest.Name);
                Out.WriteByte(0);
            }

            Log.Info("QuestInterface", "Sended Quest : " + _Quests.Count);
            GetPlayer().SendPacket(Out);
        }

        public void SendQuest(Character_quest CQuest)
        {
            if (CQuest == null)
            {
                Log.Error("QuestsInterface", "SendQuest CQuest == null");
                return;
            }

            PacketOut Packet = new PacketOut((byte)Opcodes.F_QUEST_INFO);
            Packet.WriteUInt16(CQuest.QuestID);
            Packet.WriteByte(0);
            BuildQuestHeader(Packet, CQuest.Quest, true);

            Dictionary<Item_Info, uint> Rewards = GenerateRewards(CQuest.Quest, GetPlayer());

            Packet.WriteByte(CQuest.Quest.ChoiceCount);
            Packet.WriteByte(0);
            Packet.WriteByte((byte)Rewards.Count);

            foreach (KeyValuePair<Item_Info, uint> Kp in Rewards)
            {
                Item.BuildItem(ref Packet, null, Kp.Key, 0, (ushort)Kp.Value);
            }

            Packet.WriteByte(0);

            BuildObjectives(Packet, CQuest._Objectives);

            Packet.WriteByte(1);

            Packet.WritePascalString(CQuest.Quest.Name);
            Packet.WritePascalString("Return to your giver");

            Packet.WriteUInt16(0x006A);
            Packet.WriteUInt16(0x046D);
            Packet.WriteUInt16(0x4D9E);
            Packet.WriteUInt16(0xCB65);

            Packet.Fill(0, 18);

            GetPlayer().SendPacket(Packet);
        }

        public void SendQuestState(Quest Quest,QuestCompletion State)
        {
            PacketOut Out = new PacketOut((byte)Opcodes.F_QUEST_LIST_UPDATE);
            Out.WriteUInt16(Quest.Entry);

            if (State == QuestCompletion.QUESTCOMPLETION_ABANDONED || State == QuestCompletion.QUESTCOMPLETION_DONE)
                Out.WriteByte(0);
            else
                Out.WriteByte(1);

            Out.WriteByte((byte)(State == QuestCompletion.QUESTCOMPLETION_DONE ? 1 : 0));

            Out.WriteUInt32(0x0000FFFF);
            Out.WritePascalString(Quest.Name);
            Out.WriteByte(0);
            GetPlayer().SendPacket(Out);
        }

        static public Dictionary<Item_Info,uint> GenerateRewards(Quest Q, Player Plr)
        {
            Dictionary<Item_Info,uint> Rewards = new Dictionary<Item_Info,uint>();

            foreach (KeyValuePair<Item_Info, uint> Kp in Q.Rewards)
                if (ItemsInterface.CanUse(Kp.Key, Plr, true, false, false, false))
                    Rewards.Add(Kp.Key, Kp.Value);

            return Rewards;
        }
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Common;

using FrameWork.Logger;
using FrameWork.NetWork;

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


        public void AcceptQuest(Quest Quest)
        {
            if (Quest == null)
                return;

            if (!CanStartQuest(Quest))
                return;
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

        static public List<Item_Infos> GetChoice(Player Plr, Quest Q)
        {
            List<Item_Infos> Choices = new List<Item_Infos>();

            foreach (Item_Infos Info in Q.Rewards.Keys)
                if (ItemsInterface.CanUse(Info, Plr, true, false, false, false))
                    Choices.Add(Info);

            return Choices;
        }
        static public void BuildQuestInfo(PacketOut Out,Player Plr, Quest Q)
        {
            BuildQuestHeader(Out, Q, true);

            List<Item_Infos> Choices = GetChoice(Plr, Q);

            Out.WriteByte(Q.ChoiceCount);
            Out.WriteByte(0);
            Out.WriteByte((byte)Choices.Count);

            foreach (Item_Infos Info in Choices)
                Item.BuildItem(ref Out, null, Info, 0, 1);

            Out.WriteByte((byte)Q.Objectives.Count);

            foreach (Quest_Objectives Objective in Q.Objectives)
            {
                Out.WriteByte((byte)Objective.ObjCount);
                Out.WritePascalString(Objective.Description);
            }

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
    }
}

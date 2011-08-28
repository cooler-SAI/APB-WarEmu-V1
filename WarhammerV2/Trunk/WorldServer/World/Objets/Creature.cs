using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Common;
using FrameWork;

namespace WorldServer
{
    public class Creature : Unit
    {
        public Creature_spawn Spawn;
        public uint Entry
        {
            get
            {
                if (Spawn == null)
                    return 0;

                return Spawn.Entry;
            }
        }

        public Creature()
            : base()
        {
        }

        public Creature(Creature_spawn Spawn) : this()
        {
            this.Spawn = Spawn;
            Name = Spawn.Proto.Name;
        }

        public override void Update()
        {
            base.Update();
        }

        static public ushort GenerateWounds(byte Level, byte Rank)
        {
            float Wounds = 0;
            Wounds += 52 * (Level + 1);
            Wounds += Level * 2.5f;
            if (Rank > 0)
                Wounds += Rank * (5.85f * Level * 52);
            return (ushort)(Wounds/10);
        }
        public override void OnLoad()
        {
            InteractType = GenerateInteractType(Spawn.Title);

            Faction = Spawn.Proto.Faction;
            while (Faction >= 8) Faction -= 8;
            if (Faction < 2) Rank = 0;
            else if (Faction < 4) Rank = 1;
            else if (Faction < 6) Rank = 2;
            else if (Faction < 9) Rank = 3;
            Faction = Spawn.Proto.Faction;

            ItmInterface.Load(WorldMgr.GetCreatureItems(Spawn.Entry));
            Level = (byte)RandomMgr.Next((int)Spawn.Proto.MinLevel, (int)Spawn.Proto.MaxLevel);
            StsInterface.SetBaseStat((byte)GameData.Stats.STATS_WOUNDS, GenerateWounds(Level,Rank));
            StsInterface.ApplyStats();
            Health = TotalHealth;

            X = Zone.CalculPin((uint)(Spawn.WorldX), true);
            Y = Zone.CalculPin((uint)(Spawn.WorldY), false);
            Z = (ushort)(Spawn.WorldZ * 2);

            Heading = (ushort)Spawn.WorldO;
            SetOffset((ushort)(Spawn.WorldX >> 12), (ushort)(Spawn.WorldY >> 12));
            Region.UpdateRange(this);
                
            base.OnLoad();
        }
        public override void SendMeTo(Player Plr)
        {
            List<byte> TmpState = new List<byte>();

            if (QtsInterface.CreatureHasFinisherQuest(Plr))
                TmpState.Add(7);
            else if(QtsInterface.CreatureHasStartQuest(Plr))
                TmpState.Add(5);
            else if(QtsInterface.CreatureHasRunningQuest(Plr))
                TmpState.Add(4);

            PacketOut Out = new PacketOut((byte)Opcodes.F_CREATE_MONSTER);
            Out.WriteUInt16(Oid);
            Out.WriteUInt16(0);

            Out.WriteUInt16((UInt16)Spawn.WorldO);
            Out.WriteUInt16((UInt16)Spawn.WorldZ);
            Out.WriteUInt32((UInt32)Spawn.WorldX);
            Out.WriteUInt32((UInt32)Spawn.WorldY);
            Out.WriteUInt16(0); // Speed Z
            // 18
            Out.WriteUInt16(Spawn.Proto.Model1);
            Out.WriteByte((byte)Spawn.Proto.MinScale);
            Out.WriteByte(Spawn.Proto.MinLevel);
            Out.WriteByte(Spawn.Proto.Faction);

            Out.Fill(0, 4);
            Out.WriteByte(Spawn.Emote);
            Out.WriteByte(0); // ?
            Out.WriteUInt16(Spawn.Proto._Unks[1]);
            Out.WriteByte(0);
            Out.WriteUInt16(Spawn.Proto._Unks[2]);
            Out.WriteUInt16(Spawn.Proto._Unks[3]);
            Out.WriteUInt16(Spawn.Proto._Unks[4]);
            Out.WriteUInt16(Spawn.Proto._Unks[5]);
            Out.WriteUInt16(Spawn.Proto._Unks[6]);
            Out.WriteUInt16(Spawn.Title);

            Out.WriteByte((byte)(Spawn.bBytes.Length + States.Count + TmpState.Count));
            Out.Write(Spawn.bBytes, 0, Spawn.bBytes.Length);
            Out.Write(States.ToArray(), 0, States.Count);
            Out.Write(TmpState.ToArray(), 0, TmpState.Count);

            Out.WriteByte(0);

            Out.WriteStringBytes(Name);

            Out.WriteByte(0); // ?
            Out.WriteByte(1); // ?
            Out.WriteByte(10); // ?

            Out.WriteByte(0); // ?

            Out.WriteUInt16(0); // ?
            Out.WriteByte(Spawn.Icone);
            Out.WriteByte((byte)Spawn.Proto._Unks[0]);

            Out.WriteByte(0);

            Out.Fill(0, 8); // Flags;

            Out.WriteByte(100); // Health %

            Out.WriteUInt16(Zone.ZoneId);

            Out.Fill(0, 48);

            Plr.SendPacket(Out);

            base.SendMeTo(Plr);
        }
        public override void SendInteract(Player Plr, InteractMenu Menu)
        {
            Log.Success("SendInteract", "" + Name + " -> " + Plr.Name + ",Type="+InteractType);

            if (!IsDead)
            {
                Plr.QtsInterface.HandleEvent(Objective_Type.QUEST_SPEACK_TO, Spawn.Entry, 1);

                string Text = WorldMgr.GetCreatureText(Spawn.Entry, Spawn.Title);

                switch (InteractType)
                {
                    case GameData.InteractType.INTERACTTYPE_DYEMERCHANT:
                        {
                            if (Menu.Menu == 9) // List des objets a vendre
                                WorldMgr.SendVendor(Plr, Spawn.Entry);
                            else if (Menu.Menu == 11) // Achat d'un item
                                WorldMgr.BuyItemVendor(Plr, Menu, Spawn.Entry);
                            else if (Menu.Menu == 14) // Vend un Item
                                Plr.ItmInterface.SellItem(Menu);
                            else if (Menu.Menu == 36) // Rachette un item
                                Plr.ItmInterface.BuyBackItem(Menu);
                            else
                            {
                                PacketOut Out = new PacketOut((byte)Opcodes.F_INTERACT_RESPONSE);
                                Out.WriteByte(0);
                                Out.WriteUInt16(Oid);
                                Out.WriteUInt16(0);
                                Out.WriteByte(0x40); // Dye
                                Out.WriteByte(0x22); // Vendors
                                Out.WriteByte(0);
                                Out.WritePascalString(Text);
                                Out.WriteByte(0);
                                Plr.SendPacket(Out);
                            }
                        } break;

                    default:
                        QtsInterface.HandleInteract(Plr, Menu);
                        break;
                };
            }

            base.SendInteract(Plr, Menu);
        }

        public override void SetDeath(Unit Killer)
        {
            Killer.QtsInterface.HandleEvent(Objective_Type.QUEST_KILL_MOB, Spawn.Entry,1);
            base.SetDeath(Killer);
        }
    }
}

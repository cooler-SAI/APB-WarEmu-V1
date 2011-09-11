﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Common;
using FrameWork;

namespace WorldServer
{
    public class Unit : Object
    {
        #region Static

        static public int HEALTH_REGEN_TIME = 4000; // 4secondes
        static public int ACTION_REGEN_TIME = 1000; // 1seconde
        static public int STATE_INTERVAL = 10000; // 10secondes

        static public GameData.InteractType GenerateInteractType(UInt16 Title)
        {
            GameData.InteractType Type = GameData.InteractType.INTERACTTYPE_IDLE_CHAT;

            switch (Title)
            {
                case 1: goto case 40;
                case 2: goto case 40;
                case 3: goto case 40;
                case 40:
                    Type = GameData.InteractType.INTERACTTYPE_TRAINER;
                    break;

                case 18: goto case 131;
                case 131:
                    Type = GameData.InteractType.INTERACTTYPE_FLIGHT_MASTER;
                    break;

                case 10: goto case 135;
                case 11: goto case 135;
                case 12: goto case 135;
                case 13: goto case 135;
                case 14: goto case 135;
                case 15: goto case 135;
                case 16: goto case 135;
                case 31: goto case 135;
                case 34: goto case 135;
                case 115: goto case 135;
                case 116: goto case 135;
                case 117: goto case 135;
                case 118: goto case 135;
                case 119: goto case 135;
                case 120: goto case 135;
                case 121: goto case 135;
                case 122: goto case 135;
                case 125: goto case 135;
                case 126: goto case 135;
                case 127: goto case 135;
                case 128: goto case 135;
                case 129: goto case 135;
                case 130: goto case 135;
                case 135:
                    Type = GameData.InteractType.INTERACTTYPE_DYEMERCHANT;
                break;

            };

            return Type;
        }

        #endregion

        public List<byte> States = new List<byte>();
        public GameData.InteractType InteractType = GameData.InteractType.INTERACTTYPE_IDLE_CHAT;

        public Point3D SpawnPoint = new Point3D(0, 0, 0);
        public UInt16 SpawnHeading = 0;

        public Unit()
            : base()
        {
            ItmInterface = new ItemsInterface(this);
            CbtInterface = new CombatInterface(this);
            StsInterface = new StatsInterface(this);
            QtsInterface = new QuestsInterface(this);
            MvtInterface = new MovementInterface(this);
            AbtInterface = new AbilityInterface(this);
        }

        public override void OnLoad()
        {
            SpawnPoint.X = X;
            SpawnPoint.Y = Y;
            SpawnPoint.Z = Z;
            SpawnHeading = Heading;
            if (EvtInterface == null)
                EvtInterface = new EventInterface(this);

            base.OnLoad();
        }

        public override void Dispose()
        {
            ItmInterface.Stop();
            CbtInterface.Stop();
            StsInterface.Stop();
            QtsInterface.Stop();
            MvtInterface.Stop();
            AbtInterface.Stop();
            EvtInterface.Stop();

            base.Dispose();
        }
        public override void Update()
        {
            int Tick = Environment.TickCount;

            UpdateHealth(Tick);
            CbtInterface.Update(Tick);
            ItmInterface.Update(Tick);
            StsInterface.Update(Tick);
            QtsInterface.Update(Tick);
            MvtInterface.Update(Tick);
            AbtInterface.Update(Tick);
            EvtInterface.Update(Tick);

            if (NextSend < Tick)
            {
                NextSend = Tick + STATE_INTERVAL;
                SendState(null);
            }

            base.Update();
        }

        #region Sender

        private int NextSend = 0;

        public override void SendMeTo(Player Plr)
        {
            ItmInterface.SendEquiped(Plr);
            SendState(Plr);
            base.SendMeTo(Plr);
        }
        public virtual void SendState(Player Plr)
        {
            if (IsPlayer())
                return;

            PacketOut Out = new PacketOut((byte)Opcodes.F_OBJECT_STATE);
            Out.WriteUInt16(Oid);
            Out.WriteUInt16((ushort)X);
            Out.WriteUInt16((ushort)Y);
            Out.WriteUInt16((ushort)Z);
            Out.WriteByte(PctHealth);
            Out.WriteUInt16(Zone.ZoneId);
            Out.Fill(0, 5);
            Out.WriteUInt16R(Heading);
            if (Plr == null)
                DispatchPacket(Out, false);
            else
                Plr.SendPacket(Out);
        }

        #endregion

        #region Interfaces

        public MovementInterface MvtInterface;
        public ItemsInterface ItmInterface;
        public CombatInterface CbtInterface;
        public StatsInterface StsInterface;
        public QuestsInterface QtsInterface;
        public AbilityInterface AbtInterface;

        #endregion

        #region Health&&Damage

        public uint _Health=0;
        public uint MaxHealth = 0;
        public uint BonusHealth = 0;

        public uint Health
        {
            get { return _Health; }
            set
            {
                _Health = value;
                if (IsPlayer())
                    GetPlayer().SendHealh();
                else
                    SendState(null);
            }
        }

        public uint TotalHealth { get { return MaxHealth + BonusHealth; } }
        public byte PctHealth { get { return (byte)((Health * 100) / TotalHealth); } }
        public bool IsDead 
        { 
            get 
            {
                if (Health <= 0)
                    return true;

                return false;
            } 
        }

        public int NextHpRegen = 0;
        public void UpdateHealth(int Tick)
        {

            if (Tick >= NextHpRegen)
            {
                NextHpRegen = Tick + HEALTH_REGEN_TIME;

                if (CbtInterface.IsFighting())
                    return;

                if (!IsDead && Health < TotalHealth)
                {
                    uint Regen = TotalHealth / 8;

                    if (Health + Regen > TotalHealth)
                        Health = TotalHealth;
                    else
                        Health += Regen;
                }
            }
        }

        public virtual void Strike(Unit Target)
        {
            if (Target == null || Target.IsDead)
                return;

            int Damage = 0;
            int DmgReduce = 0;
            int RealDamage = 0;

            Damage = StsInterface.CalculDamage();
            DmgReduce = Target.StsInterface.CalculReduce();

            DmgReduce = Math.Min(75, DmgReduce);

            RealDamage = (int)(Damage-( (float)(DmgReduce/100 * DmgReduce)));

            Log.Success("Strike","["+ Name + "] Strike -> " + Target.Name +"Dmg="+Damage+",Reduce="+DmgReduce);

            SendAttackState(Target, (UInt16)RealDamage, (UInt16)Damage);
            SendAttackMovement(Target);
            DealDamage(Target,RealDamage,Damage,0);
        }
        public void SendAttackState(Unit Target,UInt16 RealDamage,UInt16 Damage)
        {
            if (Target == null)
                return;

            // Frappe
            {
                PacketOut Out = new PacketOut((byte)Opcodes.F_USE_ABILITY);
                Out.WriteUInt16(0);
                Out.WriteUInt16(Oid);
                Out.WriteUInt16(Oid);
                Out.WriteUInt16(0);
                Out.WriteUInt16(Target.Oid);
                Out.WriteByte(2);
                Out.Fill(0, 9);
                DispatchPacket(Out, true);
            }

            {
                PacketOut Out = new PacketOut((byte)Opcodes.F_CAST_PLAYER_EFFECT);
                Out.WriteUInt16(Oid);
                Out.WriteUInt16(Target.Oid);
                Out.WriteUInt16(0);
                Out.WriteByte(0);
                Out.WriteByte((byte)GameData.CombatEvent.COMBATEVENT_HIT);
                Out.WriteByte(0x13);
                Out.WriteByte((byte)(RealDamage > 0 ? (RealDamage * 2) - 1 : 0));
                Out.WriteByte((byte)( (Damage-RealDamage)*2));
                Out.WriteByte(0);
                DispatchPacket(Out, true);
            }
        }
        public void SendAttackMovement(Unit Target)
        {
            Log.Success("SendAttackMovement", "Target =" + Target.Name);

            /*PacketOut Out = new PacketOut((byte)Opcodes.F_HIT_PLAYER);
            Out.WriteUInt16(Oid);
            Out.WriteUInt16(Target.Oid);
            Out.WriteByte(2);
            Out.WriteByte(Target.PctHealth);
            Out.WriteByte(Target.PctHealth);
            Out.WriteByte(0);
            DispatchPacket(Out, true);*/
        }
        public virtual void DealDamage(Unit Target, int RealDamage, int Mitiged, byte Event)
        {
            if (Target == null || Target.IsDead)
                return;

            Log.Success("DealDamage",Name + " Deal " + RealDamage + "/"+Target.Health + "/"+Target.PctHealth + "% To " + Target.Name);
            CbtInterface.OnDealDamage(Target, (UInt32)RealDamage);

            if (Target.Health <= RealDamage)
            {
                Target.SetDeath(this);
                CbtInterface.OnTargetDie(Target);
            }
            else
            {
                Target.Health -= (uint)RealDamage;
                Target.CbtInterface.OnTakeDamage(this, (UInt32)RealDamage);
            }
        }
        public virtual void SetDeath(Unit Killer)
        {
            Health = 0;

            States.Add(3); // Death State

            PacketOut Out = new PacketOut((byte)Opcodes.F_OBJECT_DEATH);
            Out.WriteUInt16(Oid);
            Out.WriteByte(1);
            Out.WriteByte(0);
            Out.WriteUInt16(Killer.Oid);
            Out.Fill(0, 6);
            DispatchPacket(Out, true);

            CbtInterface.Evade();

            WorldMgr.GenerateXP(Killer, this);
            WorldMgr.GenerateRenown(Killer.GetPlayer(), GetPlayer());
            GenerateLoot(Killer);
        }

        public virtual void RezUnit()
        {
            CbtInterface.Evade();
            Health = TotalHealth;
        }

        #endregion

        #region Loots

        public Loot Loots = null;

        public void GenerateLoot(Unit Killer)
        {
            if (Killer == null)
                return;

            Loots = LootsMgr.GenerateLoot(this, Killer);
            if (Loots != null && Killer.IsPlayer())
                SetLootable(true, Killer.GetPlayer());
        }

        public void SetLootable(bool Value,Player Looter)
        {
            PacketOut Out = new PacketOut((byte)Opcodes.F_UPDATE_STATE);
            Out.WriteUInt16(Oid);
            Out.WriteByte(9);
            Out.WriteByte((byte)(Value ? 1 : 0));
            Out.Fill(0, 6);
            if (Looter != null)
                Looter.SendPacket(Out);
            else
                DispatchPacket(Out, false);
        }

        #endregion

        #region Interact

        public override void SendInteract(Player Plr, InteractMenu Menu)
        {
            if (IsDead && Loots != null)
            {
                Loots.SendInteract(Plr, Menu);
                if (!Loots.IsLootable())
                    SetLootable(false, Plr);
            }

            base.SendInteract(Plr, Menu);
        }

        #endregion

        #region Values

        public UInt16 _Speed = 100;
        public UInt16 Speed
        {
            get { return _Speed; }
            set
            {
                _Speed = value;
                if (IsPlayer())
                    GetPlayer().SendSpeed();
            }
        }

        private byte _Level = 1;
        public byte Level
        {
            get
            {
                if (IsPlayer())
                    return GetPlayer()._Value.Level;
                else
                    return _Level;
            }
            set
            {
                if (IsPlayer())
                    GetPlayer().SetLevel(value);
                else
                    _Level = value;
            }
        }

        private UInt16 _Model = 0;
        public UInt16 Model
        {
            get
            {
                if (IsPlayer())
                    return GetPlayer()._Info.ModelId;
                else
                    return _Model;
            }
            set
            {
                if (IsPlayer())
                    GetPlayer()._Info.ModelId = (byte)value;
                else
                    _Model = value;
            }
        }

        public byte Rank = 0;
        public byte Faction = 0;

        #endregion
    }
}

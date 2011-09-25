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
    public class AggroInfo
    {
        public AggroInfo(UInt16 Oid)
        {
            this.Oid = Oid;
        }

        public UInt16 Oid;

        public UInt64 DamagesDeal;
        public UInt64 DamagesReceive;

        public UInt64 HealsDeal;
        public UInt64 HealsReceive;

        public float GetHate(UInt64 TotalDamage)
        {
            if (TotalDamage <= 0)
                return 0;

            float Hate = 0.0f;

            Hate = (float)((float)DamagesDeal / (float)TotalDamage) * 100.0f;

            return Hate;
        }

        public void ResetHeals()
        {
            HealsDeal = 0;
            HealsReceive = 0;
        }
        public void ResetDamages()
        {
            DamagesDeal = 0;
            DamagesReceive = 0;
        }
        public void Reset()
        {
            ResetDamages();
            ResetHeals();
        }
    }

    public class TargetInfo
    {
        public TargetInfo(UInt16 Oid,GameData.TargetTypes Type)
        {
            this.Target = Oid;
            this.Type = Type;
        }

        public UInt16 Target;
        public GameData.TargetTypes Type;
    };

    public enum AiState
    {
        STANDING = 0,
        MOVING = 1,
        FIGHTING = 2,
        BACK = 3,
    }

    public class CombatInterface : BaseInterface
    {
        public TargetInfo Target = null;
        public bool Attacking = false;
        public long NextAttackTime = 0;

        public AiState State = AiState.STANDING;

        public CombatInterface(Object Obj)
            : base(Obj)
        {

        }

        public void RemoveTarget()
        {
            Target = null;
            Attacking = false;
        }
        public void SetTarget(UInt16 Oid, GameData.TargetTypes Type)
        {
            RemoveTarget();

            if (Oid != 0)
                Target = new TargetInfo(Oid, Type);
        }
        public bool HasTarget()
        {
            if (Target == null)
                return false;

            return true;
        }
        public bool IsFriendTarget()
        {
            if (!HasTarget())
                return false;

            if (Target.Type != GameData.TargetTypes.TARGETTYPES_TARGET_ALLY)
                return false;

            return true;
        }
        public Object GetObjectTarget()
        {
            if (!HasTarget())
                return null;

            return Obj.Region.GetObject(Target.Target);
        }
        public Unit GetUnitTarget()
        {
            Object Targ = GetObjectTarget();

            if (Targ == null || !Targ.IsUnit())
                return null;

            return GetObjectTarget().GetUnit();
        }
        public Player GetPlayerTarget()
        {
            Object Targ = GetObjectTarget();

            if (Targ == null || !Targ.IsPlayer())
                return null;

            return Targ.GetPlayer();
        }

        public override void Update(long Tick)
        {
            if (!HasUnit())
                return;

            Unit Me = GetUnit();
            Unit Target = GetNextTarget();

            if (Target == null)
                Attacking = false;

            if (HasPlayer())
            {
                if (Attacking)
                {
                    if (NextAttackTime < Tick)
                    {
                        if (!Obj.IsWithinRadius(Target, 10))
                        {
                            NextAttackTime += 200;
                        }
                        else if (!Obj.IsObjectInFront(Target, 90))
                        {
                            NextAttackTime += 200;
                        }
                        else
                        {
                            Me.Strike(Target);
                            NextAttackTime = Tick + Me.ItmInterface.GetAttackTime(EquipSlot.MAIN_DROITE) * 10;
                        }
                    }
                }
            }
            else
                AttackAI(Target);
        }

        public void AttackAI(Unit Target)
        {
            if (Target == null)
            {
                if (State == AiState.FIGHTING)
                    CombatStop();

                return;
            }

            long Tick = TCPManager.GetTimeStampMS();

            LookAt(Target);

            if (NextAttackTime < Tick)
            {
                if (Obj.IsWithinRadius(Target, 10))
                {
                    Obj.GetUnit().Strike(Target);
                    NextAttackTime = Tick + 2000;
                }
            }
        }

        public void LookAt(Unit Target)
        {
            if (HasPlayer())
                return;

            if (!Obj.IsCreature())
                return;

            if (CheckSpawnRange())
                return;

            Creature Crea = Obj.GetCreature();
            Crea.MvtInterface.FollowUnit(Target, 7, 10,200, eFormationType.Protect);
        }


        #region IASystem

        public bool IsFighting()
        {
            if (State == AiState.FIGHTING)
                return true;

            return false;
        }

        public void CombatStart(Unit Fighter)
        {
            Log.Success("CombatStart", Obj.Name + " Start combat with " + Fighter.Name);
            State = AiState.FIGHTING;
        }

        public void CombatStop()
        {
            Log.Success("CombatStop", Obj.Name + " Stop combat ");
            State = AiState.STANDING;
            ClearTargets();

            if (Obj.IsCreature())
                Obj.GetCreature().MvtInterface.StopFollow();

            if(Obj.IsCreature() && !Obj.GetCreature().IsDead)
                ReturnToSpawn();
        }

        public void Evade()
        {
            foreach (AggroInfo Info in _Aggros.Values.ToArray())
            {
                Object Target = Obj.Zone.GetObject(Info.Oid);
                if (Target == null || !Target.IsUnit())
                   continue;

                Target.GetUnit().CbtInterface.RemoveAggro(Obj.Oid);
            }

            CombatStop();
        }

        public void OnTakeDamage(Unit Fighter,UInt32 DamageCount)
        {
            switch (State)
            {
                case AiState.STANDING:
                    CombatStart(Fighter);
                    break;
            };

            AddDamageReceive(Fighter.Oid, DamageCount);
        }

        public void OnDealDamage(Unit Victim,UInt32 DamageCount)
        {
            switch (State)
            {
                case AiState.STANDING:
                    CombatStart(Victim);
                    break;
            };
        }

        public void OnTargetDie(Unit Victim)
        {
            RemoveAggro(Victim.Oid);

            if (GetAggroCount() <= 0)
                CombatStop();
        }

        public bool CheckSpawnRange()
        {
            if (!Obj.IsCreature())
                return false;

            Creature Crea = Obj.GetCreature();

            if (Crea.GetDistance(Crea.SpawnPoint) > 200)
            {
                Evade();
                ReturnToSpawn();
                return true;
            }

            return false;
        }
        public void ReturnToSpawn()
        {
            if (!Obj.IsCreature())
                return;

            Creature Crea = Obj.GetCreature();
            Crea.MvtInterface.CancelWalkTo();
            Crea.SetPosition((UInt16)Crea.SpawnPoint.X, (UInt16)Crea.SpawnPoint.Y, (UInt16)Crea.SpawnPoint.Z, Crea.SpawnHeading);
        }

        #endregion

        #region AggroSystem

        public Dictionary<UInt16, AggroInfo> _Aggros = new Dictionary<ushort, AggroInfo>();
        public AggroInfo GetAggro(UInt16 Oid)
        {
            AggroInfo Info = null;
            if (!_Aggros.TryGetValue(Oid, out Info))
            {
                Info = new AggroInfo(Oid);
                _Aggros.Add(Oid, Info);
            }

            return Info;
        }
        public void RemoveAggro(UInt16 Oid)
        {
            _Aggros.Remove(Oid);
        }
        public UInt32 GetAggroCount()
        {
            return (UInt32)_Aggros.Count;
        }

        public void ClearTargets()
        {
            _Aggros.Clear();
        }
        public void AddDamageReceive(UInt16 Oid, UInt32 Damages)
        {
            AggroInfo Info = GetAggro(Oid);
            Info.DamagesDeal = Damages;
        }

        public UInt64 TotalDamageReceive()
        {
            UInt64 Damages = 0;

            foreach (AggroInfo Info in _Aggros.Values.ToArray())
                Damages += Info.DamagesDeal;

            return Damages;
        }

        public Unit GetNextTarget()
        {
            Unit Me = GetUnit();
            Unit Target = null;

            if (!Me.IsPlayer())
            {
                UInt16 TargetOid = 0;

                UInt64 TotalDamages = TotalDamageReceive();
                float MaxHate = 0;

                foreach (AggroInfo Info in _Aggros.Values.ToArray())
                {
                    float Hate = Info.GetHate(TotalDamages);

                    if (Hate > MaxHate)
                    {
                        MaxHate = Hate;
                        TargetOid = Info.Oid;
                    }
                }

                if (TargetOid <= 0)
                    Target = null;
                else
                    Target = Obj.Region.GetObject(TargetOid) as Unit;
            }
            else
            {
                if (!IsFriendTarget())
                    Target = GetUnitTarget();
            }

            if (Target == null)
                return null;

            if (Target.IsDead && State == AiState.FIGHTING)
                OnTargetDie(Target);

            if (!Target.IsInWorld())
                return null;

            if (Target.IsDead)
                return null;

            return Target;
        }

        #endregion

        #region Follower

        public List<UInt16> _Followers = new List<ushort>();

        public void AddFollower(UInt16 Oid)
        {
            if (!_Followers.Contains(Oid))
                _Followers.Add(Oid);
        }
        public void RemoveFollower(UInt16 Oid)
        {
            _Followers.Remove(Oid);
        }
        public int GetFollowId(UInt16 Oid)
        {
            for (int i = 0; i < _Followers.Count; ++i)
                if (_Followers[i] == Oid)
                    return 1+i;

            return 0;
        }
        public float GetFollowPct(UInt16 Oid)
        {
            float Id = GetFollowId(Oid);
            float Count = _Followers.Count;

            return (Id / Count) * 100;
        }

        #endregion
    }
}

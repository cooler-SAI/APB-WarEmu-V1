using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Common;
using FrameWork;

namespace WorldServer
{
    public class Group
    {
        static public int MaxGroupId = 2;

        public int GroupId;
        public List<Player> Players;
        public Player Owner;

        public Group()
        {
            Players = new List<Player>();
            GroupId = System.Threading.Interlocked.Increment(ref MaxGroupId);
        }

        public bool IsFull()
        {
            return Players.Count >= 6;
        }

        public bool IsOwner(Player Plr)
        {
            return Owner == Plr;
        }

        public void SetOwner(Player Plr)
        {
            this.Owner = Plr;

            lock(Players)
            {
                foreach (Player SubPlayer in Players)
                {
                    SubPlayer.SendLocalizeString(Owner.Name, GameData.Localized_text.TEXT_GROUP_NEW_LEADER);
                }
            }
        }

        public void AddPlayer(Player Plr)
        {
            lock (Players)
            {
                Plr.GrpInterface.CurrentGroup = this;

                foreach (Player SubPlayer in Players)
                {
                    SubPlayer.SendLocalizeString(Plr.Name, GameData.Localized_text.TEXT_PLAYER_JOIN_PARTY);
                }

                Players.Add(Plr);

                SendGroupCharacters();
            }
        }

        public void RemovePlayer(Player Plr)
        {
            lock (Players)
            {
                if (Players.Remove(Plr))
                {
                    Plr.GrpInterface.CurrentGroup = null;

                    foreach (Player SubPlayer in Players)
                    {
                        SubPlayer.SendLocalizeString(Plr.Name, GameData.Localized_text.TEXT_X_LEFT_PARTY);
                    }

                    SendEmptyGroup(Plr);

                    if (Players.Count <= 1)
                        Close();
                    else
                    {
                        if (Owner == Plr)
                            SetOwner(Players[0]);

                        SendGroupCharacters();
                    }
                }
            }
        }

        public GameData.Localized_text CanInvite(Player Plr)
        {
            if (Players.Contains(Plr))
                return GameData.Localized_text.TEXT_GROUP_ALREADY_IN_ONE;

            if (IsFull())
                return GameData.Localized_text.TEXT_PARTY_IS_FULL;

            return GameData.Localized_text.TEXT_GROUP_INVITE_BEGIN;
        }

        public void Close()
        {
            lock (Players)
            {
                foreach (Player Plr in Players)
                {
                    Plr.GrpInterface.CurrentGroup = null;
                    SendEmptyGroup(Plr);
                }
                Players.Clear();
            }
        }

        public void SendEmptyGroup(Player Plr)
        {
            PacketOut Out = new PacketOut((byte)Opcodes.F_CHARACTER_INFO);
            Out.WriteByte(6);
            Out.WriteByte(2);
            Out.WriteUInt16((ushort)0x8556);
            Out.WriteByte(0);
            Plr.SendPacket(Out);

            Plr.SendLocalizeString("", GameData.Localized_text.TEXT_YOU_LEFT_PARTY);
        }

        public void SendPacket(PacketOut Out)
        {
            foreach (Player Plr in Players)
                Plr.SendCopy(Out);
        }

        public void SendGroupCharacters()
        {
            PacketOut Out = new PacketOut((byte)Opcodes.F_CHARACTER_INFO);
            Out.WriteByte(6); // Group Info
            Out.WriteByte(2);
            Out.WriteUInt16((ushort)0x8556);

            lock (Players)
            {
                Out.WriteByte((byte)Players.Count);

                Player Plr;
                for (int i = 0; i < Players.Count; ++i)
                {
                    Plr = Players[i];

                    Out.WriteByte(0xE4);
                    Out.WriteByte(0xB3);
                    Out.WriteByte(0x7D);
                    Out.WriteByte(0x0F);

                    Out.WriteByte(0x18); // Race
                    Out.WriteByte(0x06);

                    Out.WriteByte(Plr.Level);
                    Out.WriteUInt16(Plr._Info.Career);
                    Out.WriteByte(02);
                    Out.WriteUInt16(0);
                    Out.WriteByte(1); // 1 Online, 0 Offline
                    Out.WriteByte((byte)Plr.Name.Length);
                    Out.WriteUInt16(0);
                    Out.WriteByte(0);
                    Out.WriteStringBytes(Plr.Name);
                    Out.WriteByte(0);

                    Out.WriteHexStringBytes("00 00 00 E1 A9 01 87 31 B5 5D EB 6E 9E 40 01 02 D8 01 03 64 64 00 ".Replace(" ",string.Empty));
                }
            }
            SendPacket(Out);
        }

        public void AddXp(Player Killer, Unit Victim)
        {
            lock (Players)
            {
                uint Count = 0;
                foreach (Player Plr in Players)
                {
                    if (Plr == Killer || Plr.GetDistance(Killer) < 100)
                    {
                        ++Count;
                    }
                }

                foreach (Player Plr in Players)
                {
                    if (Plr == Killer || Plr.GetDistance(Killer) < 100)
                    {
                        Plr.AddXp(WorldMgr.GenerateXPCount(Plr, Victim) / Count);
                    }
                }
            }
        }

        public void AddRenown(Player Killer, Player Victim)
        {
            lock (Players)
            {
                uint Count = 0;
                foreach (Player Plr in Players)
                {
                    if (Plr == Killer || Plr.GetDistance(Killer) < 100)
                    {
                        ++Count;
                    }
                }

                foreach (Player Plr in Players)
                {
                    if (Plr == Killer || Plr.GetDistance(Killer) < 100)
                    {
                        Plr.AddRenown(WorldMgr.GenerateRenownCount(Plr, Victim) / Count);
                    }
                }
            }
        }

        public void AddMoney(UInt32 Count)
        {
            lock (Players)
                foreach (Player Plr in Players)
                    Plr.AddMoney(Count);
        }
    }
}

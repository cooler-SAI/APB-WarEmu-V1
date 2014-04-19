/*
 * Copyright (C) 2013 APS
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
    public class GroupInterface : BaseInterface
    {
        public Group CurrentGroup;
        public Group PendingGroup;
        public Player PendingInviter;

        public bool IsInGroup()
        {
            return CurrentGroup != null;
        }

        public bool HasInvitation()
        {
            return PendingInviter != null || PendingGroup != null;
        }

        public void ReceiveInvitation(Player Sender)
        {
            if (HasInvitation())
                return;

            PendingGroup = Sender.GrpInterface.CurrentGroup;
            PendingInviter = Sender;

            GetPlayer().SendLocalizeString(Sender.Name, GameData.Localized_text.TEXT_GROUP_YOU_WERE_INVITED);

            _Owner.EvtInterface.AddEvent(CleanInvitation, 60000, 1);
            PacketOut Out = new PacketOut((byte)Opcodes.F_SHOW_DIALOG);
            Out.WriteUInt16(2);
            Out.WriteByte(0);
            Out.WriteStringToZero(Sender.Name);
            GetPlayer().SendPacket(Out);
        }

        public void AcceptInvitation()
        {
            if (IsInGroup() || !HasInvitation())
            {
                CleanInvitation();
                return;
            }

            if (PendingInviter != null)
                PendingInviter.SendLocalizeString(_Owner.Name, GameData.Localized_text.TEXT_BG_ACCEPTED_YOUR_INVITE);

            if (PendingGroup != null)
            {
                if (PendingGroup.CanInvite(GetPlayer()) == GameData.Localized_text.TEXT_GROUP_INVITE_BEGIN)
                    PendingGroup.AddPlayer(GetPlayer());
            }
            else
            {
                Group grp = new Group();
                grp.AddPlayer(PendingInviter);
                grp.AddPlayer(GetPlayer());
                grp.SetOwner(PendingInviter);
            }

            CleanInvitation();
        }

        public void DeclineInvitation()
        {
            if (!HasInvitation())
                return;

            if (PendingInviter != null)
                PendingInviter.SendLocalizeString(_Owner.Name, GameData.Localized_text.TEXT_BG_DECLINED_YOUR_INVITE);
            
            GetPlayer().SendLocalizeString("", GameData.Localized_text.TEXT_BG_YOU_DECLINED);

            _Owner.EvtInterface.RemoveEvent(CleanInvitation);
            CleanInvitation();
        }

        public void CleanInvitation()
        {
            PendingGroup = null;
            PendingInviter = null;
        }

        public GameData.Localized_text CanInvite(Player Plr)
        {
            if (Plr == null)
                return GameData.Localized_text.TEXT_PLAYERORG_GUILD_NOLONGER_EXISTS;

            if (GetPlayer() == Plr)
                return GameData.Localized_text.TEXT_GROUP_INVITE_ERR_SELF;

            if (Plr.Realm != GetPlayer().Realm)
                return GameData.Localized_text.TEXT_GROUP_REFER_ERR_ENEMY;

            if (Plr.GrpInterface.IsInGroup())
            {
                if (Plr.GrpInterface.CurrentGroup.Players.Count <= 1)
                {
                    Plr.GrpInterface.CurrentGroup.Close();
                    Plr.GrpInterface.CurrentGroup = null;
                }
                else
                    return GameData.Localized_text.TEXT_GROUP_ALREADY_IN_ONE;
            }

            if (Plr.GrpInterface.HasInvitation())
                return GameData.Localized_text.TEXT_GROUP_INVITE_ERR_INVITEGONE;

            if (CurrentGroup != null)
                return CurrentGroup.CanInvite(Plr);

            return GameData.Localized_text.TEXT_GROUP_INVITE_BEGIN;
        }

        public void QuitGroup()
        {
            if (IsInGroup())
                CurrentGroup.RemovePlayer(GetPlayer());
        }

        public void Say(string Message)
        {
            if (IsInGroup())
            {
                lock(CurrentGroup.Players)
                    foreach (Player Plr in CurrentGroup.Players)
                        Plr.SendMessage(_Owner, Message, SystemData.ChatLogFilters.CHATLOGFILTERS_GROUP);
            }
        }

        public override void Stop()
        {
            QuitGroup();
            base.Stop();
        }
    }

    static public class GroupHandler
    {
        [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.F_GROUP_COMMAND, "F_GROUP_COMMAND")]
        static public void F_GROUP_COMMAND(BaseClient client, PacketIn packet)
        {
            //Log.Dump("group", packet, true);
            GameClient cclient = client as GameClient;

            UInt32 Oid = packet.GetUint32();
            byte Type = packet.GetUint8();

            switch (Type)
            {
                case 2: // Accept
                    cclient.Plr.GrpInterface.AcceptInvitation();
                    break;

                case 3: // Quit
                    cclient.Plr.GrpInterface.QuitGroup();
                    break;

                case 6: // Decline
                    cclient.Plr.GrpInterface.DeclineInvitation();
                    break;
            }
        }
    }
}

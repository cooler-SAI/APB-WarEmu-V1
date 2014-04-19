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
    static public class MailsMgr
    {
        static public void SendMail(MailData Data, Player Plr)
        {
            PacketOut Out = new PacketOut((byte)Opcodes.F_MAIL);
            Out.WriteByte(0x0D); // Ouvrir un Mail
            Out.WriteByte(0);
            BuildPreviewMail(Out, Data, Plr.Name);
            BuildMail(Out, Data, Plr.Name);
            Plr.SendPacket(Out);
        }

        static public void SendMails(Player Plr)
        {
            PacketOut Out = new PacketOut((byte)Opcodes.F_MAIL);
            Out.WriteByte(0x0A);
            Out.WriteUInt16(0);
            lock (Plr.MlsInterface.Mails)
            {
                Out.WriteByte((byte)Plr.MlsInterface.Mails.Count);

                foreach (MailData Mail in Plr.MlsInterface.Mails)
                {
                    BuildPreviewMail(Out, Mail, Plr.Name);
                }

                Out.WriteUInt16(0);
            }

            Plr.SendPacket(Out);
        }

        static public void BuildPreviewMail(PacketOut Out, MailData Data, string ReceiverName)
        {
            Character_mail Mail = Data.Mail;
            Out.WriteUInt32(0);
            Out.WriteUInt32(Mail.Guid);
            Out.WriteUInt16((ushort)(Mail.Opened ? 1 : 0));
            Out.WriteByte(0x64); // Icon ID
            Out.WriteUInt32(Mail.SendDate);
            Out.WriteUInt32(Mail.SendDate);
            Out.WriteUInt32(Mail.SenderID);
            Out.WriteByte(0); // localized name

            Out.WriteByte(0);
            Out.WriteStringToZero(Mail.SenderName);

            Out.WriteByte(0);
            Out.WriteStringToZero(ReceiverName);

            Out.WriteByte(0);
            Out.WriteStringToZero(Mail.Title);

            Out.WriteUInt32(0); // ?
            Out.WriteUInt32(Mail.Money);

            Out.WriteUInt16((ushort)Data.Items.Count);
            for (int i = 0; i < Data.Items.Count; ++i)
            {
                Out.WriteByte(0);
                Out.WriteUInt32(Data.Items[i].Info.ModelId);
            }
        }

        static public void BuildMail(PacketOut Out, MailData Data, string ReceiverName)
        {
            Out.WriteStringToZero(Data.Mail.Message);
            Out.WriteByte((byte)Data.Items.Count);
            for (int i = 0; i < Data.Items.Count; ++i)
            {
                Item.BuildItem(ref Out, Data.Items[i], null, 0, 0);
            }
        }

        static public void SendMailsCount(Player Plr)
        {
            {
                PacketOut Out = new PacketOut((byte)Opcodes.F_MAIL);
                Out.WriteByte(0x09);
                Out.WriteByte(0); // ?
                Out.WriteUInt16(0); // Enchère Non lu
                Plr.SendPacket(Out);
            }
        }

        static public UInt32 GetMailCount(int CharacterID)
        {
            return (UInt32)CharMgr.Database.GetObjectCount<Character_mail>("ReceiverID = " + CharacterID);
        }
    }
}

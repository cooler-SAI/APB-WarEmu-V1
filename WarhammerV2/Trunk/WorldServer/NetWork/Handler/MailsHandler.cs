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
    static public class MailsHandler
    {
        [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.F_MAIL, (int)eClientState.Playing, "F_MAIL")]
        static public void F_MAIL(BaseClient client, PacketIn packet)
        {
            GameClient Client = client as GameClient;
            if (Client.Plr == null)
                return;

            byte Type = packet.GetUint8();

            switch (Type)
            {
                case 1: // Send Mail
                    {
                        packet.GetUint8(); // ?
                        string Username = packet.GetPascalString();

                        UInt16 Size = packet.GetUint16R();
                        string Title = packet.GetString(Size);

                        Size = packet.GetUint16R();
                        string Message = packet.GetString(Size);

                        Log.Success("SendMail", Username + " " + Title + " " + Message);
                    } break;
                case 2: // Open
                    {
                        packet.GetUint8();
                        UInt32 MailID = packet.GetUint32R();
                        MailData Data = Client.Plr.MlsInterface.GetMail(MailID);
                        if (Data == null)
                            return;

                        Data.Mail.Opened = true;
                        Data.Mail.Dirty = true;
                        CharMgr.Database.SaveObject(Data.Mail);

                        MailsMgr.SendMail(Data, Client.Plr);

                        MailsMgr.SendMails(Client.Plr);
                    }
                    break;

                case 3: // Reply

                    break;

                case 4: // Delete
                    {
                        packet.GetUint8();
                        UInt32 MailID = packet.GetUint32R();
                        MailData Data = Client.Plr.MlsInterface.GetMail(MailID);
                        if (Data == null)
                            return;

                        Client.Plr.MlsInterface.RemoveMail(Data);
                        CharMgr.Database.DeleteObject(Data.Mail);
                        foreach (Item Itm in Data.Items)
                        {
                            if (Itm != null)
                                CharMgr.Database.DeleteObject(Itm.CharItem);
                        }

                        MailsMgr.SendMails(Client.Plr);
                    }
                    break;

                case 8: // Take all Objects
                    {
                        packet.GetUint8();
                        UInt32 MailID = packet.GetUint32R();
                        MailData Data = Client.Plr.MlsInterface.GetMail(MailID);
                        if (Data == null)
                            return;

                        if (!Data.Mail.CR)
                        {
                            if (Data.Mail.Money != 0)
                            {
                                Client.Plr.AddMoney(Data.Mail.Money);
                                Data.Mail.Money = 0;
                                Data.Mail.Dirty = true;
                                CharMgr.Database.SaveObject(Data.Mail);
                            }
                        }

                        MailsMgr.SendMail(Data, Client.Plr);
                        MailsMgr.SendMails(Client.Plr);
                    } break;
                default:
                    Log.Debug("MailBox", "Open Type :" + Type);
                    break;
            }
        }
    }
}

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
    public class SocialHandlers : IPacketHandler
    {
        [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.F_SOCIAL_NETWORK, "onSocialNetWork")]
        static public void F_SOCIAL_NETWORK(BaseClient client, PacketIn packet)
        {
            GameClient cclient = client as GameClient;

            if (!cclient.IsPlaying() || !cclient.Plr.IsInWorld())
                return;

            Player Plr = cclient.Plr;

            byte Type = packet.GetUint8();

            switch (Type)
            {
                case 11: // Inspection
                    {
                        Player Target = Plr.CbtInterface.GetTarget() as Player;
                        if (Target == null)
                            Plr.SendLocalizeString("", GameData.Localized_text.TEXT_SN_LISTS_ERR_PLAYER_NOT_FOUND);
                        else
                            Target.ItmInterface.SendInspect(Plr);
                    }
                    break;
                case 8:
                    {
                        packet.Skip(1);
                        byte NameSize = packet.GetUint8();
                        packet.Skip(1);
                        string Name = packet.GetString(NameSize);
                        byte GuildSize = packet.GetUint8();
                        packet.Skip(1);
                        string GuildName = packet.GetString(GuildSize);
                        packet.Skip(1);
                        UInt16 Career = packet.GetUint16();
                        packet.Skip(4);
                        UInt16 ZoneId = packet.GetUint16();

                        while (ZoneId > 256)
                            ZoneId -= 256;

                        while (packet.GetUint8() != 0xFF) ;

                        packet.Skip(2 + (ZoneId == 255 ? 0 : 1));

                        byte MinLevel = packet.GetUint8();
                        byte MaxLevel = packet.GetUint8();

                        Plr.SocInterface.SendPlayers(Player.GetPlayers(Name, GuildName, Career, ZoneId, MinLevel, MaxLevel));

                    } break;

            }
        }
    }
}

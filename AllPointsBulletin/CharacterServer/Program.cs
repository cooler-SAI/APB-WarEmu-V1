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
using System.Reflection;

using FrameWork;

using Common;

namespace CharacterServer
{
    public class Program
    {
        static public CharacterServerConfig Config;
        static public RpcServer Server;

        [STAThread]
        static void Main(string[] args)
        {
            Log.Texte("", "-------------------------------", ConsoleColor.DarkBlue);
            Log.Texte("", "          _____   _____ ", ConsoleColor.Cyan);
            Log.Texte("", "    /\\   |  __ \\ / ____|", ConsoleColor.Cyan);
            Log.Texte("", "   /  \\  | |__) | (___  ", ConsoleColor.Cyan);
            Log.Texte("", "  / /\\ \\ |  ___/ \\___ \\ ", ConsoleColor.Cyan);
            Log.Texte("", " / ____ \\| |     ____) |", ConsoleColor.Cyan);
            Log.Texte("", "/_/    \\_\\_|    |_____/ APB-Char", ConsoleColor.Cyan);
            Log.Texte("", "http://AllPrivateServer.com", ConsoleColor.DarkCyan);
            Log.Texte("", "-------------------------------", ConsoleColor.DarkBlue);

            Assembly.Load("Common");

            Log.Info("CharacterServer", "Starting...");

            ConfigMgr.LoadConfigs();
            Config = ConfigMgr.GetConfig<CharacterServerConfig>();

            if (!Log.InitLog(Config.LogLevel, "CharacterServer"))
                ConsoleMgr.WaitAndExit(2000);

            CharacterMgr.Database = DBManager.Start(Config.CharacterDatabase.Total(), ConnectionType.DATABASE_MYSQL, "CharDB");
            if(CharacterMgr.Database == null)
                ConsoleMgr.WaitAndExit(2000);

            Server = new RpcServer(Config.RpcInfo.RpcClientStartingPort, 0);
            if (!Server.Start(Config.RpcInfo.RpcIp, Config.RpcInfo.RpcPort))
                ConsoleMgr.WaitAndExit(2000);

            Log.Success("CharacterServer", "Server loaded.");
            ConsoleMgr.Start();
        }
    }
}

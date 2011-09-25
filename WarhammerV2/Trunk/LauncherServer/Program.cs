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
using System.IO;

using Common;
using FrameWork;

namespace LauncherServer
{
    class Program
    {
        static public RpcClient Client = null;
        static public LauncherConfig Config = null;
        static public TCPServer Server = null;

        static public int Version
        {
            get
            {
                return Config.Version;
            }
        }
        static public string Message
        {
            get
            {
                return Config.Message;
            }
        }

        static public FileInfo Info;
        static public string StrInfo;

        static public AccountMgr AcctMgr
        {
            get
            {
                return Client.GetServerObject<AccountMgr>();
            }
        }

        [STAThread]
        static void Main(string[] args)
        {
            AppDomain.CurrentDomain.UnhandledException += new UnhandledExceptionEventHandler(onError);

            Log.Texte("", "-------------------------------", ConsoleColor.DarkBlue);
            Log.Texte("", "          _____   _____ ", ConsoleColor.Cyan);
            Log.Texte("", "    /\\   |  __ \\ / ____|", ConsoleColor.Cyan);
            Log.Texte("", "   /  \\  | |__) | (___  ", ConsoleColor.Cyan);
            Log.Texte("", "  / /\\ \\ |  ___/ \\___ \\ ", ConsoleColor.Cyan);
            Log.Texte("", " / ____ \\| |     ____) |", ConsoleColor.Cyan);
            Log.Texte("", "/_/    \\_\\_|    |_____/ Warhammer", ConsoleColor.Cyan);
            Log.Texte("", "http://AllPrivateServer.com", ConsoleColor.DarkCyan);
            Log.Texte("", "-------------------------------", ConsoleColor.DarkBlue);

            // Loading all configs files
            ConfigMgr.LoadConfigs();
            Config = ConfigMgr.GetConfig<LauncherConfig>();

            // Loading log level from file
            if (!Log.InitLog(Config.LogLevel, "LauncherServer"))
                ConsoleMgr.WaitAndExit(2000);

            Client = new RpcClient("LauncherServer", Config.RpcInfo.RpcLocalIp, 1);
            if (!Client.Start(Config.RpcInfo.RpcServerIp, Config.RpcInfo.RpcServerPort))
                ConsoleMgr.WaitAndExit(2000);

            Info = new FileInfo("Configs/mythloginserviceconfig.xml");
            if (!Info.Exists)
            {
                Log.Error("Configs/mythloginserviceconfig.xml", "Config file missing !");
                ConsoleMgr.WaitAndExit(5000);
            }

            StrInfo = Info.OpenText().ReadToEnd();

            if (!TCPManager.Listen<TCPServer>(Config.LauncherServerPort, "LauncherServer"))
                ConsoleMgr.WaitAndExit(2000);

            Server = TCPManager.GetTcp<TCPServer>("LauncherServer");

            ConsoleMgr.Start();
        }

        static void onError(object sender, UnhandledExceptionEventArgs e)
        {
            Log.Error("OnError", e.ExceptionObject.ToString());
        }
    }
}

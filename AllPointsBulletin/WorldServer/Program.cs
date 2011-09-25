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
using FrameWork.Logger;

using Common;

namespace WorldServer
{
    class Program
    {
        static public CharacterMgr CharMgr;
        static public FileManager FileMgr;

        static public string WorldName;
        static public int WorldId;

        static void Main(string[] args)
        {
            Assembly.Load("Common");
            Log.Info("WorldServer", "Démarrage ...");

            // Initialisation des Config de log et Générales
            if (!EasyServer.InitLog("World", "Configs/WorldLog.conf")
                || !EasyServer.InitConfig("Configs/World.xml", "World"))
                return;

            // Initialisation du Client Rpc pour le FileServer
            if (!EasyServer.InitRpcClient("WorldFile",
                                            EasyServer.GetConfValue<string>("World", "FileServer", "Key"),
                                            EasyServer.GetConfValue<string>("World", "FileServer", "Ip"),
                                            EasyServer.GetConfValue<int>("World", "FileServer", "Port")))
                return;

            // Initialisation du Client Rpc pour le CharacterServer
            if (!EasyServer.InitRpcClient("WorldChar",
                                            EasyServer.GetConfValue<string>("World", "CharacterServer", "Key"),
                                            EasyServer.GetConfValue<string>("World", "CharacterServer", "Ip"),
                                            EasyServer.GetConfValue<int>("World", "CharacterServer", "Port")))
                return;

            // Initialisation du TcpManager pour les Clients
            if (!EasyServer.Listen<TcpServer>(EasyServer.GetConfValue<int>("World", "ClientServer", "Port"), "ClientServer"))
                return;

            CharMgr = new CharacterMgr();
            FileMgr = new FileManager();

            WorldName = EasyServer.GetConfValue<string>("World", "ClientServer", "Name");
            WorldId = EasyServer.GetConfValue<int>("World", "ClientServer", "Id");

            CharMgr.RegisterWorld(EasyServer.GetRpcClientId("WorldChar"),
                                    WorldId,
                                    WorldName,
                                    EasyServer.GetConfValue<int>("World", "ClientServer", "Port"),
                                    EasyServer.GetConfValue<string>("World", "ClientServer", "Ip"));


            EasyServer.StartConsole();
        }
    }
}

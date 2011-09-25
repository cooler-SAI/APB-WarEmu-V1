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

namespace CharacterServer
{
    class Program
    {
        [STAThread]
        static void Main(string[] args)
        {
            Assembly.Load("Common");

            Log.Info("CharacterServer", "Démarrage...");

            // Initialisation des Config de log et Générales
            if (!EasyServer.InitLog("Char", "Configs/CharLog.conf")
                || !EasyServer.InitConfig("Configs/Char.xml", "Char"))
                return;

            // Initialisation du Client Rpc pour le FileServer
            if (!EasyServer.InitRpcServer("Character",
                                            EasyServer.GetConfValue<string>("Char", "CharacterServer", "Key"),
                                            EasyServer.GetConfValue<int>("Char", "CharacterServer", "Port")))
                return;

            // Initialisation de la Base de donnée
            if (!EasyServer.InitMysqlDB("Configs/CharDB.xml", "CharDB"))
                return;

            CharacterMgr.Database = EasyServer.GetDatabase("CharDB");

            Log.Succes("CharacterServer", "Initialisation du serveur terminée.");
            EasyServer.StartConsole();
        }
    }
}

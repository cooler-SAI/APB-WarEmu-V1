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

namespace FileServer
{
    class Program
    {
        [STAThread]
        static void Main(string[] args)
        {
            Assembly.Load("Common");

            Log.Info("FileServer", "Démarrage...");

            // Initialisation des Config de log et Générales
            if (!EasyServer.InitLog("File", "Configs/FileLog.conf")
                || !EasyServer.InitConfig("Configs/File.xml", "File"))
                return;

            // Initialisation du Client Rpc pour le FileServer
            if (!EasyServer.InitRpcServer("FileServer",
                                            EasyServer.GetConfValue<string>("File", "FileServer", "Key"),
                                            EasyServer.GetConfValue<int>("File", "FileServer", "Port")))
                return;

            Log.Succes("FileServer", "Initialisation du serveur terminée.");
            EasyServer.StartConsole();
        }
    }
}

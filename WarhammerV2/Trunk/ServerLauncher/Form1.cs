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
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Diagnostics;
using System.Threading;

namespace ServerLauncher
{
    public partial class Form1 : Form
    {
        public Process AccountCacher;
        public Process LauncherServer;
        public Process LobbyServer;
        public Process WorldServer;

        public Form1()
        {
            InitializeComponent();
        }

        private void B_start_Click(object sender, EventArgs e)
        {
            AccountCacher = new Process();
            AccountCacher.StartInfo.FileName = "AccountCacher.exe";
            AccountCacher.StartInfo.WindowStyle = ProcessWindowStyle.Minimized;
            AccountCacher.Start();

            Thread.Sleep(500);

            LauncherServer = new Process();
            LauncherServer.StartInfo.FileName = "LauncherServer.exe";
            LauncherServer.StartInfo.WindowStyle = ProcessWindowStyle.Minimized;
            LauncherServer.Start();

            Thread.Sleep(500);

            LobbyServer = new Process();
            LobbyServer.StartInfo.FileName = "LobbyServer.exe";
            LobbyServer.StartInfo.WindowStyle = ProcessWindowStyle.Minimized;
            LobbyServer.Start();

            Thread.Sleep(500);

            WorldServer = new Process();
            WorldServer.StartInfo.FileName = "WorldServer.exe";
            WorldServer.StartInfo.WindowStyle = ProcessWindowStyle.Minimized;
            WorldServer.Start();
        }

        private void B_stop_Click(object sender, EventArgs e)
        {
            try
            {
                WorldServer.Kill();
            }
            catch (Exception)
            {

            }
            try
            {
                LobbyServer.Kill();
            }
            catch (Exception)
            {

            }
            try
            {
                LauncherServer.Kill();
            }
            catch (Exception)
            {

            }
            try
            {
                AccountCacher.Kill();
            }
            catch (Exception)
            {

            }
        }
    }
}

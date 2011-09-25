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
using System.Security.Cryptography;

namespace Launcher
{
    public partial class Accueil : Form
    {
        static public Accueil Acc;
        public Accueil()
        {
            InitializeComponent();
            Acc = this;
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void Disconnect(object sender, FormClosedEventArgs e)
        {
            Client.Close();
        }

        private void B_start_Click(object sender, EventArgs e)
        {
            string Username = T_username.Text.ToLower();
            string NCPass = T_password.Text.ToLower();

            Client.User = Username;

            SHA256Managed Sha = new SHA256Managed();
            string CP = Username + ":" + NCPass;
            byte[] Result = Sha.ComputeHash(UTF8Encoding.UTF8.GetBytes(CP));

            PacketOut Out = new PacketOut((byte)Opcodes.CL_START);
            Out.WriteString(Username);
            Out.WriteUInt32((uint)Result.Length);
            Out.Write(Result, 0, Result.Length);

            Client.SendTCP(Out);
            B_start.Enabled = false;
        }

        public void ReceiveStart()
        {
            B_start.Enabled = true;
        }

        public void Print(string Message)
        {
            T_Log.Text += Message + "\n" + "\r";
        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            Client.Language = L_selection.Text;
            Print("Selection de : " + Client.Language);
            Client.UpdateLanguage();
        }

        private void label3_Click(object sender, EventArgs e)
        {

        }

        private void b_realms_Click(object sender, EventArgs e)
        {
            Client.UpdateRealms();
        }

        public void ClearRealms()
        {
            Realms.Rows.Clear();
        }

        public void AddRealm(string Name, bool Online, uint Players, uint Destruction, uint Order)
        {
            Realms.Rows.Add(Name, Online ? "true" : "false", "" + Players, "" + Destruction, "" + Order);
        }
    }
}

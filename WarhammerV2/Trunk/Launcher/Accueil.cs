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
            Print("Cp=" + CP);
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
    }
}

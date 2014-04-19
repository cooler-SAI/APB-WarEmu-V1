using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Net;
using System.Windows.Forms;
using MYPHandler;
using nsHashDictionary;
using System.IO;
using System.Configuration;
using System.Diagnostics;
using System.Resources;
using System.Security.Cryptography;

using Launcher;

namespace WarEmu
{
    public partial class Form1 : Form
    {
        static public Form1 Instance;
        public string path = "";
        public string username = "";
        public string password = "";

        public Form1()
        {
            Instance = this;
            InitializeComponent();

            this.SetStyle(System.Windows.Forms.ControlStyles.SupportsTransparentBackColor, true);
            this.TransparencyKey = Color.DarkSlateGray;
            this.BackColor = Color.DarkSlateGray;
           // readConfig();
        }

        protected override void WndProc(ref Message m)
        {
            base.WndProc(ref m);

            int WM_NCHITTEST = 0x84;
            if (m.Msg == WM_NCHITTEST)
            {
                int HTCLIENT = 1;
                int HTCAPTION = 2;
                if (m.Result.ToInt32() == HTCLIENT)
                    m.Result = (IntPtr)HTCAPTION;
            }
        }

        private void patch()
        {
            try
            {
                using (Stream stream = new FileStream(path + @"\WAR.exe", FileMode.OpenOrCreate))
                {
                    int encryptAddress = (0x00957FBE + 3) - 0x00400000;
                    stream.Seek(encryptAddress, SeekOrigin.Begin);
                    stream.WriteByte(0x01);



                    byte[] decryptPatch1 = { 0x90, 0x90, 0x90, 0x90, 0x57, 0x8B, 0xF8, 0xEB, 0x32 };
                    int decryptAddress1 = (0x009580CB) - 0x00400000;
                    stream.Seek(decryptAddress1, SeekOrigin.Begin);
                    stream.Write(decryptPatch1, 0, 9);

                    byte[] decryptPatch2 = { 0x90, 0x90, 0x90, 0x90, 0xEB, 0x08 };
                    int decryptAddress2 = (0x0095814B) - 0x00400000;
                    stream.Seek(decryptAddress2, SeekOrigin.Begin);
                    stream.Write(decryptPatch2, 0, 6);

                    //stream.WriteByte(0x01);
                }
            }
            catch
            {

            }
        }
        public void updateMYP(FileStream fs)
        {
            try
            {
                //string OldDirectory = Directory.GetCurrentDirectory();
                //FileStream fs = new FileStream(Application.StartupPath + "\\PortalSettings.xml", FileMode.Open, FileAccess.Read);
                Directory.SetCurrentDirectory(path);

                HashDictionary hashDictionary = new HashDictionary();
                hashDictionary.AddHash(0x3FE03665, 0x349E2A8C, "F4FCD464_3FE03665349E2A8C.xml", 0);


                MYPHandler.MYPHandler mypHandler = new MYPHandler.MYPHandler("data.myp", null, null, hashDictionary);
                mypHandler.GetFileTable();


                FileInArchive theFile = mypHandler.SearchForFile("F4FCD464_3FE03665349E2A8C.xml");
                if (theFile != null)
                    mypHandler.ReplaceFile(theFile, fs);
            }
            catch
            {

            }

            //Directory.SetCurrentDirectory(OldDirectory);
        }

        public void updateWarhammerName(FileStream fs)
        {
            string OldDirectory = Directory.GetCurrentDirectory();

            //FileStream fs = new FileStream(Application.StartupPath + "\\PortalSettings.xml", FileMode.Open, FileAccess.Read);
            Directory.SetCurrentDirectory(path);

            HashDictionary hashDictionary = new HashDictionary();
            hashDictionary.AddHash(0x3FE03665, 0x349E2A8C, "D94BB637_5181D982E63A3766.dds", 0);


            MYPHandler.MYPHandler mypHandler = new MYPHandler.MYPHandler("data.myp", null, null, hashDictionary);
            mypHandler.GetFileTable();

            FileInArchive theFile = mypHandler.SearchForFile("D94BB637_5181D982E63A3766.dds");
            if (theFile != null)
                mypHandler.ReplaceFile(theFile, fs);

            Directory.SetCurrentDirectory(OldDirectory);
        }


        private void unpatch()
        {
            using (Stream stream = new FileStream(path + @"\WAR.exe", FileMode.OpenOrCreate))
            {
                int encryptAddress = (0x00957FBE + 3) - 0x00400000;
                stream.Seek(encryptAddress, SeekOrigin.Begin);
                stream.WriteByte(0x00);


                int decryptAddress = (0x009580CB + 3) - 0x00400000;
                stream.Seek(decryptAddress, SeekOrigin.Begin);
                stream.WriteByte(0x00);
            }
        }
        private void button3_Click(object sender, EventArgs e)
        {
           
        }

        private void button1_Click(object sender, EventArgs e)
        {
            string startupPath = Application.StartupPath;
            using (FolderBrowserDialog dialog = new FolderBrowserDialog())
            {
                dialog.Description = "Open the folder for Warhammer";
                dialog.ShowNewFolderButton = false;
                dialog.RootFolder = Environment.SpecialFolder.MyComputer;
                if (dialog.ShowDialog() == DialogResult.OK)
                {
                    path = dialog.SelectedPath;
                    textBox3.Text = path;
                    UpdateSetting("path", path);
                }
            }
        }

        static public string ConvertSHA256(string value)
        {
            SHA256 sha = SHA256.Create();
            byte[] data = sha.ComputeHash(Encoding.Default.GetBytes(value));
            StringBuilder sb = new StringBuilder();
            for (int i = 0; i < data.Length; i++)
            {
                sb.Append(data[i].ToString("x2"));
            }
            return sb.ToString();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            username = textBox1.Text;
            password = textBox2.Text;

            if(username == null || username.Length <= 0)
                MessageBox.Show("Invalid Username");
            else if(password == null || password.Length <=0)
                MessageBox.Show("Invalid Password");
            else if (path != "" && username != "" && password != "")
            {
                string Port = ConfigurationManager.AppSettings["port"];
                if (Port != null && Port.Length > 0)
                    Client.Port = int.Parse(Port);

                Client.Language = "En";
                Client.User = username;
                Client.Auth = ConvertSHA256(username + ":" + password);
                Client.Connect();
            }
            else
                MessageBox.Show("Invalid Warhammer folder.");
        }

        public void StartClient(string Token)
        {
            if(path != "")
            {
                patch();

                Process Pro = new Process();
                Pro.StartInfo.FileName = path + @"\WAR.exe";
                if (username != "" && password != "")
                {
                    Pro.StartInfo.Arguments = " --acctname=" + System.Convert.ToBase64String(ASCIIEncoding.ASCII.GetBytes(Client.User)) + " --sesstoken=" + Token; //System.Convert.ToBase64String(ASCIIEncoding.ASCII.GetBytes(password)); //REIxREUwMEEwQjFEMzlFMkNFMzdBODEwMDBFRjY3REM=";
                    Pro.Start();
                }
                else
                    MessageBox.Show("Error: Invalid Username or Password");

                Application.Exit();
            }
            else
            {
                MessageBox.Show("Error: path is missing");
            }
        }

        private static void UpdateSetting(string key, string value)
        {
            try
            {
                Configuration configuration = ConfigurationManager.OpenExeConfiguration(ConfigurationUserLevel.None);
                configuration.AppSettings.Settings[key].Value = value;
                configuration.Save();

                ConfigurationManager.RefreshSection("appSettings");
            }
            catch
            {

            }
        }



        private void Form1_Load(object sender, EventArgs e)
        {
            path = ConfigurationManager.AppSettings["path"];
            if (path == null || path.Length <= 0)
                path = Directory.GetCurrentDirectory();

            textBox3.Text = path;

            username = ConfigurationManager.AppSettings["username"];
            textBox1.Text = username;

            password = ConfigurationManager.AppSettings["password"];
            textBox2.Text = password;

            Client.IP = ConfigurationManager.AppSettings["ip"];
        }

        private void textBox3_TextChanged(object sender, EventArgs e)
        {
            path = textBox3.Text;
            UpdateSetting("path", path);
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {
            UpdateSetting("username", textBox1.Text);
        }

        private void textBox2_TextChanged(object sender, EventArgs e)
        {
            UpdateSetting("password", textBox2.Text);
        }

        private void QuitButton_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void label2_Click(object sender, EventArgs e)
        {

        }

        private void ArmoryButton_MouseEnter(object sender, EventArgs e)
        {
            this.ArmoryButton.BackgroundImage = global::Patcher.Properties.Resources.armory_hover;
        }

        private void ArmoryButton_MouseLeave(object sender, EventArgs e)
        {
            this.ArmoryButton.BackgroundImage = global::Patcher.Properties.Resources.armory;
        }

        private void MyAccountButton_MouseEnter(object sender, EventArgs e)
        {
            this.ForumButton.BackgroundImage = global::Patcher.Properties.Resources.forum_hover;
        }

        private void MyAccountButton_MouseLeave(object sender, EventArgs e)
        {
            this.ForumButton.BackgroundImage = global::Patcher.Properties.Resources.forum;
        }

        private void QuitButton_MouseEnter(object sender, EventArgs e)
        {
            this.QuitButton.BackgroundImage = global::Patcher.Properties.Resources.quit_hover;
        }

        private void QuitButton_MouseLeave(object sender, EventArgs e)
        {
            this.QuitButton.BackgroundImage = global::Patcher.Properties.Resources.quit;
        }

        private void button1_MouseEnter(object sender, EventArgs e)
        {
            this.button1.BackgroundImage = global::Patcher.Properties.Resources.browse_hover;
        }

        private void button1_MouseLeave(object sender, EventArgs e)
        {
            this.button1.BackgroundImage = global::Patcher.Properties.Resources.browse;
        }

        private void ConnectButton_MouseEnter(object sender, EventArgs e)
        {
            this.ConnectButton.BackgroundImage = global::Patcher.Properties.Resources.play_now_hover;
        }

        private void ConnectButton_MouseLeave(object sender, EventArgs e)
        {
            this.ConnectButton.BackgroundImage = global::Patcher.Properties.Resources.play_now;
        }

        private void MyAccountButton_Click(object sender, EventArgs e)
        {
            ProcessStartInfo sInfo = new ProcessStartInfo("http://persotom.no-ip.biz/warhammer/forum/");
            Process.Start(sInfo);
        }

        private void ArmoryButton_Click(object sender, EventArgs e)
        {
            ProcessStartInfo sInfo = new ProcessStartInfo("http://www.warhammer-server.com/armory/");
            Process.Start(sInfo);
        }

        private void ChangelogButton_Click(object sender, EventArgs e)
        {
            ProcessStartInfo sInfo = new ProcessStartInfo("http://www.warhammer-server.com/changelog/");
            Process.Start(sInfo);
        }

        private void BugTrackerButton_Click(object sender, EventArgs e)
        {
            ProcessStartInfo sInfo = new ProcessStartInfo("http://www.warhammer-server.com/bug-tracker/");
            Process.Start(sInfo);
        }

        private void BugTrackerButton_MouseEnter(object sender, EventArgs e)
        {
            this.BugTrackerButton.BackgroundImage = global::Patcher.Properties.Resources.bugtracker_hover;
        }

        private void BugTrackerButton_MouseLeave(object sender, EventArgs e)
        {
            this.BugTrackerButton.BackgroundImage = global::Patcher.Properties.Resources.bugtracker;
        }

        private void ChangelogButton_MouseEnter(object sender, EventArgs e)
        {
            this.ChangelogButton.BackgroundImage = global::Patcher.Properties.Resources.changelog_hover;
        }

        private void ChangelogButton_MouseLeave(object sender, EventArgs e)
        {
            this.ChangelogButton.BackgroundImage = global::Patcher.Properties.Resources.changelog;
        }
    }
}

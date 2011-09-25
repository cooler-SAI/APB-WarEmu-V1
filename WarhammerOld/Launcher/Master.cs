using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Windows.Forms;
using System.Reflection;
using System.Security.Cryptography;
using System.Net;
using System.Net.Sockets;
using System.Threading;

using nsHashDictionary;
using MYPHandler;

using FrameWork;
using FrameWork.Logger;
using FrameWork.Config;

namespace LauncherV2
{
    public class Master : Form
    {
        enum AuthOpcodes
        {
            CMSG_CONNECT = 0x01,
            SMSG_CONNECT_RESPONSE = 0x02,
            CMSG_USER_LOGIN = 0x03,
            SMSG_USER_LOGIN_RESPONSE = 0x04,
            CMSG_AUTHENTICATE = 0x05,
            SMSG_AUTHENTICATE_RESPONSE = 0x06,
            CMSG_GET_SERVER_LIST = 0x07,
            SMSG_GET_SERVER_LIST_RESPONSE = 0x08,
            CMSG_GET_CHARACTER_SUMMARIES = 0x09,
            SMSG_GET_CHARACTER_SUMMARIES_RESPONSE = 0x0a,

            CMSG_REFRESH_AUTHENTICATION = 0x0d,
            SMSG_REFRESH_AUTHENTICATION_RESPONSE = 0x0e,
            CMSG_LOGIN_CLEARANCE = 0x0f, //supposed, never seen
            SMSG_LOGIN_CLEARANCE_RESPONSE = 0x10, //supposed, never seen
            CMSG_GET_PRODUCT_ACCOUNT_PROPERTY_LIST = 0x11,
            SMSG_GET_PRODUCT_ACCOUNT_PROPERTY_LIST_RESPONSE = 0x12,
            MAX_OPCODE = 0x13,
        };

        const UInt16 UNKNOWN_SIZE = 0xffff;
        UInt16[] PacketSize = new UInt16[] 
        {
            0x00, ///< no packet
            0x08, ///< CMSG_CONNECT
            0x0a, ///< SMSG_CONNECT_RESPONSE
            UNKNOWN_SIZE, ///< CMSG_USER_LOGIN
            0x22, ///< SMSG_USER_LOGIN_RESPONSE
            UNKNOWN_SIZE, ///< CMSG_AUTHENTICATE
            0x07, ///< SMSG_AUTHENTICATE_RESPONSE
            0x04, ///< CMSG_GET_SERVER_LIST
            UNKNOWN_SIZE, ///< SMSG_GET_SERVER_LIST_RESPONSE
            0x48, ///< CMSG_TOKEN_AUTHENTICATE
            0x07, ///< SMSG_TOKEN_AUTHENTICATE_RESPONSE
            0x04, ///< CMSG_GET_CHARACTER_SUMMARIES
            UNKNOWN_SIZE, ///< SMSG_GET_CHARACTER_SUMMARIES_RESPONSE
            UNKNOWN_SIZE, ///< CMSG_REFRESH_AUTHENTICATION \\\ /todo check this packet
            UNKNOWN_SIZE, ///< SMSG_REFRESH_AUTHENTICATION_RESPONSE ///\todo check this packet
            UNKNOWN_SIZE, ///< CMSG_LOGIN_CLEARANCE ///\todo check this packet
            UNKNOWN_SIZE, ///< SMSG_LOGIN_CLEARANCE_RESPONSE ///\todo check this packet
            0x04, ///< CMSG_GET_PRODUCT_ACCOUNT_PROPERTY_LIST
            UNKNOWN_SIZE ///< SMSG_GET_PRODUCT_ACCOUNT_PROPERTY_LIST_RESPONSE
        };

        public List<Control> _Controls = new List<Control>();
        public Master()
        {
            Log.Info("Master", "Creating windows");

            LoadConfig();

            this.SuspendLayout();
            foreach (Control Con in _Controls)
                this.Controls.Add(Con);

            this.ResumeLayout(false);
            this.PerformLayout();
        }

        public XMLConfigFile Config = null;
        public void LoadConfig()
        {
            if (!EasyServer.InitConfig("Configs/Conf.xml", "Config"))
                return;

            Config = EasyServer.GetConf("Config");

            foreach (KeyValuePair<string, ConfigElement> Elements in Config.Children)
            {
                ConfigElement Element = Elements.Value;

                Log.Info("Control", "New control : " + Elements.Key);

                if (Elements.Key == "Master")
                {
                    string Name = Element["Name"].GetString("Launcher");
                    int W = Element["Width"].GetInt(100);
                    int H = Element["Height"].GetInt(100);
                    int Resize = Element["Resize"].GetInt(100);

                    Log.Info("Scale", "X=" + W + ",Y=" + H);

                    this.Name = Name;
                    this.Text = Name;

                    this.AutoScaleDimensions = new SizeF(W, H);
                    this.AutoScaleMode = AutoScaleMode.Font;
                    this.FormBorderStyle = Resize <= 0 ? FormBorderStyle.FixedToolWindow : FormBorderStyle.Sizable;
                    this.ClientSize = new Size(Width, Height);
                    this.Size = new System.Drawing.Size(W, H);
                }
                else
                {
                    string Type = Element["Type"].GetString("label");
                    string Name = Element["Name"].GetString("Unknown");
                    string Text = Element["Text"].GetString("");
                    int W = Element["Width"].GetInt(100);
                    int H = Element["Height"].GetInt(20);
                    int X = Element["X"].GetInt(0);
                    int Y = Element["Y"].GetInt(0);
                    int Transparency = Element["Tranparency"].GetInt(0);
                    string Tag = Element["Tag"].GetString("");
                    string[] BackColor = Element["BackColor"].GetString("1,1,1").Split(',');
                    string[] ForeColor = Element["ForeColor"].GetString("255,255,255").Split(',');

                    Log.Info("Tag", "Tag=" + Tag);

                    Control Con;
                    switch (Type)
                    {
                        case "textArea":
                            int Masked = Element["Masked"].GetInt(0);

                            if (Masked == 0)
                                Con = new TextBox();
                            else
                                Con = new MaskedTextBox();

                            if (Transparency > 0)
                                (Con as TextBoxBase).BorderStyle = BorderStyle.None;

                            break;

                        case "Label":
                            Con = new Label();
                            break;

                        case "button":
                            Con = new Button();
                            Con.Click += new EventHandler(this.OnClick);
                            break;

                        case "picture":

                            Con = new PictureBox();
                            (Con as PictureBox).ImageLocation = Element["Image"].GetString();
                            (Con as PictureBox).InitialImage = null;
                            break;

                        default:
                            Con = new Label();
                            break;
                    }

                    Con.Name = Name;
                    Con.Text = Text;
                    Con.Location = new System.Drawing.Point(X, Y);
                    Con.Size = new System.Drawing.Size(W, H);

                    if (Con is Label)
                        (Con as Label).AutoSize = true;

                    Con.Tag = Tag;

                    if (BackColor.Length >= 3)
                        Con.BackColor = Color.FromArgb(int.Parse(BackColor[0]), int.Parse(BackColor[1]), int.Parse(BackColor[2]));

                    if (ForeColor.Length >= 3)
                        Con.ForeColor = Color.FromArgb(int.Parse(ForeColor[0]), int.Parse(ForeColor[1]), int.Parse(ForeColor[2]));

                    _Controls.Add(Con);
                }
            }
        }

        public void OnClick(object sender, EventArgs e)
        {
            Control Con = (sender as Button);
            MethodInfo Info = GetType().GetMethod(Con.Tag as string);
            Log.Info("Function", "Name=" + (string)Con.Tag);

            if (Info == null)
            {
                Log.Error("Function", "Function not find : " + (string)Con.Tag);
                return;
            }

            Config = EasyServer.GetConf("Config");

            List<string> Params = new List<string>();
            ConfigElement Button = null; ;
            foreach (ConfigElement Element in Config.Children.Values)
            {
                if (Element["Tag"].GetString() == Con.Tag as string)
                {
                    Button = Element;
                    break;
                }
            }

            foreach (ConfigElement Element in Button.Children["OnClick"].Children.Values)
            {
                string TagToFind = Element.GetString();
                string TagValue = GetTag(TagToFind);

                Log.Info("OnClick", TagToFind + " Add value : " + TagValue);

                if (TagValue.Length > 0)
                    Params.Add(TagValue);
            }


            if (Info.GetParameters().Length != Params.Count)
            {
                Log.Error("OnClick", "Function :" + Con.Tag + ", Invalid Params Count : Function=" + Info.GetParameters().Length + ",Xml=" + Params.Count);
                return;
            }

            if (Info != null)
                Info.Invoke(this, Params.ToArray());
        }

        public string GetTag(string Tag)
        {
            Control Con = _Controls.Find(info => (string)info.Tag == Tag);
            if (Con != null)
                return Con.Text;
            else
                return "";
        }

        public void ConnectToServer(string Ip, string Port, string Account, string Password, string WarhammerDirectory)
        {
            Log.Info("ConnectToServer", "Attempt : " + Ip + ":" + Port);

            ///- Check the values we have
            if (WarhammerDirectory == "")
            {
                MessageBox.Show("Please choose the directory where WAR is installed", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            IPAddress ipAddress;
            IPAddress[] Dnsresolve;
            try
            {
                if (IPAddress.TryParse(Ip, out ipAddress))
                {
                    ipAddress = IPAddress.Parse(Ip);
                }
                else
                {
                    Dnsresolve = Dns.GetHostAddresses(Ip);
                    ipAddress = Dnsresolve[0];
                }
            }
            catch (Exception eIPAddress)
            {
                MessageBox.Show("Invalid Server IP address. Please review your settings file\n" + eIPAddress.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            ///- Connect to the Login Server 
            TcpClient tcpClient;
            try
            {
                tcpClient = TimeOutSocket.Connect(new IPEndPoint(ipAddress, int.Parse(Port)), 4000); //Stop trying to connect after 7 secs
            }
            catch (Exception eConnect)
            {
                MessageBox.Show("Cannot connect to Login Server.\n" + eConnect.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            tcpClient.ReceiveTimeout = 5000; //Forget about reading after 5 seconds

            ///- Get the keybits
            MemoryStream toSend = new MemoryStream();
            try
            {
            }
            catch (Exception eLogin)
            {
                tcpClient.Close();
                MessageBox.Show("Cannot connect to Login Server.\n" + eLogin.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            string ToEncode = Account + ":" + Password;
            SHA256Managed Man = new SHA256Managed();
            byte[] Result = Man.ComputeHash(ASCIIEncoding.ASCII.GetBytes(ToEncode));
            string AuthToken = BitConverter.ToString(Result).Replace("-", string.Empty);

            toSend.SetLength(0);
            toSend.Position = 0;

            try
            {
                NetworkStream ns = tcpClient.GetStream();

                toSend.WriteByte((byte)(4 + (4 + Account.Length) + (4 + AuthToken.Length)));
                toSend.WriteByte((byte)AuthOpcodes.CMSG_AUTHENTICATE);

                toSend.Write(new byte[] { 0, 0, 0, 1 }, 0, 4); // Sequence


                byte[] AccountSize = BitConverter.GetBytes((uint)Account.Length);
                Array.Reverse(AccountSize);
                toSend.Write(AccountSize, 0, 4); // size of username
                toSend.Write(ASCIIEncoding.Default.GetBytes(Account), 0, Account.Length); // Unknown

                byte[] authResultSize = BitConverter.GetBytes((uint)AuthToken.Length);
                Array.Reverse(authResultSize);
                toSend.Write(authResultSize, 0, 4); // size of username
                toSend.Write(ASCIIEncoding.Default.GetBytes(AuthToken), 0, AuthToken.Length); // Unknown

                toSend.WriteTo(ns);

                byte[] readBytes = new byte[(byte)PacketSize[(int)AuthOpcodes.SMSG_AUTHENTICATE_RESPONSE] + 2];
                int left = readBytes.Length;
                Log.Info("1", "13");
                while (left != 0) //that's brutal
                    left -= ns.Read(readBytes, readBytes.Length - left, left);
                Log.Info("1", "14");
                UInt16 response = BitConverter.ToUInt16(readBytes, 7);

                if (response == 14)
                {
                    tcpClient.Close();
                    MessageBox.Show("Account Suspended", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                }

                if (response != 0)
                {
                    tcpClient.Close();
                    MessageBox.Show("Incorrect username or password", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                }
            }
            catch (Exception eLogin)
            {
                tcpClient.Close();
                MessageBox.Show("Cannot connect to Login Server.\n" + eLogin.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            /// \todo here we could send the last packet to check if the password is correct before launching the game
            tcpClient.Close();
            Log.Info("1", "7");
            System.Diagnostics.ProcessStartInfo si = new System.Diagnostics.ProcessStartInfo();
            si.FileName = WarhammerDirectory + "\\war.exe";
            Log.Info("1", "8");
            si.WorkingDirectory = WarhammerDirectory;

            si.Arguments = "--acctname=" + EncodeTo64(Account) + " --authtoken=" + AuthToken;

            try
            {
                if (!UpdateWar(WarhammerDirectory))
                    return;
            }
            catch (Exception)
            {

            }

            try
            {
                System.Diagnostics.Process.Start(si);
            }
            catch (Exception eLaunch)
            {
                MessageBox.Show("Cannot launch the game client.\n" + eLaunch.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        static public string EncodeTo64(string toEncode)
        {

            byte[] toEncodeAsBytes

                  = System.Text.ASCIIEncoding.ASCII.GetBytes(toEncode);

            string returnValue

                  = System.Convert.ToBase64String(toEncodeAsBytes);

            return returnValue;

        }

        private bool UpdateWar(string WarhammerDirectory)
        {
            FileStream fs = new FileStream(Application.StartupPath + "\\mythloginserviceconfig.xml", FileMode.Open, FileAccess.Read);

            Directory.SetCurrentDirectory(WarhammerDirectory);

            HashDictionary hashDictionary = new HashDictionary();
            hashDictionary.AddHash(0x3FE03665, 0x349E2A8C, "mythloginserviceconfig.xml", 0);
            MYPHandler.MYPHandler mypHandler = new MYPHandler.MYPHandler("data.myp", null, null, hashDictionary);
            mypHandler.GetFileTable();

            FileInArchive theFile = mypHandler.SearchForFile("mythloginserviceconfig.xml");

            if (theFile == null)
            {
                MessageBox.Show("Can not find config file in data.myp");
                return false;
            }

            if (File.Exists(Application.StartupPath + "\\mythloginserviceconfig.xml") == false)
            {
                MessageBox.Show("Missing file : mythloginserviceconfig.xml");
                return false;
            }

            mypHandler.ReplaceFile(theFile, fs);

            fs.Close();

            return true;
        }
    }
}
class TimeOutSocket
{
        private static bool IsConnectionSuccessful = false;
        private static Exception socketexception;
        private static ManualResetEvent TimeoutObject = new ManualResetEvent(false);

        public static TcpClient Connect(IPEndPoint remoteEndPoint, int timeoutMSec)
        {
            TimeoutObject.Reset();
            socketexception = null;

            string serverip = Convert.ToString(remoteEndPoint.Address);
            int serverport = remoteEndPoint.Port;
            TcpClient tcpclient = new TcpClient();

            tcpclient.BeginConnect(serverip, serverport,
                new AsyncCallback(CallBackMethod), tcpclient);

            if (TimeoutObject.WaitOne(timeoutMSec, false))
            {
                if (IsConnectionSuccessful)
                {
                    return tcpclient;
                }
                else
                {
                    throw socketexception;
                }
            }
            else
            {
                tcpclient.Close();
                throw new TimeoutException("TimeOut Exception");
            }
        }
        private static void CallBackMethod(IAsyncResult asyncresult)
        {
            try
            {
                IsConnectionSuccessful = false;
                TcpClient tcpclient = asyncresult.AsyncState as TcpClient;

                if (tcpclient.Client != null)
                {
                    tcpclient.EndConnect(asyncresult);
                    IsConnectionSuccessful = true;
                }
            }
            catch (Exception ex)
            {
                IsConnectionSuccessful = false;
                socketexception = ex;
            }
            finally
            {
                TimeoutObject.Set();
            }
        }
    }
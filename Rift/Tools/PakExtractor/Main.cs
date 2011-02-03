using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace PakExtractor
{
    public partial class Extractor : Form
    {
        static public string FilesFolder = "";
        static public Extractor Instance = null;
        static public DataGridView Data = null;
        static public ComboBox Box = null;
        static public Timer Time = new Timer();

        public Extractor()
        {
            InitializeComponent();

            Instance = this;
            Data = datagrid;
            Box = l_files;

            Time.Enabled = true;
            Time.Interval = 20;
            Time.Tick += new EventHandler(Update);
            Time.Start();

            l_files.SelectedValueChanged+=new EventHandler(l_files_SelectedValueChanged);

            ExtractorMgr.LoadHeaders();
            ExtractorMgr.StartExtractorThread();

            this.FormClosing += new FormClosingEventHandler(OnExit);
        }

        private void b_listing_Click(object sender, EventArgs e)
        {
            if (FilesFolder.Length <= 0)
            {
                FolderBrowserDialog Dial = new FolderBrowserDialog();
                Dial.ShowDialog();
                FilesFolder = Dial.SelectedPath;
            }

            tip_status.Text = "Selected Folder : " + FilesFolder;

            String[] fichiers = Directory.GetFiles(FilesFolder, "*.pak", SearchOption.AllDirectories);
            foreach (string FileLink in fichiers)
            {
                FileInfo Info = new FileInfo(FileLink);
                l_files.Items.Add(Info);
            }

            tip_status.Text = fichiers.Length + " Fichiers";
        }

        private void l_files_SelectedValueChanged(object sender, EventArgs e)
        {
            if (l_files.SelectedItem == null)
                return;

            ExtractorMgr.DecodePak((l_files.SelectedItem as FileInfo),true);
            ExtractorMgr.PrintPak((l_files.SelectedItem as FileInfo));
        }

        public void Tool(string Text)
        {
            tip_status.Text = Text;
        }

        public int ProgressValue;
        public void Progress(int value)
        {
            ProgressValue = value;
        }

        public void ClearData()
        {
            datagrid.Rows.Clear();
        }
        public void AddData(params object[] Params)
        {
            datagrid.Rows.Add(Params);
        }

        public void Update(object sender,EventArgs a)
        {
            tool_progress.Value = ProgressValue;
        }

        public void OnExit(object sender, FormClosingEventArgs a)
        {
            Time.Stop();

            ExtractorMgr.IsRunning = false;
        }

        private void b_extract_Click(object sender, EventArgs e)
        {
            ExtractorMgr.ExtractPack();
        }

        private void b_allpak_Click(object sender, EventArgs e)
        {
            try
            {
                for (int i = 1; i < l_files.Items.Count; ++i)
                {
                    if (!ExtractorMgr.IsRunning)
                        break;

                    FileInfo Info = l_files.Items[i] as FileInfo;

                    PakFile Pak = ExtractorMgr.DecodePak(Info, true);

                    foreach (PakElement Element in Pak._Elements)
                        ExtractorMgr.AddToExtract(Element);
                }
            }
            catch (Exception ex)
            {
                Tool(ex.ToString());
            }
        }
    }
}

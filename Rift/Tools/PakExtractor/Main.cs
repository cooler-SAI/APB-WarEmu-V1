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
        static public PakFile Decoding = null;
        static public PakFile Extracting = null;

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

            b_allpak.Enabled = false;
            b_extract.Enabled = false;
            l_files.Enabled = false;

            this.FormClosing += new FormClosingEventHandler(OnExit);
        }

        private void b_listing_Click(object sender, EventArgs e)
        {
            if (FilesFolder.Length <= 0)
            {
                FolderBrowserDialog Dial = new FolderBrowserDialog();
                Dial.ShowDialog();
                FilesFolder = Dial.SelectedPath;
                if (FilesFolder.Length <= 0)
                    return;
            }

            b_allpak.Enabled = true;
            b_extract.Enabled = true;
            l_files.Enabled = true;

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

            l_files.Enabled = false;
            ClearData();
            Decoding = ExtractorMgr.DecodePak((l_files.SelectedItem as FileInfo));
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

            if (datagrid.Rows.Count <= 0 && Decoding != null && Decoding.Decoded)
            {
                Decoding.PrintHeaders();
                l_files.Enabled = true;
            }

            if (Extracting != null && Extracting.Decoded)
            {
                foreach (PakElement Elem in Extracting._Elements)
                    ExtractorMgr.AddToExtract(Elem);

                Extracting = null;
            }

            if (_ToExtract.Count > 0 && Extracting == null)
            {
                Extracting = ExtractorMgr.DecodePak(_ToExtract.First());
            }
        }

        public void OnExit(object sender, FormClosingEventArgs a)
        {
            lock (_ToExtract)
                _ToExtract.Clear();

            Extracting = null;
            Decoding = null;

            Time.Stop();

            ExtractorMgr.IsRunning = false;
        }

        private void b_extract_Click(object sender, EventArgs e)
        {
            ExtractorMgr.ExtractPack();
        }

        public List<FileInfo> _ToExtract = new List<FileInfo>();
        private void b_allpak_Click(object sender, EventArgs e)
        {
            if (ExtractorMgr.ExtractingFolder.Length <= 0)
            {
                FolderBrowserDialog Dial = new FolderBrowserDialog();
                Dial.SelectedPath = Directory.GetCurrentDirectory();
                Dial.ShowDialog();
                ExtractorMgr.ExtractingFolder = Dial.SelectedPath;

                if (ExtractorMgr.ExtractingFolder.Length <= 0)
                    return;
            }

            try
            {
                for (int i = 0; i < l_files.Items.Count; ++i)
                {
                    Decoding = null;

                    if (!ExtractorMgr.IsRunning)
                        break;

                    lock (_ToExtract)
                        _ToExtract.Clear();

                    FileInfo Info = l_files.Items[i] as FileInfo;

                    lock (_ToExtract)
                        _ToExtract.Add(Info);
                }
            }
            catch (Exception ex)
            {
                Tool(ex.ToString());
            }
        }
    }
}

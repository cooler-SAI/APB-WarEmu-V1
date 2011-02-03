namespace PakExtractor
{
    partial class Extractor
    {
        /// <summary>
        /// Variable nécessaire au concepteur.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Nettoyage des ressources utilisées.
        /// </summary>
        /// <param name="disposing">true si les ressources managées doivent être supprimées ; sinon, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Code généré par le Concepteur Windows Form

        /// <summary>
        /// Méthode requise pour la prise en charge du concepteur - ne modifiez pas
        /// le contenu de cette méthode avec l'éditeur de code.
        /// </summary>
        private void InitializeComponent()
        {
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.tip_status = new System.Windows.Forms.ToolStripStatusLabel();
            this.tool_progress = new System.Windows.Forms.ToolStripProgressBar();
            this.label1 = new System.Windows.Forms.Label();
            this.datagrid = new System.Windows.Forms.DataGridView();
            this.b_listing = new System.Windows.Forms.Button();
            this.b_extract = new System.Windows.Forms.Button();
            this.l_files = new System.Windows.Forms.ComboBox();
            this.b_allpak = new System.Windows.Forms.Button();
            this.Id = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Extention = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.ZSize = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.RealSize = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Extract = new System.Windows.Forms.DataGridViewButtonColumn();
            this.Software = new System.Windows.Forms.DataGridViewLinkColumn();
            this.statusStrip1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.datagrid)).BeginInit();
            this.SuspendLayout();
            // 
            // statusStrip1
            // 
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.tip_status,
            this.tool_progress});
            this.statusStrip1.Location = new System.Drawing.Point(0, 382);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(750, 22);
            this.statusStrip1.TabIndex = 0;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // tip_status
            // 
            this.tip_status.Name = "tip_status";
            this.tip_status.Size = new System.Drawing.Size(633, 17);
            this.tip_status.Spring = true;
            this.tip_status.Text = "Starting...";
            // 
            // tool_progress
            // 
            this.tool_progress.Name = "tool_progress";
            this.tool_progress.Size = new System.Drawing.Size(100, 16);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(-3, 49);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(28, 13);
            this.label1.TabIndex = 2;
            this.label1.Text = "Files";
            // 
            // datagrid
            // 
            this.datagrid.AllowUserToAddRows = false;
            this.datagrid.AllowUserToDeleteRows = false;
            this.datagrid.AllowUserToOrderColumns = true;
            this.datagrid.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.datagrid.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.Id,
            this.Extention,
            this.ZSize,
            this.RealSize,
            this.Extract,
            this.Software});
            this.datagrid.Location = new System.Drawing.Point(0, 92);
            this.datagrid.Name = "datagrid";
            this.datagrid.Size = new System.Drawing.Size(750, 287);
            this.datagrid.TabIndex = 3;
            // 
            // b_listing
            // 
            this.b_listing.Location = new System.Drawing.Point(0, 16);
            this.b_listing.Name = "b_listing";
            this.b_listing.Size = new System.Drawing.Size(97, 23);
            this.b_listing.TabIndex = 4;
            this.b_listing.Text = "Pak Folder";
            this.b_listing.UseVisualStyleBackColor = true;
            this.b_listing.Click += new System.EventHandler(this.b_listing_Click);
            // 
            // b_extract
            // 
            this.b_extract.Location = new System.Drawing.Point(583, 63);
            this.b_extract.Name = "b_extract";
            this.b_extract.Size = new System.Drawing.Size(100, 23);
            this.b_extract.TabIndex = 5;
            this.b_extract.Text = "Extract this Pak";
            this.b_extract.UseVisualStyleBackColor = true;
            this.b_extract.Click += new System.EventHandler(this.b_extract_Click);
            // 
            // l_files
            // 
            this.l_files.FormattingEnabled = true;
            this.l_files.Location = new System.Drawing.Point(0, 65);
            this.l_files.Name = "l_files";
            this.l_files.Size = new System.Drawing.Size(577, 21);
            this.l_files.TabIndex = 6;
            // 
            // b_allpak
            // 
            this.b_allpak.Location = new System.Drawing.Point(104, 16);
            this.b_allpak.Name = "b_allpak";
            this.b_allpak.Size = new System.Drawing.Size(75, 23);
            this.b_allpak.TabIndex = 7;
            this.b_allpak.Text = "Extract All Pak";
            this.b_allpak.UseVisualStyleBackColor = true;
            this.b_allpak.Click += new System.EventHandler(this.b_allpak_Click);
            // 
            // Id
            // 
            this.Id.HeaderText = "Name";
            this.Id.Name = "Id";
            // 
            // Extention
            // 
            this.Extention.HeaderText = "Extention";
            this.Extention.Name = "Extention";
            // 
            // ZSize
            // 
            this.ZSize.HeaderText = "ZSize";
            this.ZSize.Name = "ZSize";
            // 
            // RealSize
            // 
            this.RealSize.HeaderText = "RealSize";
            this.RealSize.Name = "RealSize";
            // 
            // Extract
            // 
            this.Extract.HeaderText = "Extract";
            this.Extract.Name = "Extract";
            // 
            // Software
            // 
            this.Software.HeaderText = "Software";
            this.Software.Name = "Software";
            this.Software.Width = 200;
            // 
            // Extractor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(750, 404);
            this.Controls.Add(this.b_allpak);
            this.Controls.Add(this.l_files);
            this.Controls.Add(this.b_extract);
            this.Controls.Add(this.b_listing);
            this.Controls.Add(this.datagrid);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.statusStrip1);
            this.Name = "Extractor";
            this.Text = "PakExtractor";
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.datagrid)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.ToolStripStatusLabel tip_status;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.DataGridView datagrid;
        private System.Windows.Forms.Button b_listing;
        private System.Windows.Forms.Button b_extract;
        private System.Windows.Forms.ComboBox l_files;
        private System.Windows.Forms.Button b_allpak;
        private System.Windows.Forms.ToolStripProgressBar tool_progress;
        private System.Windows.Forms.DataGridViewTextBoxColumn Id;
        private System.Windows.Forms.DataGridViewTextBoxColumn Extention;
        private System.Windows.Forms.DataGridViewTextBoxColumn ZSize;
        private System.Windows.Forms.DataGridViewTextBoxColumn RealSize;
        private System.Windows.Forms.DataGridViewButtonColumn Extract;
        private System.Windows.Forms.DataGridViewLinkColumn Software;
    }
}


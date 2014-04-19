namespace WarEmu
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.ConnectButton = new System.Windows.Forms.Button();
            this.textBox3 = new System.Windows.Forms.TextBox();
            this.button1 = new System.Windows.Forms.Button();
            this.textBox2 = new System.Windows.Forms.TextBox();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.QuitButton = new System.Windows.Forms.Button();
            this.ArmoryButton = new System.Windows.Forms.Button();
            this.ForumButton = new System.Windows.Forms.Button();
            this.webBrowser1 = new System.Windows.Forms.WebBrowser();
            this.BugTrackerButton = new System.Windows.Forms.Button();
            this.ChangelogButton = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // ConnectButton
            // 
            this.ConnectButton.BackColor = System.Drawing.Color.Transparent;
            this.ConnectButton.BackgroundImage = global::Patcher.Properties.Resources.play_now;
            this.ConnectButton.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.ConnectButton.Cursor = System.Windows.Forms.Cursors.Hand;
            this.ConnectButton.FlatAppearance.BorderSize = 0;
            this.ConnectButton.FlatAppearance.CheckedBackColor = System.Drawing.Color.Transparent;
            this.ConnectButton.FlatAppearance.MouseDownBackColor = System.Drawing.Color.Transparent;
            this.ConnectButton.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
            this.ConnectButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.ConnectButton.Location = new System.Drawing.Point(369, 536);
            this.ConnectButton.Name = "ConnectButton";
            this.ConnectButton.Size = new System.Drawing.Size(304, 93);
            this.ConnectButton.TabIndex = 16;
            this.ConnectButton.UseVisualStyleBackColor = false;
            this.ConnectButton.Click += new System.EventHandler(this.button2_Click);
            this.ConnectButton.MouseEnter += new System.EventHandler(this.ConnectButton_MouseEnter);
            this.ConnectButton.MouseLeave += new System.EventHandler(this.ConnectButton_MouseLeave);
            // 
            // textBox3
            // 
            this.textBox3.BackColor = System.Drawing.Color.Black;
            this.textBox3.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.textBox3.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.textBox3.ForeColor = System.Drawing.Color.White;
            this.textBox3.Location = new System.Drawing.Point(150, 499);
            this.textBox3.Name = "textBox3";
            this.textBox3.Size = new System.Drawing.Size(434, 19);
            this.textBox3.TabIndex = 15;
            this.textBox3.TextChanged += new System.EventHandler(this.textBox3_TextChanged);
            // 
            // button1
            // 
            this.button1.BackColor = System.Drawing.Color.Transparent;
            this.button1.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("button1.BackgroundImage")));
            this.button1.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.button1.Cursor = System.Windows.Forms.Cursors.Hand;
            this.button1.FlatAppearance.BorderSize = 0;
            this.button1.FlatAppearance.CheckedBackColor = System.Drawing.Color.Transparent;
            this.button1.FlatAppearance.MouseDownBackColor = System.Drawing.Color.Transparent;
            this.button1.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
            this.button1.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.button1.Location = new System.Drawing.Point(593, 498);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(80, 23);
            this.button1.TabIndex = 13;
            this.button1.UseVisualStyleBackColor = false;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            this.button1.MouseEnter += new System.EventHandler(this.button1_MouseEnter);
            this.button1.MouseLeave += new System.EventHandler(this.button1_MouseLeave);
            // 
            // textBox2
            // 
            this.textBox2.BackColor = System.Drawing.Color.Black;
            this.textBox2.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.textBox2.Font = new System.Drawing.Font("Microsoft Sans Serif", 11F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.textBox2.ForeColor = System.Drawing.Color.White;
            this.textBox2.Location = new System.Drawing.Point(130, 589);
            this.textBox2.Name = "textBox2";
            this.textBox2.PasswordChar = '*';
            this.textBox2.Size = new System.Drawing.Size(187, 17);
            this.textBox2.TabIndex = 10;
            this.textBox2.TextChanged += new System.EventHandler(this.textBox2_TextChanged);
            // 
            // textBox1
            // 
            this.textBox1.BackColor = System.Drawing.Color.Black;
            this.textBox1.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.textBox1.Font = new System.Drawing.Font("Microsoft Sans Serif", 11F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.textBox1.ForeColor = System.Drawing.Color.White;
            this.textBox1.Location = new System.Drawing.Point(130, 557);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(187, 17);
            this.textBox1.TabIndex = 9;
            this.textBox1.TextChanged += new System.EventHandler(this.textBox1_TextChanged);
            // 
            // QuitButton
            // 
            this.QuitButton.BackColor = System.Drawing.Color.Transparent;
            this.QuitButton.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("QuitButton.BackgroundImage")));
            this.QuitButton.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.QuitButton.Cursor = System.Windows.Forms.Cursors.Hand;
            this.QuitButton.FlatAppearance.BorderSize = 0;
            this.QuitButton.FlatAppearance.CheckedBackColor = System.Drawing.Color.Transparent;
            this.QuitButton.FlatAppearance.MouseDownBackColor = System.Drawing.Color.Transparent;
            this.QuitButton.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
            this.QuitButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.QuitButton.Location = new System.Drawing.Point(462, 419);
            this.QuitButton.Name = "QuitButton";
            this.QuitButton.Size = new System.Drawing.Size(207, 66);
            this.QuitButton.TabIndex = 17;
            this.QuitButton.UseVisualStyleBackColor = false;
            this.QuitButton.Click += new System.EventHandler(this.QuitButton_Click);
            this.QuitButton.MouseEnter += new System.EventHandler(this.QuitButton_MouseEnter);
            this.QuitButton.MouseLeave += new System.EventHandler(this.QuitButton_MouseLeave);
            // 
            // ArmoryButton
            // 
            this.ArmoryButton.BackColor = System.Drawing.Color.Transparent;
            this.ArmoryButton.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("ArmoryButton.BackgroundImage")));
            this.ArmoryButton.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.ArmoryButton.Cursor = System.Windows.Forms.Cursors.Hand;
            this.ArmoryButton.FlatAppearance.BorderSize = 0;
            this.ArmoryButton.FlatAppearance.MouseDownBackColor = System.Drawing.Color.Transparent;
            this.ArmoryButton.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
            this.ArmoryButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.ArmoryButton.Location = new System.Drawing.Point(462, 150);
            this.ArmoryButton.Name = "ArmoryButton";
            this.ArmoryButton.Size = new System.Drawing.Size(207, 66);
            this.ArmoryButton.TabIndex = 18;
            this.ArmoryButton.UseVisualStyleBackColor = false;
            this.ArmoryButton.Click += new System.EventHandler(this.ArmoryButton_Click);
            this.ArmoryButton.MouseEnter += new System.EventHandler(this.ArmoryButton_MouseEnter);
            this.ArmoryButton.MouseLeave += new System.EventHandler(this.ArmoryButton_MouseLeave);
            // 
            // ForumButton
            // 
            this.ForumButton.BackColor = System.Drawing.Color.Transparent;
            this.ForumButton.BackgroundImage = global::Patcher.Properties.Resources.forum;
            this.ForumButton.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.ForumButton.Cursor = System.Windows.Forms.Cursors.Hand;
            this.ForumButton.FlatAppearance.BorderSize = 0;
            this.ForumButton.FlatAppearance.CheckedBackColor = System.Drawing.Color.Transparent;
            this.ForumButton.FlatAppearance.MouseDownBackColor = System.Drawing.Color.Transparent;
            this.ForumButton.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
            this.ForumButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.ForumButton.Location = new System.Drawing.Point(462, 216);
            this.ForumButton.Name = "ForumButton";
            this.ForumButton.Size = new System.Drawing.Size(207, 66);
            this.ForumButton.TabIndex = 19;
            this.ForumButton.UseVisualStyleBackColor = false;
            this.ForumButton.Click += new System.EventHandler(this.MyAccountButton_Click);
            this.ForumButton.MouseEnter += new System.EventHandler(this.MyAccountButton_MouseEnter);
            this.ForumButton.MouseLeave += new System.EventHandler(this.MyAccountButton_MouseLeave);
            // 
            // webBrowser1
            // 
            this.webBrowser1.AllowNavigation = false;
            this.webBrowser1.AllowWebBrowserDrop = false;
            this.webBrowser1.CausesValidation = false;
            this.webBrowser1.Location = new System.Drawing.Point(54, 164);
            this.webBrowser1.Margin = new System.Windows.Forms.Padding(0);
            this.webBrowser1.MinimumSize = new System.Drawing.Size(20, 20);
            this.webBrowser1.Name = "webBrowser1";
            this.webBrowser1.Size = new System.Drawing.Size(382, 300);
            this.webBrowser1.TabIndex = 20;
            this.webBrowser1.Url = new System.Uri("http://www.warhammer-server.com/news_launcher.php", System.UriKind.Absolute);
            // 
            // BugTrackerButton
            // 
            this.BugTrackerButton.BackColor = System.Drawing.Color.Transparent;
            this.BugTrackerButton.BackgroundImage = global::Patcher.Properties.Resources.bugtracker;
            this.BugTrackerButton.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.BugTrackerButton.Cursor = System.Windows.Forms.Cursors.Hand;
            this.BugTrackerButton.FlatAppearance.BorderSize = 0;
            this.BugTrackerButton.FlatAppearance.CheckedBackColor = System.Drawing.Color.Transparent;
            this.BugTrackerButton.FlatAppearance.MouseDownBackColor = System.Drawing.Color.Transparent;
            this.BugTrackerButton.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
            this.BugTrackerButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.BugTrackerButton.Location = new System.Drawing.Point(462, 282);
            this.BugTrackerButton.Name = "BugTrackerButton";
            this.BugTrackerButton.Size = new System.Drawing.Size(207, 66);
            this.BugTrackerButton.TabIndex = 21;
            this.BugTrackerButton.UseVisualStyleBackColor = false;
            this.BugTrackerButton.Click += new System.EventHandler(this.BugTrackerButton_Click);
            this.BugTrackerButton.MouseEnter += new System.EventHandler(this.BugTrackerButton_MouseEnter);
            this.BugTrackerButton.MouseLeave += new System.EventHandler(this.BugTrackerButton_MouseLeave);
            // 
            // ChangelogButton
            // 
            this.ChangelogButton.BackColor = System.Drawing.Color.Transparent;
            this.ChangelogButton.BackgroundImage = global::Patcher.Properties.Resources.changelog;
            this.ChangelogButton.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.ChangelogButton.Cursor = System.Windows.Forms.Cursors.Hand;
            this.ChangelogButton.FlatAppearance.BorderSize = 0;
            this.ChangelogButton.FlatAppearance.CheckedBackColor = System.Drawing.Color.Transparent;
            this.ChangelogButton.FlatAppearance.MouseDownBackColor = System.Drawing.Color.Transparent;
            this.ChangelogButton.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
            this.ChangelogButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.ChangelogButton.Location = new System.Drawing.Point(462, 348);
            this.ChangelogButton.Name = "ChangelogButton";
            this.ChangelogButton.Size = new System.Drawing.Size(207, 66);
            this.ChangelogButton.TabIndex = 22;
            this.ChangelogButton.UseVisualStyleBackColor = false;
            this.ChangelogButton.Click += new System.EventHandler(this.ChangelogButton_Click);
            this.ChangelogButton.MouseEnter += new System.EventHandler(this.ChangelogButton_MouseEnter);
            this.ChangelogButton.MouseLeave += new System.EventHandler(this.ChangelogButton_MouseLeave);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoValidate = System.Windows.Forms.AutoValidate.EnablePreventFocusChange;
            this.BackColor = System.Drawing.Color.Black;
            this.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("$this.BackgroundImage")));
            this.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.ClientSize = new System.Drawing.Size(705, 653);
            this.Controls.Add(this.ChangelogButton);
            this.Controls.Add(this.BugTrackerButton);
            this.Controls.Add(this.webBrowser1);
            this.Controls.Add(this.ForumButton);
            this.Controls.Add(this.ArmoryButton);
            this.Controls.Add(this.QuitButton);
            this.Controls.Add(this.ConnectButton);
            this.Controls.Add(this.textBox3);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.textBox2);
            this.Controls.Add(this.textBox1);
            this.DoubleBuffered = true;
            this.ForeColor = System.Drawing.Color.Black;
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Name = "Form1";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.TransparencyKey = System.Drawing.Color.Black;
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button ConnectButton;
        private System.Windows.Forms.TextBox textBox3;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.TextBox textBox2;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.Button QuitButton;
        private System.Windows.Forms.Button ArmoryButton;
        private System.Windows.Forms.Button ForumButton;
        private System.Windows.Forms.WebBrowser webBrowser1;
        private System.Windows.Forms.Button BugTrackerButton;
        private System.Windows.Forms.Button ChangelogButton;
    }
}


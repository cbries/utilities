using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using Microsoft.Win32;
using NAudio.Wave;

namespace CreatePlaylists
{
    public static class Utils
    {
        public static bool IsDirectory(string path)
        {
            if (!Directory.Exists(path))
                return false;

            DirectoryInfo info = new DirectoryInfo(path);
            if (info.Exists)
                return true;

            FileAttributes attr = File.GetAttributes(path);

            if ((attr & FileAttributes.Directory) == FileAttributes.Directory)
                return true;

            return false;
        }

        public static string GetHomeDirectory()
        {
            var homeDrive = Environment.GetEnvironmentVariable("HOMEDRIVE");

            if (homeDrive != null)
            {
              var homePath = Environment.GetEnvironmentVariable("HOMEPATH");
              if (homePath != null)
                return homeDrive + Path.DirectorySeparatorChar + homePath;
              return "C:\\";
            }

            return "C:\\";
        }

        public static void ShowExplorer(this string filepath)
        {
            if (IsDirectory(filepath))
            {
                DirectoryInfo info = new DirectoryInfo(filepath);
                if (!info.Exists)
                    return;

                string argument = @"/select, " + filepath;
                System.Diagnostics.Process.Start("explorer.exe", argument);
            }
            else
            {
                FileInfo info = new FileInfo(filepath);
                if (!info.Exists)
                    return;

                string argument = @"/select, " + filepath;
                System.Diagnostics.Process.Start("explorer.exe", argument);
            }
        }

        public static void SaveInRegistry(string path)
        {
            RegistryKey key = Registry.CurrentUser.CreateSubKey("Metadata");
            if (key != null)
            {
                key.SetValue("Folder", path);
                key.Close();
            }
        }

        public static string RestoryRegistry()
        {
            RegistryKey key = Registry.CurrentUser.OpenSubKey("Metadata");
            if (key != null)
            {
                var o = key.GetValue("Folder");
                if (o == null)
                    return GetHomeDirectory();
                return (string)o;
            }
            return GetHomeDirectory();
        }
    }

    public class FileItem
    {
        public string Name { get; set; }
        public string Filename { get; set; }

        public override string ToString()
        {
            return Name;
        }
    }

    public partial class MainWindow
    {
        private readonly SynchronizationContext _ctx;

        public MainWindow()
        {
            InitializeComponent();

            _ctx = SynchronizationContext.Current;
        }

        private void MainWindow_OnInitialized(object sender, EventArgs e)
        {
            TxtDirectory.Text = Utils.RestoryRegistry();
            if (!string.IsNullOrEmpty(TxtDirectory.Text))
                LoadFiles(TxtDirectory.Text.Trim());
        }

        private List<string> GetFilesInFolder(string folderpath)
        {
            return Directory.GetFiles(folderpath, "*.mp3", SearchOption.TopDirectoryOnly).ToList();
        }

        private void BtnSelectDirectory_Click(object sender, RoutedEventArgs e)
        {
            var dlg = new FolderSelectDialog {
                InitialDirectory = Utils.RestoryRegistry(),
                Title = "Select a folder of music"
            };
            if (dlg.Show()) {
                TxtDirectory.Text = dlg.FileName;
                Utils.SaveInRegistry(dlg.FileName);
                LoadFiles(dlg.FileName);
            }
        }

        private void ResetFiles()
        {
            TxtInfo.Clear();
        }

        private long GetDuration(string musicPath)
        {
            if (string.IsNullOrEmpty(musicPath))
                return -1;

            AudioFileReader audioFileReader = new AudioFileReader(musicPath);
            IWavePlayer outwave = new WaveOut();
            outwave.Init(audioFileReader);
            return audioFileReader.Length;
        }

        private void LoadFiles(string folderpath)
        {
            Mp3Files.SelectionMode = SelectionMode.Multiple;
            Mp3Files.Items.Clear();
            var files = GetFilesInFolder(folderpath);
            foreach (var file in files)
            {
                TagLib.File f = TagLib.File.Create(file);

                string name = Path.GetFileName(file);

                //string tagPerfomer = string.Join(",", f.Tag.PerformersSort);
                string tagAlbum = f.Tag.Album;
                string tagTitle = f.Tag.Title;

                string additionalInformation = string.Format("{0} - {1}", 
                    string.IsNullOrEmpty(tagAlbum) ? "x" : tagAlbum,
                    string.IsNullOrEmpty(tagTitle) ? "x" : tagTitle
                    );

                var item = new FileItem()
                {
                    Name = string.Format("{0} ({1})", name, additionalInformation),
                    Filename = file
                };

                Mp3Files.Items.Add(item);
            }
            Mp3Files.SelectAll();
        }

        private async Task<bool> GenerateM3U(string folderpath)
        {
            if (string.IsNullOrEmpty(folderpath) || !Directory.Exists(folderpath))
            {
                ResetFiles();
                return false;
            }

            TxtDirectory.Text = folderpath.Trim();

            var files = Mp3Files.SelectedItems;
            bool isChkAfterPatternChecked = 
                ChkAfterPattern.IsChecked != null && ChkAfterPattern.IsChecked.Value;
            string pattern = TxtPattern.Text;

            return await Task.Run(delegate
            {
                string cnt = "#EXTM3U\r\n";

                foreach (var file in files)
                {
                    var item = file as FileItem;
                    if (item == null)
                        continue;

                    long duration = GetDuration(item.Filename);
                    string filename = Path.GetFileNameWithoutExtension(item.Filename);
                    if (string.IsNullOrEmpty(filename))
                        filename = Path.GetFileName(item.Filename);
                    if (string.IsNullOrEmpty(filename))
                        continue;
                    int p = filename.IndexOf(pattern, StringComparison.OrdinalIgnoreCase);
                    if (isChkAfterPatternChecked)
                        p += pattern.Length;
                    string artist;
                    string title;
                    if (p != -1)
                    {
                        artist = filename.Substring(0, p).Trim();
                        title = filename.Substring(p + 1).Trim();
                    }
                    else
                    {
                        artist = filename;
                        title = filename;
                    }
                    cnt += string.Format("#EXTINF:{0},{1} - {2}\r\n", duration, artist, title);
                    cnt += Path.GetFileName(item.Filename) + "\r\n";
                }

                _ctx.Post(o =>
                {
                    TxtInfo.Text = cnt;
                }, null);

                string name = Path.GetFileNameWithoutExtension(folderpath);
                string targetFilename = Path.Combine(folderpath, name + ".m3u");
                File.WriteAllText(targetFilename, cnt, Encoding.UTF8);
                if (File.Exists(targetFilename))
                    targetFilename.ShowExplorer();
                else
                {
                    return false;
                }

                return true;
            });
        }

        private async void BtnGenerate_Click(object sender, RoutedEventArgs e)
        {
            BtnGenerate.IsEnabled = false;
            Task<bool> task = GenerateM3U(TxtDirectory.Text.Trim());
            bool r = await task;
            if (!r)
                MessageBox.Show(this, "M3U generation failed");
            BtnGenerate.IsEnabled = true;
        }

        private void Mp3Files_OnKeyUp(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.A)
            {
                if ((Keyboard.Modifiers & ModifierKeys.Control) == ModifierKeys.Control)
                {
                    Mp3Files.SelectedItems.Clear();
                    Mp3Files.SelectAll();
                }
            }

            if (e.Key == Key.Escape)
                Mp3Files.SelectedItems.Clear();
        }

        private void Mp3Files_OnSelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            BtnGenerate.IsEnabled = Mp3Files.SelectedItems.Count > 0;
        }

        private void MainWindow_OnKeyUp(object sender, KeyEventArgs e)
        {
            // ignore
        }
    }
}

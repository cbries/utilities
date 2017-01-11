using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.IO.Ports;
using System.Text;
using System.Threading;
using System.Windows;
using System.Windows.Controls;
using Microsoft.Win32;
using Newtonsoft.Json.Linq;

namespace PlotTempi
{
    public partial class MainWindow : Window
    {
        private bool _initialized;
        private int _recentIndex = -1;
        private SerialPort _serialPort;
        private SynchronizationContext _ctx;
        private string _dataContent = "";
        private object _dataContentLock = new object();

        public MainWindow()
        {
            InitializeComponent();

            _ctx = SynchronizationContext.Current;
        }

        private void MainWindow_OnInitialized(object sender, EventArgs e)
        {
            CmbComPorts.Items.Clear();

            var comInfos = ComPortInformationFactory.GetInfos();
            if (comInfos != null)
                Trace.WriteLine("COM Ports: " + comInfos.Count);
            CmbComPorts.Items.Add("--");
            if (comInfos != null)
            {
                foreach (var item in comInfos)
                    CmbComPorts.Items.Add(item);
            }

            CmbComPorts.SelectedIndex = 0;

            _initialized = true;
        }

        private void CmbComPorts_OnDropDownClosed(object sender, EventArgs e)
        {
            if (!_initialized)
                return;

            int index = ((ComboBox) sender).SelectedIndex;
            if (index == 0)
            {
                CloseCom();
            }
            else
            {
                if (_recentIndex == index)
                    return;

                CloseCom();
                OpenCom();

                _recentIndex = index;
            }
        }

        private void CloseCom()
        {
            if (_serialPort != null)
            {
                _serialPort.ErrorReceived -= SerialPortOnErrorReceived;
                _serialPort.DataReceived -= SerialPortOnDataReceived;
                AutoResetEvent ev = new AutoResetEvent(false);
                Thread t = new Thread(new ThreadStart(delegate
                {
                    if (_serialPort != null)
                    {
                        _serialPort.RtsEnable = false;
                        _serialPort.DtrEnable = false;
                        if (_serialPort.IsOpen)
                            _serialPort.Close();
                    }
                }));
                t.Start();
                ev.WaitOne(2500);

                _serialPort.Dispose();
                _serialPort = null;
            }
        }

        private void OpenCom()
        {
            TxtLogView.Clear();
            ComPortInformation comInfo = CmbComPorts.SelectedItem as ComPortInformation;
            if (comInfo == null)
                throw new Exception("COM Port is invalid");
            Log("Open COM Port: " + comInfo.DeviceCaption);
            CloseCom();
            _serialPort = new SerialPort(comInfo.DeviceId, 9600, Parity.None);
            _serialPort.DataReceived += SerialPortOnDataReceived;
            _serialPort.ErrorReceived += SerialPortOnErrorReceived;
            try
            {
                _serialPort.Open();
            }
            catch(Exception ex)
            {
                Log(string.Format("Open failed: {0}", ex.Message));
                CloseCom();
            }
        }

        private void SerialPortOnErrorReceived(object sender, SerialErrorReceivedEventArgs ev)
        {
            Log(string.Format("err -> {0}", ev.EventType));
        }

        private void SerialPortOnDataReceived(object sender, SerialDataReceivedEventArgs ev)
        {
            if (!_serialPort.IsOpen)
                return;

            string dataLine = null;

            try
            {
                dataLine = _serialPort.ReadLine();
            }
            catch
            {
                return;
            }

            if (string.IsNullOrEmpty(dataLine))
                Log(string.Format("{0}: empty line", ev.EventType));
            else
            {
                lock (_dataContentLock)
                {
                    _dataContent += "\r\n" + dataLine.Trim();
                }

                Log(string.Format("{0}: {1}", ev.EventType, dataLine.Trim()));

                //try
                //{
                //    JObject o = JObject.Parse(dataLine);
                //    int counter = (int) o["counter"];
                //    int ir = (int) o["ir"];
                //    string lastAccess = (string) o["lastAccess"];

                //    Log(string.Format("Counter: {0}, IR: {1}, Last Access: {2}", counter, ir, lastAccess));
                //}
                //catch (Exception ex)
                //{
                //    Log(string.Format("err -> incorrect JSON format for: {0} ({1})", dataLine.Trim(), ex.Message));
                //}
            }
        }

        private void Log(string msg)
        {
            if (_ctx != null)
            {
                _ctx.Send(state =>
                {
                    string currentMsg = TxtLogView.Text.Trim();
                    TxtLogView.Text = msg.Trim() + "\r\n" + currentMsg;
                }, null);

            }
        }

        private void CmdClearLog_OnClick(object sender, RoutedEventArgs e)
        {
            if (_ctx != null)
            {
                _ctx.Send(state => { 
                    TxtLogView.Clear();
                }, null);
            }
        }

        private void CmdSaveAsCsv_OnClick(object sender, RoutedEventArgs e)
        {
            CloseCom();

            SaveFileDialog dlg = new SaveFileDialog();
            if ((bool) dlg.ShowDialog())
            {
                string targetFilename = dlg.FileName;
                try
                {
                    lock (_dataContentLock)
                    {
                        // todo
                    }
                }
                catch (Exception ex)
                {
                    Log(string.Format("Creation of CSV failed: {0}", ex.Message));
                }
                finally
                {
                    // ignore
                }
            }

            lock (_dataContentLock)
            {
                _dataContent = "";
            }

            OpenCom();
        }
    }
}

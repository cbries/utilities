using System.Collections.Generic;
using System.Linq;
using System.Management;

namespace PlotGas
{
    public class ComPortInformation
    {
        public string DeviceId { get; set; }
        public string DeviceCaption { get; set; }

        public override string ToString()
        {
            return string.Format("{0} : {1}", DeviceId, DeviceCaption);
        }
    }

    public static class ComPortInformationFactory
    {
        private static void QueryComPortInformation(out List<ComPortInformation> infos)
        {
            infos = new List<ComPortInformation>();
            using (var searcher = new ManagementObjectSearcher
                ("SELECT * FROM WIN32_SerialPort"))
            {
                var ports = searcher.Get().Cast<ManagementBaseObject>().ToList();

                foreach (ManagementBaseObject item in ports)
                {
                    ComPortInformation info = new ComPortInformation()
                    {
                        DeviceId = (string)item["DeviceID"],
                        DeviceCaption = (string)item["Caption"]
                    };

                    if (!infos.Contains(info))
                        infos.Add(info);
                }
            }
        }

        public static List<ComPortInformation> GetInfos()
        {
            List<ComPortInformation> infos = null;
            QueryComPortInformation(out infos);
            return infos;
        }
    }
}

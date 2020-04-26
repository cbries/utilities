/*
   Copyright (c) 2020  Christian Benjamin Ries
   
   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:
   
   The above copyright notice and this permission notice shall be included in all
   copies or substantial portions of the Software.
   
   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
 */
using System;
using System.Linq;

namespace QueryStockData
{
    public class StockData
    {
        const string TradegateUrl = @"https://www.tradegate.de/orderbuch.php?lang=en&isin={0}";

        public string Name { get; set; }
        public string ISIN { get; set; }

        public string LastError { get; private set; }

        public double High { get; private set; }
        public double Low { get; private set; }
        public double Last { get; private set; }

        public StockData(string name, string isin)
        {
            Name = name;
            ISIN = isin;
        }

        public string GetTradegateUrl()
        {
            return string.Format(TradegateUrl, ISIN);
        }

        private bool HasPattern(string line, string pattern)
        {
            if (string.IsNullOrEmpty(line)) return false;
            if (string.IsNullOrEmpty(pattern)) return false;
            return line.IndexOf(pattern, StringComparison.OrdinalIgnoreCase) != -1;
        }

        private string RemoveHtmlTags(string htmlLine)
        {
            if (string.IsNullOrEmpty(htmlLine)) return htmlLine;
            var res = htmlLine.Trim();
            var startIdx = res.IndexOf("<", StringComparison.OrdinalIgnoreCase);
            if (startIdx == -1) return res;
            while(startIdx != -1)
            {
                if (res.Length < startIdx) return res;
                var endIdx = res.IndexOf(">", startIdx, StringComparison.OrdinalIgnoreCase);
                if (endIdx == -1) return res;
                res = res.Remove(startIdx, endIdx - startIdx + 1);
                startIdx = res.IndexOf("<", StringComparison.OrdinalIgnoreCase);
            }
            return res;
        }

        private double GetValueOf(string str)
        {
            if (double.TryParse(str, out var v))
                return v;
            return 0.0;
        }

        public bool Update()
        {
            try
            {
                var url = GetTradegateUrl();
                var cnt = Utilities.DownloadWebData(url);

                // trivial approach
                // split cnt into lines 
                // iterate any line and search for pattern

                var lines = cnt.Split(new[] { '\n' }, StringSplitOptions.RemoveEmptyEntries).ToList();
                var nLines = lines.Count;
                for(var idx = 0; idx < nLines; ++idx)
                {
                    var line = lines[idx];
                    if (string.IsNullOrEmpty(line)) continue;
                    
                    if(HasPattern(line, "<th>Last</th>"))
                    {
                        ++idx;
                        var resLine = lines[idx];
                        var plainCnt = RemoveHtmlTags(resLine);
                        Last = GetValueOf(plainCnt);
                    }
                    else if(HasPattern(line, "<th>High</th>"))
                    {
                        ++idx;
                        var resLine = lines[idx];
                        var plainCnt = RemoveHtmlTags(resLine);
                        High = GetValueOf(plainCnt);
                    }
                    else if(HasPattern(line, "<th>Low</th>"))
                    {
                        ++idx;
                        var resLine = lines[idx];
                        var plainCnt = RemoveHtmlTags(resLine);
                        Low = GetValueOf(plainCnt);
                    }
                    else
                    {
                        // ignore
                    }
                }
            }
            catch(Exception ex)
            {
                LastError = ex.Message;
            }

            return string.IsNullOrEmpty(LastError);
        }

        public void Show()
        {
            Console.Write("{0} ({1})", Name, ISIN);
            Console.WriteLine("  Last( {0} )   High( {1} )  Low( {2} )", Last, High, Low);
        }
    }
}
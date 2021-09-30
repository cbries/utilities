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
using System.Collections.Generic;
using System.IO;
using Newtonsoft.Json.Linq;

namespace QueryStockData
{
    public class StockDataList : List<StockData>
    {
        public bool Parse(string inputFile)
        {
            if (string.IsNullOrEmpty(inputFile)) return false;
            if (!File.Exists(inputFile)) return false;

            try
            {
                var cnt = File.ReadAllText(inputFile);
                if (string.IsNullOrEmpty(cnt)) return false;

                var ar = JArray.Parse(cnt);
                foreach(var it in ar)
                {
                    var o = it as JObject;
                    if (o == null) continue;

                    var name = string.Empty;
                    var isin = string.Empty;

                    if (o["name"] != null)
                        name = o["name"].ToString();
                    if (o["isin"] != null)
                        isin = o["isin"].ToString();

                    if (string.IsNullOrEmpty(name)) continue;
                    if (string.IsNullOrEmpty(isin)) continue;

                    Add(new StockData(name, isin));
                }

                return Count > 0;
            }
            catch
            {
                return false;
            }
        }
    }
}
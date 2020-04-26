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
using System.IO;

namespace QueryStockData
{
    class Program
    {
        static string _stockListFile = @"StockList.json";

        static int Main(string [] args)
        {
            if (args.Length > 0)
                _stockListFile = args[0];

            if(!File.Exists(_stockListFile))
            {
                Console.WriteLine($"Provide a list of stocks in: {_stockListFile}'");
                return 1;
            }
            
            var stockList = new StockDataList();
            var res = stockList.Parse(_stockListFile);
            if(!res)
            {
                Console.WriteLine($"Parse of {_stockListFile} failed.");
                return 1;
            }

            var rnd = new Random();

            foreach (var it in stockList)
            {
                if (it == null) continue;
                Console.WriteLine($"Query data for {it.Name}...");
                it.Update();
                // do not spam the target server
                // just pause some milliseconds
                var msecs = rnd.Next(1, 3) * 1000;
                System.Threading.Thread.Sleep(msecs);
            }

            foreach (var it in stockList)
                it?.Show();

            Console.WriteLine("Enter any key...");
            Console.ReadKey();

            return 0;
        }
    }
}

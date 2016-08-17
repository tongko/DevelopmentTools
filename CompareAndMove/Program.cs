using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace CompareAndMove
{
    class Program
    {
        static void Main(string[] args)
        {
            if (args == null || args.Length == 0)
                args = new[] { "D:\\Synergy_Dev\\bin", "D:\\Projects\\DEV\\bin" };

            var dir1 = args[0];
            var dir2 = args[1];

            var files1 = Directory.GetFiles(dir1, "*.*");
            var files2 = Directory.GetFiles(dir2, "*.*");

            foreach (var item in files1)
            {
                var f1 = Path.GetFileName(item);
                if (!files2.Any(f => Path.GetFileName(f) == f1))
                {
                    var target = Path.Combine(dir2, Path.GetFileName(item));
                    File.Copy(item, target);
                    Console.WriteLine($"Copy [{item}] to [{target}]");
                }
            }

            Console.WriteLine("Done");
            Console.ReadKey(true);
        }
    }
}

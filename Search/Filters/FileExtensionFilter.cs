using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Search.Filters
{
    class FileExtensionFilter : FileFilter
    {
        private HashSet<string> _extensions;

        public FileExtensionFilter(IEnumerable<string> extensions)
        {
            _extensions = new HashSet<string>(extensions, new ExtComparer());
        }

        protected override bool TargetIsFiltered(FileInfo target)
        {
            var ext = target.Extension;
            return _extensions.Contains(ext);
        }

        private class ExtComparer : IEqualityComparer<string>
        {
            public bool Equals(string x, string y)
            {
                if (x == null) return y == null;
                if (y == null) return false;

                return x.Equals(y, StringComparison.OrdinalIgnoreCase);
            }

            public int GetHashCode(string obj)
            {
                return obj.GetHashCode();
            }
        }
    }
}

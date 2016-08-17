using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Search
{
    enum SearchType
    {
        None,
        FileName,
        Content
    }

    [Flags]
    enum SearchInType
    {
        None,
        Text,
        Binary,
        Archive
    }

    class SearchDescriptor
    {
        public string Term { get; set; }

        public System.IO.DirectoryInfo Scope { get; set; }

        public SearchType Type { get; set; }

        public SearchInType FileType { get; set; }
    }
}

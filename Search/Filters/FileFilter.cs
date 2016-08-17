using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace Search.Filters
{
    enum FilterOperators
    {
        Equals,
        NotEquals,
        LessThan,
        GreaterThan,
        LessThanEquals,
		GreaterThanEquals
    }

    abstract class FileFilter
    {
        public bool Filtered(FileInfo target)
        {
            if (target == null)
                throw new ArgumentNullException("target");

            return TargetIsFiltered(target);
        }

        protected abstract bool TargetIsFiltered(FileInfo target);
    }
}

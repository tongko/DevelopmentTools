using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Search.Operators;

namespace Search.Filters
{
	class SizeFilter : FileFilter
	{
		private long[] _sizes;
		private Operator<long> _operator;

		public SizeFilter(long[] sizes, FilterOperators op)
		{
			_sizes = sizes;
			switch (op)
			{
				case FilterOperators.Equals:
					_operator = new OpCompare<long>(CompareOptions.Equal, false);
					break;
				case FilterOperators.NotEquals:
					_operator = new OpCompare<long>(CompareOptions.Equal, false, true);
					break;
				case FilterOperators.LessThan:
					_operator = new OpCompare<long>(CompareOptions.Lesser, false);
					break;
				case FilterOperators.GreaterThan:
					_operator = new OpCompare<long>(CompareOptions.Greater, false);
					break;
				case FilterOperators.LessThanEquals:
					_operator = new OpCompare<long>(CompareOptions.Lesser, true);
					break;
				case FilterOperators.GreaterThanEquals:
					_operator = new OpCompare<long>(CompareOptions.Greater, true);
					break;
				default:
					break;
			}
		}

		protected override bool TargetIsFiltered(FileInfo target)
		{
			var size = target.Length;

			return _operator.Evaluate(size, _sizes[0], _sizes[1]);
		}
	}
}

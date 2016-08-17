using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Search.Operators
{
	class OpRange<T> : Operator<T> where T : struct, IComparable<T>
	{
		private bool _inclusive;
		private bool _isIntersect;

		public OpRange(bool inclusive, bool isIntersect, bool neglect = false)
			: base(neglect)
		{
			_inclusive = inclusive;
			_isIntersect = isIntersect;
		}

		protected override bool DoEvaluate(params T[] values)
		{
			if (values.Length < 3 || (_isIntersect && values.Length < 4))
				return false;

			return _isIntersect ?
				EvalIntersect(values[0], values[1], values[2], values[3]) :
				EvalBetween(values[0], values[1], values[2]);
		}

		private bool EvalBetween(T value, T from, T to)
		{
			var r1 = value.CompareTo(from);
			var r2 = value.CompareTo(to);

			return _inclusive ?
				r1 >= 0 && r2 <= 0 :
				r1 > 0 && r2 < 0;
		}

		private bool EvalIntersect(T from1, T to1, T from2, T to2)
		{
			var r1 = from1.CompareTo(to2);
			var r2 = to1.CompareTo(from2);

			return _inclusive ?
				r1 <= 0 && r2 >= 0 :
				r1 < 0 && r2 > 0;
		}
	}
}

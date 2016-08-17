using System;

namespace Search.Operators
{
	enum CompareOptions
	{
		Lesser = -1,
		Equal = 0,
		Greater = 1
	}

	class OpCompare<T> : Operator<T> where T : struct, IComparable<T>
	{
		private bool	_inclusive;
		private int		_option;

		public OpCompare(CompareOptions option, bool inclusive, bool neglect = false)
			: base(neglect)
		{
			_inclusive = inclusive;
			_option = (int) option;
		}

		protected override bool DoEvaluate(params T[] values)
		{
			var result = values[0].CompareTo(values[1]);

			return _inclusive ? (result == 0 || result == _option) : result == _option;
		}
	}
}

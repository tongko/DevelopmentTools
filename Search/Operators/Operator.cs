using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Search.Operators
{
	abstract class Operator<T> where T : struct, IComparable<T>
	{
		private bool _neglect;

		protected Operator(bool neglect)
		{
			_neglect = neglect;
		}

		public bool Evaluate(params T[] values)
		{
			if (values == null || values.Length < 2)
				return false;

			var final = DoEvaluate(values);

			return _neglect ? !final : final;
		}

		protected abstract bool DoEvaluate(params T[] values);
	}
}

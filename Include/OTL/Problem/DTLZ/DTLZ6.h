/*!
Copyright (C) 2014, 申瑞珉 (Ruimin Shen)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <vector>
#include "DTLZ.h"
#include "Distance/Distance3.h"
#include "Transform/Degenerate.h"
#include "Shape/Utility.h"
#include "Shape/Concave.h"

namespace otl
{
namespace problem
{
namespace dtlz
{
template <typename _TReal>
class DTLZ6 : public DTLZ<_TReal>
{
public:
	typedef _TReal TReal;
	typedef DTLZ<TReal> TSuper;
	typedef typename TSuper::TDecision TDecision;
	typedef typename TSuper::TSolution TSolution;
	typedef typename TSuper::TRange TRange;
	typedef typename TSuper::TBoundary TBoundary;

	DTLZ6(const size_t nObjectives, const size_t distDecisions = 10);
	~DTLZ6(void);

protected:
	size_t _DoEvaluate(TSolution &solution);
	void _Evaluate(const TDecision &decision, std::vector<TReal> &objective);
};

template <typename _TReal>
DTLZ6<_TReal>::DTLZ6(const size_t nObjectives, const size_t distDecisions)
	: TSuper(nObjectives, TBoundary(nObjectives - 1 + distDecisions, std::make_pair<TReal, TReal>(0, 1)))
{
}

template <typename _TReal>
DTLZ6<_TReal>::~DTLZ6(void)
{
}

template <typename _TReal>
size_t DTLZ6<_TReal>::_DoEvaluate(TSolution &solution)
{
	_Evaluate(solution.decision_, solution.objective_);
	return 1;
}

template <typename _TReal>
void DTLZ6<_TReal>::_Evaluate(const TDecision &decision, std::vector<TReal> &objective)
{
	assert(this->IsInside(decision));
	objective.resize(TSuper::GetNumberOfObjectives());
	const size_t posDecisions = TSuper::GetNumberOfObjectives() - 1;
	const TReal distance = distance::Distance3<TReal>(decision.begin() + posDecisions, decision.end(), 0.1);
	const TReal radius = 1 + distance;
	TDecision angles(decision.begin(), decision.begin() + posDecisions);
	transform::Degenerate(angles.begin(), angles.end(), distance, 1);
	shape::ConvertAngles<TReal>(angles.begin(), angles.end());
	shape::Concave(angles.begin(), angles.end(), objective.begin(), objective.end(), radius);
}
}
}
}

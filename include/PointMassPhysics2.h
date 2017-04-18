#pragma once

#include "../Fwd.h"

namespace ls
{

	enum class ImpactType
	{
		Reflection,
		Penetration
	};

	template <class T>
	void resolveImpactVelocities(PointMass2<T>& p1, PointMass2<T>& p2, const Vec2<T>& normal, const T& restitutionCoeff, ImpactType impactType = ImpactType::Reflection);

}

#include "../src/PointMassPhysics2.cpp"

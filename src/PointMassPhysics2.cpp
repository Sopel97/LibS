#include "..\include\PointMassPhysics2.h"

#include <cmath>

#include "Vec2.h"

namespace ls
{
	template <class T>
	void resolveImpactVelocities(PointMass2<T>& p1, PointMass2<T>& p2, const Vec2<T>& normal, const T& restitutionCoeff, ImpactType impactType)
	{
		using std::sqrt;

		const Vec2<T>& n = normal.normalized();
		const Vec2<T>& nperp = n.normal();

		const auto& v1 = p1.velocity();
		const auto& m1 = p1.mass();

		const auto& v2 = p2.velocity();
		const auto& m2 = p2.mass();

		const T v1par = v1.dot(n);
		const T v1perp = v1.dot(nperp);

		const T v2par = v2.dot(n);
		const T v2perp = v2.dot(nperp);

		const T m1v1_plus_m2v2 = m1*v1par + m2*v2par;
		const T m1_plus_m2 = m1 + m2;

		const T sign = impactType == ImpactType::Reflection ? 1.0 : -1.0;
		const T u1par = (restitutionCoeff*m2*(v2par - v1par)*sign + m1v1_plus_m2v2) / m1_plus_m2;
		const T u2par = (restitutionCoeff*m1*(v1par - v2par)*sign + m1v1_plus_m2v2) / m1_plus_m2;

		p1.setVelocity(nperp*v1perp + n*u1par);
		p2.setVelocity(nperp*v2perp + n*u2par);

		return;
	}

}
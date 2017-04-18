#pragma once

#include "Vec2.h"

namespace ls
{
	template <class T>
	class PointMass2
	{
		static_assert(std::is_floating_point<T>::value, "T must be a floating-point type");
	protected:
		Vec2<T> m_position;
		Vec2<T> m_velocity;
		Vec2<T> m_force;
		T m_mass;

	public:
		PointMass2() = default;

		PointMass2(const PointMass2&) = default;
		PointMass2(PointMass2&&) = default;

		PointMass2& operator=(const PointMass2&) = default;
		PointMass2& operator=(PointMass2&&) = default;
		
		PointMass2(const Vec2<T>& position, const T& mass);

		const Vec2<T>& position() const;
		const Vec2<T>& velocity() const;
		const Vec2<T>& force() const;
		T mass() const;

		void setPosition(const Vec2<T>& newPosition);
		void setVelocity(const Vec2<T>& newVelocity);
		void setForce(const Vec2<T>& newForce);

		void move(const Vec2<T>& displacement);
		void addVelocity(const Vec2<T>& deltaVelocity);
		void addForce(const Vec2<T>& deltaForce);
		void addImpulse(const Vec2<T>& impulse);

		// simple update, relies only on current values
		void update(const T& dt);
	};

	using PointMass2D = PointMass2<double>;
	using PointMass2F = PointMass2<float>;

	extern template class PointMass2<double>;
	extern template class PointMass2<float>;
}

#include "../src/PointMass2.cpp"

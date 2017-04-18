#include "..\include\PointMass2.h"

namespace ls
{	
	template <class T>
	PointMass2<T>::PointMass2(const Vec2<T>& position, const T& mass) :
		m_position(position),
		m_velocity(0,0),
		m_force(0,0),
		m_mass(mass)
	{

	}

	template <class T>
	const Vec2<T>& PointMass2<T>::position() const
	{
		return m_position;
	}
	template <class T>
	const Vec2<T>& PointMass2<T>::velocity() const
	{
		return m_velocity;
	}
	template <class T>
	const Vec2<T>& PointMass2<T>::force() const
	{
		return m_force;
	}
	template <class T>
	T PointMass2<T>::mass() const
	{
		return m_mass;
	}

	template <class T>
	void PointMass2<T>::setPosition(const Vec2<T>& newPosition)
	{
		m_position = newPosition;
	}
	template <class T>
	void PointMass2<T>::setVelocity(const Vec2<T>& newVelocity)
	{
		m_velocity = newVelocity;
	}
	template <class T>
	void PointMass2<T>::setForce(const Vec2<T>& newForce)
	{
		m_force = newForce;
	}

	template <class T>
	void PointMass2<T>::move(const Vec2<T>& displacement)
	{
		m_position += displacement;
	}
	template <class T>
	void PointMass2<T>::addVelocity(const Vec2<T>& deltaVelocity)
	{
		m_velocity += deltaVelocity;
	}
	template <class T>
	void PointMass2<T>::addForce(const Vec2<T>& deltaForce)
	{
		m_force += deltaForce;
	}
	template <class T>
	void PointMass2<T>::addImpulse(const Vec2<T>& impulse)
	{
		m_velocity += impulse / m_mass;
	}


	template <class T>
	void PointMass2<T>::update(const T& dt)
	{
		m_velocity += m_force / m_mass;
		m_position += m_velocity;
	}
}
#pragma once

#include "../Shapes/Vec3.h"
#include "../Matrix.h"

namespace ls
{
    namespace gl
    {
        class Camera
        {
        public:
            Camera(float fov, float aspect) :
                m_position(0, 0, 0),
                m_horizontalAngle(ls::Angle2F::radians(0)),
                m_verticalAngle(ls::Angle2F::radians(0)),
                m_near(0.1f),
                m_far(100.0f),
                m_fov(fov),
                m_aspect(aspect)
            {

            }

            void move(const ls::Vec3F& delta)
            {
                m_position += delta;
            }
            void setPosition(const ls::Vec3F& newPosition)
            {
                m_position = newPosition;
            }
            void addAngles(const ls::Angle2F& hor, const ls::Angle2F& vert)
            {
                m_horizontalAngle -= hor;
                m_verticalAngle -= vert;

                clampLook();
            }
            void setAngles(const ls::Angle2F& hor, const ls::Angle2F& vert)
            {
                m_horizontalAngle = hor;
                m_verticalAngle = vert;
                clampLook();
            }
            void lookAt(const ls::Vec3F& target)
            {
                const ls::Vec3F delta = target - m_position;

                const ls::Vec2F zxDir = ls::Vec2F(delta.z, delta.x).normalized();
                m_horizontalAngle = zxDir.angle();

                const ls::Vec2F vertDir = ls::Vec2F(ls::Vec2F(delta.x, delta.z).length(), delta.y).normalized();
                m_verticalAngle = vertDir.angle();
            }
            void setFov(float fov)
            {
                m_fov = fov;
            }
            void setAspect(float aspect)
            {
                m_aspect = aspect;
            }
            void setNear(float near)
            {
                m_near = near;
            }
            void setFar(float far)
            {
                m_far = far;
            }

            ls::Matrix4x4F viewMatrix() const
            {
                return ls::Matrix4x4F::lookAt(
                    m_position, // Camera is at (4,3,3), in World Space
                    m_position + forward(), // and looks at the origin
                    up()  // Head is up (set to 0,-1,0 to look upside-down)
                );
            }
            ls::Matrix4x4F projectionMatrix() const
            {
                return ls::Matrix4x4F::perspective(ls::Angle2F::degrees(m_fov), m_aspect, m_near, m_far);
            }

            ls::Vec3F forward() const
            {
                return ls::Vec3F(
                    m_verticalAngle.cos() * m_horizontalAngle.sin(),
                    m_verticalAngle.sin(),
                    m_verticalAngle.cos() * m_horizontalAngle.cos()
                );
            }
            ls::Vec3F right() const
            {
                return ls::Vec3F(
                    (m_horizontalAngle - ls::Angle2F::degrees(90)).sin(),
                    0,
                    (m_horizontalAngle - ls::Angle2F::degrees(90)).cos()
                );
            }
            ls::Vec3F up() const
            {
                return right().cross(forward());
            }

            float near() const
            {
                return m_near;
            }
            float far() const
            {
                return m_far;
            }
            float fov() const
            {
                return m_fov;
            }
            float aspect() const
            {
                return m_aspect;
            }
            const ls::Vec3F& position() const
            {
                return m_position;
            }
            const ls::Angle2F& horizontalAngle() const
            {
                return m_horizontalAngle;
            }
            const ls::Angle2F& verticalAngle() const
            {
                return m_verticalAngle;
            }

        private:
            ls::Vec3F m_position;
            ls::Angle2F m_horizontalAngle;
            ls::Angle2F m_verticalAngle;
            float m_near;
            float m_far;
            float m_fov;
            float m_aspect;

            void clampLook()
            {
                m_horizontalAngle.normalize();

                if (m_verticalAngle > ls::Angle2F::degrees(90)) m_verticalAngle = ls::Angle2F::degrees(90);
                if (m_verticalAngle < ls::Angle2F::degrees(-90)) m_verticalAngle = ls::Angle2F::degrees(-90);
            }
        };
    }
}

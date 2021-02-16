#pragma once
#include <qmatrix4x4.h>

class Transform final
{
public:
	QMatrix4x4 transform;
	QVector3D position;

	void translate(const QVector3D& translation)
	{
		position += translation;
		transform.translate(translation);
	}
	void rotate(float angle, const QVector3D& axis)
	{
		transform.rotate(QQuaternion::fromAxisAndAngle(axis, angle));
	}
	void rotate(const QQuaternion& quaternion)
	{
		QMatrix4x4 m;
		m.translate(position);
		m.rotate(quaternion);
		m.translate(-position);

		transform = m * transform;
	}
	void reset()
	{
		position = QVector3D(0, 0, 0);
		transform.setToIdentity();
	}
};

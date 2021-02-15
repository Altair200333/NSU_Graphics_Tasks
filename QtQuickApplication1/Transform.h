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
		transform.rotate(angle, axis);
	}
	void reset()
	{
		position = QVector3D(0, 0, 0);
		transform.setToIdentity();
	}
};
#pragma once
#include <qmatrix4x4.h>
#include <qvector3d.h>
#include <QtMath>

class GLCamera final
{
public:
	float FOV = 60;
	float yaw = -90.0f;
	float pitch = 0.0f;

	QVector3D front = { 0, 0, 1 };
	QVector3D right = { 1, 0, 0 };
	QVector3D up = { 0, 1, 0 };
	QVector3D position = {0,0, -1};

	QMatrix4x4 getProjectionMatrix()
	{
		QMatrix4x4 projection;
		projection.perspective(FOV, 4.0f / 3.0f, 0.1f, 100.0f);
		return projection;
	}
	QMatrix4x4 getViewMatrix()
	{
		QMatrix4x4 view;
		view.lookAt(position, (position + front), up);
		return view;
	}
	void updateCameraVectors()
	{
		QVector3D forward;

		forward.setX(cos(qDegreesToRadians(yaw)) * cos(qDegreesToRadians(pitch)));
		forward.setZ(sin(qDegreesToRadians(pitch)));
		forward.setY(-sin(qDegreesToRadians(yaw)) * cos(qDegreesToRadians(pitch)));
		front = (forward).normalized();

		right = QVector3D::crossProduct(forward, QVector3D( 0, 0, 1 )).normalized();
		up = QVector3D::crossProduct(right, front).normalized();
	}
	void translate(QVector3D mult)
	{
		position += front * mult.z();
		position += right * mult.x();
		position += up * mult.y();
	}
};

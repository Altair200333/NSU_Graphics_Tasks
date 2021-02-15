#pragma once

class LightSource final
{
public:
	QColor color = {250,250,250 };
	QVector3D position = {0,0,0};

	LightSource(QVector3D _position = { 0,0,0 }, QColor _color = { 250,250,250 }):position(_position), color(_color)
	{	
	}
};

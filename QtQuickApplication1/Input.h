#pragma once
#include <qmap.h>

class Input final
{
	QMap<int, bool> keys;
	static Input& instance()
	{
		static Input input;
		return input;
	}
public:
	static void pressKey(int keyCode)
	{
		instance().keys[keyCode] = true;
	}
	static void releaseKey(int keyCode)
	{
		instance().keys[keyCode] = false;
	}
	static bool keyPressed(int keyCode)
	{
		return instance().keys[keyCode];
	}
};

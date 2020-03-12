#include <particle.h>

particle::particle(int16_t  *path, uint16_t pathlength, uint16_t loopstart, uint8_t red, uint8_t green, uint8_t blue, bool gamma)
{
	_pointpos = 0;
	_loopstart = loopstart;
	_path = path;
	_pathlength = pathlength / 4;
	_sr = _r = red;
	_sg =_g = green;
	_sb = _b = blue;
	//_fx = NULL;
	_enabled = false;
	_gamma = gamma;
	_brightness = _brightness_r = _brightness_g = _brightness_b = 1.;
	_x = -999; _y = -999;

	for(int i = 0; i < SPARKLEMAX; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			_sparkles[i][j] = 0;
		}
	}
	_sparkleptr = 0;

}

int16_t particle::getPointX()
{
	if(_x != -999)
	{
		return _x;
	}
	else if(_path != NULL)
	{
		return *(_path + _pointpos*2);
	}
	return 0;
}

int16_t particle::getPointY()
{
	if(_y != -999)
	{
		return _y;
	}
	else if(_path != NULL)
	{
		return *(_path + _pointpos*2 + 1);
	}
	return 0;
}

uint16_t particle::getPathLength()
{
	return _pathlength;
}

int16_t particle::nextPoint()
{
	_pointpos++;
	if(_pointpos >= _pathlength)
	{
		_pointpos = _loopstart;
	}
	return _pointpos;
}

#define SPARKLEDELTA 8

void particle::addSparkle()
{
	int rnum = random(8,16);
	uint16_t dx, dy;

	switch(rnum)
	{
		case 0:
			dx = 0 - SPARKLEDELTA; dy = SPARKLEDELTA;
			break;
		case 1:
			dx = 0; dy = SPARKLEDELTA;
			break;
		case 2:
			dx = SPARKLEDELTA; dy = SPARKLEDELTA;
			break;
		case 3:
			dx = SPARKLEDELTA; dy = 0;
			break;
		case 4:
			dx = SPARKLEDELTA; dy = 0 - SPARKLEDELTA;
			break;
		case 5:
			dx = 0; dy = 0 - SPARKLEDELTA;
			break;
		case 6:
			dx = 0 - SPARKLEDELTA; dy = 0 - SPARKLEDELTA;
			break;
		case 7:
			dx = 0 - SPARKLEDELTA; dy = 0;
			break;

		// in-between sparkles
		case 8:
			dx = 0 - SPARKLEDELTA/2; dy = SPARKLEDELTA;
			break;
		case 9:
			dx = SPARKLEDELTA/2; dy = SPARKLEDELTA;
			break;
		case 10:
			dx = SPARKLEDELTA/2; dy = 0 - SPARKLEDELTA;
			break;
		case 11:
			dx = 0 - SPARKLEDELTA/2; dy = 0 - SPARKLEDELTA;
			break;
		case 12:
			dx = 0 - SPARKLEDELTA; dy = SPARKLEDELTA/2;
			break;
		case 13:
			dx = SPARKLEDELTA; dy = SPARKLEDELTA/2;
			break;
		case 14:
			dx = SPARKLEDELTA; dy = 0 - SPARKLEDELTA/2;
			break;
		case 15:
			dx = 0 - SPARKLEDELTA; dy = 0 - SPARKLEDELTA/2;
			break;
	}
	_sparkles[_sparkleptr][0] = getPointX() + dx;
	_sparkles[_sparkleptr][1] = getPointY() + dy;
	_sparkles[_sparkleptr][2] = SPARKLECYCLE;
	//Serial.print("set sparkle ");Serial.print(_sparkleptr);Serial.print(": ");Serial.print(_sparkles[_sparkleptr][0]);
	//	Serial.print(" ");Serial.print(_sparkles[_sparkleptr][1]);Serial.print(" ");Serial.println(_sparkles[_sparkleptr][2]);
	_sparkleptr = (_sparkleptr+1)%SPARKLEMAX;
}

void particle::clearSparkles()
{
	for(int i = 0; i < SPARKLEMAX; i++)
	{
		_sparkles[i][2] = 0;
	}
}
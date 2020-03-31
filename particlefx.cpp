#include <particlefx.h>
#include <math.h>

particleEffectList* particleEffectList::getNext()
{
	particleEffectList* next;
	next = _next;
	while(next != NULL && !next->_item->isEnabled())
	{
		next = next->_next;
	}
	return next;
};

particleEffectList* particleEffectList::getPrevious()
{
	particleEffectList* previous;
	previous = _previous;
	while(previous != NULL && !previous->_item->isEnabled())
	{
		previous = previous->_previous;
	}
	return _previous;
};

void particleEffectList::appendEffect(particleEffect *effect)
{
	particleEffectList *list = this;
	//Serial.println("in appendEffect()");
	int effectcount = 0;
	while(list->_next != NULL)
	{
		list = list->_next;
		effectcount++;
		//Serial.print("count:"); Serial.print(effectcount); Serial.print(" / "); Serial.println((uint32_t)list);
	}
	//Serial.print("appending effect "); Serial.println(effectcount);
	particleEffectList *item = new particleEffectList(effect);
	list->_next = item;
	item->_previous = list;
	//Serial.println("done appendEffect()");
}

void particleEffectList::removeEffects()
{
	particleEffectList* listitem;
	//Serial.println("removeEffects()");
	//effect completed, remove effects from list
	while(listitem = this->popLast())
	{
		if(listitem != NULL)
		{
			//Serial.println("removing effect");
			if(listitem != this)
				free(listitem->getItem());
			free(listitem);
		}
	}
}

particleEffectList* particleEffectList::popLast()
{
	uint16_t counter = 0;
	particleEffectList *list, *last;
	list = last = this;
	while(list->_next != NULL)
	{
		last = list;
		list = list->_next;
		counter++;
	}
	last->_next = NULL;
	if(list == this)
	{
		return NULL;
	}
	//Serial.print("popLast(): removing item ");Serial.println(counter);
	return list;
}

// higher speed: slower
effectDim::effectDim(particle *particle, bool wait, uint16_t startb, uint16_t endb, int16_t speed)
{
	_particle = particle;
	setWait(wait);
	_start_brightness = startb;
	_end_brightness = endb;
	_speed = _counter = speed;
};

effectDim::effectDim(particle *particle, bool wait, uint16_t brightness, int16_t speed)
{
	_particle = particle;
	setWait(wait);
	_start_brightness = _particle->getBrightness();
	_end_brightness = brightness;
	_speed = _counter = speed;
};

bool effectDim::doEffect()
{
	if(_debug > 0)
		Serial.println("effectDim:doEffect()");
	if(_particle->isEnabled())
	{
		if(_counter > 0)
		{
			_counter--;
			if(_debug > 1)
			{
				Serial.print("counter/speed: ");Serial.print(_counter);Serial.print("/");Serial.println(_speed);
				Serial.print("dim value: ");Serial.println(_end_brightness + (_start_brightness - _end_brightness)*((float)_counter / (float)_speed));

			}
			float tb = _end_brightness + (_start_brightness - _end_brightness)*((float)_counter / (float)_speed);
			_particle->setBrightness(tb);
			if(tb < .2)
				_particle->setGreenBrightness(0);
			return getWait();
		}
		else
		{
			_counter = 0;
			_particle->setBrightness(_end_brightness);
			if(!isDone())
			{
				if(_debug > 1 || true)
					Serial.println("dim effect completed");
				setDone(true);
			}
		}
	}
	return false;
}

bool effectPause::doEffect()
{
	if(_debug > 0)
		Serial.println("effectPause:doEffect()");
	if(_particle->isEnabled())
	{
		if(_debug > 1)
			Serial.print("Pause countdown: ");Serial.println(_counter);
		if(_counter > 0)
		{
			_counter--;
			setDone(false);
			return getWait();
		}
		if(_debug > 1)
			Serial.println("effectPause completed");
		setDone(true);
		return false;
	}
	return false;
}

effectMove::effectMove(particle *particle, bool wait, int16_t movetox, int16_t movetoy, uint16_t speed, bool moveback)
{
	_particle = particle;
	setWait(wait);
	_finalx = movetox;
	_finaly = movetoy;
	_startx = particle->getPointX();
	_starty = particle->getPointY();
	_speed = speed;
	_count = 0;
	_pause = 0;
	_moveback = moveback;
	_movemode = 0;
	setDone(false);
}

bool effectMove::doEffect()
{
	// higher speed = slower movement
	if(_debug > 0)
		Serial.println("effectMove:doEffect()");
	if(_particle->isEnabled())
	{
		if(_debug > 1)
		{
		if(_movemode == 0)
			Serial.println("moving from " + String(_particle->getPointX()) + ", " + String(_particle->getPointY()) + " to " + String(_finalx) + ", " + String(_finaly));
		else
			Serial.println("moving from " + String(_particle->getPointX()) + ", " + String(_particle->getPointY()) + " back to " + String(_startx) + ", " + String(_starty));
		}

	}
	if(_particle->isEnabled() && !isDone() &&
		!(!_moveback && (_particle->getPointX() == _finalx) && (_particle->getPointY() == _finaly)) &&
		!(_moveback && (_movemode == 1) && (_particle->getPointX() == _startx) && (_particle->getPointY() == _starty))
	)
	{
		int16_t diffx = _finalx - _particle->getPointX();
		int16_t diffy = _finaly - _particle->getPointY();
		if(_count >= _speed && _movemode == 0)
		{
			_particle->setPointX(_finalx);
			_particle->setPointY(_finaly);
			_movemode = 1;
			//setDone(true);
			//return false;
		}
		else
		{
			if(_movemode == 0)
			{
				// move to point
				_count++;
				int16_t newx = _startx + ((float) _count / _speed)*(_finalx - _startx);
				int16_t newy = _starty + ((float) _count / _speed)*(_finaly - _starty);
				_particle->setPointX(newx);
				_particle->setPointY(newy);
				if(_debug > 1)
				{
					Serial.print("move point: ");Serial.print(_particle->getPointX());Serial.print(",");Serial.print(_particle->getPointY());Serial.print(" speed:");Serial.print(_count);Serial.print("/");Serial.println(_speed);
				}
			}
			else
			{
				if(_pause > 0)
				{
					// wait here a bit
					_pause--;
					//Serial.println("waiting during move");
				}
				else
				{
					// move back to start
					_count--;
					int16_t newx = _startx + ((float) _count / _speed)*(_finalx - _startx);
					int16_t newy = _starty + ((float) _count / _speed)*(_finaly - _starty);
					_particle->setPointX(newx);
					_particle->setPointY(newy);
					if(_debug > 1)
					{
						Serial.print("move back point: ");Serial.print(_particle->getPointX());Serial.print(",");Serial.print(_particle->getPointY());Serial.print(" speed:");Serial.println(_speed);
					}
					if(_count <= 0)
					{
						_particle->setPointX(_startx);
						_particle->setPointY(_starty);
						setDone(true);
					}
				}
			}
		}
	}
	if(isDone() || (!_moveback && (_particle->getPointX() == _finalx) && (_particle->getPointY() == _finaly))
	|| (_moveback && (_movemode == 1) && (_particle->getPointX() == _startx) && (_particle->getPointY() == _starty)))
	{
		// we are done
		if(_debug > 1)
			Serial.println("Move completed");
		setDone(true);
		_movemode = 0;
		_moveback = false;
		return false;
	}
	return getWait();
}

effectFloat::effectFloat(particle *particle, bool wait, uint16_t speed, uint16_t amplitude)
{
	_particle = particle;
	setWait(wait);
	_originaly = _particle->getPointY();
	_speed = speed;
	_amplitude = amplitude;
}

bool effectFloat::doEffect()
{
	if(_debug > 0)
		Serial.println("effectFloat:doEffect()");
	if(_particle->isEnabled())
	{
		int16_t floatdelta = _sinedata320[_counter] - 128;
		_particle->setPointY(_originaly + (int)(floatdelta * _amplitude / 128));
		if(_debug > 1)
		{
			Serial.print("effectFloat point: ");Serial.print(_particle->getPointX());Serial.print(",");Serial.println(_particle->getPointY());
		}
		_counter = (_counter + _speed)%320;
	}
	return getWait();
}

effectMotionBlur::effectMotionBlur(particle *particle, bool wait, particlegfx *particlegfx, Adafruit_NeoMatrix *matrix, uint16_t framecount, float dimvalue)
{
	_particle = particle;
	setWait(wait);
	_particlegfx = particlegfx;
	_matrix = matrix;
	_framecount = max(min(framecount,100),1);
	_frameptr = 0;
	_dimvalue = dimvalue;
	reset();
}

void effectMotionBlur::reset()
{
	for(int i = 0; i < _framecount; i++)
	{
		_frame[i][0] = -999;
		_frame[i][1] = -999;
	}
	setDone(false);
}

bool effectMotionBlur::doEffect()
{
	if(this->isEnabled())
	{
		int dpp = 8; // dots per pixel (64 / 8)
		if(_debug > 0)
			Serial.println("effectDoMotionBlur::doEffect(): brightness = " + String(_particle->getBrightness(),2));
		float brightness = _particle->getBrightness();
		int16_t x = _particle->getPointX();
		int16_t y = _particle->getPointY();
		_particlegfx->drawParticle(*_matrix, _particle, true);
		//Serial.print(_frame[_frameptr][0]);Serial.print(",");Serial.println(_frame[_frameptr][1]);
		if(!((_frame[_frameptr][0]) && (_frame[_frameptr][1] < 0)))
		{
			if(_debug > 1)
				Serial.println("motion blur from " + String(_frame[_frameptr][0]) + "," + String(_frame[_frameptr][1]) + " to " + String(x) + "," + String(y));
			int16_t lastframe = -1;
			int16_t lastx = 1, lasty = -1;
			for(int i = 0; i < _framecount; i++)
			{
				int16_t newx = _frame[(_frameptr + i) % _framecount][0];
				int16_t newy = _frame[(_frameptr + i) % _framecount][1];
				if(newx != lastx || newy != lasty)
				{
					//Serial.println("drawing frame " + String(i) + " @ " + String(newx) + ", " + String(newy));
					_particle->setPointX(newx);
					_particle->setPointY(newy);
					// reduce brightness for motion blur from dimvalue
					float tbrightness = brightness * _dimvalue + brightness * (1.0 - _dimvalue) * (i + 1) / _framecount;
					_particle->setBrightness(tbrightness);
					//if(tbrightness < .25)
					//	_particle->setGreenBrightness(0); // special case for green
					//_particle->setBrightness(1.0);
					//_particle->setBrightness(brightness * i / _framecount);
					_particlegfx->drawParticle(*_matrix, _particle, true);
					lastx = newx;
					lasty = newy;
				}
			}
			// reset back to original values
			_particle->setBrightness(brightness);
			_particle->setPointX(x);
			_particle->setPointY(y);
		}

		// store frame for next time
		_frame[_frameptr][0] = x;
		_frame[_frameptr][1] = y;
		_frameptr = (_frameptr + 1 ) % _framecount;
		return getWait();
	}
	return false;
}

//pulsating light
effectTwinkle::effectTwinkle(particle *particle, bool wait, uint16_t speed, float dimvalue)
{
	_particle = particle;
	setWait(wait);
	_speed = max(speed, 4);
	_counter = 0;
	_dimvalue = dimvalue;
}

bool effectTwinkle::doEffect()
{
	if(_debug > 0)
		Serial.println("effectTwinkle::doEffect()");
	// btype: 0: increase brightness, 1: decrease brightness
	int btype = (int)((float)_counter / (_speed / 2)) % 2;
	int bmode = _counter % (_speed / 2);
	if(btype == 1)
	{
		bmode = (_counter / 2) - bmode;
	}
	_particle->setBrightness(_dimvalue + bmode/(_speed/2.)*(1.0 - _dimvalue));
	if(_debug > 1)
		Serial.println("Twinkle brightness: " + String(_particle->getBrightness(),2));

	_counter = (_counter+1)%_speed;

	return false;  // always on
}

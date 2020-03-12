#ifndef _PARTICLEFX_H
#define _PARTICLEFX_H

#include <arduino.h>
#include <particle.h>
#include <particlegfx.h>

class particleEffect; // forward declaration

class particleEffectList {
	private:
		particleEffect *_item;
		particleEffectList *_next;
		particleEffectList *_previous;
	public:
		particleEffectList(particleEffect *ff = NULL){_item = ff; _next = _previous = NULL;};
		particleEffectList* getNext();
		particleEffectList* getPrevious();
		particleEffect* getItem(){return _item;};
		void appendEffect(particleEffect *effect);
		void removeEffects();
		particleEffectList* popLast();
};

class particleEffect {
	public:
		particleEffect(){_wait = false; _done = false; _enabled = true;};
		virtual bool doEffect() = 0;
		virtual char *effectName(){return "generic";};
		void setWait(bool wait){_wait = wait;};
		bool getWait(){return _wait;};
		bool isDone(){return _done;};
		void setDone(bool done){_done = done;};
		bool isEnabled(){return _enabled;};
		void setEnabled(bool e){_enabled = e;};
		void setDebug(int debug){_debug = debug;};
	protected:
		particle *_particle;
		bool _wait;
		bool _done;
		bool _enabled;
		uint8_t _debug;
};

// dim to a specified brightness and speed
class effectDim : public particleEffect {
	public:
		char *effectName(){return "effectDim";};
		bool doEffect();
		effectDim(particle *particle, bool wait, uint16_t brightness, int16_t speed);
		void setBrightness(float value){_counter = _speed; _end_brightness = value; _start_brightness = _particle->getBrightness(); setDone(false);};
		void setSpeed(int16_t speed){_speed = _counter = speed; setDone(false);};

	private:
		float _start_brightness;
		float _end_brightness;
		int16_t _speed;
		int16_t _counter;
};

// pause before next effect
class effectPause : public particleEffect {
	public:
		char *effectName(){return "effectPause";};
		effectPause(particle *particle, bool wait, uint16_t speed){_particle = particle; setWait(wait); _speed = _counter = speed; setDone(false);};
		void setSpeed(int16_t speed){_speed = _counter = speed;setDone(false);};
		bool doEffect();

	private:
		int16_t _speed;
		int16_t _counter;
};

// set color of light
class effectsetColor : public particleEffect {
	public:
		char *effectName(){return "effectsetColor";};
		effectsetColor(particle *particle, bool wait, uint8_t red, uint8_t green, uint8_t blue){_particle = particle; setWait(wait); _r = red; _g = green; _b = blue;};
		bool doEffect(){_particle->setColor(_r, _g, _b); return getWait();;};

	private:
		uint8_t _r, _g, _b;
};

// move to a new point
class effectMove : public particleEffect {
	public:
		char *effectName(){return "effectMove";};
		effectMove(particle *particle, bool wait, int16_t movetox, int16_t movetoy, uint16_t speed = 1, bool moveback = false);
		bool doEffect();
		void setPoint(int16_t movetox, int16_t movetoy, bool moveback = false){_finalx = movetox; _finaly = movetoy; _startx = _particle->getPointX(); _starty = _particle->getPointY(); setDone(false); _moveback = moveback; _movemode = 0; _count = 0;};
		void setPointX(int16_t movetox, bool moveback = false){_finalx = movetox; _startx = _particle->getPointX(); _starty = _particle->getPointY(); setDone(false); _moveback = moveback; _movemode = 0; _count =0;};
		void setPointY(int16_t movetoy, bool moveback = false){_finaly = movetoy; _startx = _particle->getPointX(); _starty = _particle->getPointY(); setDone(false); _moveback = moveback; _movemode = 0; _count = 0;};
		uint16_t setSpeed(uint16_t speed){_speed = speed; _count = 0; setDone(false);};
		void setPause(uint16_t pause){_pause = pause;};
		void setMoveBack(bool moveback){_moveback = moveback; _movemode = 0;};

	private:
		int16_t _startx, _starty;
		int16_t _finalx, _finaly;
		uint16_t _speed, _count;
		uint16_t _pause;
		int8_t _movemode;
		bool _moveback = false;
};

// simulate floating
class effectFloat : public particleEffect {
	public:
		char *effectName(){return "effectFloat";};
		effectFloat(particle *particle, bool wait, uint16_t speed = 1, uint16_t amplitude = 4);
		bool doEffect();
		void setY(int16_t y){_originaly = y;};
		void setSpeed(uint16_t speed){_speed = speed;};

	private:
	int16_t _counter = 0;
	int16_t _originaly;
	uint16_t _speed = 1;
	uint16_t _amplitude = 4;

	// see https://daycounter.com/Calculators/Sine-Generator-Calculator.phtml

	const int16_t _sinedata320[320] = {
		128,130,133,135,138,140,142,145,
		147,150,152,155,157,160,162,165,
		167,169,172,174,176,179,181,183,
		185,188,190,192,194,196,198,200,
		202,204,206,208,210,212,214,216,
		218,219,221,223,224,226,228,229,
		231,232,234,235,236,237,239,240,
		241,242,243,244,245,246,247,248,
		249,250,250,251,251,252,253,253,
		253,254,254,254,255,255,255,255,
		255,255,255,255,255,254,254,254,
		253,253,253,252,251,251,250,250,
		249,248,247,246,245,244,243,242,
		241,240,239,237,236,235,234,232,
		231,229,228,226,224,223,221,219,
		218,216,214,212,210,208,206,204,
		202,200,198,196,194,192,190,188,
		185,183,181,179,176,174,172,169,
		167,165,162,160,157,155,152,150,
		147,145,142,140,138,135,133,130,
		128,125,122,120,117,115,113,110,
		108,105,103,100,98,95,93,90,
		88,86,83,81,79,76,74,72,
		70,67,65,63,61,59,57,55,
		53,51,49,47,45,43,41,39,
		37,36,34,32,31,29,27,26,
		24,23,21,20,19,18,16,15,
		14,13,12,11,10,9,8,7,
		6,5,5,4,4,3,2,2,
		2,1,1,1,0,0,0,0,
		0,0,0,0,0,1,1,1,
		2,2,2,3,4,4,5,5,
		6,7,8,9,10,11,12,13,
		14,15,16,18,19,20,21,23,
		24,26,27,29,31,32,34,36,
		37,39,41,43,45,47,49,51,
		53,55,57,59,61,63,65,67,
		70,72,74,76,79,81,83,86,
		88,90,93,95,98,100,103,105,
		108,110,113,115,117,120,122,125
	};

};

class effectMotionBlur : public particleEffect {
	public:
		char *effectName(){return "effectMotionBlur";};
		effectMotionBlur(particle *particle, bool wait, particlegfx *particlegfx, Adafruit_NeoMatrix *matrix, uint16_t framecount, float dimvalue = .7);
		bool doEffect();
		void reset();

	private:
	Adafruit_NeoMatrix* _matrix;
	particlegfx* _particlegfx;
	uint16_t _framecount;
	uint16_t _frameptr;
	int16_t	_frame[100][2];
	float _dimvalue;
};

class effectTwinkle : public  particleEffect {
	public:
		char *effectName(){return "effectTwinkle";};
		effectTwinkle(particle *particle, bool wait, uint16_t speed = 10, float dimvalue = .5 );
		bool doEffect();
		void setSpeed(uint16_t speed){_speed = min(speed,2); _counter = 0;};
		void setBrightness(float dimvalue){_dimvalue = dimvalue;};
	private:
		float _dimvalue;
		uint16_t _speed;
		uint16_t _counter;
};

#endif

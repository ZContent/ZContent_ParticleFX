#ifndef _PARTICLE_H
#define _PARTICLE_H

#include <arduino.h>

#define SPARKLEMAX	10
#define SPARKLECYCLE 400

class particle {
		public:
			particle(){_path = 0; };
			particle(int16_t *path, uint16_t pathlength, uint16_t loopstart, uint8_t red = 0, uint8_t green = 0xff, uint8_t blue = 0, bool gamma = false);
			uint16_t getPathLength();
			uint16_t nextPoint();
			bool getGamma(){return _gamma;};
			void setGamma(bool gamma){_gamma = gamma;};
			void resetPoint(){ _pointpos = 0;};
			void setColor(uint8_t r, uint8_t g, uint8_t b){ _r = r; _g = g; _b = b; _sr = r; _sg = g; _sb = b;};
			int16_t getPointX();
			int16_t getPointY();
			void setPointX(int16_t x){ _x = x;};
			void setPointY(int16_t y){ _y = y;};
			void setPoint(int16_t x, int16_t y){ _x = x; _y = y;};
			uint32_t getColor(){return (_r << 16) + (_g << 8) + _b;};
			uint8_t getRed(){return _r;};
			uint8_t getGreen(){return _g;};
			uint8_t getBlue(){return _b;};
			uint8_t getSparkleRed(){return _sr;};
			uint8_t getSparkleGreen(){return _sg;};
			uint8_t getSparkleBlue(){return _sb;};
			float getBrightness(){return _brightness;};
			void setBrightness(float brightness){_brightness = _brightness_r = _brightness_g = _brightness_b = brightness;};
			float getRedBrightness(){return _brightness_r;};
			float getGreenBrightness(){return _brightness_g;};
			float getBlueBrightness(){return _brightness_b;};
			void setRedBrightness(float brightness){_brightness_r = brightness;};
			void setGreenBrightness(float brightness){_brightness_g = brightness;};
			void setBlueBrightness(float brightness){_brightness_b = brightness;};
			bool isEnabled(){return _enabled;};
			void setEnabled(bool enabled){_enabled = enabled;};
			void setSparkleColor(uint8_t r, uint8_t g, uint8_t b){ _sr = r; _sg = g; _sb = b;};
			void addSparkle();
			void clearSparkles();
			uint16_t *getSparkle(int s){return _sparkles[s];};


		private:
			float _brightness, _brightness_r, _brightness_g, _brightness_b;
			bool _enabled, _gamma;
			uint16_t _pointpos;
			uint16_t _loopstart;
			int16_t *_path;
			uint16_t _pathlength;
			int16_t _x, _y;
			uint8_t _r, _g, _b;
			uint8_t _sr, _sg, _sb;
			uint8_t _sparklecount, _sparkleptr;
			uint16_t _sparkles[SPARKLEMAX][3];
};

#endif

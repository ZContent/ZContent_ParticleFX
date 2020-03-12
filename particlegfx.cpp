#include <particlegfx.h>

particlegfx::particlegfx(uint8_t gridx, uint8_t gridy, uint8_t dpp): GFXcanvas16(gridx*dpp, gridy*dpp)
{
	_gridx = gridx;
	_gridy = gridy;
	_dpp = dpp;
};

void particlegfx::drawPixel(int16_t x, int16_t y, uint16_t color) {

  if((x < 0) || (y < 0) || (x >= _width) || (y >= _height)) return;

  int16_t t;
  switch(rotation) {
   case 1:
    t = x;
    x = WIDTH  - 1 - y;
    y = t;
    break;
   case 2:
    x = WIDTH  - 1 - x;
    y = HEIGHT - 1 - y;
    break;
   case 3:
    t = x;
    x = y;
    y = HEIGHT - 1 - t;
    break;
  }
  uint16_t *buffer = getBuffer();
  buffer[y*_gridy*_dpp + x] = color;
}

void particlegfx::drawPixelRGB(Adafruit_NeoMatrix &_matrix, int16_t x, int16_t y, uint8_t r, uint8_t g, uint8_t b, bool gammacorrect)
{
	uint8_t newr, newg, newb;
	newr = r;
	newg = g;
	newb = b;
	if(gammacorrect)
	{
		newr = gamma8[newr];
		newg = gamma8[newg];
		newb = gamma8[newb];
	}
	drawPixel(x, y, _matrix.Color(newr, newg, newb));
}


void particlegfx::resizeBitmap(Adafruit_NeoMatrix &_matrix) {
	uint16_t *buffer = getBuffer();
	uint16_t pixelbuffer[64];
	for(int t = 0; t < 64; t++)
	{
		pixelbuffer[t] = 0;
	}
	for(int i = 0; i < _gridy*_dpp; i++)
	{
		for(int j = 0; j < _gridx*_dpp; j++)
		{
			int16_t x = (j * 8 / _gridx*_dpp);
			int16_t y = (i * 8 / _gridy*_dpp);
			uint16_t pixel = buffer[i * _gridx*_dpp + j];
			uint16_t gcolor = (pixelbuffer[y*_dpp + x] & 0xFC) >> 3;
			uint16_t pgcolor = (pixel & 0xFC) >>3;
			if(pixel != 0 && pgcolor > gcolor)
			{
				//Serial.println(String(pixelbuffer[y*8 + x]) + ":" + String(pgcolor) + " < " + String(pixel) + ":" + String(gcolor) + "?");
				_matrix.drawPixel(x,y,pixel);
				pixelbuffer[y*_dpp + x] = pixel;
			}
			//Serial.print(String("(") + String(x) + String(",") + String(y) + String(") ") + String(pixel) + String(" "));
		}

	}
}

void particlegfx::drawParticle(Adafruit_NeoMatrix &_matrix, particle *ff, bool distancedim)
{
		int16_t px, py, newx, newy, distbrightness;
		uint8_t newr, newg, newb;
		int16_t x = ff->getPointX();
		int16_t y = ff->getPointY();
		uint8_t r = ff->getRed();
		uint8_t g = ff->getGreen();
		uint8_t b = ff->getBlue();
		if(ff->isEnabled())
		{
			for(int i = -1; i < 2; i++)
			for(int j = -1; j < 2; j++)
			{
				px = (int)(x * 1. / _dpp + .5) + j;
				py = (int)(y * 1. / _dpp + .5) + i;
				newx = px * _dpp;
				newy = py * _dpp;
				int16_t dist = (x - newx)*(x - newx) + (y - newy)*(y - newy);
				distbrightness = 0;
				if(dist < (_dpp*_dpp))
				{
					//Serial.println("dist: " + String(dist));

					if(distancedim)
						distbrightness = 100 - dist * 100 / (_dpp*_dpp);
					else
						distbrightness = 100;
					//Serial.println(String(j) + ", " + String(i) + ": " + String(x) + "/" + String(newx) + ", " + String(y) + "/" + String(newy) + " distance; " + String(dist) + ", brightness: " + String(distbrightness));
					/*
					if(distancedim)
					{
						//distbrightness = 100 - dist * 100 / DPP*DPP;

						if(dist < (_dpp*_dpp))
						{
							distbrightness = 100;
							//Serial.println("brightness: " + String(distbrightness));
						}
						else
						{
							distbrightness = 100 - (dist - _dpp*_dpp/2) * 100 / (_dpp * _dpp / 2);
							//distbrightness = 100 - (dist - (_dpp*_dpp/2)) * 100 / (_dpp*_dpp/2);
							//Serial.println("brightness: " + String(distbrightness));
						}

					}
					else
						distbrightness = 100;
					*/
				}
				if(distbrightness > 0)
				{
					//Serial.println("point " + String(px) + "," + String(py) + ": old RGB " + String(r) + " " + String(g) + " " + String(b) + " brightness: " + String(distbrightness));
					newr = r * distbrightness / 100. * ff->getRedBrightness(); // * _brightness;
					newg = g * distbrightness / 100. * ff->getGreenBrightness(); // * _brightness;
					newb = b * distbrightness / 100. * ff->getBlueBrightness(); // * _brightness;
					if(ff->getGreenBrightness() < 1. && (newr > 0 || newb > 0))
					{
						// compensate for green, which is too bright when multiple LEDs enabled
						if(newg < .2)
							newg = 0;
						else
							newg = newg * max(0,ff->getGreenBrightness() - .1);
						/*
						if(ff->getGreenBrightness() < .2)
							newg = newg * 0;
						else if(ff->getGreenBrightness() < .4)
							newg = newg * .3;
						else if(ff->getGreenBrightness() < .7)
							newg = newg * .6;
						else
							newg = newg * .8;
						*/
					}
					if(ff->getGamma())
					{
						newr = gamma8[newr];
						newg = gamma8[newg];
						newb = gamma8[newb];
					}
					_matrix.drawPixel(px, py, _matrix.Color(newr,newg,newb));
					//Serial.println("dpp: " + String(_dpp));
					//Serial.println("point: " + String(newx) + "," + String(newy) + ": new RGB " + String(newr) + "," + String(newg) + "," + String(newb) + " brightness: " + String(ff->getBrightness(),2));
				}
			}
			// add sparkles
			for(int i = 0; i < SPARKLEMAX; i++)
			{
				uint16_t *sparkle = ff->getSparkle(i);
				//Serial.print("sparkle[2]: ");Serial.println(sparkle[2]);
				if(sparkle[2] > 0)
				{
					sparkle[2] = sparkle[2] - 1;
					//Serial.print("sparkle brightness: "); Serial.println(ff->getBrightness(),2);
					if(ff->getBrightness() > .9)
					{
						//Serial.print("sparkle ");Serial.print(i);Serial.print(": ");Serial.print(sparkle[0]);Serial.print(" ");Serial.print(sparkle[1]);Serial.print(" ");Serial.println(sparkle[2]);
						px = (int)(sparkle[0] * 1. / _dpp + .5);
						py = (int)(sparkle[1] * 1. / _dpp + .5);
						// btype: 0: increase brightness, 1: decrease brightness
						int btype = (int)((float)sparkle[2] / (SPARKLECYCLE / 2)) % 2;
						int bmode = sparkle[2] % (SPARKLECYCLE / 2);
						if(btype == 1)
						{
							bmode = (SPARKLECYCLE / 2) - bmode;
						}
						newr = ff->getSparkleRed() * ff->getRedBrightness() * bmode/(SPARKLECYCLE/2);
						newg = ff->getSparkleGreen() * ff->getGreenBrightness() * bmode/(SPARKLECYCLE/2);
						newb = ff->getSparkleBlue() * ff->getBlueBrightness() * bmode/(SPARKLECYCLE/2);
						if(ff->getGamma())
						{
							newr = gamma8[newr];
							newg = gamma8[newg];
							newg = gamma8[newb];
						}
						_matrix.drawPixel(px,py,_matrix.Color(newr,newg,newb));
					}
				}
			}
		}
}

#ifndef __STB_MATH_H
#define __STB_MATH_H

int _ifloor(float x) {
    if (x >= 0) {
        return (int)x;
    } else {
        int y = (int)x;
        return ((float)y == x) ? y : y - 1;
    }
}

#define STBTT_ifloor(x)  _ifloor(x)

int _iceil(float num) {
    int inum = (int)num;
    if (num == (float)inum) {
        return inum;
    }
    return inum + 1;
}

#define STBTT_iceil(x)  _iceil(x)

float _sin(float x) {
  float sin;
  if (x < -3.14159265)
    x += 6.28318531;
  else
   if (x >  3.14159265)
     x -= 6.28318531;

  if (x < 0) {
    sin = 1.27323954 * x + .405284735 * x * x;
    
    if (sin < 0)
      sin = .225 * (sin *-sin - sin) + sin;
    else
      sin = .225 * (sin * sin - sin) + sin;
  } else {
    sin = 1.27323954 * x - 0.405284735 * x * x;
    if (sin < 0)
      sin = .225 * (sin *-sin - sin) + sin;
    else
      sin = .225 * (sin * sin - sin) + sin;
  }
  return sin;
}

#define STBTT_sin(x)  _sin(x)

float _cos(float x) {
	float cos;
    if (x < -3.14159265)
      x += 6.28318531;
    else
      if (x >  3.14159265)
        x -= 6.28318531;
	x += 1.57079632;
	if (x >  3.14159265) x -= 6.28318531;
	if (x < 0) {
    cos = 1.27323954 * x + 0.405284735 * x * x;
    if (cos < 0)
        cos = .225 * (cos *-cos - cos) + cos;
    else
        cos = .225 * (cos * cos - cos) + cos;
	} else {
    cos = 1.27323954 * x - 0.405284735 * x * x;
    if (cos < 0)
        cos = .225 * (cos *-cos - cos) + cos;
    else
        cos = .225 * (cos * cos - cos) + cos;
	}
	return cos;
}

#define STBTT_cos(x)  _cos(x)

float _acos(float x) {
   return (-0.69813170079773212 * x * x - 0.87266462599716477) * x + 1.5707963267948966;
}

#define STBTT_acos(x)  _acos(x)

float _fmod(float a, float b) {
   return (a - b * STBTT_ifloor(a / b));
}

#define STBTT_fmod(x,y)  _fmod(x,y)

float _sqrt(float n) {
  float x = n;
  float y = 1;
  float e = 0.000001;
  while(x - y > e) {
    x = (x + y)/2;
    y = n/x;
  }
  return x;
}

#define STBTT_sqrt(x) _sqrt(x)

float _pow(float x, float y) {
    if (y == 0)
        return 1;
    else if (_fmod(y,2) == 0)
        return _pow(x, y/2)*_pow(x, y/2);
    else
        return x*_pow(x, y/2)*_pow(x, y/2);
}

#define STBTT_pow(x,y) _pow(x,y)

#endif /* __STB_MATH_H */

#include "vec2D.h"

//Constructors
vec2D::vec2D(float u, float v)
{
    this.u = u;
    this.v = v;
}

vec2D::vec2D(void)
{
    this.u = 0;
    this.v = 0;
}

//Operators
vec2D& vec2D::operator=(const vec2D& vec)
{
  _assign(vec);
  return *this;
}


bool vec2D::operator==(const vec2D& vec) const
{
  return (u == vec.u) && (v == vec.v);
}


bool vec2D::operator!=(const vec2D& vec) const
{
  return (u != vec.u) || (v != vec.v);
}


vec2D vec2D::operator-(void) const
{
  return vec2D(-u, -v);
}

vec2D vec2D::operator+(const vec2D& vec) const
{
  return vec2D(u + vec.u, v + vec.v);
}


vec2D vec2D::operator-(const vec2D& vec) const
{
  return vec2D(u - vec.u, v - vec.v);
}


vec2D vec2D::operator*(const float scale) const
{
  return vec2D(u * scale, v * scale);
}


vec2D vec2D::operator/(vec2D::const float scale) const
{
  return vec2D(u / scale, v / scale);
}


vec2D& vec2D::operator+=(const vec2D& vec)
{
  u += vec.u;
  v += vec.v;
  return *this;
}


vec2D& vec2D::operator-=(const vec2D& vec)
{
  u -= vec.u;
  v -= vec.v;
  return *this;
}


vec2D& vec2D::operator*=(const float scale)
{
  u *= scale;
  v *= scale;
  return *this;
}


vec2D& vec2D::operator/=(const float scale)
{
  u /= scale;
  v /= scale;
  return *this;
}

//Methods
float dot(const vec2D& vec) const
{
    return u * vec.u + v * vec.v;
}

float cross(const vec2D &vec) const
{
    return u * vec.v - v * vec.u;
}

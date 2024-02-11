#include <cmath>

class vec2D
{
public:
    //Constructors
    vec2D(void);
    vec2D(float u, float v);

    //Members
    float u;
    float v;

    //Operators
    vec2D& operator=(const vec2D& vec);

    bool operator==(const vec2D& vec) const;
    bool operator!=(const vec2D& vec) const;

    vec2D operator-(void) const;

    vec2D operator+(const vec2D& vec) const;
    vec2D operator-(const vec2D& vec) const;
    vec2D operator*(const float scale) const;
    vec2D operator/(const float scale) const;

    vec2D& operator+=(const vec2D& vec);
    vec2D& operator-=(const vec2D& vec);
    vec2D& operator*=(const float scale);
    vec2D& operator/=(const float scale);

    //Methods
    float dot(const vec2D& vec) const;
    vec2D cross(const vec2D &vec) const;
    float magnitude(const vec2D &vec);
};

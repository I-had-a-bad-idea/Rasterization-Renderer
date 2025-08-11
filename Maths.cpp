#include <algorithm>

// A 3D vector, which can also be used for rgb-values
class float3 {
    public:
        float x, y, z;

        float3(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z) {}

        float r() const { return x; }
        float g() const { return y; }
        float b() const { return z; }

        void set_r(float value) { x = std::clamp(value, 0.0f, 1.0f); }
        void set_g(float value) { y = std::clamp(value, 0.0f, 1.0f); }
        void set_b(float value) { z = std::clamp(value, 0.0f, 1.0f); }

        float3& operator+=(const float3& rhs) {
            x += rhs.x; y += rhs.y; z += rhs.z;
            return *this;
        }

        float3& operator*=(float scalar) {
            x *= scalar; y *= scalar; z *= scalar;
            return *this;
        }
};

// A 2D vector
class float2 {
    public:
        float x, y;

        float2(float x = 0.0f, float y = 0.0f) : x(x), y(y){}

};

class Maths{
    public:

        // Calculate the dot product of a and b
        static float dot(float2 a, float2 b){
            return a.x * b.x + a.y * b.y;
        }
        
        // Rotate vec by 90 degrees (clockwise)
        static float2 perpendicular(float2 vec){
            return float2(vec.y, -vec.x);
        } 
        
        // Check if point pp is on the right side of the line a -> b
        static bool point_on_right_side_of_line(float2 a, float2 b, float2 p){
            float2 ap(p.x - a.x, p.y - a.y);
            float2 ab_perp(perpendicular(float2(b.x - a.x, b.y - a.y)));

            return dot(ap, ab_perp) > 0.0f;
        }


        // Test if point p is inside triangle abc
        static bool point_in_triangle(float2 a, float2 b, float2 c, float2 p){

            bool side_ab = point_on_right_side_of_line(a, b, p);
            bool side_bc = point_on_right_side_of_line(b, c, p);
            bool side_ca = point_on_right_side_of_line(c, a, p);

            return (side_ab == side_bc) && (side_bc == side_ca);
        }
};

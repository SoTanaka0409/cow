#include <iostream>
#include <cmath>

struct VECTOR { float x, y, z; };

VECTOR VScale(VECTOR v, float s) { return {v.x * s, v.y * s, v.z * s}; }
float VSquareSize(VECTOR v) { return v.x*v.x + v.y*v.y + v.z*v.z; }
float VSize(VECTOR v) { return std::sqrt(VSquareSize(v)); }
VECTOR VNorm(VECTOR v) {
    float len = VSize(v);
    if (len < 0.0001f) return {0,0,0}; // Simulating a potential DxLib limit
    return {v.x/len, v.y/len, v.z/len};
}

int main() {
    VECTOR vel = {0,0,0};
    VECTOR target = {1,0,0};
    vel = VScale(target, 0.001f);
    vel = VNorm(vel);
    std::cout << vel.x << "," << vel.y << "," << vel.z << std::endl;
    return 0;
}

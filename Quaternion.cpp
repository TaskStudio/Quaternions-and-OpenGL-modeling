#include "include/Quaternion.h"

Quaternion::Quaternion(float w, float x, float y, float z) : w(w), x(x), y(y), z(z) {}

Quaternion Quaternion::operator+(const Quaternion& q) const {
    return Quaternion(w + q.w, x + q.x, y + q.y, z + q.z);
}

Quaternion Quaternion::operator*(const Quaternion& q) const {
    return Quaternion(
            w * q.w - x * q.x - y * q.y - z * q.z,
            w * q.x + x * q.w + y * q.z - z * q.y,
            w * q.y - x * q.z + y * q.w + z * q.x,
            w * q.z + x * q.y - y * q.x + z * q.w
    );
}

Quaternion Quaternion::conjugate() const {
    return Quaternion(w, -x, -y, -z);
}

float Quaternion::norm() const {
    return std::sqrt(w * w + x * x + y * y + z * z);
}

Quaternion Quaternion::normalize() const {
    float n = norm();
    return Quaternion(w / n, x / n, y / n, z / n);
}

float Quaternion::dot(const Quaternion& q) const {
    return w * q.w + x * q.x + y * q.y + z * q.z;
}

// Affichage d'un quaternion
void Quaternion::to4x4Matrix(float matrix[16]) const {
    float a = w, b = x, c = y, d = z;
    matrix[0] = a;  matrix[1] = -b; matrix[2] = -c; matrix[3] = -d;
    matrix[4] = b;  matrix[5] = a;  matrix[6] = -d; matrix[7] = c;
    matrix[8] = c;  matrix[9] = d;  matrix[10] = a; matrix[11] = -b;
    matrix[12] = d; matrix[13] = -c; matrix[14] = b; matrix[15] = a;
}

Quaternion Quaternion::from4x4Matrix(const float matrix[16]) {
    float a = matrix[0];  // a
    float b = matrix[4];  // b
    float c = matrix[8];  // c
    float d = matrix[12]; // d
    return Quaternion(a, b, c, d);
}

void Quaternion::multiplyMatrices4x4(const float mat1[16], const float mat2[16], float result[16]) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result[i * 4 + j] = 0;
            for (int k = 0; k < 4; ++k) {
                result[i * 4 + j] += mat1[i * 4 + k] * mat2[k * 4 + j];
            }
        }
    }
}

void Quaternion::toRotationMatrix3x3(float matrix[9]) const {
    float a = w, b = x, c = y, d = z;
    matrix[0] = 1 - 2*c*c - 2*d*d;
    matrix[1] = 2*b*c - 2*a*d;
    matrix[2] = 2*b*d + 2*a*c;
    matrix[3] = 2*b*c + 2*a*d;
    matrix[4] = 1 - 2*b*b - 2*d*d;
    matrix[5] = 2*c*d - 2*a*b;
    matrix[6] = 2*b*d - 2*a*c;
    matrix[7] = 2*c*d + 2*a*b;
    matrix[8] = 1 - 2*b*b - 2*c*c;
}

Quaternion Quaternion::fromRotationMatrix3x3(const float matrix[9]) {
    float trace = matrix[0] + matrix[4] + matrix[8];
    float w, x, y, z;

    if (trace > 0) {
        float s = 0.5f / std::sqrt(trace + 1.0f);
        w = 0.25f / s;
        x = (matrix[7] - matrix[5]) * s;
        y = (matrix[2] - matrix[6]) * s;
        z = (matrix[3] - matrix[1]) * s;
    } else {
        if (matrix[0] > matrix[4] && matrix[0] > matrix[8]) {
            float s = 2.0f * std::sqrt(1.0f + matrix[0] - matrix[4] - matrix[8]);
            w = (matrix[7] - matrix[5]) / s;
            x = 0.25f * s;
            y = (matrix[1] + matrix[3]) / s;
            z = (matrix[2] + matrix[6]) / s;
        } else if (matrix[4] > matrix[8]) {
            float s = 2.0f * std::sqrt(1.0f + matrix[4] - matrix[0] - matrix[8]);
            w = (matrix[2] - matrix[6]) / s;
            x = (matrix[1] + matrix[3]) / s;
            y = 0.25f * s;
            z = (matrix[5] + matrix[7]) / s;
        } else {
            float s = 2.0f * std::sqrt(1.0f + matrix[8] - matrix[0] - matrix[4]);
            w = (matrix[3] - matrix[1]) / s;
            x = (matrix[2] + matrix[6]) / s;
            y = (matrix[5] + matrix[7]) / s;
            z = 0.25f * s;
        }
    }

    return Quaternion(w, x, y, z);
}

std::ostream& operator<<(std::ostream& os, const Quaternion& q) {
    os << "(" << q.w << " + " << q.x << "i + " << q.y << "j + " << q.z << "k)";
    return os;
}

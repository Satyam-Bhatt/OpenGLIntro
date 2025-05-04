#ifndef VECTOR_H
#define VECTOR_H

#include <cmath>
#include <iostream>

namespace Vector {

    struct Vector2 {
        float x;
        float y;

        // Constructors
        Vector2() : x(0.0f), y(0.0f) {}
        Vector2(float _x, float _y) : x(_x), y(_y) {}

        // Copy constructor
        Vector2(const Vector2& other) : x(other.x), y(other.y) {}

        // Assignment operator
        Vector2& operator=(const Vector2& other) {
            if (this != &other) {
                x = other.x;
                y = other.y;
            }
            return *this;
        }

        // Vector addition
        Vector2 operator+(const Vector2& other) const {
            return Vector2(x + other.x, y + other.y);
        }

        // Vector subtraction
        Vector2 operator-(const Vector2& other) const {
            return Vector2(x - other.x, y - other.y);
        }

        // Scalar multiplication
        Vector2 operator*(float scalar) const {
            return Vector2(x * scalar, y * scalar);
        }

        // Scalar division
        Vector2 operator/(float scalar) const {
            if (scalar != 0.0f) {
                return Vector2(x / scalar, y / scalar);
            }
            std::cerr << "Warning: Division by zero!" << std::endl;
            return *this;
        }

        // Compound assignment operators
        Vector2& operator+=(const Vector2& other) {
            x += other.x;
            y += other.y;
            return *this;
        }

        Vector2& operator-=(const Vector2& other) {
            x -= other.x;
            y -= other.y;
            return *this;
        }

        Vector2& operator*=(float scalar) {
            x *= scalar;
            y *= scalar;
            return *this;
        }

        Vector2& operator/=(float scalar) {
            if (scalar != 0.0f) {
                x /= scalar;
                y /= scalar;
            }
            else {
                std::cerr << "Warning: Division by zero!" << std::endl;
            }
            return *this;
        }

        // Comparison operators
        bool operator==(const Vector2& other) const {
            return (x == other.x && y == other.y);
        }

        bool operator!=(const Vector2& other) const {
            return !(*this == other);
        }

        // Dot product
        float Dot(const Vector2& other) const {
            return x * other.x + y * other.y;
        }

        // Cross product (returns the magnitude of the cross product)
        float Cross(const Vector2& other) const {
            return x * other.y - y * other.x;
        }

        // Length (magnitude) of the vector
        float Length() const {
            return std::sqrt(x * x + y * y);
        }

        // Squared length (avoids the square root calculation)
        float LengthSquared() const {
            return x * x + y * y;
        }

        // Normalize the vector (make it unit length)
        Vector2 Normalized() const {
            float length = Length();
            if (length > 0.0f) {
                return Vector2(x / length, y / length);
            }
            return *this;
        }

        // Normalize this vector in-place
        void Normalize() {
            float length = Length();
            if (length > 0.0f) {
                x /= length;
                y /= length;
            }
        }

        // Distance between two vectors
        static float Distance(const Vector2& a, const Vector2& b) {
            return (b - a).Length();
        }

        // Squared distance between two vectors
        static float DistanceSquared(const Vector2& a, const Vector2& b) {
            return (b - a).LengthSquared();
        }

        // Angle between two vectors (in radians)
        static float Angle(const Vector2& a, const Vector2& b) {
            float dot = a.Dot(b);
            float lengthProduct = a.Length() * b.Length();
            if (lengthProduct > 0.0f) {
                // Clamp to avoid rounding errors
                float cosAngle = std::max(-1.0f, std::min(1.0f, dot / lengthProduct));
                return std::acos(cosAngle);
            }
            return 0.0f;
        }

        // Linear interpolation between two vectors
        static Vector2 Lerp(const Vector2& a, const Vector2& b, float t) {
            t = std::max(0.0f, std::min(1.0f, t)); // Clamp t between 0 and 1
            return a * (1.0f - t) + b * t;
        }

        // Reflect a vector around a normal
        static Vector2 Reflect(const Vector2& direction, const Vector2& normal) {
            Vector2 normalizedNormal = normal.Normalized();
            return direction - normalizedNormal * 2.0f * direction.Dot(normalizedNormal);
        }

        // Print the vector (for debugging)
        void Print() const {
            std::cout << "Vector2(" << x << ", " << y << ")" << std::endl;
        }
    };

    struct Vector3 {
        float x;
        float y;
        float z;

        // Constructors
        Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
        Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

        // Constructor from Vector2 (extends to 3D with z=0)
        explicit Vector3(const Vector2& v, float _z = 0.0f) : x(v.x), y(v.y), z(_z) {}

        // Copy constructor
        Vector3(const Vector3& other) : x(other.x), y(other.y), z(other.z) {}

        // Assignment operator
        Vector3& operator=(const Vector3& other) {
            if (this != &other) {
                x = other.x;
                y = other.y;
                z = other.z;
            }
            return *this;
        }

        // Vector addition
        Vector3 operator+(const Vector3& other) const {
            return Vector3(x + other.x, y + other.y, z + other.z);
        }

        // Vector subtraction
        Vector3 operator-(const Vector3& other) const {
            return Vector3(x - other.x, y - other.y, z - other.z);
        }

        // Scalar multiplication
        Vector3 operator*(float scalar) const {
            return Vector3(x * scalar, y * scalar, z * scalar);
        }

        // Scalar division
        Vector3 operator/(float scalar) const {
            if (scalar != 0.0f) {
                return Vector3(x / scalar, y / scalar, z / scalar);
            }
            std::cerr << "Warning: Division by zero!" << std::endl;
            return *this;
        }

        // Compound assignment operators
        Vector3& operator+=(const Vector3& other) {
            x += other.x;
            y += other.y;
            z += other.z;
            return *this;
        }

        Vector3& operator-=(const Vector3& other) {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            return *this;
        }

        Vector3& operator*=(float scalar) {
            x *= scalar;
            y *= scalar;
            z *= scalar;
            return *this;
        }

        Vector3& operator/=(float scalar) {
            if (scalar != 0.0f) {
                x /= scalar;
                y /= scalar;
                z /= scalar;
            }
            else {
                std::cerr << "Warning: Division by zero!" << std::endl;
            }
            return *this;
        }

        // Unary negation
        Vector3 operator-() const {
            return Vector3(-x, -y, -z);
        }

        // Comparison operators
        bool operator==(const Vector3& other) const {
            return (x == other.x && y == other.y && z == other.z);
        }

        bool operator!=(const Vector3& other) const {
            return !(*this == other);
        }

        // Dot product
        float Dot(const Vector3& other) const {
            return x * other.x + y * other.y + z * other.z;
        }

        // Cross product
        Vector3 Cross(const Vector3& other) const {
            return Vector3(
                y * other.z - z * other.y,
                z * other.x - x * other.z,
                x * other.y - y * other.x
            );
        }

        // Length (magnitude) of the vector
        float Length() const {
            return std::sqrt(x * x + y * y + z * z);
        }

        // Squared length (avoids the square root calculation)
        float LengthSquared() const {
            return x * x + y * y + z * z;
        }

        // Normalize the vector (make it unit length)
        Vector3 Normalized() const {
            float length = Length();
            if (length > 0.0f) {
                return Vector3(x / length, y / length, z / length);
            }
            return *this;
        }

        // Normalize this vector in-place
        void Normalize() {
            float length = Length();
            if (length > 0.0f) {
                x /= length;
                y /= length;
                z /= length;
            }
        }

        // Convert to Vector2 (drops the z component)
        Vector2 ToVector2() const {
            return Vector2(x, y);
        }

        // Distance between two vectors
        static float Distance(const Vector3& a, const Vector3& b) {
            return (b - a).Length();
        }

        // Squared distance between two vectors
        static float DistanceSquared(const Vector3& a, const Vector3& b) {
            return (b - a).LengthSquared();
        }

        // Angle between two vectors (in radians)
        static float Angle(const Vector3& a, const Vector3& b) {
            float dot = a.Dot(b);
            float lengthProduct = a.Length() * b.Length();
            if (lengthProduct > 0.0f) {
                // Clamp to avoid rounding errors
                float cosAngle = std::max(-1.0f, std::min(1.0f, dot / lengthProduct));
                return std::acos(cosAngle);
            }
            return 0.0f;
        }

        // Linear interpolation between two vectors
        static Vector3 Lerp(const Vector3& a, const Vector3& b, float t) {
            t = std::max(0.0f, std::min(1.0f, t)); // Clamp t between 0 and 1
            return a * (1.0f - t) + b * t;
        }

        // Reflect a vector around a normal
        static Vector3 Reflect(const Vector3& direction, const Vector3& normal) {
            Vector3 normalizedNormal = normal.Normalized();
            return direction - normalizedNormal * 2.0f * direction.Dot(normalizedNormal);
        }

        // Project a vector onto another vector
        static Vector3 Project(const Vector3& vector, const Vector3& onNormal) {
            Vector3 normal = onNormal.Normalized();
            return normal * vector.Dot(normal);
        }

        // Get the perpendicular component of a vector relative to a normal
        static Vector3 Perpendicular(const Vector3& vector, const Vector3& normal) {
            return vector - Project(vector, normal);
        }

        // Print the vector (for debugging)
        void Print() const {
            std::cout << "Vector3(" << x << ", " << y << ", " << z << ")" << std::endl;
        }
    };

    // Global scalar multiplication (allows scalar * vector syntax)
    inline Vector2 operator*(float scalar, const Vector2& vector) {
        return vector * scalar;
    }

    inline Vector3 operator*(float scalar, const Vector3& vector) {
        return vector * scalar;
    }

} // namespace Vector

#endif // VECTOR_H

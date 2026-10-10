# dmath
Library that provides math structures and operations on them in C++23.

It is very much WIP. 

The informal goal is to make an API that is simple from a maths perspective. Documentation on it will be done when I'm done soon.
## Requirements
- C++23 compiler
- CMake

## Example
```cpp
#include <iostream>
#include <cmath>

#include <dmath/dmath.hpp>

int main() {
    // 1. Initialize a 2D position vector using vec2f
    vec2f position{3.0f, 4.0f};
    std::cout << "Initial Position: (" << position.x << ", " << position.y << ")\n";

    // 2. Compute the vector's length/magnitude
    float initial_dist = position.length();
    std::cout << "Distance from origin: " << initial_dist << "\n\n";

    // 3. Create a 2D rotation matrix for a 90-degree turn (pi / 2 radians)
    float angle = 3.1415926535f / 2.0f;
    mat2f rot_matrix = mat2f::rotation(angle);
    std::cout << "Rotation Matrix (90 degrees):\n";
    std::cout << "[ " << rot_matrix(1, 1) << ", " << rot_matrix(1, 2) << " ]\n";
    std::cout << "[ " << rot_matrix(2, 1) << ", " << rot_matrix(2, 2) << " ]\n\n";

    // 4. Apply matrix operations and transform the vector coordinates
    // Using matrix indexing and basic vector scaling
    vec2f transformed;
    transformed.x = rot_matrix(1, 1) * position.x + rot_matrix(1, 2) * position.y;
    transformed.y = rot_matrix(2, 1) * position.x + rot_matrix(2, 2) * position.y;

    // 5. Test matrix properties and scaling
    mat2f scaled_matrix = rot_matrix * 2.0f;
    std::cout << "Scaled Matrix Trace: " << scaled_matrix.trace() << "\n";
    std::cout << "Is matrix singular? " << (scaled_matrix.is_singular() ? "Yes" : "No") << "\n";

    // 6. Output final transformed position
    std::cout << "Transformed Position: (" << transformed.x << ", " << transformed.y << ")\n";

    return 0;
}

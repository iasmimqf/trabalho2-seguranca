using namespace std;

#ifndef GF16
#define GF16

/*  This struct implements arithmetic over the finite field GF(2^4)
    using the irreducible polynomial x⁴ + x + 1 (binary: 10011, decimal: 19)
    The field has 16 elements (0–15), and addition/multiplication are
    performed using polynomial arithmetic modulo the primitive polynomial
    **Addition in GF(2^4) is just a bitwise XOR
*/
struct GF_16 {
    // Reduces a polynomial (as an int) modulo x⁴ + x + 1
    // Assumes input x may be up to degree 6
    int mod(int x) {
        // Start with the part of x that already fits in GF(16) (degree ≤ 3)
        int ans = x & 0b1111;  // Keep lower 4 bits

        // For each term with degree ≥ 4, reduce it using x⁴ ≡ x + 1 mod primitive
        for (int i = 4; i <= 6; i++) {
            if ((x >> i) & 1) {
                // For each high-degree bit, simulate reduction:
                // x^i ≡ x^(i-4) * (x + 1)
                ans ^= mul(3, (1 << (i - 4)));  // 3 == x + 1
            }
        }

        return ans;
    }

    // Multiplies two polynomials in GF(2⁴)
    int mul(int x, int y) {
        int ans = 0;

        // Multiply polynomials as binary numbers (shift and XOR)
        for (int i = 0; i < 4; i++) {
            if ((x >> i) & 1) {
                ans ^= (y << i);  // Add y * x^i to the result
            }
        }

        // Reduce the result modulo the primitive polynomial
        return mod(ans);
    }
};

#endif

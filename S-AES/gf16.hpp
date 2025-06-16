using namespace std;

#ifndef GF16
#define GF16

/**
 * @struct GF_16
 * @brief Implements arithmetic in the finite field GF(2^4) defined by the irreducible polynomial x⁴ + x + 1.
 *
 * @details
 * The field GF(2^4) has 16 elements represented as 4-bit numbers (0 to 15).
 * Arithmetic operations are done modulo the primitive polynomial x⁴ + x + 1
 * (binary representation 10011, decimal 19).
 *
 * Addition in GF(2^4) corresponds to bitwise XOR.
 * Multiplication is polynomial multiplication modulo the primitive polynomial.
 */
struct GF_16 {
    /**
     * @brief Reduces a polynomial (represented as an integer) modulo the primitive polynomial x⁴ + x + 1.
     *
     * @param x Polynomial to reduce, possibly up to degree 6 (bits beyond 3).
     * @return The reduced polynomial as an integer within GF(2^4) (4 bits).
     */
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

    /**
     * @brief Multiplies two polynomials in GF(2^4).
     *
     * @param x First polynomial operand (4-bit integer).
     * @param y Second polynomial operand (4-bit integer).
     * @return The product modulo the primitive polynomial as a 4-bit integer.
    */
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

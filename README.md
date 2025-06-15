## AES and Simplified AES (S-AES) Implementation

This repository contains the implementation of the Simplified Advanced Encryption Standard (S-AES), designed purely for educational purposes. The goal of this project is to compare the original AES algorithm with the simplified version in execution time, efficiency, and security concepts.

The code also includes performance analysis between AES and SAES (both with ECB) and among the operation modes of AES (ECB, CBC, CFB, OFB, and CTR), highlighting their differences in behavior and speed.

<b>Note</b>: S-AES is intended for educational use only. It is not safe for real world applications

### How to run

To run the S-AES implementation, just compile the file S-AES/main.cpp using gcc

<pre> g++ S-AES/main.cpp -o saes
./saes</pre>


Then, follow the instructions printed in the terminal
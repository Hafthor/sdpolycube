#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define Num long
#define PrtNum "%ld"
#define byte unsigned char

const int N = 23;
const int filterDepth = 5;
const int maxLeftStackLen = 4 * (N - filterDepth) - 2;

const int X = (N + 5) / 4 * ((N + 5) / 4 * 3 - 2);
const int Y = X + 1;
const int Z = X + (N + 5) / 4 * 3;

Num countExtensions(const int filter, const int depth, byte **refStack, byte **stackPtr, byte **stackLimit) {
    Num count = 0;
    byte **stackTopOriginal = stackPtr;
    while (stackPtr != refStack) {
        byte *index = *--stackPtr;
        byte **stackTopInner = stackPtr;

        byte *p;
        if (!++*(p = index - X)) *stackTopInner++ = p;
        if (!++*(p = index - Y)) *stackTopInner++ = p;
        if (!++*(p = index - Z)) *stackTopInner++ = p;
        if (!++*(p = index + X)) *stackTopInner++ = p;
        if (!++*(p = index + Y)) *stackTopInner++ = p;
        if (!++*(p = index + Z)) *stackTopInner++ = p;

        if (depth == 4) {
            byte **stackTop = stackTopInner;
            const int length = stackTop-refStack;
            count += (length * (length - 1) * (length - 2) / 6);
            byte **stackTopTemp = stackTop;
            for (const int lengthPlus = (length << 1) - 511; stackTopTemp != refStack;) {
                byte *i = *--stackTopTemp, *ii;
                int neighbors = 0, subCount = 128;
                byte b;
                if ((b = *(ii = i - X)) > 127) {
                    count += *ii = --b;
                    subCount += *(i - X - X) + *(i - X - Y) + *(i - X - Z) + *(i - X + Y) + *(i - X + Z);
                    neighbors++;
                }
                if ((b = *(ii = i - Y)) > 127) {
                    count += *ii = --b;
                    subCount += *(i - Y - Y) + *(i - Y - X) + *(i - Y - Z) + *(i - Y + X) + *(i - Y + Z);
                    neighbors++;
                }
                if ((b = *(ii = i - Z)) > 127) {
                    count += *ii = --b;
                    subCount += *(i - Z - Z) + *(i - Z - X) + *(i - Z - Y) + *(i - Z + X) + *(i - Z + Y);
                    neighbors++;
                }
                if ((b = *(ii = i + X)) > 127) {
                    count += *ii = --b;
                    subCount += *(i + X + X) + *(i + X + Y) + *(i + X + Z) + *(i + X - Y) + *(i + X - Z);
                    neighbors++;
                }
                if ((b = *(ii = i + Y)) > 127) {
                    count += *ii = --b;
                    subCount += *(i + Y + Y) + *(i + Y + X) + *(i + Y + Z) + *(i + Y - X) + *(i + Y - Z);
                    neighbors++;
                }
                if ((b = *(ii = i + Z)) > 127) {
                    count += *ii = --b;
                    subCount += *(i + Z + Z) + *(i + Z + X) + *(i + Z + Y) + *(i + Z - X) + *(i + Z - Y);
                    neighbors++;
                }
                count += ((subCount >> 8) + (neighbors * (neighbors + lengthPlus) >> 1));
            }
            while (stackTop != refStack) {
                byte *i = *--stackTop, *ii;
                byte b;
                b = *(ii = i - X); b |= b >> 4; *ii = b;
                b = *(ii = i - Y); b |= b >> 4; *ii = b;
                b = *(ii = i - Z); b |= b >> 4; *ii = b;
                b = *(ii = i + X); b |= b >> 4; *ii = b;
                b = *(ii = i + Y); b |= b >> 4; *ii = b;
                b = *(ii = i + Z); b |= b >> 4; *ii = b;
            }
        } else if (depth != filterDepth || stackPtr - refStack == filter) {
            count += countExtensions(filter, depth - 1, refStack, stackTopInner, stackLimit);
        }

        --*(index - X);
        --*(index - Y);
        --*(index - Z);
        --*(index + X);
        --*(index + Y);
        --*(index + Z);

        *--stackLimit = index;
    }

    while (stackPtr != stackTopOriginal)
        *stackPtr++ = *stackLimit++;
    return count;
}

Num countExtensionsSubset(const int filter) {
    byte byteBoard[(N + 2) * Z];
    memset(&byteBoard[Z + 1], 255, sizeof(byteBoard) - Z - 1);
    byte *refStack[(N - 2) * 4];
    *refStack = &byteBoard[Z];
    return countExtensions(filter, N, refStack, &refStack[1], &refStack[sizeof(refStack) / sizeof(*refStack)]);
}

int main(int argc, char **argv) {
    if (argc == 2) {
        const int filter = atoi(argv[1]);
        if (filter < 0 || filter > maxLeftStackLen) {
            printf("Filter must be between 0 and %i\n", maxLeftStackLen);
            return 1;
        }
        const Num count = countExtensionsSubset(filter);
        printf(PrtNum "\n", count);
        return 0;
    }
    printf("sdpolycube trivial symmetries calculator for N=%i\n", N);
    printf("Usage: sdpolycube <filter> - where filter is 0 to %i\n", maxLeftStackLen);
    return 1;
}
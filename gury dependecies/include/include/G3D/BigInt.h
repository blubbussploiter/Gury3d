/**
 @file BigInt.h
 
 
 @maintainer Morgan McGuire, matrix@graphics3d.com
 
 @created 2006-04-06
 @edited  2006-04-06

 Copyright 2000-2006, Morgan McGuire.
 All rights reserved.
 */

#include "G3D/platform.h"
#include "G3D/g3dmath.h"

namespace G3D {

/** 
 Representation of arbitrary length integers.
 The implementation is not optimized for performance.
 */
class BigInt {
private:

    // Can't use Array in here or we would not be able to 
    /** Big endian, unsigned. */
    uint8*      byte;
    int         numBytes;

    /** 1 or -1 */
    int         sign;

    void setByteSize(int n);

public:

    /** Zero */
    BigInt();
    BigInt(int32 x);
    BigInt(int64 x);
    BigInt(const BigInt&);
    explicit BigInt(class BinaryInput&);

    /** If the string begins with "0x", "+0x", or "-0x" it is parsed as hex, 
        otherwise it is parsed as decimal.*/
    BigInt(const std::string& s);
    BigInt(const char* s);

    static BigInt random(const BigInt& low, const BigInt& high);

    /** Generates a random number between 0 and (2^numBits)-1. */
    static BigInt randomNumBits(int numBits);

    /** Returns byte i, starting from the low-order end. 
      */
    uint8 operator[](int i) const;
    BigInt& operator=(const BigInt&);
    BigInt operator-() const;
    BigInt operator+(const BigInt& x) const;
    BigInt operator-(const BigInt& x) const;
    BigInt operator&(const BigInt& x) const;
    BigInt operator|(const BigInt& x) const;
    BigInt operator^(const BigInt& x) const;
    bool operator>(const BigInt& x) const;
    bool operator<(const BigInt& x) const;
    bool operator>=(const BigInt& x) const;
    bool operator<=(const BigInt& x) const;
    bool operator==(const BigInt& x) const;
    bool operator!=(const BigInt& x) const;
    BigInt operator%(const BigInt& x) const;

    void serialize(class BinaryOutput&) const;
    void deserialize(class BinaryInput&);

    /** Decimal representation */
    std::string toString() const;

    std::string toHexString() const;

};

} // namespace G3D

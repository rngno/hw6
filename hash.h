#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <cmath>
#include <random>
#include <chrono>

typedef std::size_t HASH_INDEX_T;

struct MyStringHash {
    HASH_INDEX_T rValues[5] { 983132572, 1468777056, 552714139, 984953261, 261934300 };
    MyStringHash(bool debug = true)
    {
        if(false == debug){
            generateRValues();
        }
    }
    // hash function entry point (i.e. this is h(k))
    HASH_INDEX_T operator()(const std::string& k) const
    {
        // Add your code here
        unsigned long long w[5] = {0, 0, 0, 0, 0};
        
        // gonna have to process backward so we can handle words that are too long
        int strLen = k.length();
        int wIndex = 4; // Start filling w[4] first
        
        for (int i = strLen; i > 0 && wIndex >= 0; i -= 6) {
            int start = i - 6;
            if (start < 0) {
                start = 0; // for the first part if its less than 6 chars
            }
            
            // now we go to the base 36 conversion 
            // god's strongest soldier for not whining about nesting rn
            unsigned long long chunkValue = 0;
            unsigned long long pow = 1; // need this to keep track of how many exponents we did so far
            // this is that base conversion thing so i dont spam pow()
            for (int j = i-1; j >= start; --j) {
                unsigned long long charValue = letterDigitToNumber(k[j]);
                chunkValue += charValue * pow;
                pow *= 36;
            }
            
            w[wIndex] = chunkValue;
            wIndex--;
        }

        // r[i] * w[i] from h(k) formula in the guide part, idk im so confused here
        unsigned long long hashSum = 0;
        for (int i = 0; i < 5; ++i) {
            hashSum += (rValues[i] * w[i]);
        }

        return hashSum;

    }

    // A likely helper function is to convert a-z,0-9 to an integral value 0-35
    HASH_INDEX_T letterDigitToNumber(char letter) const
    {
        // Add code here or delete this helper function if you do not want it

        // need to convert upper to lower here
        if (letter >= 'A' && letter <= 'Z') {
            letter = letter + 32; 
        }

        // convert digits to our designated range
        if (letter >= '0' && letter <= '9') {
            return letter - '0' + 26;
        }
        
        // now we just handle lowers, supa ez
        return letter - 'a';

    }

    // Code to generate the random R values
    void generateRValues()
    {
        // obtain a seed from the system clock:
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 generator (seed);  // mt19937 is a standard random number generator

        // Simply call generator() [it has an operator()] to get another random number
        for(int i{ 0 }; i < 5; ++i)
        {
            rValues[i] = generator();
        }
    }
};

#endif

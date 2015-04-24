/*
 * base64_unittest.cxx
 *
 *  Created on: 2015.3.4
 *      Author: Fifi Lyu
 */

#include <gtest/gtest.h>
#include "../src/base64.h"

// Hello, World! => SGVsbG8sIFdvcmxkIQ==
const string G_PLAINTEXT("Hello, World!");
const string G_CIPER("SGVsbG8sIFdvcmxkIQ==");

TEST(Base64Test, base64_encode) {
    EXPECT_STREQ(G_CIPER.c_str(), base64_encode(G_PLAINTEXT).c_str());
}

TEST(Base64Test, base64_decode) {
    EXPECT_STREQ(G_PLAINTEXT.c_str(), base64_decode(G_CIPER).c_str());
}

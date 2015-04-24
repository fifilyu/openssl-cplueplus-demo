/*
 * base64.h
 *
 *  Created on: 2015.4.24
 *      Author: Fifi Lyu
 */

#ifndef SRC_BASE64_H_
#define SRC_BASE64_H_

#include <string>

using std::string;

string base64_encode(const string &plaintext);

string base64_decode(const string &cipher);

#endif  // SRC_BASE64_H_

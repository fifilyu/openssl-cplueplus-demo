/*
 * base64.h
 *
 *  Created on: 2015.4.24
 *      Author: Fifi Lyu
 */

#ifndef SRC_BASE64_H_
#define SRC_BASE64_H_

#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>
#include <string>

using std::string;

/*
    计算密文对应明文的长度


    Base64：http://zh.wikipedia.org/zh/Base64
    将 3 byte 的数据，放入一个24bit的缓冲区中。
    数据不足 3 byte 的话，缓冲区剩下的bit用0补足(Base64中的等号)。
    然后，每次取出 6 个 bit。不断进行，直到全部输入数据转换完成。
    //
    补0说明：
    如果要编码的字节数不能被3整除，最后会多出1个或2个字节， 那么可以使用下面的方法进行处理：
    先使用0字节值在末尾补足，使其能够被3整除，然后再进行base64的编码。
    在编码后的base64文本后加上一个或两个'='号，代表补足的字节数。也就是说，
    如果最后剩余一个八位字节（1个byte）时，最后一个6位的base64字节块有四位是0值，最后附加上2个等号；
    如果最后剩余两个八位字节（2个byte）时，最后一个6位的base字节块有两位是0值，最后附加1个等号
    //
    也就是
        1 byte = 8 bit
        1 base64 = 6 bit(可能含末尾补足的0)
    //
    明文长度 = (密文长度 * 6) / 8 - 等号数量
    明文长度 = (密文长度 * 3) / 4 - 等号数量
*/
size_t plaintext_size(const string &cipher);

string base64_encode(const string &plaintext);

string base64_decode(const string &cipher);
#endif  // SRC_BASE64_H_

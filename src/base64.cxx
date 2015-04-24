/*
 * base64.cxx
 *
 *  Created on: 2015.4.24
 *      Author: Fifi Lyu
 */

#include "base64.h"
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>

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
size_t plaintext_size(const string &cipher) {
    const size_t size_ = cipher.size();
    size_t padding_ = 0;

    if (cipher[size_ - 1] == '=' && cipher[size_ - 2] == '=')
        padding_ = 2;
    else if (cipher[size_ - 1] == '=')
        padding_ = 1;
    else
        padding_ = 0;

    return (size_ * 3) / 4 - padding_;
}

string base64_encode(const string &plaintext) {
    BIO *bio_ = NULL;
    BIO *b64_ = NULL;
    BUF_MEM *bufferPtr_ = NULL;

    b64_ = BIO_new(BIO_f_base64());
    bio_ = BIO_new(BIO_s_mem());
    bio_ = BIO_push(b64_, bio_);

    //Ignore newlines - write everything in one line
    BIO_set_flags(bio_, BIO_FLAGS_BASE64_NO_NL);
    BIO_write(bio_, plaintext.c_str(), plaintext.size());
    BIO_flush(bio_);
    BIO_get_mem_ptr(bio_, &bufferPtr_);
    BIO_set_close(bio_, BIO_NOCLOSE);
    BIO_free_all(bio_);

    return string(bufferPtr_->data, bufferPtr_->length);
}

string base64_decode(const string &cipher) {
    BIO *bio_ = NULL;
    BIO *b64_ = NULL;

    const int p_size_ = plaintext_size(cipher.c_str());
    char buffer[p_size_ + 1];

    bio_ = BIO_new_mem_buf((void*)cipher.c_str(), -1);
    b64_ = BIO_new(BIO_f_base64());
    bio_ = BIO_push(b64_, bio_);

    //Do not use newlines to flush buffer
    BIO_set_flags(bio_, BIO_FLAGS_BASE64_NO_NL);
    const size_t read_size_ = BIO_read(bio_, buffer, p_size_);
    BIO_free_all(bio_);

    if (read_size_ == p_size_) {
        buffer[p_size_] = '\0';
        return string(buffer);
    }

    return "";
}

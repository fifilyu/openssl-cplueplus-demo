/*
 * base64.cxx
 *
 *  Created on: 2015.4.24
 *      Author: Fifi Lyu
 */

#include "base64.h"

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

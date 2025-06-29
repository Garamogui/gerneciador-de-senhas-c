#ifndef CRYPTOGRAPHY_H
#define CRYPTOGRAPHY_H

void encrypt_file(const char *inName, const char *outName, int key);
void decrypt_file(const char *inName, const char *outName, int key);

#endif

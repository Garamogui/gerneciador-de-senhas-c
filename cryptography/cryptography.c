#include <stdio.h>
#include "cryptography.h"

void encrypt_file(const char *inName, const char *outName, int key) {
    FILE *fin = fopen(inName, "r");
    if (!fin) return;
    FILE *fout = fopen(outName, "w");
    if (!fout) { fclose(fin); return; }
    int ch;
    while ((ch = fgetc(fin)) != EOF) {
        fputc((ch + key) & 0xFF, fout);
    }
    fclose(fin);
    fclose(fout);
}

void decrypt_file(const char *inName, const char *outName, int key) {
    FILE *fin = fopen(inName, "r");
    if (!fin) return;
    FILE *fout = fopen(outName, "w");
    if (!fout) { fclose(fin); return; }
    int ch;
    while ((ch = fgetc(fin)) != EOF) {
        fputc((ch - key) & 0xFF, fout);
    }
    fclose(fin);
    fclose(fout);
}

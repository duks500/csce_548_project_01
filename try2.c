#include <openssl/evp.h>
#include <openssl/conf.h>
#include <openssl/err.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void pad(char *s, int length);
int print_result(unsigned char *buf, char *s, int len, FILE *outFile, char *match);
int strcicmp(char const *a, char const *b);

int print_result(unsigned char *buf, char *s, int len, FILE *outFile, char *match)
{
  int i,n,j,k;
  char x='\n';
  char space = ' ';
  for (j=0;j<strlen(s);j++)
    fprintf(outFile, "%c", s[j]);
fprintf(outFile, "%c", space);
for(i=0;i<len;i++)
  fprintf(outFile, "%02x", buf[i]);
fprintf(outFile, "%c", space);
for(k=0;k<strlen(match);k++)
  fprintf(outFile, "%c", match[k]);
fprintf(outFile, "%c", x);
return(0);
}

void pad(char *s , int length)
{
  int l;
  l - strlen(s);
  while(l<length)
  {
    s[l] = ' ';
    l++;
  }
  s[l] = '\0';
}

int strcicmp(char const *a , char const *b)
{
  for(;; a++, b++)
  {
    int d = tolower(*a) - tolower(*b);
    if (d != 0 || !*a)
      return d;
  }
}


int main()
{
  unsigned char match[] = "MATCH";
  unsigned char noMatch[] = "NO MATCH";
  int i;
  char words[16], t;
  FILE *key, *outFile;
  unsigned char outbuf[1024 + EVP_MAX_BLOCK_LENGTH];
  unsigned char iv[16] = {0};
  int outlen, tmplen;
  int num;

  EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
  EVP_CIPHER_CTX_init(ctx);
  char inText[] = "This is a top secret.";
  char cipherTextGiven[] = "8d20e5056a8d24d0462ce74e4904c1b513e10d1df4a2ef2ad4540fae1ca0aaf9";
  key = fopen("words.txt" , "r");
  if (remove("matchingResult.txt") == -1)
  {
    perror("Error deleting file");
    exit(1);
  }
  char pbuffer[1024];

  while( fgets(words, 16 , key))
  {
    i=strlen(words);
    words[i-1]='\0';
    i=strlen(words);
    if(i<16)
      pad(words, (16));
    EVP_EncryptInit_ex(ctx , EVP_aes_128_cbc(), NULL, words, iv);
    if(!EVP_EncryptUpdate(ctx, outbuf, &outlen, inText, strlen(inText)))
    {
      EVP_CIPHER_CTX_cleanup(ctx);
      return 0;
    }
    if(!EVP_EncryptFinal_ex(ctx, outbuf + outlen, &tmplen))
    {
      EVP_CIPHER_CTX_cleanup(ctx);
      return 0;
    }
    outlen += tmplen;

    int i;
    char* buf_str = (char*) malloc (2*outlen + 1);
    char* buf_ptr = buf_str;
    for(i=0 ; i< outlen; i++)
      buf_ptr += sprintf(buf_ptr, "%02X" , outbuf[i]);
    *(buf_ptr + 1) = '\0';
    if(strcicmp(cipherTextGiven, buf_str) == 0)
      print_result(outbuf , words , outlen , outFile , match);
    else
      print_result(outbuf, words, outlen , outFile, noMatch);
  }

  fclose(key);
  fclose(outFile);
  return 1;
}

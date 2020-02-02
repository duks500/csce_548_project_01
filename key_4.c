#include <openssl/evp.h>
#include <openssl/conf.h>
#include <openssl/err.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void pad(char *s, int length);
int print_result(unsigned char *buf, char *s, int len, FILE *outFile, char *match);
int strcicmp(char const *a, char const *b);

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


int main (){

//text
char text[] = "This is a top secret.";
//cipher
char cipher_text[] = "8d20e5056a8d24d0462ce74e4904c1b513e10d1df4a2ef2ad4540fae1ca0aaf9";
FILE *key_open, *file_out;
//each word in dic is 16
char words[16];
int text_str;
char iv[16] = {0}; //all zero

char outbuf[1024 + EVP_MAX_BLOCK_LENGTH];
int outlen;
//oprn the word.txt file and read, open final_res
key_open = fopen("words.txt" , "r");
file_out = fopen("final_res.txt", "a+");

//check for errors
if( file_out < 0 || key_open < 0)
{
  cout << "error with opning the file";
  exit(1);
}
EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
EVP_CIPHER_CTX_init(ctx);

//get each word from dic
while( fgets(words, 16 , key_open))
{
  text_str = strlen(words); // before rmeoving
  words[text_str-1] = '\0'; //remove the end of the file = null
  text_str = strlen(words); // after removing

  if ( text_str < 16) // we have less that 16, we pad the end of the word
    pad(words , (16));

}
EVP_EncryptInit_ex(&ctx , EVP_aes_128_cbc() , NULL , words , iv);
if(!EVP_EncryptUpdate(ctx, outbuf, &outlen, text , strlen(text)))
{
  EVP_CIPHER_CTX_cleanup(ctx);
  return 0;
}

return 0;
}

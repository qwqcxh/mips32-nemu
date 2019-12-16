#include "klib.h"

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  size_t ans=0;
  for(const char* p=s;*p;p++) ans++;
  return ans;
}

char *strcpy(char* dst,const char* src) {
  if(src==NULL) return dst=NULL;
  for(size_t i=0;;i++){
    dst[i]=src[i];
    if(dst[i]=='\0') break;
  }
  return dst;
}

char* strncpy(char* dst, const char* src, size_t n) {
  if(src==NULL) return dst=NULL;
  for(size_t i=0;i<n;i++){
    dst[i]=src[i];
    if(dst[i]=='\0') break;
  }
  return dst;
}

char* strcat(char* dst, const char* src) {
  size_t dstlen=strlen(dst);
  for(size_t i=0;;i++){
    dst[dstlen+i]=src[i];
    if(src[i]=='\0') break;
  }
  return dst;
}

int strcmp(const char* s1, const char* s2) {
  while(*s1&&*s2){
    if(*s1>*s2) return 1;
    else if(*s1<*s2) return -1;
    s1++;
    s2++;
  }
  if(*s1) return 1;
  else if(*s2) return -1;
  else return 0; 
}

int strncmp(const char* s1, const char* s2, size_t n) {
  while(*s1&&*s2&&n){
    if(*s1>*s2) return 1;
    else if(*s1<*s2) return -1;
    s1++;
    s2++;
    n--;
  }
  if(n==0) return 0;
  else if(*s1) return 1;
  else if(*s2) return -1;
  else return 0;
}

void* memset(void* v,int c,size_t n) {
  for(size_t i=0;i<n;i++)
    ((char*)v)[i]=c;
  return v;
}

/* Nonzero if either X or Y is not aligned on a "long" boundary.  */
#define UNALIGNED(X, Y) \
  (((long)X & (sizeof (long) - 1)) | ((long)Y & (sizeof (long) - 1)))

/* How many bytes are copied each iteration of the 4X unrolled loop.  */
#define BIGBLOCKSIZE    (sizeof (long) << 2)

/* How many bytes are copied each iteration of the word copy loop.  */
#define LITTLEBLOCKSIZE (sizeof (long))

/* Threshhold for punting to the byte copier.  */
#define TOO_SMALL(LEN)  ((LEN) < BIGBLOCKSIZE)
void* memcpy(void* out, const void* in, size_t n) {
  char *dst = (char*)out;
  const char *src = (char*)in;
  long *aligned_dst;
  const long *aligned_src;

  /* If the size is small, or either SRC or DST is unaligned,
     then punt into the byte copy loop.  This should be rare.  */
  if (!TOO_SMALL(n) && !UNALIGNED (src, dst))
    {
      aligned_dst = (long*)dst;
      aligned_src = (long*)src;

      /* Copy 4X long words at a time if possible.  */
      while (n >= BIGBLOCKSIZE)
        {
          *aligned_dst++ = *aligned_src++;
          *aligned_dst++ = *aligned_src++;
          *aligned_dst++ = *aligned_src++;
          *aligned_dst++ = *aligned_src++;
          n -= BIGBLOCKSIZE;
        }

      /* Copy one long word at a time if possible.  */
      while (n >= LITTLEBLOCKSIZE)
        {
          *aligned_dst++ = *aligned_src++;
          n -= LITTLEBLOCKSIZE;
        }

       /* Pick up any residual with a byte copier.  */
      dst = (char*)aligned_dst;
      src = (char*)aligned_src;
    }

  while (n--)
    *dst++ = *src++;

  return dst;
  // for(size_t i=0;i<n;i++)
  //   ((char*)out)[i]=((char*)in)[i];
  // for(size_t i=0;i<n;i++)
  //   if(i%4==0){
  //     printf("i %d memcpy debug: %x %x\n",i,((uint32_t*)in)[i/4],((uint32_t*)out)[i/4]);
  //   }
  // return out;
}

int memcmp(const void* s1, const void* s2, size_t n){
  char* p=(char*)s1;
  char* q=(char*)s2;
  for(size_t i=0;i<n;i++){
    if(p[i]>q[i]) return 1;
    else if(p[i]<q[i]) return -1;
  }
  return 0;
}

#endif

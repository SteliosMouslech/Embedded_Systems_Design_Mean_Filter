#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>


#define N 330    //number of rows
#define M 440    //number of columns
#define RADIUS 2 //so kernel is 5x5
#define file_in "dog_440x330.yuv"
#define file_out "outDogCollapse.y"


#pragma arm section zidata="ram"
int image_Y[N][M]; // Original image only Y(luminance)
int newImg[N][M];  //final image after filter, only Y(luminance)

#pragma arm section

//read image file
void read() {
  int i;
  int *p=&image_Y[0][0];
  FILE *frame_c;
  if((frame_c=fopen(file_in,"rb"))==NULL) {
    printf("current frame doesn't exist\n");
    exit(-1);
  }
  for(i=0;i<N*M;i++) {
    *p++=fgetc(frame_c);
  }
  fclose(frame_c);
}

//write Y image to file
void write() {
  int i;
  int *p=&newImg[0][0];
  FILE *frame_yuv;
  frame_yuv=fopen(file_out,"wb");
  for(i=0;i<N*M;i++) {
    fputc(*p++,frame_yuv);
  }
  fclose(frame_yuv);
}

//min and max functions to use in meanFilter
int min(int x, int y) {
  if(x<y) {
    return x;
  }
  else {
    return y;
  }
}

int max(int x, int y) {
  if(x>y) {
    return x;
  }
  else {
    return y;
  }
}

//only for positive values
int myround(double x)
{
    return (int)(x + 0.5);
}

//mean filter implementation
void meanFilter(int windowRadius) {
  int x,y,i,j,xx,yy,k;
  int imgWidth=M;
  int imgHeight=N;
  int *p=&newImg[0][0];

  for (k=0; k<imgWidth*imgHeight; k++) {
    *p=0;
	x=k%M;
	y=k/M;
    for (i=-windowRadius; i<=windowRadius; i++) {
      for (j=-windowRadius; j<=windowRadius; j++) {
        xx=min(max(x+i,0),imgWidth-1);
        yy=min(max(y+j,0),imgHeight-1);
        *p=*p + image_Y[yy][xx];
      }
    }
    *p=myround((double)*p / pow((double)(2*windowRadius+1),2));
	p++;
  }
}


int main() {
  read();
  meanFilter(RADIUS);
  write();
  
  return 0;
}




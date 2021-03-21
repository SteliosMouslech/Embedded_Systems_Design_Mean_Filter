#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>


#define N 330    //number of rows
#define M 440    //number of columns
#define RADIUS 2 //so kernel is 3x3
#define file_in "dog_440x330.yuv"
#define file_out "outDogIntegral.y"


#pragma arm section zidata="ram"
int image_Y[N+2*RADIUS][M+2*RADIUS]; // Original image only Y(luminance)
int newImg[N][M];  //final image after filter, only Y(luminance)

#pragma arm section

//read image file
void read() {
  int i,j;
  FILE *frame_c;
  if((frame_c=fopen(file_in,"rb"))==NULL) {
    printf("current frame doesn't exist\n");
    exit(-1);
  }
  for(i=RADIUS;i<N+RADIUS;i++) {
    for(j=RADIUS;j<M+RADIUS;j++) {
      image_Y[i][j]=fgetc(frame_c);
    }
  }
  fclose(frame_c);
}

//write Y image to file
void write() {
  int i,j;
  FILE *frame_yuv;
  frame_yuv=fopen(file_out,"wb");
  for(i=0;i<N;i++) {
    for(j=0;j<M;j++) {
      fputc(newImg[i][j],frame_yuv);
    }
  }
  fclose(frame_yuv);
}

//only for positive values
int myround(double x)
{
    return (int)(x + 0.5);
}

//mean filter implementation
void meanFilter(int windowRadius, int integralImg[N+(2*RADIUS)+1][M+(2*RADIUS)+1]) {
  int x,y,xx,yy,sum;
  int imgWidth=M;
  int imgHeight=N;
  int tli, tlj, rbi, rbj;

  for (x=windowRadius; x<imgWidth+windowRadius; x++) {
    for (y=windowRadius; y<imgHeight+windowRadius; y++) {
      sum=0;
      xx=x+1;
      yy=y+1;
      tli=yy-windowRadius;
	  tlj=xx-windowRadius;
      rbi=yy+windowRadius;
	  rbj=xx+windowRadius;
      sum=integralImg[rbi][rbj] - integralImg[tli-1][rbj] -integralImg[rbi][tlj-1]+integralImg[tli-1][tlj-1];

      newImg[y-windowRadius][x-windowRadius]=myround((double)sum / pow((double)(2*windowRadius+1),2));
      
    }
  }
}


int main() {
  int i,j;
  int Mp=M+2*RADIUS;
  int Np=N+2*RADIUS;
  int integralImg[N+(2*RADIUS)+1][M+(2*RADIUS)+1];
  
  for(i=0; i<Np; i++){
    integralImg[i][0]=0;
  }
  for(i=1; i<Mp; i++) {
    integralImg[0][i]=0;
  }
  read();


  //Get the values for corner cases of augmented matrix
  for (i=0; i<RADIUS; i++) {
    for (j=0; j<RADIUS; j++){

      //upper left and right corner values
      image_Y[i][j] = image_Y[RADIUS][RADIUS];
      image_Y[i][RADIUS+M+j] = image_Y[RADIUS][RADIUS+M-1];

      //lower left and right corner values
      image_Y[RADIUS+N+i][j] = image_Y[RADIUS+N-1][RADIUS];
      image_Y[RADIUS+N+i][RADIUS+M+j] = image_Y[RADIUS+N-1][RADIUS+M-1];
    }
  }

  //get the values for upper and lower rows of augmented matrix
  for (i=0; i<RADIUS; i++) {
    for (j=RADIUS; j<RADIUS+M; j++){
      image_Y[i][j]=image_Y[RADIUS][j];
      image_Y[RADIUS+N+i][j]=image_Y[RADIUS+N-1][j];
    }
  }
  //get the values for outer left and outer right columns of augmented matrix
  for (i=RADIUS; i<RADIUS+N; i++) {
    for (j=0; j<RADIUS; j++){
      image_Y[i][j]=image_Y[i][RADIUS];
      image_Y[i][RADIUS+M+j]=image_Y[i][RADIUS+M-1];
    }
  }

//Pre Processing
  for (i=0; i<Mp;i++)
  {
    integralImg[1][i+1] = image_Y[0][i];
  }

  for (i=1; i<Np; i++)
  {
      for (j=0; j<Mp; j++)
      {
         integralImg[i+1][j+1] = image_Y[i][j] + integralImg[i-1+1][j+1];
      }
  }

  for (i=0; i<Np; i++)
  {
      for (j=1; j<Mp; j++)
      {
          integralImg[i+1][j+1] +=integralImg[i+1][j+1-1];
      }
  }


  
  meanFilter(RADIUS,integralImg);
  write();

  return 0;
}

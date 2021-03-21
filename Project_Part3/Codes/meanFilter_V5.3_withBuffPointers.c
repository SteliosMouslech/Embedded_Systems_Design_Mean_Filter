#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define N 330    //number of rows
#define M 440    //number of columns
#define RADIUS 2 //so kernel is 5x5
#define file_in "dog_440x330.yuv"
#define file_out "outDogV1.y"
#define UNROLL 4  //if unroll factor changes loops must also be changed


#pragma arm section zidata="array"
int image_Y[N+2*RADIUS][M+2*RADIUS]; // Original image only Y(luminance)
int newImg[N][M];  //final image after filter, only Y(luminance)
int integralImg[N+(2*RADIUS)+1][M+(2*RADIUS)+1];
#pragma arm section

#pragma arm section zidata="vars"
int x,y,xx,sum,point;
int  tlj,rbj;
double pixels_inKernel;
int k;
#pragma arm section


#pragma arm section zidata="buffers"
int buffer1[M+(2*RADIUS)+1];
int buffer2[M+(2*RADIUS)+1];
int buffer3[M+(2*RADIUS)+1];
int buffer4[M+(2*RADIUS)+1];
int buffer5[M+(2*RADIUS)+1];
int buffer6[M+(2*RADIUS)+1];
#pragma arm section

#pragma arm section rwdata="othervars"
int imgWidth=M;
int imgHeight=N;
int *buff_pointer1=buffer1;
int *buff_pointer2=buffer2;
int *buff_pointer3=buffer3;
int *buff_pointer4=buffer4;
int *buff_pointer5=buffer5;
int *buff_pointer6=buffer6;
int *tempbuffer=0;
#pragma arm section


//read image file
void read() {
  int i,j;

  FILE *frame_c;
  if((frame_c=fopen(file_in,"rb"))==NULL) {
    printf("current frame doesn't exist\n");
    exit(-1);
  }
  point=M%UNROLL; //UNROLL factor is 4
  for(i=RADIUS; i<N+RADIUS; i++) {
    for(j=RADIUS; j<point+RADIUS; j++) {
      image_Y[i][j]=fgetc(frame_c);
    }
    for(j=point+RADIUS; j<M+RADIUS; j+=UNROLL) {
       image_Y[i][j]=fgetc(frame_c);
       image_Y[i][j+1]=fgetc(frame_c);
       image_Y[i][j+2]=fgetc(frame_c);
       image_Y[i][j+3]=fgetc(frame_c);
	}
  }
  fclose(frame_c);
}

//write Y image to file
void write() {
  int i,j;
  FILE *frame_yuv;
  frame_yuv=fopen(file_out,"wb");
  point=M%UNROLL; //UNROLL factor is 4
  for(i=0; i<N; i++) {
    for(j=0; j<point; j++) {
      fputc(newImg[i][j],frame_yuv);
    }
    for(j=point; j<M; j+=UNROLL) {
      fputc(newImg[i][j],frame_yuv);
      fputc(newImg[i][j+1],frame_yuv);
      fputc(newImg[i][j+2],frame_yuv);
      fputc(newImg[i][j+3],frame_yuv);

	}
  }
  fclose(frame_yuv);
}


//mean filter implementation
void meanFilter(int windowRadius) {
  //computing num of pixels in Kernel outside for (only once)

  pixels_inKernel = (double)((windowRadius<<1)+1)*((windowRadius<<1)+1);

  point=imgWidth%UNROLL;
  for (y=windowRadius; y<imgHeight+windowRadius; y++) {
    //Buffer changes
    if (y==windowRadius) {
      for (k=0;k<M+(2*RADIUS)+1;k++) {
        buffer1[k]=integralImg[0][k];
        buffer2[k]=integralImg[1][k];
        buffer3[k]=integralImg[2][k];
        buffer4[k]=integralImg[3][k];
        buffer5[k]=integralImg[4][k];
        buffer6[k]=integralImg[5][k];
      }
    }
    else {
      tempbuffer=buff_pointer1;
      buff_pointer1=buff_pointer2;
      buff_pointer2=buff_pointer3;
      buff_pointer3=buff_pointer4;
      buff_pointer4=buff_pointer5;
      buff_pointer5=buff_pointer6;
      buff_pointer6=tempbuffer;
      for (k=0;k<M+(2*RADIUS)+1;k++) {
          buff_pointer6[k]=integralImg[y+3][k];
      }
    }

    for (x=windowRadius; x<point+windowRadius; x++) {
      sum=0;
      xx=x+1;
	    tlj=xx-windowRadius;
	    rbj=xx+windowRadius;
      sum=buff_pointer6[rbj] - buff_pointer1[rbj] - buff_pointer6[tlj-1] + buff_pointer1[tlj-1];
      newImg[y-windowRadius][x-windowRadius]=(int)(((double)sum / pixels_inKernel)+0.5);
    }
    for (x=windowRadius+point; x<imgWidth+windowRadius; x++) {
      sum=0;
      xx=x+1;
	    tlj=xx-windowRadius;
	    rbj=xx+windowRadius;
      sum=buff_pointer6[rbj] - buff_pointer1[rbj] - buff_pointer6[tlj-1] + buff_pointer1[tlj-1];
      newImg[y-windowRadius][x-windowRadius]=(int)(((double)sum / pixels_inKernel)+0.5);

      x++;

      sum=0;
      xx=x+1;
	    tlj=xx-windowRadius;
	    rbj=xx+windowRadius;
      sum=buff_pointer6[rbj] - buff_pointer1[rbj] - buff_pointer6[tlj-1] + buff_pointer1[tlj-1];
      newImg[y-windowRadius][x-windowRadius]=(int)(((double)sum / pixels_inKernel)+0.5);

      x++;

      sum=0;
      xx=x+1;
	    tlj=xx-windowRadius;
	    rbj=xx+windowRadius;
      sum=buff_pointer6[rbj] - buff_pointer1[rbj] - buff_pointer6[tlj-1] + buff_pointer1[tlj-1];
      newImg[y-windowRadius][x-windowRadius]=(int)(((double)sum / pixels_inKernel)+0.5);

      x++;

      sum=0;
      xx=x+1;
	    tlj=xx-windowRadius;
	    rbj=xx+windowRadius;
      sum=buff_pointer6[rbj] - buff_pointer1[rbj] - buff_pointer6[tlj-1] + buff_pointer1[tlj-1];
      newImg[y-windowRadius][x-windowRadius]=(int)(((double)sum / pixels_inKernel)+0.5);

    }
  }
}


int main() {
  int i,j;
  int Mp=M+2*RADIUS;
  int Np=N+2*RADIUS;
  point=M%UNROLL;

  for(i=0; i<Np+1; i++){
    integralImg[i][0]=0;
  }
  for(i=1; i<Mp+1; i++) {
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
	//FUSION
	//get the values for upper and lower rows of augmented matrix
	for(j=RADIUS; j<point+RADIUS; j++) {
      image_Y[i][j]=image_Y[RADIUS][j];
      image_Y[RADIUS+N+i][j]=image_Y[RADIUS+N-1][j];
	}
    for(j=point+RADIUS; j<RADIUS+M; j++){
      image_Y[i][j]=image_Y[RADIUS][j];
      image_Y[RADIUS+N+i][j]=image_Y[RADIUS+N-1][j];

	  j++;
	  image_Y[i][j]=image_Y[RADIUS][j];
      image_Y[RADIUS+N+i][j]=image_Y[RADIUS+N-1][j];

	  j++;
	  image_Y[i][j]=image_Y[RADIUS][j];
      image_Y[RADIUS+N+i][j]=image_Y[RADIUS+N-1][j];

	  j++;
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



  meanFilter(RADIUS);
  write();

  return 0;
}

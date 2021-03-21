#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>


#define N 330    //number of rows
#define M 440    //number of columns
#define RADIUS 2 //so kernel is 5x5
#define UNROLL 4 //if unroll factor changes loops must also be changed
#define file_in "dog_440x330.yuv"
#define file_out "outDogV2.y"


#pragma arm section zidata="ram"
int image_Y[N][M]; // Original image only Y(luminance)
int newImg[N][M];  //final image after filter, only Y(luminance)

#pragma arm section

//read image file
void read() {
  int i,j;
  int point;
  
  FILE *frame_c;
  if((frame_c=fopen(file_in,"rb"))==NULL) {
    printf("current frame doesn't exist\n");
    exit(-1);
  }
  point=M%UNROLL; //UNROLL factor is 4
  for(i=0; i<N; i++) {
    for(j=0; j<point; j++) {
      image_Y[i][j]=fgetc(frame_c);
    }
    for(j=point; j<M; j+=UNROLL) {
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
  int point;
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
  int x,y,i,j,xx,yy;
  int imgWidth=M;
  int imgHeight=N;
  double pixels_inKernel;
  int sum; //local sum variable
  int max; 
  
  //num of pixels in Kernel
  pixels_inKernel = (double)((windowRadius<<1)+1)*((windowRadius<<1)+1);

  for (y=0; y<imgHeight; y++) {
    for (x=0; x<imgWidth; x++) {
      sum=0;
      for (j=-windowRadius; j<=windowRadius; j++) {
		//to fully unroll we assume to know the windowRadius beforehand!
	    //here we use windowRadius = 2
		
		i=-windowRadius;
		// inline of xx=min(max(x+i,0),imgWidth-1);
        if(x+i>0) {max=x+i;} //max
		else {max=0;}
		if(max<imgWidth-1) {xx=max;}  //min
		else {xx=imgWidth-1;}
		//inline of yy=min(max(y+j,0),imgHeight-1);
		if(y+j>0) {max=y+j;} //max
		else {max=0;}
		if(max<imgHeight-1) {yy=max;} //min
		else {yy=imgHeight-1;}
        sum=sum + image_Y[yy][xx];
		  
		i++;
		// inline of xx=min(max(x+i,0),imgWidth-1);
        if(x+i>0) {max=x+i;} //max
		else {max=0;}
		if(max<imgWidth-1) {xx=max;}  //min
		else {xx=imgWidth-1;}
		//inline of yy=min(max(y+j,0),imgHeight-1);
		if(y+j>0) {max=y+j;} //max
		else {max=0;}
		if(max<imgHeight-1) {yy=max;} //min
		else {yy=imgHeight-1;}
        sum=sum + image_Y[yy][xx];

		i++;
		// inline of xx=min(max(x+i,0),imgWidth-1);
        if(x+i>0) {max=x+i;} //max
		else {max=0;}
		if(max<imgWidth-1) {xx=max;}  //min
		else {xx=imgWidth-1;}
		//inline of yy=min(max(y+j,0),imgHeight-1);
		if(y+j>0) {max=y+j;} //max
		else {max=0;}
		if(max<imgHeight-1) {yy=max;} //min
		else {yy=imgHeight-1;}
        sum=sum + image_Y[yy][xx];
		
		i++;
		// inline of xx=min(max(x+i,0),imgWidth-1);
        if(x+i>0) {max=x+i;} //max
		else {max=0;}
		if(max<imgWidth-1) {xx=max;}  //min
		else {xx=imgWidth-1;}
		//inline of yy=min(max(y+j,0),imgHeight-1);
		if(y+j>0) {max=y+j;} //max
		else {max=0;}
		if(max<imgHeight-1) {yy=max;} //min
		else {yy=imgHeight-1;}
        sum=sum + image_Y[yy][xx];
		
		i++;
		// inline of xx=min(max(x+i,0),imgWidth-1);
        if(x+i>0) {max=x+i;} //max
		else {max=0;}
		if(max<imgWidth-1) {xx=max;}  //min
		else {xx=imgWidth-1;}
		//inline of yy=min(max(y+j,0),imgHeight-1);
		if(y+j>0) {max=y+j;} //max
		else {max=0;}
		if(max<imgHeight-1) {yy=max;} //min
		else {yy=imgHeight-1;}
        sum=sum + image_Y[yy][xx];
      }
      newImg[y][x]=(int)(((double)sum / pixels_inKernel)+0.5);//inline rounding
    }
  }
}


int main() {
  read();
  meanFilter(RADIUS);
  write();
  
  return 0;
}




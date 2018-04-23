#include "cv.h"
#include "highgui.h"
#include <conio.h>
#include <stdio.h>




IplImage* erodeimg;
IplImage* blobimg;
IplImage* colorblob;
IplImage* src;
IplImage* blobmodel;
IplImage* loadlive;
IplImage* dst;
int blobcount;
int histindex=0;
int pixelcount;
double genhist[1168];
int blue,green,red;
double CORSUM;
int totalpix;
double nonzerocountd;
int x[20000],y[20000];
int flag = 0;
static int cccc = 1;    
int boundaryflag = 0;
static int updatecounter = 0;

//int *headindex = &model[1];

struct BLOBS
{
	double blobhistogram[1168];
	int Xcor[20000];
	int Ycor[20000];
	int blobindex;

};

struct BLOBS Blob[100];



struct MODELS
{
	double modelhistogram[1168];
	double Corsum;
	int modelindex;
	int size;
};

struct MODELS Model[100];




//int modelindex1 ;
//int modelindex2;
//int headindex = 1;

int currmodelindex = 0;
int currentblobindex = 0;

int headindex = 1;
int endindex;
int minindexnumber,maxindexnumber;



void cvColorBlob(int colorindex)
{
	int red,blue,green;
	switch(colorindex)
	{
	case 1:
		{
		red = 255;
		green = 0;
		blue = 0;
		break;
		}
	case 2:
		{
		red = 0;
		green = 0;
		blue = 0;
		break;
		}
	case 3:
		{
		red = 255;
		green = 255;
		blue = 255;
		break;
		}
	case 4:
		{
		red = 255;
		green = 0;
		blue = 255;
		break;
		}
	case 5:
		{
		red = 255;
		green =255;
		blue = 0;
		break;
		}
	case 6:
		{
		red = 0;
		green = 255;
		blue = 255;
		break;
		}
	case 7:
		{
		red = 0;
		green = 255;
		blue = 0;
		break;
		}
	}
	for(int l=0;l<pixelcount;l++)
	{
		((uchar*)(blobimg->imageData + blobimg->widthStep*Blob[currentblobindex].Xcor[l]))[Blob[currentblobindex].Ycor[l]*3+2] = red;
		((uchar*)(blobimg->imageData + blobimg->widthStep*Blob[currentblobindex].Xcor[l]))[Blob[currentblobindex].Ycor[l]*3+1] = green;
		((uchar*)(blobimg->imageData + blobimg->widthStep*Blob[currentblobindex].Xcor[l]))[Blob[currentblobindex].Ycor[l]*3+0] = blue;
	}

		cvShowImage("BOX",blobimg); 


//	cvWaitKey(0);********************************************************************************************************
}



void cvNewModelAdder()
{

		endindex = endindex + 1;
		for(int j=0;j<1168;j++)
			Model[endindex].modelhistogram[j] = Blob[currentblobindex].blobhistogram[j];

		Model[endindex].Corsum = 0;
		Model[endindex].modelindex = endindex;
		Model[endindex].size = pixelcount;
////////////////////////////////////////////////////////////////////////////////		printf("\n In New Model Adder"); 
						
					//	printf("\n Printing Blob info");
					//	for(int cc=0;cc<1168;cc++)
					//		printf(" %d ",blob[currentblobindex].histogram[cc]);
					//	cvWaitKey(0); 
///////////////////////////////////////////////////////////////////////						printf("\n Printing Model info");
/////////////////////////////////////////////////////////////////////////						for(int cc=0;cc<1168;cc++)
/////////////////////////////////////////////////////////////////////////							printf(" %lf ",Model[endindex].modelhistogram[cc]);
////////////////////////////////////////////////////////////////////////				   		printf("Index number = %d ",Model[endindex].modelindex);
//////////////////////////////////////////////////////////////////////						printf(" Corsum = %lf", Model[endindex].Corsum);
//////////////////////////////////////////////////////////////////////						printf(" Size = %d", Model[endindex].size);
						cvColorBlob(endindex);

/////////////////////////////////////////////////////////////////////////////////						cvWaitKey(0);  


}


double cvMaxCorsum()
{
	double max=-10000;
	int index;
	int in = headindex;
/////////////////////////////////////////////////////////////////////////	printf("\n\nHEADMODEL CORSUM ======== %lf\n\n ", Model[in].Corsum);
//	find maximum in Corsum array and return the index of array to ModelChooser function.

   
   for(int i=1;i<=endindex;i++)
   {
 ///////////////////////////////////////////////////////////////////     printf("\nI'm in Max Corsum calculation");
	   if(max <= Model[in].Corsum)
	  {
		  max = Model[in].Corsum;	
		  index = Model[in].modelindex;
////////////////////////////////////////////////////////////////////////		  printf("  Max Corsum = %lf ",Model[in].Corsum);
	  }
/////////////////////////////////////////////////////////////////////		   printf("\n NEW VALUE...... model index number is thisssssss %d",Model[in].modelindex);
	 in++;
   }
	maxindexnumber = index;
   return (max);
}



double cvMinCorsum()
{
	double min=10000000000;
	int index;
	int in = headindex;
///////////////////////////////////////////////////////////////////////////////	printf("\n\nHEADMODEL CORSUM ======== %lf\n\n ", Model[in].Corsum);
//	find maximum in Corsum array and return the index of array to ModelChooser function.

   
   for(int i=1;i<=endindex;i++)
   {
////////////////////////////////////////////////////////////////////////      printf("\nI'm in Min Corsum calculation");
	   if(min >= Model[in].Corsum)
	  {
		  min = Model[in].Corsum;	
		  index = Model[in].modelindex;
//////////////////////////////////////////////////////////////////////////		  printf("  Min Corsum = %lf ",Model[in].Corsum);
	  }
//////////////////////////////////////////////////////////////////////////		   printf("\n NEW VALUE...... model index number is thisssssss %d",Model[in].modelindex);
	 in++;
   }
	minindexnumber = index;
   return (min);
}



void cvAssignModel()
{
	double max,min;
	max = cvMaxCorsum();
//	min = cvMinCorsum();
	int changeindex = headindex;
//	printf("\n HEADMODEL INDEXXXXXXXXX.....  %d ",a->modelindex);
	while(Model[changeindex].modelindex != maxindexnumber)
	{
//		printf("\n Model values........index......  %d ",a->modelindex);
		changeindex++;

	}
///////////////////////////////////////////////////////////////////////////////	printf("\nMODEL INDEX TO BE CHANGED= %d ", maxindexnumber);

// testing code......
//		for(int j=0;j<512;j++)
//			printf(" %d  %d ", blob->histogram[j],model->histogram[j]);

///////////////////////////////////////////////////////////////////////////////////////	printf("\nMAX ============  %lf\n",max);
	printf("\nMAX ============= %lf", max);

	if( flag == 0 || (flag == 1 && max < 6.0) )
	{
		
		printf("\nNew Model number = %d",cccc++);
		cvNewModelAdder();
	}

	else if( (flag == 1) && (boundaryflag == 1) )          
	{

//////////////////////////////////////////////////////////////////////////////////////////		printf("\n In model updation module");
		
		
		// testing **********************************************************************************
		//for(int j=0;j<1168;j++)     // model updation.........
		//{

//			Model[changeindex].modelhistogram[j] = Blob[currentblobindex].blobhistogram[j]; 
//			printf("\n blob %d  model %d ", blob->histogram[j],model->histogram[j]);
		//}
		

		Model[changeindex].size = pixelcount;
///////////////////////////////////////////////////////////////////////////		printf("\n Model values :::: Corsum %lf  Index %d  Size %d",Model[changeindex].Corsum,Model[changeindex].modelindex,Model[changeindex].size);
		cvColorBlob(changeindex);

	}
	else
		cvColorBlob(changeindex);

	int dispindex = headindex; 
////////////////////////////////////////////////////////////////////////////	printf("\n  The Model values are as follows: ");
////////////////////////////////////////////////////////////////////////////	printf("\n  Model ");
///////////////////////////////////////////////////////////////////////////	for(int ll=1;ll <= endindex;ll++)
//	{
/////////////////////////////////////////////////////////////////////////////////		printf(" \nCorsum  = %lf  Index = %d  Size = %d ",Model[dispindex].Corsum,Model[dispindex].modelindex,Model[dispindex].size);
////////////////////////////////////////////////////////////////////////////////		dispindex++;
//	}


//	printf("\n  The Blob values are as follows: ");
//	printf("\n  Blob s");


//		for(int l=1;l <= 30;l++)
//		printf(" \t Index = %d  ",blob[l].blobindex);



  
}









void cvHistogramCorrelation(int *modelnumber)
{
	printf(" \nI'm in Correlation");
	double sumA,sumB,meanA,meanB;
	sumA = sumB = meanA = meanB = 0;
	CORSUM=0;;
	double Corr[1168];
//	double NewMean = 0;
//	int maxmodel,maxblob;
//	maxblob = maxmodel = 0;
	
/*
						printf("\n Printing Blob info\n");
						for(int cc=0;cc<1168;cc++)
							printf(" %d ",blob[currentblobindex].histogram[cc]);
						cvWaitKey(0); 
						printf("\n Printing Model info\n");
						for(cc=0;cc<1168;cc++)
							printf(" %d ",Model[*modelnumber].histogram[cc]);
						cvWaitKey(0);
						
//	cvAbsDiff(Model[*modelnumber].histogram, blob[currentblobindex].histogram, Corr);
	


	for(int i=0;i<1168;i++)
	{
		Corr[i] =  (Model[*modelnumber].histogram[i] - blob[currentblobindex].histogram[i]) * exp(2);
		CORSUM = CORSUM + Corr[i];
//		printf("\n BLOB VALUE = %lf   MODEL VALUE = %lf   CORR VALUE = %lf  CORSUM = %lf ",blob[currentblobindex].histogram[i], Model[*modelnumber].histogram[i],Corr[i],CORSUM);
	}
*/
//	int nonzero = cvCountNonZero(Corr);
//	double NewMean = CORSUM / nonzero;


///////////////// old method //////////////////////////////////////////////
	for(int i=0;i<1168;i++)
	{
		sumA = sumA + Blob[currentblobindex].blobhistogram[i];
//		printf("hiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii");
		sumB = sumB + Model[*modelnumber].modelhistogram[i];
		//**************************************************************************
		//**************************************************************************
//		printf(" %d**%d ",Blob[currentblobindex].blobhistogram[i],Model[*modelnumber].modelhistogram[i]);
	}
	meanA = sumA / nonzerocountd;
	meanB = sumB / nonzerocountd;
    
	printf("SumA ==== %lf MeanA ==== %lf NonZeroCount ==== %lf",sumA,meanA,nonzerocountd);
	 




	for(int j=0;j<1168;j++)
	{
//		printf(" %d %d ",tempblob->histogram[j],tempmodel->histogram[j]);
		CORSUM = CORSUM +  (Blob[currentblobindex].blobhistogram[j] - meanA)  * (Model[*modelnumber].modelhistogram[j] - meanB) ;
//		printf(" %lf ",Corrsum);
//		Model[*modelnumber].Corsum = CORSUM;
	}
	


		/*

						printf("\n Corr info \n");
						for(int cc=0;cc<1168;cc++)
							printf(" %d ",Corr[cc]);
						cvWaitKey(0);

*/








	Model[*modelnumber].Corsum = CORSUM;
//	model = tempmodel;
//	blob = tempblob;

	printf("\n Model Corsum = %lf   Model size = %d  Model Index = %d ", Model[*modelnumber].Corsum,Model[*modelnumber].size,Model[*modelnumber].modelindex);

	printf("\nModel corsum = %lf Corrsum = %lf SumA = %lf SumB = %lf  meanA = %lf meanB = %lf  Model Pixelcount = %d",Model[*modelnumber].Corsum, CORSUM,sumA,sumB,meanA,meanB,Model[*modelnumber].size);

//	cvWaitKey(0);    ***************************************************

	
}










void cvHistogramCorrelation1(int modelnumber)
{
//////////////////////////////////////////////////////////////////////	printf(" \nI'm in Correlation");
	double sumA,sumB,meanA,meanB;
	sumA = sumB = meanA = meanB = 0;
	CORSUM=0;;
	double Corr[1168];
//	double NewMean = 0;
//	int maxmodel,maxblob;
//	maxblob = maxmodel = 0;
	
/*
						printf("\n Printing Blob info\n");
						for(int cc=0;cc<1168;cc++)
							printf(" %d ",blob[currentblobindex].histogram[cc]);
						cvWaitKey(0); 
						printf("\n Printing Model info\n");
						for(cc=0;cc<1168;cc++)
							printf(" %d ",Model[*modelnumber].histogram[cc]);
						cvWaitKey(0);
						
//	cvAbsDiff(Model[*modelnumber].histogram, blob[currentblobindex].histogram, Corr);
	


	for(int i=0;i<1168;i++)
	{
		Corr[i] =  (Model[*modelnumber].histogram[i] - blob[currentblobindex].histogram[i]) * exp(2);
		CORSUM = CORSUM + Corr[i];
//		printf("\n BLOB VALUE = %lf   MODEL VALUE = %lf   CORR VALUE = %lf  CORSUM = %lf ",blob[currentblobindex].histogram[i], Model[*modelnumber].histogram[i],Corr[i],CORSUM);
	}
*/
//	int nonzero = cvCountNonZero(Corr);
//	double NewMean = CORSUM / nonzero;


///////////////// old method //////////////////////////////////////////////
	for(int i=0;i<1168;i++)
	{
		sumA = sumA + Blob[currentblobindex].blobhistogram[i];
//		printf("hiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii");
		sumB = sumB + Model[modelnumber].modelhistogram[i];
		//**************************************************************************
		//**************************************************************************
//		printf(" Blob's histogram = %lf  Models histogram = %lf \n",Blob[currentblobindex].blobhistogram[i],Model[modelnumber].modelhistogram[i]);
	}
	meanA = sumA / nonzerocountd;
	meanB = sumB / nonzerocountd;

	 

printf("SumA ==== %lf MeanA ==== %lf NonZeroCount ==== %lf",sumA,meanA,nonzerocountd);
printf("SumB ==== %lf MeanB ==== %lf NonZeroCount ==== %lf",sumB,meanB,nonzerocountd);

	for(int j=0;j<1168;j++)
	{
//		printf(" %d %d ",tempblob->histogram[j],tempmodel->histogram[j]);
		CORSUM = CORSUM +  (Blob[currentblobindex].blobhistogram[j] - meanA)  * (Model[modelnumber].modelhistogram[j] - meanB) ;
//		printf("CORSUM == %lf \n",CORSUM);
//		Model[*modelnumber].Corsum = CORSUM;
	}
	


		/*

						printf("\n Corr info \n");
						for(int cc=0;cc<1168;cc++)
							printf(" %d ",Corr[cc]);
						cvWaitKey(0);

*/








	Model[modelnumber].Corsum = CORSUM;
//	model = tempmodel;
//	blob = tempblob;

/////////////////////////////////////////////////////////////////////////////////////////////	printf("\n Model Corsum = %lf   Model size = %d  Model Index = %d ", Model[modelnumber].Corsum,Model[modelnumber].size,Model[modelnumber].modelindex);

	printf("\nModel corsum = %lf Corrsum = %lf SumA = %lf SumB = %lf  meanA = %lf meanB = %lf  Model Pixelcount = %d",Model[modelnumber].Corsum, CORSUM,sumA,sumB,meanA,meanB,Model[modelnumber].size);

//	cvWaitKey(0);    // changed

	
}








void cvModelBuilder()
{

	if (currmodelindex == 0)
	{
		currmodelindex = 1;
		flag = 1;
		for(int j=0;j<1168;j++)
		{
			Model[currmodelindex].modelhistogram[j] = Blob[currentblobindex].blobhistogram[j];  /// needs a change.........................................
//			printf("\n blob %d  model %d ", blob->histogram[j],model->histogram[j]);
		}
		Model[currmodelindex].Corsum = 0;
		Model[currmodelindex].modelindex = currmodelindex;
		Model[currmodelindex].size = pixelcount;
	
		
//		headindex = model;
		endindex = currmodelindex;

//////////////////////////////////////////////////////////////////////////		printf("\nHI in model builder for %d pixelcount blob  ...... ModelIndex === %d",pixelcount,Model[currmodelindex].modelindex);
		cvColorBlob(currmodelindex);
//		cvWaitKey(0);   
	}
	else
	{							
		
		
		for(int j=0;j<1168;j++)
		{
			Model[currmodelindex].modelhistogram[j] = Blob[currentblobindex].blobhistogram[j];  /// needs a change.........................................
//			printf("\n blob %d  model %d ", blob->histogram[j],model->histogram[j]);
		}
		Model[currmodelindex].Corsum = 0;
		Model[currmodelindex].modelindex = currmodelindex;
		Model[currmodelindex].size = pixelcount;
		
		endindex = currmodelindex;
		
///////////////////////////////////////////////////////////////////////		printf("\nHI in model 2 builder Modelindex = %d  ", Model[currmodelindex].modelindex);
	}
}








void cvBlobBuilder(double *hist)
{

	if (currentblobindex == 0)
	{
		currentblobindex = 1;
		for(int i=0;i<pixelcount;i++)
		{
			Blob[currentblobindex].Xcor[i] = x[i];
			Blob[currentblobindex].Ycor[i] = y[i];
//			printf("\n  %d  %d  %d %d %d ",pixelcount,blob->Xcor[i], blob->Ycor[i], x[i],y[i]);
		}


//		for(i=0;i<pixelcount;i++)
//		{
//			printf("\n  %d  %d  %d ",pixelcount,temp->Xcor[i], temp->Ycor[i]);
//		}


		for(int j=0;j<1168;j++)
		{
			Blob[currentblobindex].blobhistogram[j] = hist[j];  /// needs a change.........................................
//			printf(" %d**%.2lf**%.2lf ",j,Blob[currentblobindex].blobhistogram[j], hist[j]);
		}

		Blob[currentblobindex].blobindex = blobcount;



//			printf("\nHI in blob builder tempindex   blobcount  %d  headblob %d  ", blob->blobindex,HEADBLOB->blobindex);
//			cvWaitKey(0);
	}
	else
	{							

		for(int i=0;i<pixelcount;i++)
		{
			Blob[currentblobindex].Xcor[i] = x[i];
			Blob[currentblobindex].Ycor[i] = y[i];
//			printf("\n  %d  %d  %d %d %d ",pixelcount,blob->Xcor[i], blob->Ycor[i], x[i],y[i]);
		}


//		for(i=0;i<pixelcount;i++)
//		{
//			printf("\n  %d  %d  %d ",pixelcount,temp->Xcor[i], temp->Ycor[i]);
//		}


		for(int j=0;j<1168;j++)
		{
			Blob[currentblobindex].blobhistogram[j] = hist[j];  /// needs a change.........................................
//			printf("\n count   %d  blob value  %d  passed value   %d  ",j,blob->histogram[j], histogram[j]);
		}

		Blob[currentblobindex].blobindex = currentblobindex;
		
		
///////////////////////////////////////////////////////////		printf("\nHI in blob 2  builder   %d   ", Blob[currentblobindex].blobindex);
//		cvWaitKey(0);


	}



/*

						printf("\n Printing blob info");
//						cvWaitKey(0);  **********************************************************************
//						for(int x=0;x<1168;x++)
//							printf(" %d ",blob->histogram[x]);
				   		printf("Index number = %d ",blob->blobindex);
//						printf(" Corsum = %d", blob->Corsum);
//						blob = blob->link;
//						cvWaitKey(0);    ********************************************************************
//					}
//
*/



}





int cvGetIntensity(IplImage* filename,int x, int y)
{
	return ( ((uchar*)(filename->imageData + filename->widthStep*x)) [y] );
	
}



int cvGetIntensity(IplImage* filename,int x, int y,int color)
{
	return ( ((uchar*)(filename->imageData + filename->widthStep*x)) [(y*3)+color] );
	
}








void cvComputeHistogram()
{
	nonzerocountd = 0.0;
	double maximum = 0;
	for(int i =0; i< pixelcount;i++)
	{

		blue = ((uchar*)(loadlive->imageData + loadlive->widthStep*x[i]))[y[i]*3+0];
		green = ((uchar*)(loadlive->imageData + loadlive->widthStep*x[i]))[y[i]*3+1];
		red = ((uchar*)(loadlive->imageData + loadlive->widthStep*x[i]))[y[i]*3+2];

		histindex = (int)(red/16) + 8*((int)(green/16)) + 64*((int)(blue/16));

        genhist[histindex] = genhist[histindex] + 1;
//		printf(" %d*%d*%d*%d*%.2lf \n", blue, green, red,histindex,genhist[histindex]);

    }
//	cvWaitKey(0);

	for(i=0;i<1168;i++)
	{
		if(genhist[i] != 0)
			nonzerocountd++;
		if (maximum < genhist[i])
			maximum = genhist[i];
	}



//printf("  \nnonzerocountd = %d", nonzerocountd);

//////////////// Normalising the histogram ////////////////////////////////////////

	for(i=0;i<1168;i++)
		genhist[i] = (genhist[i] / (maximum/2));
	

	
	cvBlobBuilder(genhist);

//	blobcount++;
//	intblobcount++;

	if(currmodelindex == 0)
	{
		cvModelBuilder();
	}
	else
	{
		for(int i = 1;i<=endindex;i++)
		{
			//cvHistogramCorrelation(&i);
			cvHistogramCorrelation1(i);
		}

		cvAssignModel();

	}
	
}

















void cvSeperateBlob()
{
//	currentblobindex = 0;
	int intblobcount = 0;
	int xmin,ymin,xmax,ymax;
	xmin = ymin = 320;
	xmax = ymax = 0;
	int xc,yc;      
	xc = yc = 0;
	boundaryflag = 0;
//	blobcount++;
	int count = 0;
	pixelcount = 0;

	for(int i=0;i<240;i++)
		for(int j=0;j<320;j++)
		{
			if( (cvGetIntensity(src,i,j)== 255)) //& (cvGetIntensity(dst,i,j,1)== 255) & (cvGetIntensity(dst,i,j,2)== 255))
			{
				((uchar*)(src->imageData + src->widthStep*i))[j]=0;
//				((uchar*)(dst->imageData + dst->widthStep*i))[j*3+2]=0;				
//				((uchar*)(dst->imageData + dst->widthStep*i))[j*3+1]=0;
//				((uchar*)(dst->imageData + dst->widthStep*i))[j*3]=0;


				((uchar*)(colorblob->imageData + colorblob->widthStep*i))[j*3+2]=((uchar*)(loadlive->imageData + loadlive->widthStep*i))[j*3+2];				
				((uchar*)(colorblob->imageData + colorblob->widthStep*i))[j*3+1]=((uchar*)(loadlive->imageData + loadlive->widthStep*i))[j*3+1];
				((uchar*)(colorblob->imageData + colorblob->widthStep*i))[j*3]=((uchar*)(loadlive->imageData + loadlive->widthStep*i))[j*3];
				

				pixelcount++;

				x[count] = i;
				y[count] = j;

				
				if(xmin > x[count])
					xmin = x[count];
					
				if(xmax < x[count])
					xmax = x[count];
			
				if(ymin > y[count])
					ymin = y[count];
				
				if(ymax < y[count])
					ymax = y[count];
				
				count++;

				
//				printf("\t %d %d %d",count,x[count],y[count]);
			}
			else
			{
				((uchar*)(blobimg->imageData + blobimg->widthStep*i))[j*3+2] = 0;
				((uchar*)(blobimg->imageData + blobimg->widthStep*i))[j*3+1] = 0;
				((uchar*)(blobimg->imageData + blobimg->widthStep*i))[j*3+0] = 255;



				((uchar*)(colorblob->imageData + colorblob->widthStep*i))[j*3+2] = 0;
				((uchar*)(colorblob->imageData + colorblob->widthStep*i))[j*3+1] = 0;
				((uchar*)(colorblob->imageData + colorblob->widthStep*i))[j*3+0] = 255;

			}
		}



	
	

	//		printf("\n\n%d\t%d\t%d\t%d\n\n",xmin,xmax,ymin,ymax);
// BOUNDING BOX Computation.........................................................................

//		cvRectangle( loadlive, cvPoint(ymax,xmin),cvPoint(ymin,xmax),CV_RGB(255,0,0), 1 );
//		cvLine( loadlive, cvPoint(xmin,ymin), cvPoint(xmax,ymax), CV_RGB(255,0,0), 2, 8 );

		
//		cvWaitKey(0); ****************************************************************************************
//		cvBoundingBox(xmin,ymin,xmax,ymax);
		



//	totalpix = totalpix + pixelcount;



//printf("\n  BLOB %d 's PIXELCOUNT = %d ..TOTALPIXCOUNT = %d.. COUNT = %d ........ C = %d",blobcount,pixelcount,totalpix,count,cc);

//	printf("%d ", pixelcount);
	if(pixelcount > 2000) 
	{
		
		
		
		cvShowImage("colorblob",colorblob);
/////////////////////////////////////////////////////////////////////////////////////////////		cvSaveImage("e:\\colorblob.jpg",colorblob);
			
//			((uchar*)(blob->imageData + blob->widthStep*xcor[l]))[ycor[l]*3+2]=((uchar*)(loadlive->imageData + loadlive->widthStep*xcor[l]))[ycor[l]*3+2];				
//			((uchar*)(blob->imageData + blob->widthStep*xcor[l]))[ycor[l]*3+1]=((uchar*)(loadlive->imageData + loadlive->widthStep*xcor[l]))[ycor[l]*3+1];
//			((uchar*)(blob->imageData + blob->widthStep*xcor[l]))[ycor[l]*3]=((uchar*)(loadlive->imageData + loadlive->widthStep*xcor[l]))[ycor[l]*3];
				

//			cvWaitKey(0);

		
		
		
		//////////// centroid computation ///////////////////////////
		
		for(int l=0;l<count;l++)
		{
			xc = xc + x[l];
			yc = yc + y[l];
//			((uchar*)(blobimg->imageData + blobimg->widthStep*x[l]))[y[l]*3+2]=((uchar*)(loadlive->imageData + loadlive->widthStep*x[l]))[y[l]*3+2];				
//			((uchar*)(blobimg->imageData + blobimg->widthStep*x[l]))[y[l]*3+1]=((uchar*)(loadlive->imageData + loadlive->widthStep*x[l]))[y[l]*3+1];
//			((uchar*)(blobimg->imageData + blobimg->widthStep*x[l]))[y[l]*3]=((uchar*)(loadlive->imageData + loadlive->widthStep*x[l]))[y[l]*3];
				
		}

		xc = xc / count;
		yc = yc / count;




		////////////// centroid plot////////////////////////////////

		((uchar*)(loadlive->imageData + loadlive->widthStep*xc))[yc*3+2] = 0;
		((uchar*)(loadlive->imageData + loadlive->widthStep*xc))[yc*3+1] = 255;
		((uchar*)(loadlive->imageData + loadlive->widthStep*xc))[yc*3] = 0;



		
		
		//////////// flag for blob consideration -----> checking if the blob is close to boundary ////////////////////////

		if( (xmin > 5) && (xmax < 235) && (ymin > 5) && (ymax < 315) )
			boundaryflag = 1;
	




		//////////// Bounding box calculation//////////////////////////////////
		cvRectangle( blobimg, cvPoint(315,5),cvPoint(5,235),CV_RGB(255,0,0), 1 );
		cvRectangle( blobimg, cvPoint(ymax,xmin),cvPoint(ymin,xmax),CV_RGB(255,0,0), 1 );
//		cvLine( blobimg, cvPoint(ymax,xmin),cvPoint(ymin,xmax),CV_RGB(255,255,0), 3, 8 );
	



		///////////////////  Box features////////////////     AREA     ///////////////////

		int side1 = abs(xmax - xmin);
		int side2 = abs(ymax - ymin);
		int area = side1 * side2;
		currentblobindex++;   //*******************************************************************



		/////////////// Head Tracker ///////////////////////////////////
		int a = 0;
		int headbase = 0;
		if(side1 > side2 ? a = side1 : a = side2)
		{
			headbase = a / 2;
			headbase = headbase / 3;
		}
		
		int headcor = xmin + headbase;

		cvLine( blobimg, cvPoint(ymin,headcor), cvPoint(ymax,headcor), CV_RGB(0,255,0), 2, 8 );

	  
///////////////////////////////////////////////////////////////////////////////////////////////////////		cvSaveImage("e:\\track_motion.jpg",blobimg);

	
		///////////////////// Histogram computation ////////////////////////
		printf("\n FLAG =============== %d",boundaryflag);
		if(boundaryflag == 1)
			cvComputeHistogram();
		
	}

}









void cvFindBlob()
{
	
	dst = cvCreateImage( cvGetSize(src), 8, 1 );

	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* contour = 0;

	CvScalar color = CV_RGB( 255,255,255 );
	CvScalar colorlow = CV_RGB(0,255,0);
	CvScalar colorhigh = CV_RGB(255,0,0);


	cvFindContours( src, storage, &contour, sizeof(CvContour), CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );
	cvZero( src );


	totalpix=0;
	for( ; contour != 0; contour = contour->h_next )
	{
		cvDrawContours( src, contour, color, colorhigh, 0, CV_FILLED, 8 );
		cvSeperateBlob();
		blobcount++;
		cvShowImage("Blob",blobimg);
///////////////////////////////////////////////////////////////////////////////////////////////		cvSaveImage("e:\\blobimage.jpg",blobimg);


		//cvWaitKey(0);		************************************************************************************

	}
}







void main()
{
	
//	modelindex1 = (int*)&model[0];
//	modelindex2 = (int*)&model[1];




	CvSize imgSize;
	IplConvKernel* B = cvCreateStructuringElementEx(3,3,2,2,CV_SHAPE_RECT,NULL);
	//CvArr* mask=0;
	int winback = cvNamedWindow("Back",CV_WINDOW_AUTOSIZE);
//	CvCapture* capture = cvCaptureFromAVI("e:/aravidh_umbrella_vct.avi");
//	printf("HI");
//	CvCapture* capture = cvCaptureFromAVI("d:/arjun.avi");
	CvCapture* capture = cvCaptureFromAVI("C:/subash/III sem/Computer vision/PROJECT/arjun.avi");
//	printf("hihihihiiih");
//	CvCapture* capture = cvCaptureFromAVI("e:/TwoLeaveShop2cor_vct.avi");
	IplImage* back=cvQueryFrame(capture);
	


//	cvNamedWindow("image",CV_WINDOW_AUTOSIZE);
	cvShowImage("Back",back);
//	printf("hIIIIIIIIIIIIIIIIIIIIIIIIIIIII");
	imgSize.width = back->width;
	imgSize.height = back->height;
	cvSaveImage("C:/subash/III sem/Computer vision/PROJECT/zzz.jpg",back);

double prop1 = cvGetCaptureProperty( capture, 3 );
printf("\n Property %lf = ", prop1);

double prop2 = cvGetCaptureProperty( capture, 4 );
printf("\n Property %lf = ", prop2);
	
	IplImage* live = cvCreateImage(imgSize,8,3 );
	IplImage* live1 = cvCreateImage(imgSize,8,3 );
	IplImage* live2 = cvCreateImage(imgSize,8,3 );
	IplImage* live3 = cvCreateImage(imgSize,8,3 );
	IplImage* live4 = cvCreateImage(imgSize,8,3 );
	IplImage* live5 = cvCreateImage(imgSize,8,3 );
	IplImage* live6 = cvCreateImage(imgSize,8,3 );


	loadlive = cvCreateImage(imgSize,8,3 );
	IplImage* loadlive1 = cvCreateImage(imgSize,8,3 );
	IplImage* loadlive2 = cvCreateImage(imgSize,8,3 );
	IplImage* loadlive3 = cvCreateImage(imgSize,8,3 );
	IplImage* loadlive4 = cvCreateImage(imgSize,8,3 );
	IplImage* loadlive5 = cvCreateImage(imgSize,8,3 );
	IplImage* loadlive6 = cvCreateImage(imgSize,8,3 );

	
	IplImage* loadback=cvLoadImage("C:/subash/III sem/Computer vision/PROJECT/zzz.jpg",3);

//	cvNamedWindow("LOADBACK",1);
//	cvShowImage("LOADBACK",loadback);
	IplImage* grayback = cvCreateImage(imgSize,8,1 ); 
	IplImage* graylive = cvCreateImage(imgSize,8,1 );
	IplImage* graysubimg = cvCreateImage(imgSize,8,1 );
	IplImage* threshimg = cvCreateImage(imgSize,8,1 );
	IplImage* erodeimg = cvCreateImage(imgSize,8,1 );
	blobimg = cvCreateImage(imgSize,8,3 );
	blobmodel = cvCreateImage(imgSize,8,3 );
	colorblob = cvCreateImage(imgSize,8,3 );
	src = cvCreateImage(imgSize,8,1 );

	cvCvtColor(loadback,grayback,CV_BGR2GRAY);
				

	cvNamedWindow("LIVE",1);
	cvNamedWindow("Blob",1);
	cvNamedWindow("BOX",1);
	cvNamedWindow("colorblob",1);

//	cvNamedWindow("Sep Blob image",CV_WINDOW_AUTOSIZE);
//	cvShowImage("Background image",back);
//	cvWaitKey(0);
//	cvNamedWindow("Gray image",CV_WINDOW_AUTOSIZE);
//	cvShowImage("Gray image",grayback);
  //	cvWaitKey(0);
	
	
	
//	cvWaitKey(0);  changed
	
/*	
	
	cvNamedWindow("Test1",1);
	cvNamedWindow("Test2",1);
	cvNamedWindow("Test3",1);
	cvNamedWindow("Test4",1);
	cvNamedWindow("Test5",1);
	cvNamedWindow("Test6",1);
	cvWaitKey(0);
	

	//	printf("HIIIIIIII");
	for(int l=1;l<150;l++)
	{
		live1=cvQueryFrame(capture);
		cvSaveImage("e:\\zz1.jpg",live1);
	}
//	printf("hello");
//	loadlive1=cvLoadImage("e:\\zz1.jpg",3);
	cvShowImage("Test1",live1);
//	cvWaitKey(0);

	for(l=1;l<250;l++)
	{
		live2=cvQueryFrame(capture);
		cvSaveImage("e:\\zz2.jpg",live2);
	}
//	printf("hello");
//	loadlive2=cvLoadImage("e:\\zz2.jpg",3);
	cvShowImage("Test2",live2);
//	cvWaitKey(0);
	
	for(l=1;l<250;l++)
	{
		live3=cvQueryFrame(capture);
		cvSaveImage("e:\\zz3.jpg",live3);
	}
//	printf("hello");
//	loadlive3=cvLoadImage("e:\\zz3.jpg",3);
	cvShowImage("Test3",live3);
//	cvWaitKey(0);
	
	for(l=1;l<250;l++)
	{
		live4=cvQueryFrame(capture);
		cvSaveImage("e:\\zz4.jpg",live4);
	}
//	printf("hello");
//	loadlive4=cvLoadImage("e:\\zz4.jpg",3);
	cvShowImage("Test4",live4);
//	cvWaitKey(0);

	for(l=1;l<250;l++)
	{
		live5=cvQueryFrame(capture);
		cvSaveImage("e:\\zz5.jpg",live5);
	}
//	loadlive5=cvLoadImage("e:\\zz5.jpg",3);
	cvShowImage("Test5",live5);
//	cvWaitKey(0);

//	for(l=1;l<400;l++)
//	{
//		live6=cvQueryFrame(capture);
//		cvSaveImage("e:\\zz6.jpg",live6);
//	}
//	loadlive6=cvLoadImage("e:\\zz6.jpg",3);
//	cvShowImage("Test6",live6);
//	cvWaitKey(0);

	for(int i=1;i<9;i++)
	{
		switch(i)
		{
		case 1:
			{
				loadlive=cvLoadImage("e:\\jayaram.jpg",3);
				break;
			}
		case 2:
			{
				loadlive=cvLoadImage("e:\\arjun.jpg",3);
				break;
			}
		case 3:
			{
				loadlive=cvLoadImage("e:\\subash.jpg",3);
				break;
			}
		case 4:
			{
				loadlive=cvLoadImage("e:\\vasu.jpg",3);
				break;
			}
		case 5:
			{
				loadlive=cvLoadImage("e:\\arjun4.jpg",3);
				break;
			}
		case 6:
			{
				loadlive=cvLoadImage("e:\\arjun7.jpg",3);
				break;
			}
		case 7:
			{
				loadlive=cvLoadImage("e:\\arjun8.jpg",3);
				break;
			}
		case 8:
		{
			loadlive=cvLoadImage("e:\\subash2.jpg",3);
			break;
		}

		}
	

*/
//	for(int l=1;l<300;l++)
//		live=cvQueryFrame(capture);
int flag123;
int l,loopcount=0;
	for( l=1;l<50;l++)
	{

		/*if(l>=75 && l<=97)
		{
			flag123=1;
		}
		if(l==98)
			flag123=0;   */
	flag123=0;
	while(flag123==0)
	{
		if(loopcount==130)
		flag123=1;		
	    loopcount++;
		
		for( int k=1;k<10;k++)
			live=cvQueryFrame(capture);

		live=cvQueryFrame(capture);
		cvSaveImage("C:/subash/III sem/Computer vision/PROJECT/zz.jpg",live);
		loadlive=cvLoadImage("C:/subash/III sem/Computer vision/PROJECT/zz.jpg",3);
		cvShowImage("LIVE",loadlive);

		blobcount = 1;

//		loadlive=cvLoadImage("e:\\zz1.jpg",3);
		cvCvtColor(loadlive,graylive,CV_BGR2GRAY);

//		cvNamedWindow("Gray live",CV_WINDOW_AUTOSIZE);  
//		cvShowImage("Gray live",graylive);

		//cvSub(graylive,grayback,graysubimg,mask);
		cvAbsDiff(grayback,graylive,graysubimg);
//		cvNamedWindow("Subtracted image",CV_WINDOW_AUTOSIZE);
//		cvShowImage("Subtracted image",graysubimg);

		cvThreshold(graysubimg,threshimg,30,255,CV_THRESH_BINARY);
//		cvNamedWindow("Threshold image",CV_WINDOW_AUTOSIZE);
//		cvShowImage("Threshold image",threshimg);
		cvErode(threshimg,src,B,1);
//		cvNamedWindow("Erode image",CV_WINDOW_AUTOSIZE);
//		cvShowImage("Erode image",erodeimg);

		
			cvSaveImage("C:/subash/III sem/Computer vision/PROJECT/live.jpg",loadlive);


/*
		for(int l=1;l<150;l++)
		{
			live=cvQueryFrame(capture);
			cvSaveImage("e:\\arjun.jpg",live);
		}
		cvShowImage("BOX",live);
*/

		
		cvFindBlob();
		cvWaitKey(0);  // changed********************************************************************************************************* 
		printf("\n ..... Frame %d ended",l);
		printf("\n INdex number of model is........ %d",minindexnumber);
		l++;
	}

	}
	
	printf("\n....... Done\n");



}









/*


	printf("\n Enter the values...Corsum modelindex and size \n");
	scanf("%d %d %d",&model[*Currentmodelindex].Corsum, &model[*Currentmodelindex].modelindex, &model[*Currentmodelindex].size);
	printf("\nEnter the histogram values\n");
	for(int i = 0;i<10;i++)
		scanf("%d",&model[*Currentmodelindex].histogram[i]);


	printf("\n Enter the values...Corsum modelindex and size \n");
	scanf("%d %d %d",&model[2].Corsum, &model[2].modelindex, &model[2].size);



	printf("\n The values are ..............\n");
	printf(" Corsum = %d   Modelindex = %d  Size = %d ", model[*Currentmodelindex].Corsum, model[*Currentmodelindex].modelindex, model[*Currentmodelindex].size);
	
	for(i =0;i<10;i++)
		model[*cc].histogram[i] = model[*Currentmodelindex].histogram[i];

	printf("\n The values are ....model %d  =========    ",*cc);
	for(i =0;i<100;i++)
		printf(" %d ",model[*cc].histogram[i]);

//	printf("\n The values are ..............\n");
//	printf(" Corsum = %d   Modelindex = %d  Size = %d ", model[1].Corsum, model[1].modelindex, model[1].size);

	getch();
}
	
	
	
	
	
	
	
	
	
	int i;
int j = cvNamedWindow("Test",CV_WINDOW_AUTOSIZE);

//CvCapture* capture = cvCaptureFromFile("d:\\cap.avi");
CvCapture* capture = cvCaptureFromCAM(-1);


for(i=1;i<=900;i++)
{
	IplImage* fra=cvQueryFrame(capture);
cvShowImage("Test",fra);

}

getch();



  
}
*/ 
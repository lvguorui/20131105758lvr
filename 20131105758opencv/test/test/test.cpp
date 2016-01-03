#include<opencv2\opencv.hpp>
#include <opencv2/core/core.hpp> 
#include<iostream>
#include<opencv2/nonfree/features2d.hpp>
#include<opencv2/nonfree/nonfree.hpp>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv; 
int main()
{
	
	char*path = "001.jpg";//ָ��
	IplImage*frame = cvLoadImage(path);
	if(!frame)return 0;
	
    //��ֵ�˲�
	cvSmooth(frame,frame,CV_MEDIAN);
	//�Ҷ�ͼ
	IplImage*gray=cvCreateImage(cvGetSize(frame),frame->depth,1);
	cvCvtColor(frame,gray,CV_BGR2GRAY);
	
	//��Ե���
	IplImage*temp=cvCreateImage(cvGetSize(gray),IPL_DEPTH_16S,1);
	//x�����ݶȣ���ֱ��Ե
	cvSobel(gray,temp,2,0,3);
	IplImage*sobel=cvCreateImage(cvGetSize(temp),IPL_DEPTH_8U,1);
	cvConvertScale(temp,sobel,1,0);
	
	//��ֵ��
	IplImage*threshold=cvCreateImage(cvGetSize(sobel),gray->depth,1);
	cvThreshold(sobel,threshold,0,255,CV_THRESH_BINARY|CV_THRESH_OTSU);
	
	//��̬ѧ�仯
	IplConvKernel*kernal;
	IplImage*morph=cvCreateImage(cvGetSize(threshold),threshold->depth,1);
	//�Զ���1*3�ĺ˽���x��������͸�ʴ
	kernal=cvCreateStructuringElementEx(3,1,1,0,CV_SHAPE_RECT);
	cvDilate(threshold,morph,kernal,2);//x������ͨ����
	cvErode(morph,morph,kernal,4);//x��ʴȥ����Ƭ
	cvDilate(morph,morph,kernal,4);//x���ͻظ���̬
	//�Զ���3*1�ĺ˽���y��������͸�ʴ
	kernal=cvCreateStructuringElementEx(1,3,0,1,CV_SHAPE_RECT);
	cvErode(morph,morph,kernal,1);//y��ʴȥ����Ƭ
	cvDilate(morph,morph,kernal,3);//y���ͻظ���̬

	//�������
	IplImage * frame_draw = cvCreateImage(cvGetSize(frame), frame->depth, frame->nChannels);
    cvCopy(frame, frame_draw);
    CvMemStorage * storage = cvCreateMemStorage(0);  
    CvSeq * contour = 0;   
    int count = cvFindContours(morph,storage,&contour,sizeof(CvContour),CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE);   
    CvSeq * _contour = contour;   
    for( ;contour != 0; contour = contour->h_next)
    {  		
	    double tmparea = fabs(cvContourArea(contour));  		 
	    CvRect aRect = cvBoundingRect( contour, 0 ); 
	    if(tmparea > ((frame->height*frame->width)/10))   
	    {  
		    cvSeqRemove(contour,0); //ɾ�����С���趨ֵ������,1/10   
		    continue;  
	    } 
	    if (aRect.width < (aRect.height*2))  
	    {  
		    cvSeqRemove(contour,0); //ɾ����߱���С���趨ֵ������   
		    continue;  
	    }
	    if ((aRect.width/aRect.height) > 4 )
	    {  
		    cvSeqRemove(contour,0); //ɾ����߱���С���趨ֵ������   
		    continue;  
	    }
	    if((aRect.height * aRect.width) < ((frame->height * frame->width)/100))
	    {  
		    cvSeqRemove(contour,0); //ɾ����߱���С���趨ֵ������   
		    continue;  
	    }
	    CvScalar color = CV_RGB( 255, 0, 0); 
	    cvDrawContours(frame_draw, contour, color, color, 0, 1, 8 );//�����ⲿ���ڲ�������
    }
    cvNamedWindow("����", 1);
    cvShowImage("����", frame_draw);
	waitKey();
}
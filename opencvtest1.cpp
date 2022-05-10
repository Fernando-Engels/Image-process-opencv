#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;
class manip {//which will allow you to display a frame (at a time t) from a video file and
    //2 other images
public:
    VideoCapture video;
    Mat frame;//the framed captured in the video

    manip(VideoCapture video_) {//constructor function
        video = video_;
       

    }
    void screenshot(double t) {//displaying the frame capture
        
        video.set(CAP_PROP_POS_MSEC, t);//capturing the frame a time t in miliseconds
        

        // Capture frame-by-frame

        video >> frame;
        imshow("frame", frame);
        waitKey(0);
    }
    void displayall(double t) {//function that displays the two images
        //One with 3x3 images draw on it, corresponding to the following gray images: the R channel
        //gray image(from RGB), B(from RGB), G(from RGB), H(from HSV), S(from HSV), V(from HSV),
        // L* (from L*a*b*), a* (from L*a*b*) and b* (from L*a*b*). Each of these gray images at 1/3
        //resolution of the original image and the other is 3 x 3 histograms draw on it
        // corresponding to the following gray images: the R channel
        //gray image(from RGB), B(from RGB), G(from RGB), H(from HSV), S(from HSV), V(from HSV),
        //L* (from L * a * b*), a* (from L * a * b*) and b* (from L * a * b*).
        
        Mat  HSV, LAB, outfinal;//the hsv image of the original, the lab image of the original, and the outfinal is the final image displayed
        vector<Mat> imagesrgb(3), imageshsv(3), imageslab(3), out(3);//all the channels
        //that will be extracted and out is for joining this image into one
        video.set(CAP_PROP_POS_MSEC, t);//capturing the frame
        video >> frame;

        int width = frame.size().width;
        int height = frame.size().height;
        cvtColor(frame, HSV, COLOR_BGR2HSV);//turn image into HSV
        cvtColor(frame, LAB, COLOR_BGR2Lab);//turn image into LAB
        //extracting the channels
        extractChannel(frame, imagesrgb[0], 0);    // Blue
        extractChannel(frame, imagesrgb[1], 1);    // Green
        extractChannel(frame, imagesrgb[2], 2);    // Red
        extractChannel(HSV, imageshsv[0], 0);    // H
        extractChannel(HSV, imageshsv[1], 1);    // S
        extractChannel(HSV, imageshsv[2], 2);    // V
        extractChannel(LAB, imageslab[0], 0);    // L
        extractChannel(LAB, imageslab[1], 1);    // a
        extractChannel(LAB, imageslab[2], 2);    // B
        //resizing the images
        for (int i = 0; i < 3; i++)
        {
            resize(imagesrgb[i], imagesrgb[i], Size(width / 3, height / 3), INTER_LINEAR);
        }
        for (int i = 0; i < 3; i++)
        {
            resize(imageshsv[i], imageshsv[i], Size(width / 3, height / 3), INTER_LINEAR);
        }
        for (int i = 0; i < 3; i++)
        {
            resize(imageslab[i], imageslab[i], Size(width / 3, height / 3), INTER_LINEAR);
        }
        //concatanating every image for display
        hconcat(imagesrgb, out[0]);
        hconcat(imageshsv, out[1]);
        hconcat(imageslab, out[2]);
        vconcat(out, outfinal);
        imshow("all collor spaces, first RGB,then hsv and Lab", outfinal);
        waitKey(0);
        displayhist(imagesrgb, imageshsv, imageslab);//calling the function that will display the histograms
    }
    void displayhist(vector<Mat> imagesrgb, vector<Mat> imageshsv,vector<Mat> imageslab) {//functions  that displays the histograms
        
        vector<Mat> imageshistrgb(3), imageshisthsv(3), imageshistlab(3),outrgb,outhsv,outlab,display;//vectors of matrices  of the matrices
        //of histograms, of matrices of the display of these histograms and the display of these histograms
        Mat displayfinal;//final display of the image

        int histSize = 256;
        
        
        float range[] = { 0, 256 }; //the upper boundary is exclusive
        const float* histRange[] = { range };
        bool uniform = true, accumulate = false;
        int hist_w = 512; int hist_h = 200;//size of histogram
        Mat histImage(hist_h, hist_w, CV_8UC1, Scalar(255));//base histogram image

        //initializing the matrices that will display the histogram with white values
        for (int i = 0; i < 3 ; i++)
        {
            outrgb.push_back(Mat(hist_h,hist_w, CV_8UC1, Scalar(255)));
        }
        for (int i = 0; i < 3; i++)
        {
            outhsv.push_back(Mat(hist_h, hist_w, CV_8UC1, Scalar(255)));
        }
        for (int i = 0; i < 3; i++)
        {
            outlab.push_back(Mat(hist_h, hist_w, CV_8UC1, Scalar(255)));
        }
        for (int i = 0; i < 3; i++)
        {
            display.push_back(Mat(hist_h, hist_w, CV_8UC1, Scalar(255)));
        }
       
       
        
        cout << imagesrgb[0].dims << endl;

        //calculating the histogram and normalizing it
        calcHist(&imagesrgb[0], 1, 0, Mat(), imageshistrgb[0], 1, &histSize, histRange, uniform, accumulate);
        normalize(imageshistrgb[0], imageshistrgb[0], 0, histImage.rows, NORM_MINMAX, -1, Mat());
        calcHist(&imagesrgb[1], 1, 0, Mat(), imageshistrgb[1], 1, &histSize, histRange, uniform, accumulate);
        normalize(imageshistrgb[1], imageshistrgb[1], 0, histImage.rows, NORM_MINMAX, -1, Mat());
        calcHist(&imagesrgb[2], 1, 0, Mat(), imageshistrgb[2], 1, &histSize, histRange, uniform, accumulate);
        normalize(imageshistrgb[2], imageshistrgb[2], 0, histImage.rows, NORM_MINMAX, -1, Mat());
        int bin_w = cvRound((double)hist_w / histSize);
        

            
            
           
            //drawing the histogram for rgb
            for (int i = 1; i < histSize; i++)
            {
                    line(outrgb[0], Point(bin_w * (i - 1), hist_h - cvRound(imageshistrgb[0].at<float>(i - 1))),
                    Point(bin_w * (i), hist_h - cvRound(imageshistrgb[0].at<float>(i))),
                    Scalar(0), 2, 8, 0);
                    line(outrgb[1], Point(bin_w * (i - 1), hist_h - cvRound(imageshistrgb[1].at<float>(i - 1))),
                        Point(bin_w * (i), hist_h - cvRound(imageshistrgb[1].at<float>(i))),
                        Scalar(0), 2, 8, 0);
                    line(outrgb[2], Point(bin_w * (i - 1), hist_h - cvRound(imageshistrgb[2].at<float>(i - 1))),
                        Point(bin_w * (i), hist_h - cvRound(imageshistrgb[2].at<float>(i))),
                        Scalar(0), 2, 8, 0);
                   


            }
            
            

            
        
       
       
            //calculating the histogram and normalizing it
        calcHist(&imageshsv[0], 1, 0, Mat(), imageshisthsv[0], 1, &histSize, histRange, uniform, accumulate);
        normalize(imageshisthsv[0], imageshisthsv[0], 0, histImage.rows, NORM_MINMAX, -1, Mat());
        calcHist(&imageshsv[1], 1, 0, Mat(), imageshisthsv[1], 1, &histSize, histRange, uniform, accumulate);
        normalize(imageshisthsv[1], imageshisthsv[1], 0, histImage.rows, NORM_MINMAX, -1, Mat());
        calcHist(&imageshsv[2], 1, 0, Mat(), imageshisthsv[2], 1, &histSize, histRange, uniform, accumulate);
        normalize(imageshisthsv[2], imageshisthsv[2], 0, histImage.rows, NORM_MINMAX, -1, Mat());
       
        //drawing the histogram for hsv
        for (int j = 0; j < 3; j++)
        {
           
            
            for ( int i = 1; i < histSize; i++)
            {
                line(outhsv[j], Point(bin_w * (i - 1), hist_h - cvRound(imageshisthsv[j].at<float>(i - 1))),
                    Point(bin_w * (i), hist_h - cvRound(imageshisthsv[j].at<float>(i))),
                    Scalar(0), 2, 8, 0);

            }
        }
        //calculating the histogram and normalizing it
        calcHist(&imageslab[0], 1, 0, Mat(), imageshistlab[0], 1, &histSize, histRange, uniform, accumulate);
        normalize(imageshistlab[0], imageshistlab[0], 0, histImage.rows, NORM_MINMAX, -1, Mat());
        calcHist(&imageslab[1], 1, 0, Mat(), imageshistlab[1], 1, &histSize, histRange, uniform, accumulate);
        normalize(imageshistlab[1], imageshistlab[1], 0, histImage.rows, NORM_MINMAX, -1, Mat());
        calcHist(&imageslab[2], 1, 0, Mat(), imageshistlab[2], 1, &histSize, histRange, uniform, accumulate);
        normalize(imageshistlab[2], imageshistlab[2], 0, histImage.rows, NORM_MINMAX, -1, Mat());
        //drawing the histogram for lab
        for (int j = 0; j < 3; j++)
        {
            
           
            for (int i = 1; i < histSize; i++)
            {
                line(outlab[j], Point(bin_w * (i - 1), hist_h - cvRound(imageshistlab[j].at<float>(i - 1))),
                    Point(bin_w * (i), hist_h - cvRound(imageshistlab[j].at<float>(i))),
                    Scalar(0), 2, 8, 0);

            }
        }
        //concatanating the images
        hconcat(outrgb, display[0]);
        hconcat(outhsv, display[1]);
        hconcat(outlab, display[2]);
        vconcat(display, displayfinal);
        imshow("all histogrggams, first RGB,then hsv and Lab",displayfinal);
        waitKey(0);


    }
};
int main(void)
{//testing the class
    double t;
    t = 3000;
    Mat frame;
    VideoCapture video("video.mp4");
    manip v(video);
    v.screenshot(t);
    vector<Mat> imageshistrgb(3);
   
    v.displayall(t);
    return 0;
}

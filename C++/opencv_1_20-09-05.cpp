// opencv_1.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。
//
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>
#include <math.h>
#include <typeinfo>
#include <thread>
#include <atomic>
//#include <chrono>

using namespace std;
using namespace cv;


const float PI_f = 3.14159265358979;

enum ImreadModes
{
    IMREAD_UNCHANGED = -1,
    IMREAD_GRAYSCALE = 0,
    IMREAD_COLOR = 1,
    IMREAD_ANYDEPTH = 2,
    IMREAD_ANYCOLOR = 4,
    IMREAD_LOAD_GDAL = 8,
    IMREAD_REDUCED_GRAYSCALE_2 = 16,
    IMREAD_REDUCED_COLOR_2 = 17,
    IMREAD_REDUCED_GRAYSCALE_4 = 32,
    IMREAD_REDUCED_COLOR_4 = 33,
    IMREAD_REDUCED_GRAYSCALE_8 = 64,
    IMREAD_REDUCED_COLOR_8 = 65,
    IMREAD_IGNORE_ORIENTATION = 128,
};

enum ImwriteEXRTypeFlags
{
    IMWRITE_EXR_TYPE_HALF = 1,
    IMWRITE_EXR_TYPE_FLOAT = 2
};

float sRGB_EOTF_8U(int V)
{
    if (V < 11) //0.040449936*255 = 10.31473368
    {
        return float(V) / 3294.6;
    }
    else
    {
        return pow(((float(V) / 255 + 0.055) / 1.055), 2.4);
    }
}

int sRGB_OETF_8U(float L)
{
    if (L <= 0.0031308)
    {
        return 255*(12.92 * L);
    }
    else
    {
        return int(255*(1.055 * pow(L, 0.41666666) - 0.055));
    }
}

int sRGB_OETF_16U(float L)
{
    if (L <= 0.0031308)
    {
        return 65535 * (12.92 * L);
    }
    else
    {
        return int(65535 * (1.055 * pow(L, 0.41666666) - 0.055));
    }
}

int PQ_OETF_16U(float L)
{
    return 65535 * pow((0.8359375 + 18.8515625 * pow(L, 0.1593017578125)) / (1 + 18.6875 * pow(L, 0.1593017578125)), 78.84375);  //((0.8359375 + 18.8515625 * L * *0.1593017578125) / (1 + 18.6875 * L * *0.1593017578125)) * *78.84375
}

int PQ_OETF_8U(float L)
{
    return 255 * pow((0.8359375 + 18.8515625 * pow(L, 0.1593017578125)) / (1 + 18.6875 * pow(L, 0.1593017578125)), 78.84375);  //((0.8359375 + 18.8515625 * L * *0.1593017578125) / (1 + 18.6875 * L * *0.1593017578125)) * *78.84375
}

int PQ_OETF_8U_h(float L)
{
    float temp = pow(L, 0.1593017578125);
    return 255 * pow((0.8359375 + 18.8515625 * temp) / (1 + 18.6875 * temp), 78.84375);  //((0.8359375 + 18.8515625 * L * *0.1593017578125) / (1 + 18.6875 * L * *0.1593017578125)) * *78.84375
}

float PQ_EOTF_8U(int V)
{
    return pow((-((128 * pow(float(V) / 255, 0.0126833135) - 107) / (2392 * pow(float(V) / 255, 0.0126833135) - 2413))), 6.2773946360);  // (-((128 * V_PQ * *(32 / 2523) - 107) / (2392 * V_PQ * *(32 / 2523) - 2413))) * *(8192 / 1305)
}

float PQ_EOTF_8U_h(int V)
{
    float temp = pow(float(V) / 255, 0.0126833135);
    //return float(V);
    return pow((-((128 * temp - 107) / (2392 * temp - 2413))), 6.2773946360);  // (-((128 * V_PQ * *(32 / 2523) - 107) / (2392 * V_PQ * *(32 / 2523) - 2413))) * *(8192 / 1305)
}

void PQ_EOTF_8U_p(float *L, unsigned char *V)
{
    *L = pow((-((128 * pow(float(*V) / 255, 0.0126833135) - 107) / (2392 * pow(float(*V) / 255, 0.0126833135) - 2413))), (6.2773946360));  // (-((128 * V_PQ * *(32 / 2523) - 107) / (2392 * V_PQ * *(32 / 2523) - 2413))) * *(8192 / 1305)
}

void PQ_EOTF_8U_p_h(float* L, unsigned char *V)
{
    float temp = pow(float(*V) / 255, 0.0126833135);
    //*L = float(*V);
    *L = pow((-((128 * temp - 107) / (2392 * temp - 2413))), (6.2773946360));  // (-((128 * V_PQ * *(32 / 2523) - 107) / (2392 * V_PQ * *(32 / 2523) - 2413))) * *(8192 / 1305)
}

void PQ_EOTF_8U_p_h_v2(Vec3f* L, Vec3b* V)
{
    //float temp = pow(float(*V) / 255, 0.0126833135);
    *(L + 1) = 0.5;//float(uint8_t(*V[0]));
    L[1] = 1;// float(uint8_t(*V[1]));
    L[2] = 1;// float(uint8_t(*V[2]));
    //*L = Vec3f(float(uint8_t(0.1, 0.2, 0.3);
    //*L = pow((-((128 * temp - 107) / (2392 * temp - 2413))), (6.2773946360));  // (-((128 * V_PQ * *(32 / 2523) - 107) / (2392 * V_PQ * *(32 / 2523) - 2413))) * *(8192 / 1305)
}

float PQ_EOTF_16U(int V)
{
    return pow((-((128 * pow(float(V)/65535, 0.0126833135) - 107) / (2392 * pow(float(V)/65535, 0.0126833135) - 2413))), (6.2773946360));  // (-((128 * V_PQ * *(32 / 2523) - 107) / (2392 * V_PQ * *(32 / 2523) - 2413))) * *(8192 / 1305)
}

float PQ_EOTF_16U_h(int V)
{
    float temp = pow(float(V) / 65535, 0.0126833135);
    return pow((-((128 * temp - 107) / (2392 * temp - 2413))), (6.2773946360));  // (-((128 * V_PQ * *(32 / 2523) - 107) / (2392 * V_PQ * *(32 / 2523) - 2413))) * *(8192 / 1305)
}

int OETF_2_8U(float L)
{
    if (L < 0.0031308)
    {
        return L * 3294.6;
    }
    else if (L < 0.237335)
    {
        return (1.055 * pow(L, 0.41666666) - 0.055) * 255;
    }
    else
    {
        return ((atan(6 * L + 1.1274908) * 4 / PI_f) - 1) * 255;
    }
}

Vec<float, 3>* __ptr64 white(Vec<float, 3>* __ptr64 *a)
{
    //*a = 1;
    return *a;
}

void Process_v1(Vec<float, 3>* __ptr64 im_3f_p, Vec<float, 3>* __ptr64 im_3f_2_p,int rows, int cols ,int* p, float* color_3f, float* size, int* range)
{
    float t;
    Vec3f* rxcols;
    Vec3f* rxcols_2;
    for (int r = p[0] - range[0]; r < p[0] + range[0] + 1; r++)
    {
        rxcols = im_3f_p + r * cols;
        rxcols_2 = im_3f_2_p + r * cols;
        for (int c = p[1] - range[1]; c < p[1] + range[1] + 1; c++)
        {
            if (r > -1 && r<rows && c>-1 && c < cols && (r != p[0] || c != p[1]))//(r != p[0] || c != p[1])
            {
                t = (pow((float(r) - p[0]) / size[0], 2) + pow((float(c) - p[1]) / size[1], 2));
                rxcols_2[c] = rxcols_2[c] + Vec3f(color_3f[0] / t, color_3f[1] / t, color_3f[2] / t);//(((r-p[0])^2)+ ((c - p[1]) ^ 2) )^-1
                //cout << p[0] << p[1] << endl;
            }
        }
    }
    //cout << *(im_3f_p + p[0] * cols + p[1]) << endl;
}

void Process_v2(Vec<float, 3>* __ptr64 im_3f_p, Vec<float, 3>* __ptr64 im_3f_2_p, int rows, int cols, int* p, float* color_3f, float* size, int* range)
{
    float t, dcolor_3f[3];
    Vec3f* position_p = im_3f_2_p + p[0] * cols + p[1];
    uint ixi;
    for (int r = 1; r < range[0] + 1; r++)
    {
        //rxcols = im_3f_p + r * im.cols;
        for (int c = 1; c < range[1] + 1; c++)
        {
            t = (pow(float(r) / size[0], 2) + pow(float(c) / size[1], 2));
            dcolor_3f[0] = color_3f[0] / t;
            dcolor_3f[1] = color_3f[1] / t;
            dcolor_3f[2] = color_3f[2] / t;
            //cout << "r=" << r << " c=" << c << " " << p[0] << "," << p[1] << endl;
            if (p[0] + r > -1 && p[0] + r <rows && p[1] + c>-1 && p[1] + c < cols)//&&(r != p[0] || c != p[1])
            {
                //cout << "A " << p[0] + r << " " << p[1] + c << endl;
                //t = 1/ (pow((float(r) - p[0]) / size[0], 2) + pow((float(c) - p[1]) / size[1], 2));
                *(position_p + r * cols + c) = *(position_p + r * cols + c) + Vec3f(dcolor_3f[0], dcolor_3f[1], dcolor_3f[2]);
                //sum += color_3f[1] / (pow((float(r) - p[0]) / size, 2) + pow((float(c) - p[1]) / size, 2));
                //sum += t;
                //cout << r << "," << c << " s4 " << t << endl;
            }
            if (p[0] + r > -1 && p[0] + r <rows && p[1] - c>-1 && p[1] - c < cols)
            {
                //cout << "B " << p[0] + r << " " << p[1] - c << endl;
                *(position_p + r * cols - c) = *(position_p + r * cols - c) + Vec3f(dcolor_3f[0], dcolor_3f[1], dcolor_3f[2]);//
            }
            if (p[0] - r > -1 && p[0] - r <rows && p[1] + c>-1 && p[1] + c < cols)
            {
                //cout << "C " << p[0] - r << " " << p[1] + c << endl;
                *(position_p - r * cols + c) = *(position_p - r * cols + c) + Vec3f(dcolor_3f[0], dcolor_3f[1], dcolor_3f[2]);//
            }
            if (p[0] - r > -1 && p[0] - r <rows && p[1] - c>-1 && p[1] - c < cols)
            {
                //cout << "D " << p[0] - r << " " << p[1] - c << endl;
                *(position_p - r * cols - c) = *(position_p - r * cols - c) + Vec3f(dcolor_3f[0], dcolor_3f[1], dcolor_3f[2]);//
            }
            //cout << "r=" << r << " c=" << c << " "<< p[0] << "," << p[1] << endl;
        }
    }

    for (int i = 1; i < range[0] + 1; i++)
    {
        ixi = i * i;
        //cout << im_3f_2_p << " " << p[0] * cols + p[1] << " " << prow << " " << im_3f_2_p + (p[0] + i) * cols + p[1] << " " << endl;
        if ( p[0] + i < rows)
        {
            *(position_p + i * cols) = *(position_p + i * cols) + Vec3f(color_3f[0] / ixi, color_3f[1] / ixi, color_3f[2] / ixi);
        }
        if ( p[0] - i > -1 )
        {
            *(position_p - i * cols) = *(position_p - i * cols) + Vec3f(color_3f[0] / ixi, color_3f[1] / ixi, color_3f[2] / ixi);
        }
    }
    for (int i = 1; i < range[1] + 1; i++)
    {
        ixi = i * i;
        if ( p[1] + i < cols)
        {
            *(position_p + i) = *(position_p + i) + Vec3f(color_3f[0] / ixi, color_3f[1] / ixi, color_3f[2] / ixi);
        }
        if ( p[1] - i > -1)
        {
            *(position_p - i) = *(position_p - i) + Vec3f(color_3f[0] / ixi, color_3f[1] / ixi, color_3f[2] / ixi);
        }
    }
}

void Process_v3(Vec<float, 3>* __ptr64 input, Vec<float, 3>* __ptr64 im_3f_p, Vec<float, 3>* __ptr64 im_3f_2_p, int rows, int cols, int* p, float* color_3f, float* size, int* range)
{
    float t, dcolor_3f[3];
    Vec3f* position_p = im_3f_2_p + p[0] * cols + p[1];
    uint ixi;
    for (int r = 1; r < range[0] + 1; r++)
    {
        //rxcols = im_3f_p + r * im.cols;
        for (int c = 1; c < range[1] + 1; c++)
        {
            t = (pow(float(r) / size[0], 2) + pow(float(c) / size[1], 2));
            //dcolor_3f[0] = color_3f[0] / t;
            //dcolor_3f[1] = color_3f[1] / t;
            //dcolor_3f[2] = color_3f[2] / t;
            //*(position_p + r * cols + c) = *(position_p + r * cols + c) + Vec3f(dcolor_3f[0], dcolor_3f[1], dcolor_3f[2]);
            *(position_p + r * cols - c) = *(position_p + r * cols - c) + Vec3f(dcolor_3f[0], dcolor_3f[1], dcolor_3f[2]);
            *(position_p - r * cols + c) = *(position_p - r * cols + c) + Vec3f(dcolor_3f[0], dcolor_3f[1], dcolor_3f[2]);
            *(position_p - r * cols - c) = *(position_p - r * cols - c) + Vec3f(dcolor_3f[0], dcolor_3f[1], dcolor_3f[2]);
            *(position_p + r * cols + c) = *(position_p + r * cols + c) + Vec3f(dcolor_3f[0], dcolor_3f[1], dcolor_3f[2]);
            //*(position_p + r * cols - c) = *(position_p + r * cols - c) + *(input + r * cols + c) / t;
            //*(position_p - r * cols + c) = *(position_p - r * cols + c) + *(input + r * cols + c) / t;
            //*(position_p - r * cols - c) = *(position_p - r * cols - c) + *(input + r * cols + c) / t;
            //*(position_p + r * cols + c) = *(position_p + r * cols + c) + *(input + r * cols + c) / t;
        }
    }

    for (int i = 1; i < range[0] + 1; i++)
    {
        ixi = i * i;
        *(position_p + i * cols) = *(position_p + i * cols) + Vec3f(color_3f[0] / ixi, color_3f[1] / ixi, color_3f[2] / ixi);
        *(position_p - i * cols) = *(position_p - i * cols) + Vec3f(color_3f[0] / ixi, color_3f[1] / ixi, color_3f[2] / ixi);
    }
    for (int i = 1; i < range[1] + 1; i++)
    {
        ixi = i * i;
        *(position_p + i) = *(position_p + i) + Vec3f(color_3f[0] / ixi, color_3f[1] / ixi, color_3f[2] / ixi);
        *(position_p - i) = *(position_p - i) + Vec3f(color_3f[0] / ixi, color_3f[1] / ixi, color_3f[2] / ixi);
    }
}


int time()
{
    auto begin = chrono::high_resolution_clock::now(); //typeid = class std::chrono::time_point<struct std::chrono::steady_clock,class std::chrono::duration<__int64,struct std::ratio<1,1000000000> > >
    //int x;
    //cin >> x;      // wait for user input
    for (int i = 0; i < 100000; i++)
    {

    }
    auto end = chrono::high_resolution_clock::now();
    auto dur = end - begin;
    uint ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
    uint64 micros = std::chrono::duration_cast<std::chrono::microseconds>(dur).count();
    uint64 nanos = std::chrono::duration_cast<std::chrono::nanoseconds>(dur).count();
    uint s = std::chrono::duration_cast<std::chrono::seconds>(dur).count();
    cout << "dur :" << typeid(dur).name() << endl;
    cout << "end :" << typeid(end).name() << endl;
    cout << "ms :" << ms << endl;
    cout << "micros :" << micros << typeid(micros).name() << endl;
    cout << "nanos :"  << nanos << typeid(nanos).name() << endl;
    cout << "s :" << s << endl;
    class std::chrono::time_point<struct std::chrono::steady_clock,class std::chrono::duration<__int64,struct std::ratio<1,1000000000> > > x = chrono::high_resolution_clock::now();
    //return long(chrono::high_resolution_clock::now());
    return nanos;
}

int main()
{
    auto begin = chrono::high_resolution_clock::now(); //typeid = class std::chrono::time_point<struct std::chrono::steady_clock,class std::chrono::duration<__int64,struct std::ratio<1,1000000000> > >
    auto t1 = chrono::high_resolution_clock::now();
    auto t2 = chrono::high_resolution_clock::now();
    auto t3 = chrono::high_resolution_clock::now();
    auto t4 = chrono::high_resolution_clock::now();
    //cout << time() << endl;
    //cout << PQ_OETF_16U(0.5) << endl;
    //cout << PQ_EOTF_16U(65535 * 0.7518271) << endl;
    //namedWindow("HelloCV", WINDOW_AUTOSIZE);
    //Mat im = imread("C:\\Users\\Leo\\Documents\\source\\python\\crop -5ev_sRGB_1.png");
    //Mat im = imread("C:\\Users\\Leo\\Documents\\source\\python\\crop_-13ev_PQ_2.png",-1);
    Mat im = imread("C:\\Users\\Leo\\source\\repos\\opencv_1\\opencv_1\\opencv_1\\crop_3f_-13ev.exr", -1);
    //Mat im = imread("C:\\Users\\Leo\\Documents\\source\\python\\test_8U3F.png", -1);
    //Mat im = imread("C:\\Users\\Leo\\source\\repos\\opencv_1\\opencv_1\\opencv_1\\out.exr",-1);
    //Mat im = imread("C:\\Users\\Leo\\source\\repos\\opencv_1\\opencv_1\\x64\\Debug\\1_480p.png");
    cout << sizeof(im) << endl;
    //imshow("www", im);
    cout << "type " << im.type() << endl;
    cout << "channels " << im.channels() << endl;
    cout << "rows " << im.rows << endl;
    cout << "cols " << im.cols << endl;
    //im.at<Vec3b>(100, 100)[1] = 4;

    Mat im_3f = Mat::zeros(im.rows, im.cols, 21); //float
    Vec3f* im_3f_p = im_3f.ptr<Vec3f>(0);

    Mat im_3f_2 = Mat::zeros(im.rows, im.cols, 21); //float
    Vec3f* im_3f_2_p = im_3f_2.ptr<Vec3f>(0);
    /*
    *(im_3f_2_p + 255 * im_3f_2.rows + 255) = Vec3f(1, 0.5, 0.1);
    cout << *(im_3f_2_p + 255 * im_3f_2.rows + 255) << endl;
    *(im_3f_2_p + 255 * im_3f_2.rows + 255) = Vec3f(1, 0.5, 0.1)/3;
    cout << *(im_3f_2_p + 255 * im_3f_2.rows + 255) << endl;
    *(im_3f_2_p + 255 * im_3f_2.rows + 255) = *(im_3f_2_p + 255 * im_3f_2.rows + 255) / 3;
    cout << *(im_3f_2_p + 255 * im_3f_2.rows + 255) << endl;
    cout << im_3f_2.at<Vec3f>(255, 255) << im_3f_2.at<Vec3f>(100, 100)[1] << typeid(im_3f_2.at<Vec3f>(100, 100)[1]).name() << typeid(im_3f_2.at<Vec3f>(100, 100)).name() << endl;
    //system("pause");*/

    /*
    Mat crop_3f = Mat::zeros(1080, 1920, 21);
    Vec3f* crop_3f_p = crop_3f.ptr<Vec3f>(0);
    im.copyTo(im_3f);
    int crop[2] = { 3200,11000 };
    for (int r = 0; r < crop_3f.rows; r++)
    {
        for (int c = 0; c < crop_3f.cols; c++)
        {
            *(crop_3f_p + r * crop_3f.cols + c) = *(im_3f_p + (crop[0] + r) * im_3f.cols + crop[1] + c)/8192;
        }
    }
    imshow("crop_3f", crop_3f);
    imwrite("crop_3f_-13ev.exr", crop_3f);

    system("pause");
    */
    Mat im_3b(im.rows, im.cols, 16); //CV_8UC3   uchar
    Vec3b* im_3b_p = im_3b.ptr<Vec3b>(0);

    Mat out_3b(im.rows, im.cols, 16); //CV_8UC3   uchar
    Vec3b* out_3b_p = out_3b.ptr<Vec3b>(0);

    Mat out_3w(im.rows, im.cols, 18); //CV_16UC3   ushort
    Vec3w* out_3w_p = out_3w.ptr<Vec3w>(0);

    cout << "im_3f_p :" << typeid(im_3f_p).name() << endl;

    if(im.type()==16)
    {
        im_3b_p = im.ptr<Vec3b>(0);

        cout << "im" << im_3b_p << endl;
        //cout << *(im_3b + 200*3) << endl;
        //cout << *(im_3b_p) << endl;
        //cout << *(im_3b_p + 200 * 960 + 0) << endl;
        //cout << *(im_3b_p + 200) << endl;
        //cout << *(im_3b_p + 200 * 960 + 100) << endl;
        //cout << *(im_3b_p + 200 * 960 + 150) << endl;
        //cout << *(im_3b_p + 100 * 960 + 100) << endl;
        //cout << *(im_3b_p + 479 * 960 + 959) << endl;
        //cout << *(im_3b_p+99*854*3+100*3) << endl;
        //cout << int(im.at<Vec3b>(200, 0)[1]) << " " << im.at<Vec3b>(100, 100)[2] * 2 << endl;
        //cout << "form im_3b_p" << *(im_3b_p + 200 * im.cols + 0) << endl;
        //cout << int(*(&im.at<Vec3b>(200, 0)[1])) << " " << im_3b_p + 200 * 960 << endl;
        //&im[row,col](a,b) = ptr + a*col + b
        //im[row,col](a,b) = *(ptr + a*col + b)
    }
    else if (im.type() == 18)
    {

    }
    else if (im.type() == 21)
    {
        //im.copyTo(im_3f);
        *(im_3f_p + 400 * 1920 + 600) = Vec3f(1, 1, 1);
        //Mat im_3f = im.clone();
        cout << "im_3f : " << typeid(im_3f).name() << endl;
        cout << *(im_3b_p + 200 * 960 + 150) << endl;
        cout << *(im_3f_p + 200 * 960 + 150) << endl;
        //Vec3f* im3f_p = im.ptr<Vec3f>(0);
        //imshow("out 8UC3", im_3f);
    }
    //imshow("im_3f_1", im_3f);
    t1 = chrono::high_resolution_clock::now();

    int p[2] = { 511, 511 }, range[2] = { 16,16 };
    float t, sum = 0, size[2] = { 1,1 }, color_3f[3] = { 0.3,1,0.1 }, dcolor_3f[3];
    Vec3f* rxcols;
    cout << "size[rows,columns] = ";
    cin >> size[0] >> size[1];
    cout << size[0] << "," << size[1] << endl;
    cout << "range[rows,columns] = ";
    cin >> range[0] >> range[1];
    cout << range[0] << "," << range[1] << endl;


    /*for (int c = p[1] - range[1]; c < p[1] + range[1]; c++)
    {
        for (int r = p[0] - range[0]; r < p[0] + range[0]; r++)
        {
            *(im_3f_p + r * im.cols + c) = Vec3f(0, 1, 0);
        }
    }*/
    /*
    int temp;
    for (int r = p[0] - range[0]; r < p[0] + range[0]; r++)
    {
        temp = r * im.cols;
        for (int c = p[1] - range[1]; c < p[1] + range[0]; c++)
        {
            *(im_3f_p + temp + c) = Vec3f(0, 1, 0);
        }
    }*/


    for (int i=1; i<range[0]+1; i++)
    {
        for (int j=1; j < range[1]+1;j++)
        {
            //t = (r - p[0])/100;
            //t = color_3f[1]/(pow((float(r) - p[0])/ size,2)+ pow((float(c) - p[1])/ size, 2));
            //cout << t << " " << sum << endl;
            sum += 1 / (pow(i / size[0], 2) + pow(j / size[1], 2));
            //cout << "s1 " << 1 / (pow(i / size[0], 2) + pow(j / size[1], 2)) << endl;
        }
    }
    for (int i = 1; i < range[0] + 1; i++)
    {
        sum += float(1) / i / i / 2;
        //cout << "s2 " << float(1) / i / i / 2 << endl;
    }
    for (int i = 1; i < range[1] + 1; i++)
    {
        sum += float(1) / i / i / 2;
        //cout << "s3 " << float(1) / i / i / 2 << endl;
    }
    cout << "sum = "<< sum << endl;
    t2 = chrono::high_resolution_clock::now();
    cout << "sum : " << chrono::duration_cast<chrono::milliseconds>(t2 - t1).count() << "ms" << endl;

    //sum = 0;

 /*
    t1 = chrono::high_resolution_clock::now();
    for (int r = p[0] - range[0]; r < p[0] + range[0] + 1; r++)
    {
        rxcols = im_3f_p + r * im.cols;
        for (int c = p[1] - range[1]; c < p[1] + range[1] + 1; c++)
        {
            if (r > -1 && r<im.rows && c>-1 && c < im.cols && (r != p[0] || c != p[1]))
            {
                t = (pow((float(r) - p[0]) / size[0], 2) + pow((float(c) - p[1]) / size[1], 2));
                //rxcols[c] = Vec3f(color_3f[0] / (pow((float(r) - p[0]) / size[0], 2) + pow((float(c) - p[1]) / size[1], 2)), color_3f[1] / (pow((float(r) - p[0]) / size[0], 2) + pow((float(c) - p[1]) / size[1], 2)), color_3f[2] / (pow((float(r) - p[0]) / size[0], 2) + pow((float(c) - p[1]) / size[1], 2)));//(((r-p[0])^2)+ ((c - p[1]) ^ 2) )^-1

                rxcols[c] = Vec3f(color_3f[0] / t, color_3f[1] / t, color_3f[2] / t);//(((r-p[0])^2)+ ((c - p[1]) ^ 2) )^-1
                //sum += color_3f[1] / (pow((float(r) - p[0]) / size, 2) + pow((float(c) - p[1]) / size, 2));
                sum += t;
                //cout << r << "," << c << " s4 " << t << endl;
            }
        }
    }

    cout << sum / size[0] / size[1] << *(im_3f_p + p[0] * im.cols + p[1]) << endl;
    *(im_3f_p + p[0] * im.cols + p[1]) = Vec3f(1, 0, 1);

    t2 = chrono::high_resolution_clock::now();
    cout << "Process : " << chrono::duration_cast<chrono::milliseconds>(t2 - t1).count() << "ms" << endl;

    cout << "im_3f_p = " << im_3f_p << endl;
    //Process v2====================================================================================================================================================
    t1 = chrono::high_resolution_clock::now();
    for (int r = 0; r < range[0]+1; r++)
    {
        //rxcols = im_3f_p + r * im.cols;
        for (int c = 0; c < range[1]+1; c++)
        {
            t = (pow(float(r) / size[0], 2) + pow(float(c) / size[1], 2));
            dcolor_3f[0] = color_3f[0] / t;
            dcolor_3f[1] = color_3f[1] / t;
            dcolor_3f[2] = color_3f[2] / t;
            if (p[0] + r >-1 && p[0] + r <im.rows && p[1] + c>-1 && p[1] + c < im.cols)//&&(r != p[0] || c != p[1])
            {

                //t = 1/ (pow((float(r) - p[0]) / size[0], 2) + pow((float(c) - p[1]) / size[1], 2));
                *(im_3f_p + (p[0] + r) * im.cols + p[1] + c) = Vec3f(dcolor_3f[0], dcolor_3f[1], dcolor_3f[2]);
                //sum += color_3f[1] / (pow((float(r) - p[0]) / size, 2) + pow((float(c) - p[1]) / size, 2));
                //sum += t;
                //cout << r << "," << c << " s4 " << t << endl;
            }
            if (p[0] + r > -1 && p[0] + r <im.rows && p[1] - c>-1 && p[1] - c < im.cols)
            {
                *(im_3f_p + (p[0] + r) * im.cols + p[1] - c) = Vec3f(dcolor_3f[0], dcolor_3f[1], dcolor_3f[2]);//
            }
            if (p[0] - r > -1 && p[0] - r <im.rows && p[1] + c>-1 && p[1] + c < im.cols)
            {
                *(im_3f_p + (p[0] - r) * im.cols + p[1] + c) = Vec3f(dcolor_3f[0], dcolor_3f[1], dcolor_3f[2]);//
            }
            if (p[0] - r > -1 && p[0] - r <im.rows && p[1] - c>-1 && p[1] - c < im.cols)
            {
                *(im_3f_p + (p[0] - r) * im.cols + p[1] - c) = Vec3f(dcolor_3f[0], dcolor_3f[1], dcolor_3f[2]);//
            }
        }
    }
    //sum = 10.6164*4
    //sum = 42.4849
    //sum = 35.9177
    cout << sum/size[0]/size[1] << *(im_3f_p + p[0] * im.cols + p[1]) << endl;
    //*(im_3f_p + p[0] * im.cols + p[1]) = Vec3f(1, 0, 1);

    t2 = chrono::high_resolution_clock::now();
    cout << "Process v2 : " << chrono::duration_cast<chrono::milliseconds>(t2 - t1).count() << "ms" << endl;

    t1 = chrono::high_resolution_clock::now();
    Process_v1(im_3f_p, im.rows, im.cols, p, color_3f, size, range);
    t2 = chrono::high_resolution_clock::now();
    cout << "Process_v1 : " << chrono::duration_cast<chrono::milliseconds>(t2 - t1).count() << "ms" << endl;

    imshow("out 3FC3", im_3f);
    */



    t1 = chrono::high_resolution_clock::now();

    if (im.type() == 16)
    {
        for (int i = 0; i < im.rows; i++)
        {
            Vec3b* input = im.ptr<Vec3b>(i);
            Vec3f* output = im_3f.ptr<Vec3f>(i);
            for (int j = 0; j < im.cols; j++)
            {
                output[j] = Vec3f(sRGB_EOTF_8U(input[j][0]), sRGB_EOTF_8U(input[j][1]), sRGB_EOTF_8U(input[j][2]));
                //output[j] = Vec3f(PQ_EOTF_8U(input[j][0]), PQ_EOTF_8U(input[j][1]), PQ_EOTF_8U(input[j][2]));
                //output[j] = Vec3f(PQ_EOTF_8U_h(input[j][0]), PQ_EOTF_8U_h(input[j][1]), PQ_EOTF_8U_h(input[j][2]));
                //PQ_EOTF_8U_p_h(&output[j][0], &input[j][0]);
                //PQ_EOTF_8U_p_h(&output[j][1], &input[j][1]);
                //PQ_EOTF_8U_p_h(&output[j][2], &input[j][2]);
                //PQ_EOTF_8U_p_h_v2(&output[j], &input[j]);
                //output[j] = Vec3f(PQ_EOTF_8U_h(input[j])

                //im_3f.at<Vec3f>(i, j)[0] = sRGB_EOTF_8U(*(im_3b_p + i * im.cols + j)[0]);
                //im_3f.at<Vec3f>(i, j)[1] = sRGB_EOTF_8U(*(im_3b_p + i * im.cols + j)[1]);
                //im_3f.at<Vec3f>(i, j)[2] = sRGB_EOTF_8U(*(im_3b_p + i * im.cols + j)[2]);
            }
            if (i == 100)
            {
                cout << "100,100 : " << &output[100];// << " " << &input[j][0] << " " << &input[j][1] << " " << &input[j][2];
                cout << "\noutput : " << typeid(output).name() << endl;
                cout << "output[100] : " << typeid(output[100]).name() << endl;
                cout << "output[100] = " << (output[100]) << *&(output[100]) << endl;
                cout << "output[100][0] : " << typeid(output[100][0]).name() << endl;
                *(&output[100][0]) = 1;
                cout << "output[100][0] = " << output[100][0] << endl;
                *(&input[100][0]) = 'W';
                cout << "input[100][0] : " << typeid(input[100][100]).name() << *(&input[100][0]) << int(&input[100][0]) << endl;
                cout << "input[100] : " << typeid(input[100]).name() << *(&input[100]) << int(&input[100]) << endl;
                //cout << "intput : " << typeid(input[100][100][0]).name() << endl;
                //cout << "intput : " << input[100][100] << endl;


                cout << "i : " << typeid(100).name() << endl;
                cout << "t : " << typeid(100).name() << endl;
            }
        }
    }
    else if (im.type() == 18)
    {
        for (int i = 0; i < im.rows; i++)
        {
            Vec3w* input = im.ptr<Vec3w>(i);
            Vec3f* output = im_3f.ptr<Vec3f>(i);
            for (int j = 0; j < im.cols; j++)
            {
                output[j] = Vec3f(PQ_EOTF_16U_h(input[j][0]), PQ_EOTF_16U_h(input[j][1]), PQ_EOTF_16U_h(input[j][2]));
            }
        }
    }

    t2 = chrono::high_resolution_clock::now();
    cout << "PQ_EOTF : " << chrono::duration_cast<chrono::milliseconds>(t2 - t1).count() << "ms" << endl;


    t1 = chrono::high_resolution_clock::now();

    for (int i = 0; i < im.rows; i++)
    {
        t3 = chrono::high_resolution_clock::now();

        Vec3f* input = im_3f.ptr<Vec3f>(i);
        Vec3b* output = out_3b.ptr<Vec3b>(i);
        for (int j = 0; j < im.cols; j++)
        {
            p[0] = i;
            p[1] = j;
            color_3f[0] = input[j][0];
            color_3f[1] = input[j][1];
            color_3f[2] = input[j][2];
            Process_v1 (im_3f_p, im_3f_2_p, im.rows, im.cols, p, color_3f, size, range);
            //input[j] = Vec3f(input[j][0] / sum / 4, input[j][1] / sum / 4, input[j][2] / sum / 4);
            //output[j] = Vec3f(sRGB_OETF_8U(input[j][0]), sRGB_OETF_8U(input[j][1]), sRGB_OETF_8U(input[j][2]));
            //output[j] = Vec3f(OETF_2_8U(input[j][0]), OETF_2_8U(input[j][1]), OETF_2_8U(input[j][2]));
            //output[j] = Vec3f(PQ_OETF_8U(input[j][0]), PQ_OETF_8U(input[j][1]), PQ_OETF_8U(input[j][2]));
            //output[j] = Vec3f(PQ_OETF_8U_h(input[j][0]), PQ_OETF_8U_h(input[j][1]), PQ_OETF_8U_h(input[j][2]));
        }
        t4 = chrono::high_resolution_clock::now();
        cout << "row " << i << " : "<< chrono::duration_cast<chrono::milliseconds>(t4 - t3).count() << "ms  total : " << chrono::duration_cast<chrono::seconds>(t4 - t1).count() <<"s  ETA : " << round(chrono::duration_cast<chrono::milliseconds>(t4 - t1).count()/(i+1)*(im.rows-i)/1000) << "s" << endl;
    }
    /*
    for (int i = 0; i < range[0]; i++)
    {
        t3 = chrono::high_resolution_clock::now();
        Vec3f* input = im_3f.ptr<Vec3f>(i);
        for (int j = 0; j < im.cols; j++)
        {
            p[0] = i;
            p[1] = j;
            color_3f[0] = input[j][0];
            color_3f[1] = input[j][1];
            color_3f[2] = input[j][2];
            Process_v1(im_3f_p, im_3f_2_p, im.rows, im.cols, p, color_3f, size, range);
        }
        t4 = chrono::high_resolution_clock::now();
        cout << "row " << i << " : " << chrono::duration_cast<chrono::milliseconds>(t4 - t3).count() << "ms  total : " << chrono::duration_cast<chrono::seconds>(t4 - t1).count() << "s  ETA : " << round(chrono::duration_cast<chrono::milliseconds>(t4 - t1).count() / (i + 1) * (im.rows - i) / 1000) << "s" << endl;
    }
    for (int i = range[0]; i < im.rows - range[0]; i++)
    {
        t3 = chrono::high_resolution_clock::now();

        Vec3f* input = im_3f.ptr<Vec3f>(i);
        for (int j = 0; j < range[1] - 1; j++)
        {
            p[0] = i;
            p[1] = j;
            color_3f[0] = input[j][0];
            color_3f[1] = input[j][1];
            color_3f[2] = input[j][2];
            Process_v1(im_3f_p, im_3f_2_p, im.rows, im.cols, p, color_3f, size, range);
        }
        for (int j = range[1] - 1; j < im.cols - range[1]; j++)
        {
            p[0] = i;
            p[1] = j;
            color_3f[0] = input[j][0];
            color_3f[1] = input[j][1];
            color_3f[2] = input[j][2];
            //input = input + i * im_3f.cols + j;
            Process_v3(input, im_3f_p, im_3f_2_p, im.rows, im.cols, p, color_3f, size, range);
        }
        for (int j = p[1] - range[1]; j < im.cols; j++)
        {
            //input = im_3f.ptr<Vec3f>(i);
            p[0] = i;
            p[1] = j;
            color_3f[0] = input[j][0];
            color_3f[1] = input[j][1];
            color_3f[2] = input[j][2];
            Process_v1(im_3f_p, im_3f_2_p, im.rows, im.cols, p, color_3f, size, range);
        }
        t4 = chrono::high_resolution_clock::now();
        cout << "row " << i << " : " << chrono::duration_cast<chrono::milliseconds>(t4 - t3).count() << "ms  total : " << chrono::duration_cast<chrono::seconds>(t4 - t1).count() << "s  ETA : " << round(chrono::duration_cast<chrono::milliseconds>(t4 - t1).count() / (i + 1) * (im.rows - i) / 1000) << "s" << endl;
    }
    for (int i = im.rows - range[0]; i < im.rows; i++)
    {
        t3 = chrono::high_resolution_clock::now();
        Vec3f* input = im_3f.ptr<Vec3f>(i);
        for (int j = 0; j < im.cols; j++)
        {
            p[0] = i;
            p[1] = j;
            color_3f[0] = input[j][0];
            color_3f[1] = input[j][1];
            color_3f[2] = input[j][2];
            Process_v1(im_3f_p ,im_3f_2_p, im.rows, im.cols, p, color_3f, size, range);
        }
        t4 = chrono::high_resolution_clock::now();
        cout << "row " << i << " : " << chrono::duration_cast<chrono::milliseconds>(t4 - t3).count() << "ms  total : " << chrono::duration_cast<chrono::seconds>(t4 - t1).count() << "s  ETA : " << round(chrono::duration_cast<chrono::milliseconds>(t4 - t1).count() / (i + 1) * (im.rows - i) / 1000) << "s" << endl;
    }*/

    sum = sum * 4;
    for (int i = 0; i < im.rows; i++)
    {
        Vec3f* input = im_3f_2.ptr<Vec3f>(i);
        Vec3b* output = out_3b.ptr<Vec3b>(i);
        for (int j = 0; j < im.cols; j++)
        {
            input[j] = Vec3f(input[j][0] / sum, input[j][1] / sum, input[j][2] / sum);
        }
        //cout << "row " << i << endl;
    }
    t2 = chrono::high_resolution_clock::now();
    cout << "Process_v1 : " << chrono::duration_cast<chrono::milliseconds>(t2 - t1).count() << "ms" << endl;
    cout << *(im_3f_p + 100 * im.cols + 100) << *(im_3f_p + 100 * im.cols + 100)+Vec3f(1,2,3) << " sum = " << sum << endl;
    imshow("im_3f_2", im_3f_2);
    imwrite("im_3f_Process_v3.exr", im_3f_2);

    im_3f = im_3f + im_3f_2;

    t1 = chrono::high_resolution_clock::now();
    for (int i = 0; i < im.rows; i++)
    {
        Vec3f* input = im_3f.ptr<Vec3f>(i);
        Vec3b* output = out_3b.ptr<Vec3b>(i);
        for (int j = 0; j < im.cols; j++)
        {
            //output[j] = Vec3f(sRGB_OETF_8U(input[j][0]), sRGB_OETF_8U(input[j][1]), sRGB_OETF_8U(input[j][2]));
            output[j] = Vec3f(OETF_2_8U(input[j][0]), OETF_2_8U(input[j][1]), OETF_2_8U(input[j][2]));
            //output[j] = Vec3f(PQ_OETF_8U(input[j][0]), PQ_OETF_8U(input[j][1]), PQ_OETF_8U(input[j][2]));
            //output[j] = Vec3f(PQ_OETF_8U_h(input[j][0]), PQ_OETF_8U_h(input[j][1]), PQ_OETF_8U_h(input[j][2]));
        }
    }
    t2 = chrono::high_resolution_clock::now();
    cout << "sRGB_OETF : " << chrono::duration_cast<chrono::milliseconds>(t2 - t1).count() << "ms" << endl;
    /*
    for (int i = 0; i < im.rows; i++)
    {
        Vec3f* input = im_3f.ptr<Vec3f>(i);
        Vec3w* output = out_3w.ptr<Vec3w>(i);
        for (int j = 0; j < im.cols; j++)
        {
            //output[j] = Vec3f(sRGB_OETF_16U(input[j][0]), sRGB_OETF_16U(input[j][1]), sRGB_OETF_16U(input[j][2]));
            output[j] = Vec3f(PQ_OETF_16U(input[j][0]), PQ_OETF_16U(input[j][1]), PQ_OETF_16U(input[j][2]));
        }
    }*/
    //cout << sizeof(out_3w);
    //imwrite("out_2.png", out_3b);
    //imwrite("out_PQ_16U.png", out_3w);
    //imwrite("3f_h.exr", im_3f);
    //imshow("linear", im_3f);
    imshow("out 8UC3", out_3b);



    cout << im_3f.at<Vec3f>(0, 0) << endl;
    //cout << float(im_3f_p.at<Vec3f>(200, 0)[1]) << endl;
    int a = 0;
    a+= 2;
    cout << "type " << out_3w.type() << endl;
    cout << a;
    waitKey(0);
    system("pause");
}



//(1.57079632679-atan((x^2+y^2)^0.5))^2  10.8137
//1/(x^2+y^2)





// 執行程式: Ctrl + F5 或 [偵錯] > [啟動但不偵錯] 功能表
// 偵錯程式: F5 或 [偵錯] > [啟動偵錯] 功能表

// 開始使用的提示:
//   1. 使用 [方案總管] 視窗，新增/管理檔案
//   2. 使用 [Team Explorer] 視窗，連線到原始檔控制
//   3. 使用 [輸出] 視窗，參閱組建輸出與其他訊息
//   4. 使用 [錯誤清單] 視窗，檢視錯誤
//   5. 前往 [專案] > [新增項目]，建立新的程式碼檔案，或是前往 [專案] > [新增現有項目]，將現有程式碼檔案新增至專案
//   6. 之後要再次開啟此專案時，請前往 [檔案] > [開啟] > [專案]，然後選取 .sln 檔案

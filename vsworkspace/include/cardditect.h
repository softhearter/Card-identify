#pragma once
#include<iostream>
#include<opencv2/opencv.hpp>
#include<vector>

int getColSum(cv::Mat &src,int cols);
int getRowSum(cv::Mat &src,int rows);
int cutTop(cv::Mat &dst,cv::Mat &result);
int cutLeft(cv::Mat &src,cv::Mat &result,cv::Mat &rImg);
bool Cutnum_Save_num(cv::Mat img,std::vector<cv::Mat>&Num_Temp);
bool Cutmod_Save_num(cv::Mat img_mod,std::vector<cv::Mat>&Card_Temp);
cv::Rect findroi(cv::Mat img);
int my_compare(cv::Mat a,cv::Mat b);
void check(std::vector<cv::Mat>Card_Temp,std::vector<cv::Mat>Num_Temp);


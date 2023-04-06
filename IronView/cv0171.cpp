#include <iostream>
#include <cmath>
#include <math.h>
#include <random>
#include <time.h>
#include <vector>
#include <fstream>

//#include "eigen-3.4.0/Eigen/Dense"
#include "cv0171.h"

#define __PI 3.141592
#define __2PI 6.283185307179586477

using namespace std;

struct EdgeInfo {
    double u;
    double v;
    int Dir;
    double Mag;
    double Ang;
};

typedef vector<vector<EdgeInfo>*> EDGEINFO;

EDGEINFO Edges;

KImageColor cv0171::Histogram_Equalization(KImageColor& Input_Img, int row, int col){
    KImageColor Histogram_Equalized_Img(row, col);

    int TotalNum = row * col;

    double r_Histo[256] = {0};
    double g_Histo[256] = {0};
    double b_Histo[256] = {0};
    double r_Histo_P[256] = {0};
    double g_Histo_P[256] = {0};
    double b_Histo_P[256] = {0};

    // 히스토그램
    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            r_Histo[(int)Input_Img[i][j].r]++;
            g_Histo[(int)Input_Img[i][j].g]++;
            b_Histo[(int)Input_Img[i][j].b]++;
        }
    }

    // 확률 히스토그램
    for(int i = 0; i < 256; i++){
        r_Histo_P[i] = r_Histo[i]/(TotalNum);
        g_Histo_P[i] = g_Histo[i]/(TotalNum);
        b_Histo_P[i] = b_Histo[i]/(TotalNum);
    }

    // CDF
    for(int i = 1; i < 256; i++){
        r_Histo_P[i] += r_Histo_P[i-1];
        g_Histo_P[i] += g_Histo_P[i-1];
        b_Histo_P[i] += b_Histo_P[i-1];
    }

    Histogram_Equalized_Img = Input_Img;

    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            Histogram_Equalized_Img[i][j].r = r_Histo_P[Histogram_Equalized_Img[i][j].r] * 255;
            Histogram_Equalized_Img[i][j].g = g_Histo_P[Histogram_Equalized_Img[i][j].g] * 255;
            Histogram_Equalized_Img[i][j].b = b_Histo_P[Histogram_Equalized_Img[i][j].b] * 255;
        }
    }
    return Histogram_Equalized_Img;
}

KImageColor cv0171::Histogram_Matching(KImageColor& Input_Img, KImageColor& Target_Img, int row, int col){
    KImageColor Histogram_Matched_Img(row, col);

    int TotalNum = row * col;

    double r_Histo_source[256] = {0};
    double g_Histo_source[256] = {0};
    double b_Histo_source[256] = {0};
    double r_Histo_P_source[256] = {0};
    double g_Histo_P_source[256] = {0};
    double b_Histo_P_source[256] = {0};

    double r_Histo_target[256] = {0};
    double g_Histo_target[256] = {0};
    double b_Histo_target[256] = {0};
    double r_Histo_P_target[256] = {0};
    double g_Histo_P_target[256] = {0};
    double b_Histo_P_target[256] = {0};

    double r_Histo_index[256] = {0};
    double g_Histo_index[256] = {0};
    double b_Histo_index[256] = {0};

    // 소스 히스토그램
    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            r_Histo_source[Input_Img[i][j].r]++;
            g_Histo_source[Input_Img[i][j].g]++;
            b_Histo_source[Input_Img[i][j].b]++;
        }
    }

    // 소스 확률 히스토그램
    for(int i = 0; i < 256; i++){
        r_Histo_P_source[i] = r_Histo_source[i]/(TotalNum);
        g_Histo_P_source[i] = g_Histo_source[i]/(TotalNum);
        b_Histo_P_source[i] = b_Histo_source[i]/(TotalNum);
    }

    // 소스 CDF
    for(int i = 1; i < 256; i++){
        r_Histo_P_source[i] += r_Histo_P_source[i-1];
        g_Histo_P_source[i] += g_Histo_P_source[i-1];
        b_Histo_P_source[i] += b_Histo_P_source[i-1];
    }

    // 타겟 히스토그램
    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            r_Histo_target[Target_Img[i][j].r]++;
            g_Histo_target[Target_Img[i][j].g]++;
            b_Histo_target[Target_Img[i][j].b]++;
        }
    }

    // 타겟 확률 히스토그램
    for(int i = 0; i < 256; i++){
        r_Histo_P_target[i] = r_Histo_target[i]/(TotalNum);
        g_Histo_P_target[i] = g_Histo_target[i]/(TotalNum);
        b_Histo_P_target[i] = b_Histo_target[i]/(TotalNum);
    }

    // 타겟 CDF
    for(int i = 1; i < 256; i++){
        r_Histo_P_target[i] += r_Histo_P_target[i-1];
        g_Histo_P_target[i] += g_Histo_P_target[i-1];
        b_Histo_P_target[i] += b_Histo_P_target[i-1];
    }

    //------------------------------------------------------------------------------//

    double min;

    for(int k = 0;k < 256;k++){
        min = 1;
        for(int l = 0;l < 256;l++){
            if(min >= abs(r_Histo_P_source[k] - r_Histo_P_target[l])){
                min = abs(r_Histo_P_source[k] - r_Histo_P_target[l]);
                r_Histo_index[k] = l;
            }
        }
    }

    for(int k = 0;k < 256;k++){
        min = 1;
        for(int l = 0;l < 256; l++){
            if(min >= abs(g_Histo_P_source[k] - g_Histo_P_target[l])){
                min = abs(g_Histo_P_source[k] - g_Histo_P_target[l]);
                g_Histo_index[k] = l;
            }
        }
    }

    for(int k = 0;k < 256;k++){
        min = 1;
        for(int l = 0;l < 256;l++){
            if(min >= abs(b_Histo_P_source[k] - b_Histo_P_target[l])){
                min = abs(b_Histo_P_source[k] - b_Histo_P_target[l]);
                b_Histo_index[k] = l;
            }
        }
    }

    for(int i = 0;i < row;i++){
        for(int j = 0;j < col; j++){
            Histogram_Matched_Img[i][j].r = r_Histo_index[Input_Img[i][j].r];
            Histogram_Matched_Img[i][j].g = g_Histo_index[Input_Img[i][j].g];
            Histogram_Matched_Img[i][j].b = b_Histo_index[Input_Img[i][j].b];
        }
    }

    return Histogram_Matched_Img;
}

KImageColor cv0171::Gaussian_Noise(KImageColor& Input_Img, double dSigma, int row, int col){
    KGaussian gaussian_noise;
    KImageColor Gaussian_Img = Input_Img;

    gaussian_noise.Create(0, dSigma*dSigma); // 평균, 분산
    gaussian_noise.OnRandom(Input_Img.Size());

    double make_noise = 0;
    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            make_noise = gaussian_noise.Generate();

            if(Gaussian_Img[i][j].r + make_noise * dSigma > 255)
                Gaussian_Img[i][j].r = 255;
            else if(Gaussian_Img[i][j].r + make_noise * dSigma < 0)
                Gaussian_Img[i][j].r = 0;
            else Gaussian_Img[i][j].r += make_noise * dSigma;

            if(Gaussian_Img[i][j].g + make_noise * dSigma > 255)
                Gaussian_Img[i][j].g = 255;
            else if(Gaussian_Img[i][j].g + make_noise * dSigma < 0)
                Gaussian_Img[i][j].g = 0;
            else Gaussian_Img[i][j].g += make_noise * dSigma;

            if(Gaussian_Img[i][j].b + make_noise * dSigma > 255)
                Gaussian_Img[i][j].b = 255;
            else if(Gaussian_Img[i][j].b + make_noise * dSigma < 0)
                Gaussian_Img[i][j].b = 0;
            else Gaussian_Img[i][j].b += make_noise * dSigma;
        }
    }
    return Gaussian_Img;
}

KImageColor cv0171::Salt_and_Pepper(KImageColor& Input_Img, double dTau, int row, int col){
    KImageColor SNP_Img = Input_Img;

    int num_of_noise_pixels = (int)(row*col*dTau);

    random_device rng;
    uniform_real_distribution<double> dist1(0, 1);

    for(int i = 0; i < num_of_noise_pixels; i++){
        int j = rand() % row;
        int k = rand() % col;

        if(dist1(rng) < dTau){
            SNP_Img[j][k].r = 255;
            SNP_Img[j][k].g = 255;
            SNP_Img[j][k].b = 255;
        }
        else if(dist1(rng) > 1 - dTau){
            SNP_Img[j][k].r = 0;
            SNP_Img[j][k].g = 0;
            SNP_Img[j][k].b = 0;
        }
    }
    return SNP_Img;
}

KImageColor cv0171::Box_Filter(KImageColor& Input_Img, QString BoxFilter_Size, int row, int col){
    KImageColor Box_Filtered_Img = Input_Img;
    int filter_size = 0;

    if(BoxFilter_Size == "3 x 3"){
        filter_size = 3;
    }
    else if(BoxFilter_Size == "5 x 5"){
        filter_size = 5;
    }
    int filter_half = filter_size/2;

    int temp_r, temp_g, temp_b= 0;
    for(int i = filter_half; i < row - filter_half; i++){
        for(int j = filter_half; j < col - filter_half; j++){
            temp_r = 0;
            temp_g = 0;
            temp_b = 0;

            for(int k = -filter_half; k <= filter_half; k++){
                for(int l = -filter_half; l <= filter_half; l++){
                    temp_r += Input_Img[i+k][j+l].r;
                    temp_g += Input_Img[i+k][j+l].g;
                    temp_b += Input_Img[i+k][j+l].b;
                }
            }
            Box_Filtered_Img[i][j].r = temp_r/(filter_size*filter_size);
            Box_Filtered_Img[i][j].g = temp_g/(filter_size*filter_size);
            Box_Filtered_Img[i][j].b = temp_b/(filter_size*filter_size);
        }
    }

    return Box_Filtered_Img;
}

KImageColor cv0171::Gaussian_Filter(KImageColor& Input_Img, double dSigma, int row, int col){
    KImageColor Gaussian_Filtered_Img = Input_Img;

    int filter_half = (int)(3 * dSigma + 0.3);
    KMatrix gaussian_mask(filter_half * 2 + 1, filter_half * 2 + 1);

    double kernel_coefficient = 1 / (2 * 3.1415926535 * dSigma * dSigma);
    double sum = 0;
    for(int i = 0, r = -filter_half; r <= filter_half; i++, r++){
        for(int j = 0, c = -filter_half; c <= filter_half; j++, c++){
            gaussian_mask[i][j] = kernel_coefficient * exp(-(r*r+c*c)/(2 * dSigma * dSigma));
            sum += gaussian_mask[i][j];
        }
    }
    gaussian_mask /= sum;

    double temp_r, temp_g, temp_b;
    for(int i = filter_half; i < row - filter_half; i++){
        for(int j = filter_half; j < col - filter_half; j++){
            temp_r = 0;
            temp_g = 0;
            temp_b = 0;

            for(int k = -filter_half, r = 0; k <= filter_half; k++, r++){
                for(int l = -filter_half, c = 0; l <= filter_half; l++, c++){
                    temp_r += Input_Img[i+k][j+l].r * gaussian_mask[r][c];
                    temp_g += Input_Img[i+k][j+l].g * gaussian_mask[r][c];
                    temp_b += Input_Img[i+k][j+l].b * gaussian_mask[r][c];
                }
            }
            Gaussian_Filtered_Img[i][j].r = temp_r;
            Gaussian_Filtered_Img[i][j].g = temp_g;
            Gaussian_Filtered_Img[i][j].b = temp_b;
        }
    }

    return Gaussian_Filtered_Img;
}

KImageColor cv0171::Median_Filter(KImageColor& Input_Img, QString MedianFilter_Size, int row, int col){
    KImageColor Median_Filtered_Img = Input_Img;

    int filter_size = 0;

    if(MedianFilter_Size == "3 x 3"){
        filter_size = 3;
    }
    else if(MedianFilter_Size == "5 x 5"){
        filter_size = 5;
    }

    int filter_half = filter_size / 2;

    //int medianstack[filter_size * filter_size];
    vector<int> medianstack_r;
    vector<int> medianstack_g;
    vector<int> medianstack_b;
    int median_r = 0;
    int median_g = 0;
    int median_b = 0;

    for(int i = filter_half; i < row - filter_half; i++){
        for(int j = filter_half; j < col - filter_half; j++){
            for(int k = -filter_half; k <= filter_half; k++){
                for(int l = -filter_half; l <= filter_half; l++){
                    medianstack_r.push_back(Input_Img[i + k][j + l].r);
                    medianstack_g.push_back(Input_Img[i + k][j + l].g);
                    medianstack_b.push_back(Input_Img[i + k][j + l].b);
                }
            }
            sort(medianstack_r.begin(), medianstack_r.end());
            sort(medianstack_g.begin(), medianstack_g.end());
            sort(medianstack_b.begin(), medianstack_b.end());
            median_r = medianstack_r[filter_half + 1];
            median_g = medianstack_g[filter_half + 1];
            median_b = medianstack_b[filter_half + 1];

            Median_Filtered_Img[i][j].r = median_r;
            Median_Filtered_Img[i][j].g = median_g;
            Median_Filtered_Img[i][j].b = median_b;

            medianstack_r.clear();
            medianstack_g.clear();
            medianstack_b.clear();
        }
    }
    return Median_Filtered_Img;
}

KImageGray cv0171::CannyEdge(KImageGray& gray_img, double sigma, int high_threshold, int low_threshold, int row, int col){
    KImageGray fdg_img(row, col);
    KImageGray maxima_img(row, col);
    KImageGray canny_edge_img(row, col);

    //EDGEINFO Edges;

    vector<EdgeInfo>* edge_col;
    for (int i = 0; i < row; i++) {
        edge_col = new vector<EdgeInfo>(col);
        Edges.push_back(edge_col);
    }

    // FDG
    // Derivative of Gaussian
    int h = (int)(3 * sigma + 0.3);
    double scale = 0;
    KMatrix mask_x(h * 2 + 1, h * 2 + 1);
    KMatrix mask_y(h * 2 + 1, h * 2 + 1);
    double DoG_constant = 1 / (2 * __PI * pow(sigma, 4));
    double DoG_temp;
    for (int i = 0, r = -h; r <= h; i++, r++) {
        DoG_temp = -r * exp(-(r * r) / (2 * sigma * sigma));
        for (int j = 0, c = -h; c <= h; j++, c++) {
            mask_x[i][j] = DoG_constant * DoG_temp * exp(-(c * c) / (2 * sigma * sigma));
            mask_y[j][i] = mask_x[i][j];

            // Half scailing
            if (r < 0) {
                scale += mask_x[i][j];
            }
        }
    }
    mask_x /= -scale;
    mask_y /= -scale;

    // Image Convolution
    double grad_x = 0, grad_y = 0;
    for (int i = h; i < row - h; i++) {
        for (int j = h; j < col - h; j++) {
            grad_x = grad_y = 0;

            // Convolution
            for (int ii = -h, r = 0; ii <= h; ii++, r++) {
                for (int jj = -h, c = 0; jj <= h; jj++, c++) {
                    grad_x += gray_img[i + ii][j + jj] * mask_x[r][c];
                    grad_y += gray_img[i + ii][j + jj] * mask_y[r][c];
                }
            }

            fdg_img[i][j] = fabs(grad_x) + fabs(grad_y);

            Edges[i]->at(j).Dir = (unsigned char)((((int)((atan2(grad_y, grad_x) * (180.0 / __PI) + 90.0) / 22.5) + 1) >> 1) & 0x00000003);
            Edges[i]->at(j).Mag = fabs(grad_x) + fabs(grad_y);
            Edges[i]->at(j).Ang = (180.0 / __PI) * atan2(grad_y, grad_x) + 0.5; // 0 ~ 360 degree
            Edges[i]->at(j).u = grad_x / sqrt(grad_x * grad_x + grad_y * grad_y);
            Edges[i]->at(j).v = grad_y / sqrt(grad_x * grad_x + grad_y * grad_y);
        }
    }

    // Non-Maxima Suppression
    maxima_img = fdg_img;
    int dx[4] = {1, 1, 0, -1};
    int dy[4] = {0, 1, 1, 1};
    int dir = 0;
    for (int i = 1; i < row - 1; i++) {
        for (int j = 1; j < col - 1; j++) {
            dir = Edges[i]->at(j).Dir;
            // edge에 수직인 방향으로 극대값이면
            if (fdg_img[i][j] > fdg_img[i + dy[dir]][j + dx[dir]] && fdg_img[i][j] > fdg_img[i - dy[dir]][j - dx[dir]]) {
                if (fdg_img[i][j] >= high_threshold) {
                    maxima_img[i][j] = 255;
                }
                else if (fdg_img[i][j] < low_threshold) {
                    maxima_img[i][j] = 0;
                }
            }
            else {
                maxima_img[i][j] = 0;
            }
        }
    }

    // Hysteresis Thresholding
    canny_edge_img = maxima_img;
    for (int i = 1; i < row - 1; i++) {
        for (int j = 1; j < col - 1; j++) {
            if (canny_edge_img[i][j] != 0 && canny_edge_img[i][j] < 255) {
                if (canny_edge_img[i - 1][j - 1] == 255 || canny_edge_img[i - 1][j] == 255 || canny_edge_img[i - 1][j + 1] == 255 ||
                    canny_edge_img[i][j - 1] == 255 || canny_edge_img[i][j + 1] == 255 ||
                    canny_edge_img[i + 1][j - 1] == 255 || canny_edge_img[i + 1][j] == 255 || canny_edge_img[i + 1][j + 1] == 255) {
                    canny_edge_img[i][j] = 255;
                }
                else {
                    canny_edge_img[i][j] = 0;
                }
            }
        }
    }

    return canny_edge_img;
}

KImageColor cv0171::GeneralizedHT(KImageGray& Input_Img, int row, int col){

    vector<pair<int, int>> points;
    vector<pair<double, double>> shape_table[4];
    KImageDouble igResult(row, col);

    // Get table info from saved file
    string location = "C:/CV/01_Homeworks_-_students/data/plug.txt";
    ifstream fin;
    fin.open(location);
    int x, y;
    double x_center = 0, y_center = 0;

    int ignore;
    fin >> ignore;
    while (!fin.eof()) {
        fin >> x >> y;
        points.push_back(make_pair(x, y));
    }
    if (fin.is_open()) {
        fin.close();
    }

    for (unsigned int i = 0; i < points.size(); i++) {
        x_center += points[i].first;
        y_center += points[i].second;
    }
    x_center /= points.size();
    y_center /= points.size();

    // Table
    int dir;                // edge 방향
    double r, alpha;
    double dx, dy;          // x, y 변화율
    for (int i = 1, ii = points.size() - 2; ii; i++, ii--) {
        dx = (double)(points[i + 1].first - points[i - 1].first);
        dy = (double)(points[i + 1].second - points[i - 1].second);

        // Edge 방향 설정
        double temp = (180 / __PI) * (atan2(dy, dx));
        temp += 90.0;
        dir = ((((int)(temp / 22.5) + 1) >> 1) & 0x00000003);

        // 중심과의 거리r, 중심과 이루는 각도 alpha
        r = sqrt((x_center - points[i].first) * (x_center - points[i].first) + (y_center - points[i].second) * (y_center - points[i].second));
        alpha = (180.0 / __PI) * atan2((double)(y_center - points[i].second), (double)(x_center - points[i].first));

        // shape table
        shape_table[dir].push_back(make_pair(r, alpha));
    }

    // 11 x 11 가우시안 weight 생성
    double gauss_vote[11][11];
    for(int i = -5; i < 6; i++){
        for(int j = -5; j < 6; j++){
            //gaussian weight
            gauss_vote[i + 5][j + 5] = 1.0 / (__2PI * pow(0.7, 2)) * exp(-0.5 * (pow(i, 2) + pow(j, 2) / pow(0.7, 2)));
        }
    }

    int n_x, n_y;
    double d_angle;
    KImageGray igCanny = cv0171::CannyEdge(Input_Img, 0.7, 80, 15, row, col);
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            if (igCanny[i][j] == 255) {
                // Rotation
                for (int theta = 0; theta < 4; theta++) {
                    d_angle = 90.0 * theta;
                    double temp = Edges[i]->at(j).Ang;
                    int Edge_Direction = ((((int)((d_angle - temp + 90.0) / 22.5) + 1) >> 1) & 0x00000003);

                    for (unsigned long long k = 0; k < shape_table[Edge_Direction].size(); k++) {
                        for (double scale = 1; scale <= 1; scale += 0.05) {
                            // Voting
                            n_x = j + scale * shape_table[Edge_Direction][k].first * cos(_RADIAN(shape_table[Edge_Direction][k].second + 90 * theta));
                            n_y = i + scale * shape_table[Edge_Direction][k].first * sin(_RADIAN(shape_table[Edge_Direction][k].second + 90 * theta));
                            if (n_x < 0 || n_y < 0 || n_x >= col || n_y >= row) {
                                continue;
                            }
                            // 가우시안 weight 적용
                            for (int y = -5; y < 6; y++) {
                                for (int x = -5; x < 6; x++) {
                                    if (n_x + x < 0 || n_y + y < 0 || n_x + x >= col || n_y + y >= row) {
                                        continue;
                                    }
                                    igResult[n_y + y][n_x + x] += 5 * gauss_vote[y + 5][x + 5];
                                }
                            }
                            // Votes to avoid edge noise
                            n_x = j - scale * shape_table[Edge_Direction][k].first * cos(_RADIAN(shape_table[Edge_Direction][k].second + 90 * theta));
                            n_y = i - scale * shape_table[Edge_Direction][k].first * sin(_RADIAN(shape_table[Edge_Direction][k].second + 90 * theta));
                            if (n_x < 0 || n_y < 0 || n_x >= col || n_y >= row) {
                                continue;
                            }
                            // 가우시안 weight 적용
                            for (int y = -5; y < 6; y++) {
                                for (int x = -5; x < 6; x++) {
                                    if (n_x + x < 0 || n_y + y < 0 || n_x + x >= col || n_y + y >= row) {
                                        continue;
                                    }
                                    igResult[n_y + y][n_x + x] += 5 * gauss_vote[y + 5][x + 5];
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // voting된 중심점 출력
    KImageGray voting_img = igResult.ToGray();
    KImageColor output_color_img = voting_img.GrayToRGB();
    int max = 0;
    int p_x, p_y;
    for (int i = 1; i < row - 1; i++) {
        for (int j = 1; j < col - 1; j++) {
            if (voting_img[i][j] >= 225) {
                if (voting_img[i][j] > max) {
                    p_x = j;
                    p_y = i;
                    max = voting_img[i][j];
                }

                output_color_img[i][j].r = 255;
                output_color_img[i][j].g = 0;
                output_color_img[i][j].b = 0;
            }
        }
    }

    // 플러그 엣지 출력
    for (dir = 0; dir < 4; dir++) {
        for (unsigned long long i = 0; i < shape_table[dir].size(); i++) {
            int temp_x = p_x + shape_table[dir][i].first * cos(-__PI / 2.0 + _RADIAN(shape_table[dir][i].second));
            int temp_y = p_y + shape_table[dir][i].first * sin(-__PI / 2.0 + _RADIAN(shape_table[dir][i].second));
            if (temp_x < 0 || temp_y < 0 || temp_x >= col || temp_y >= row) {
                continue;
            }

            output_color_img[temp_y][temp_x].r = 255;
            output_color_img[temp_y][temp_x].g = 255;
        }
    }
    return output_color_img;
}



KEdgeImg cv0171::OpticalFlow(KImageGray& igImg1, KImageGray& igImg2, int iter)
{
//    double gaussianVoting5[5][5];

//    for(int nRow = -2; nRow <3; nRow++){
//        for(int nCol= -2; nCol < 3; nCol++){
//            gaussianVoting5[nRow+2][nCol+2] = 1 / (_2PI * pow(5, 2)) * exp(-0.5 * ( pow(nRow,2) + pow(nCol,2) / pow(5, 2)));
//        }
//    }

//    int kernelSize = 5;
//    int kernalH = kernelSize/2;

//    int minSize = _MIN(igImg2.Row(), igImg2.Col());

//    KImageColor resultImg;


//    // 반복횟수로 인한 이미지 축소가 minsize 보다 크면 iter를 다시 정의=
//    if(pow(2, iter-1) > minSize / kernelSize){
//        iter = 0;
//        int tmpValue = 1;

//        while(1){
//            iter++;
//            tmpValue *= 2;
//            if(tmpValue > minSize / kernelSize){
//                iter--;
//                break;
//            }
//        }
//    }

//    // iter + 1개의 이미지 피라미드 생성(원본의 이미지도 저장되기 때문에)
//    ImgPyramid imgPyramid1(igImg1);
//    ImgPyramid imgPyramid2(igImg2);

//    imgPyramid1.Create(iter);
//    imgPyramid2.Create(iter);

//    // LK method 사용
//    vector<KEdgeImg>* pyImgSrc1 = imgPyramid1.getPyImg();
//    vector<KEdgeImg>* pyImgSrc2 = imgPyramid2.getPyImg();


//    // 상위의 피라미드 이미지(작은 이미지)부터 구하기
//    vector<vector<double>> srcMat1 = GetValMatrix((*pyImgSrc1)[iter]);
//    vector<vector<double>> srcMat2 = GetValMatrix((*pyImgSrc2)[iter]);

//    // LK를 적용하기 위한 가중치 벡터 생성
//    Eigen::Matrix<double, 25, 25> weightedMat5;   weightedMat5.fill(0.);

//    // 행렬 만들기
//    Eigen::Matrix<double, 25, 2> A;
//    Eigen::Matrix<double, 25, 1> b;
//    Eigen::Matrix<double, 2, 25> aTw;
//    Eigen::Matrix<double, 2, 1> zeta;

//    for(int i = 0; i<kernelSize; i++){
//        for(int j = 0; j<kernelSize; j++){
//            weightedMat5(i*kernelSize + j,i*kernelSize + j) = gaussianVoting5[i][j];
//            cout << weightedMat5(i*kernelSize + j, i*kernelSize + j) << " ";
//        }
//        cout << endl;
//    }

//    // dx, dy 찾기: 제일 위의 이미지에서 찾기
//    for(int nRow=kernalH+1; nRow < srcMat1.size()-kernalH-1; nRow++){
//        for(int nCol=kernalH+1; nCol < srcMat1[0].size()-kernalH-1; nCol++){
//                for(int kRow = nRow-kernalH, _i=0; kRow <= nRow+kernalH; kRow++, _i++){
//                    for(int kCol = nCol-kernalH, _j=0; kCol <= nCol+kernalH; kCol++, _j++){

//                        // 이전 이미지의 크기는 1/2배, 거리는 2배
//                        double moveX = kCol + (*pyImgSrc2)[iter][nRow]->at(nCol).dx;
//                        double moveY = kRow + (*pyImgSrc2)[iter][nRow]->at(nCol).dy;

//                        if(moveX <0) moveX = 0; if(moveX > srcMat1[0].size()-1) moveX = srcMat1[0].size()-1;
//                        if(moveY <0) moveY = 0; if(moveY > srcMat1.size()-1) moveY = srcMat1.size()-1;

//                        A.row(_i*kernelSize + _j) << srcMat1[kRow][kCol+1] - srcMat1[kRow][kCol-1], srcMat1[kRow+1][kCol] - srcMat1[kRow-1][kCol];
//                        b.row(_i*kernelSize + _j) << srcMat1[moveY][moveX] - srcMat2[kRow][kCol];

//                    }
//                }

//                //역행렬을 통해 d값을 구하는 과정
//                aTw = A.transpose() * weightedMat5;
//                zeta = (aTw * A).inverse() * aTw * b;
//                cout << zeta << endl;

//                (*pyImgSrc2)[iter][nRow]->at(nCol).dx += zeta(0,0);
//                (*pyImgSrc2)[iter][nRow]->at(nCol).dy += zeta(1,0);
//        }
//    }

//    for(int i = iter-1; i >= 0; i--){
//        srcMat1 = GetValMatrix((*pyImgSrc1)[i]);
//        srcMat2 = GetValMatrix((*pyImgSrc2)[i]);
//        KEdgeImg refMat = (*pyImgSrc2)[i+1];    //상단 이미지(래퍼런스)의 dx,dy를 가져오기 위해

//        // 커널을 옮겨가면서 dx, dy 찾기 - 맨 상단의 이미지에서 dx, dy 추출
//        for(int nRow=kernalH+1; nRow < srcMat1.size()-kernalH-1; nRow++){
//            for(int nCol=kernalH+1; nCol < srcMat1[0].size()-kernalH-1; nCol++){

//                    // 커널안의 점들을 통해 LK를 위한 행렬을 만듬
//                    for(int kRow = nRow-kernalH, _i=0; kRow <= nRow+kernalH; kRow++, _i++){
//                        for(int kCol = nCol-kernalH, _j=0; kCol <= nCol+kernalH; kCol++, _j++){
//                        // 이전 이미지의 크기는 1/2, 거리는 2배
//                        double moveX = kCol + refMat[nRow/2]->at(nCol/2).dx * 2 + (*pyImgSrc2)[i][nRow]->at(nCol).dx;
//                        double moveY = kRow + refMat[nRow/2]->at(nCol/2).dy * 2 + (*pyImgSrc2)[i][nRow]->at(nCol).dy;
//                        if(moveX <0) moveX = 0; if(moveX > srcMat1[0].size()-1) moveX = srcMat1[0].size()-1;
//                        if(moveY <0) moveY = 0; if(moveY > srcMat1.size()-1) moveY = srcMat1.size()-1;


//                        A.row(_i*kernelSize + _j) << srcMat1[kRow][kCol+1] - srcMat1[kRow][kCol-1], srcMat1[kRow+1][kCol] - srcMat1[kRow-1][kCol];
//                        b.row(_i*kernelSize + _j) << srcMat1[moveY][moveX] - srcMat2[kRow][kCol];
//                        }
//                    }

//                    //역행렬을 통해 d값을 구하는 과정
//                    aTw = A.transpose() * weightedMat5;
//                    zeta = (aTw * A).inverse() * aTw * b;

//                    (*pyImgSrc2)[i][nRow]->at(nCol).dx += zeta(0,0);
//                    (*pyImgSrc2)[i][nRow]->at(nCol).dy += zeta(1,0);
//            }
//        }
//    }

//    // 2번째 이미지를 컬러로 변환 후 자동저장
//    resultIcImg = igImg2.GrayToRGB();

//    return (*pyImgSrc2)[0];
}

// Input : 만들 이미지 피라미드 갯수
// 이미지 피라미드 만들기
void ImgPyramid::Create(int num)
{
    int minSize = _MIN(_rowSize, _colSize);
    // 예외처리
    if(pow(2, num-1) > minSize)   return;

    // 이전의 데이터가 있었다면 삭제
    if(!pyImg.empty()){
        for(unsigned int i=0; i< pyImg.size(); i++){
            for(unsigned int j=0; i< pyImg[i].size(); j++){
                delete pyImg[i][j];
            }
        }
        pyImg.clear();
    }

    // 각각의 이미지를 잠시 저장
    KEdgeImg tmpImg;

    // 미분값을 담을 이미지 생성
    vector<EdgeData> *colEdge;  // edge 데이터의 열 성분들

    for(unsigned int i=0; i < _igImg.Row(); i++){
        colEdge = new vector<EdgeData>(_igImg.Col());
        tmpImg.push_back(colEdge);
    }

    for(unsigned int nRow=0; nRow<_igImg.Row(); nRow++){
        for(unsigned int nCol=0; nCol<_igImg.Col(); nCol++){
            tmpImg[nRow]->at(nCol).val = _igImg[nRow][nCol];
        }
    }

    pyImg.push_back(tmpImg);

    //피라미드 이미지를 생성
    for(int i = 1; i <= num; i++){

        tmpImg.clear();

        for(unsigned int j=0; j < pyImg[i-1].size() / 2; j++){
            colEdge = new vector<EdgeData>(pyImg[i-1][0]->size() / 2);
            tmpImg.push_back(colEdge);
        }

        for(unsigned int nRow=0; nRow<pyImg[i-1].size() / 2; nRow++){
            for(unsigned int nCol=0; nCol<pyImg[i-1][0]->size() / 2; nCol++){
                // 제대로 하려면, 중간값을 넣든가 하는것이 좋을 듯...
                tmpImg[nRow]->at(nCol).val = pyImg[i-1][nRow*2]->at(nCol*2).val;
            }
        }
        pyImg.push_back(tmpImg);
    }
}


vector<vector<double> > cv0171::GetValMatrix(const KEdgeImg &matrix)
{
    vector<vector<double>> resultMat; vector<double> tmp;
    tmp.assign(matrix[0]->size(), 0.);
    resultMat.assign(matrix.size(), tmp);

    for(unsigned int i=0; i<resultMat.size(); i++){
        resultMat[i].reserve(matrix[0]->size());
    }

    for(unsigned int nRow=0; nRow<resultMat.size(); nRow++){
        for(unsigned int nCol=0; nCol<resultMat[0].size(); nCol++){
            resultMat[nRow][nCol] = matrix[nRow]->at(nCol).val;
        }
    }

    return resultMat;
}

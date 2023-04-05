#ifndef MAINFRAME_H
#define MAINFRAME_H

#include <QDialog>
#include "kfc.h"

namespace Ui {
class MainFrame;

}

class ImageForm;

class MainFrame : public QDialog
{
    Q_OBJECT

private:
    Ui::MainFrame *ui;

    KPtrList<ImageForm*>*   _plpImageForm;
    ImageForm*              _q_pFormFocused;
    ImageForm*              _q_pFormTarget;
    ImageForm*              _q_pFormClicked;

public:
    explicit MainFrame(QWidget *parent = 0);
    ~MainFrame();

    void            ImageFormFocused(ImageForm* q_pImageForm)
                    {   _q_pFormFocused  = q_pImageForm;   //활성화된 창의 포인터를 저장함
                        UpdateUI();                        //UI 활성화 갱신
                    }
    void            UpdateUI();
    void            Detach(ImageForm* pForm);

public:
    void            OnMousePos(const int& nX, const int& nY, ImageForm* q_pForm);

private slots:
    void on_buttonOpen_clicked();
    void on_buttonDeleteContents_clicked();    
    void on_buttonShowList_clicked();    

    void on_ApplySepia_clicked();

    void on_Apply_Otsu_Threshold_clicked();

    void on_Apply_Labeling_clicked();

    void on_Apply_Threshold_clicked();

    void on_Apply_Dilation_clicked();

    void on_Apply_Erosion_clicked();

    void on_Histogram_Equalization_clicked();

    void on_Histogram_Matching_clicked();

    void on_Register_Reference_clicked();

    void on_Apply_Gaussian_clicked();

    void on_Apply_SNP_clicked();

    void on_Apply_BoxFilter_clicked();

    void on_Apply_GaussianFilter_clicked();

    void on_Apply_MedianFilter_clicked();

    void on_Apply_Canny_edge_clicked();

    void on_Apply_HoughCircle_clicked();

    void on_Apply_Generalized_Hough_Transform_clicked();

    void on_Apply_Optical_Flow_clicked();

    void on_Apply_Compare_Img_clicked();

    void on_CameraBtn_clicked();

    void on_Checker_Corner_clicked();

protected:
    void closeEvent(QCloseEvent* event);
};

#endif // MAINFRAME_H

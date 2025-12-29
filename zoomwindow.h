#ifndef ZOOMWINDOW_H
#define ZOOMWINDOW_H

#include <QWidget>
#include <QImage>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QSlider>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolBar>
#include <QAction>
#include <QMouseEvent>
#include <QPainter>
#include <QColorDialog>

class ZoomWindow : public QWidget
{
    Q_OBJECT

public:
    ZoomWindow(const QImage &sourceImage, const QRect &region, double initialScale = 2.0, QWidget *parent = nullptr);
    ~ZoomWindow();

private slots:
    void saveImage();
    void updateZoomScale(int value);
    void toggleBrushTool();
    void selectBrushColor();
    void updateBrushSize(int size);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    void setupUI();
    void updateZoomedImage();
    QPoint mapToImageCoordinates(const QPoint &widgetPos);

    QImage originalImage;
    QImage croppedImage;
    QImage zoomedImage;
    QRect selectedRegion;
    double zoomScale;

    QLabel *imageLabel;
    QSpinBox *scaleSpinBox;
    QSlider *scaleSlider;
    QPushButton *saveButton;
    QPushButton *brushButton;
    QPushButton *colorButton;
    QSlider *brushSizeSlider;
    QLabel *brushSizeLabel;

    bool brushEnabled;
    bool isDrawing;
    QColor brushColor;
    int brushSize;
    QPoint lastDrawPoint;

    QVBoxLayout *mainLayout;
    QHBoxLayout *controlLayout;
    QHBoxLayout *brushControlLayout;
};

#endif // ZOOMWINDOW_H

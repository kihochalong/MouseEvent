#include "imageprocessor.h"
#include "imagetransform.h"
#include <QHBoxLayout>
#include <QMenuBar>
#include <QFileDialog>
#include <QDebug>
#include <QPainter>
#include <QPaintEvent>
#include <QtMath>
#include <QtGlobal>
#include <QInputDialog>

// Constants for region selection
namespace {
    constexpr int MIN_SELECTION_SIZE = 5; // Minimum width/height for valid selection
    constexpr int CLICK_SELECTION_SIZE = 120; // Default square size for click-to-zoom
    constexpr double MIN_ZOOM_SCALE = 1.0;
    constexpr double MAX_ZOOM_SCALE = 10.0;
    constexpr double DEFAULT_ZOOM_SCALE = 2.0;
}

ImageProcessor::ImageProcessor(QWidget *parent)
    : QMainWindow(parent), isSelecting(false)
{
    setWindowTitle(tr("影像處理"));
    central = new QWidget();
    QHBoxLayout *mainLayout = new QHBoxLayout(central);
    imgWin = new QLabel();
    QPixmap *initPixmap = new QPixmap(300,200);
    gWin = new ImageTransform();
    initPixmap->fill(QColor(255,255,255));
    imgWin->resize(300,200);
    imgWin->setScaledContents(true);
    imgWin->setPixmap(*initPixmap);
    mainLayout->addWidget(imgWin);
    setCentralWidget(central);
    createActions();
    createMenus();
    createToolBars();

    statusLabel = new QLabel;
    statusLabel->setText(tr("指標位置:"));
    statusLabel->setFixedWidth(100);
    MousePosLabel = new QLabel;
    MousePosLabel->setText(tr(" "));
    MousePosLabel->setFixedWidth(100);
    statusBar()->addPermanentWidget(statusLabel);
    statusBar()->addPermanentWidget(MousePosLabel);
    setMouseTracking(true);
    imgWin->setMouseTracking(true);
    central->setMouseTracking(true);

}

ImageProcessor::~ImageProcessor() {}

void ImageProcessor::createActions()
{
    openFileAction = new QAction(tr("開啟檔案&O"),this);
    openFileAction->setShortcut(tr("Ctrl+O"));
    openFileAction->setStatusTip(tr("開啟影像檔案"));
    connect(openFileAction,SIGNAL(triggered(bool)),this,SLOT(showOpenFile()));

    exitAction = new QAction(tr("結束&Q"),this);
    exitAction->setShortcut(tr("Ctrl+Q"));
    exitAction->setStatusTip(tr("退出程式"));
    connect(exitAction,SIGNAL(triggered(bool)),this,SLOT(close()));

    big = new QAction(tr("放大"),this);
    exitAction->setStatusTip(tr("放大"));
    connect(big,SIGNAL(triggered(bool)),this,SLOT(bigFile()));

    small = new QAction(tr("縮小"),this);
    exitAction->setStatusTip(tr("縮小"));
    connect(small,SIGNAL(triggered(bool)),this,SLOT(smallFile()));

    geometryAction = new QAction(tr("幾何轉換"),this);
    geometryAction->setShortcut(tr("Ctrl+G"));
    geometryAction->setStatusTip(tr("影像幾何轉換"));
    connect(geometryAction,SIGNAL(triggered(bool)),this,SLOT(showGeometryTransform()));
    connect(exitAction,SIGNAL(triggered(bool)),gWin,SLOT(close()));
}

void ImageProcessor::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("檔案&F"));
    fileMenu->addAction(openFileAction);
    fileMenu->addAction(exitAction);

    fileMenu = menuBar()->addMenu(tr("工具&T"));
    fileMenu->addAction(big);
    fileMenu->addAction(small);

    fileMenu->addAction(geometryAction);
}

void ImageProcessor::createToolBars()
{
    fileTool = addToolBar("file");
    fileTool->addAction(openFileAction);

    fileTool = addToolBar("tool");
    fileTool->addAction(big);
    fileTool->addAction(small);

    fileTool->addAction(geometryAction);
}

void ImageProcessor::loadFile(QString filename)
{
    qDebug()<<QString("file name:%1").arg(filename);
    QByteArray ba = filename.toLatin1();
    printf("FN:%s\n",(char *) ba.data());
    img.load(filename);
    imgWin->setPixmap(QPixmap::fromImage(img));
}

void ImageProcessor::showOpenFile()
{
    filename = QFileDialog::getOpenFileName(this,tr("開啟影像"),tr("."),"bmp(*.bmp);;png(*.png)"";;Jpng(*.jpg)");
    if(!filename.isEmpty()){
        if(img.isNull()){
            loadFile(filename);
        }else{
            ImageProcessor *newIPWin = new ImageProcessor();
            newIPWin->show();
            newIPWin->loadFile(filename);
        }
    }
}

void ImageProcessor::bigFile()
{
    if(!filename.isEmpty()){
        ImageProcessor *newIPWin = new ImageProcessor();
        newIPWin->show();
        newIPWin->loadFile(filename);
    }
}
void ImageProcessor::smallFile()
{
    if(!filename.isEmpty()){
        ImageProcessor *newIPWin = new ImageProcessor();
        newIPWin->show();
        newIPWin->loadFile(filename);
    }
}

void ImageProcessor::showGeometryTransform(){
    if(!img.isNull())
        gWin->srcImg=img;
    gWin->inWin->setPixmap(QPixmap::fromImage(gWin->srcImg));
    gWin->show();
}

void ImageProcessor::mouseDoubleClickEvent(QMouseEvent *event){
    QString str = "(" + QString::number(event->x()) +", " + QString::number(event->y()) + ")";
    statusBar()->showMessage(tr("雙擊:")+str,1000);
}
void ImageProcessor::mouseMoveEvent(QMouseEvent *event){
    int gray = qGray(img.pixel(event->x(),event->y()));
    QString str = "(" + QString::number(event->x()) +", " + QString::number(event->y()) + ")" + " = "+QString::number(gray);

    MousePosLabel->setText(str);
    
    // Update selection region while dragging
    if (isSelecting && !img.isNull()) {
        QPoint imgPos = mapToImageCoordinates(event->pos());
        if (imgPos.x() >= 0 && imgPos.y() >= 0) {
            selectionEnd = imgPos;
            drawSelectionRect();
        }
    }
}
void ImageProcessor::mousePressEvent(QMouseEvent *event){
    QString str = "(" + QString::number(event->x()) +", " + QString::number(event->y()) + ")";
    if(event->button()==Qt::LeftButton){
        statusBar()->showMessage(tr("左鍵:")+str,1000);
        
        // Start region selection
        QPoint imgPos = mapToImageCoordinates(event->pos());
        if (imgPos.x() >= 0 && imgPos.y() >= 0 && !img.isNull()) {
            isSelecting = true;
            selectionStart = imgPos;
            selectionEnd = imgPos;
        }
    }
    else if(event->button()==Qt::RightButton){
        statusBar()->showMessage(tr("右鍵:")+str,1000);
    }
    else if(event->button()==Qt::MiddleButton){
        statusBar()->showMessage(tr("中鍵:")+str,1000);
    }
}
void ImageProcessor::mouseReleaseEvent(QMouseEvent *event){
    QString str = "(" + QString::number(event->x()) +", " + QString::number(event->y()) + ")";
    statusBar()->showMessage(tr("釋放:")+str,1000);
    
    // Complete region selection and open zoom window
    if (isSelecting && event->button() == Qt::LeftButton && !img.isNull()) {
        isSelecting = false;
        
        QPoint imgPos = mapToImageCoordinates(event->pos());
        if (imgPos.x() >= 0 && imgPos.y() >= 0) {
            selectionEnd = imgPos;
        }
        
        QRect region;

        // Calculate the selected region
        const int x = qMin(selectionStart.x(), selectionEnd.x());
        const int y = qMin(selectionStart.y(), selectionEnd.y());
        const int width = qAbs(selectionEnd.x() - selectionStart.x());
        const int height = qAbs(selectionEnd.y() - selectionStart.y());

        if (width > MIN_SELECTION_SIZE && height > MIN_SELECTION_SIZE) {
            region = QRect(x, y, width, height);
        } else if (selectionStart == selectionEnd) {
            // Treat as click-to-zoom: create a square around the clicked point
            const int halfSize = CLICK_SELECTION_SIZE / 2;
            const int regionWidth = qMin(CLICK_SELECTION_SIZE, img.width());
            const int regionHeight = qMin(CLICK_SELECTION_SIZE, img.height());

            const int startX = qBound(0, selectionStart.x() - halfSize, img.width() - regionWidth);
            const int startY = qBound(0, selectionStart.y() - halfSize, img.height() - regionHeight);
            region = QRect(startX, startY, regionWidth, regionHeight);
        }

        // Only open zoom window if a valid region is selected
        if (region.isValid() && !region.isEmpty()) {
            selectedRegion = region.intersected(img.rect());

            if (selectedRegion.isValid() && !selectedRegion.isEmpty()) {
                bool ok = false;
                const double scale = QInputDialog::getDouble(
                    this,
                    tr("設定縮放倍率"),
                    tr("輸入縮放倍率 (1.0 - 10.0):"),
                    DEFAULT_ZOOM_SCALE,
                    MIN_ZOOM_SCALE,
                    MAX_ZOOM_SCALE,
                    1,
                    &ok);

                if (ok) {
                    ZoomWindow *zoomWin = new ZoomWindow(img, selectedRegion, scale, this);
                    zoomWin->setAttribute(Qt::WA_DeleteOnClose);
                    zoomWin->show();
                }
            }
        }

        // Clear the selection overlay
        imgWin->setPixmap(QPixmap::fromImage(img));
    }
}

void ImageProcessor::paintEvent(QPaintEvent *event) {
    QMainWindow::paintEvent(event);
}

void ImageProcessor::drawSelectionRect() {
    if (img.isNull()) return;
    
    // Create a copy of the image with the selection rectangle
    QPixmap pixmap = QPixmap::fromImage(img);
    QPainter painter(&pixmap);
    
    // Draw the selection rectangle
    painter.setPen(QPen(Qt::blue, 2, Qt::DashLine));
    
    int x = qMin(selectionStart.x(), selectionEnd.x());
    int y = qMin(selectionStart.y(), selectionEnd.y());
    int width = qAbs(selectionEnd.x() - selectionStart.x());
    int height = qAbs(selectionEnd.y() - selectionStart.y());
    
    painter.drawRect(x, y, width, height);
    
    imgWin->setPixmap(pixmap);
}

QPoint ImageProcessor::mapToImageCoordinates(const QPoint &pos) {
    // Convert window coordinates to image coordinates
    QPoint imgWinPos = imgWin->mapFrom(this, pos);
    
    if (img.isNull()) return QPoint(-1, -1);
    
    // Get the current pixmap size
    QPixmap currentPixmap = imgWin->pixmap(Qt::ReturnByValue);
    if (currentPixmap.isNull()) return QPoint(-1, -1);
    
    // Calculate scale factors
    double scaleX = (double)img.width() / currentPixmap.width();
    double scaleY = (double)img.height() / currentPixmap.height();
    
    // Map to image coordinates
    int imageX = (int)(imgWinPos.x() * scaleX);
    int imageY = (int)(imgWinPos.y() * scaleY);
    
    // Check bounds
    if (imageX >= 0 && imageX < img.width() && imageY >= 0 && imageY < img.height()) {
        return QPoint(imageX, imageY);
    }
    
    return QPoint(-1, -1);
}

#include "zoomwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QGroupBox>

ZoomWindow::ZoomWindow(const QImage &sourceImage, const QRect &region, double initialScale, QWidget *parent)
    : QWidget(parent),
      originalImage(sourceImage),
      selectedRegion(region),
      zoomScale(initialScale),
      brushEnabled(false),
      isDrawing(false),
      brushColor(Qt::red),
      brushSize(3)
{
    setWindowTitle(tr("縮放視窗"));
    
    // Extract the cropped region from the source image
    if (!originalImage.isNull() && region.isValid()) {
        croppedImage = originalImage.copy(region);
    } else {
        croppedImage = originalImage;
    }
    
    setupUI();
    updateZoomedImage();
    
    resize(800, 600);
}

ZoomWindow::~ZoomWindow() {}

void ZoomWindow::setupUI()
{
    mainLayout = new QVBoxLayout(this);
    
    // Control panel for zoom scale
    controlLayout = new QHBoxLayout();
    
    QLabel *scaleLabel = new QLabel(tr("縮放比例:"), this);
    controlLayout->addWidget(scaleLabel);
    
    scaleSlider = new QSlider(Qt::Horizontal, this);
    scaleSlider->setMinimum(10);  // 1.0x
    scaleSlider->setMaximum(100); // 10.0x
    scaleSlider->setValue((int)(zoomScale * 10));
    scaleSlider->setTickPosition(QSlider::TicksBelow);
    scaleSlider->setTickInterval(10);
    controlLayout->addWidget(scaleSlider);
    
    scaleSpinBox = new QSpinBox(this);
    scaleSpinBox->setMinimum(10);
    scaleSpinBox->setMaximum(100);
    scaleSpinBox->setValue((int)(zoomScale * 10));
    scaleSpinBox->setSuffix("x (÷10)");
    controlLayout->addWidget(scaleSpinBox);
    
    connect(scaleSlider, SIGNAL(valueChanged(int)), this, SLOT(updateZoomScale(int)));
    connect(scaleSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateZoomScale(int)));
    
    mainLayout->addLayout(controlLayout);
    
    // Brush control panel
    brushControlLayout = new QHBoxLayout();
    
    brushButton = new QPushButton(tr("筆刷工具"), this);
    brushButton->setCheckable(true);
    connect(brushButton, SIGNAL(clicked()), this, SLOT(toggleBrushTool()));
    brushControlLayout->addWidget(brushButton);
    
    colorButton = new QPushButton(tr("選擇顏色"), this);
    colorButton->setStyleSheet(QString("background-color: %1").arg(brushColor.name()));
    connect(colorButton, SIGNAL(clicked()), this, SLOT(selectBrushColor()));
    brushControlLayout->addWidget(colorButton);
    
    brushSizeLabel = new QLabel(tr("筆刷大小:"), this);
    brushControlLayout->addWidget(brushSizeLabel);
    
    brushSizeSlider = new QSlider(Qt::Horizontal, this);
    brushSizeSlider->setMinimum(1);
    brushSizeSlider->setMaximum(50);
    brushSizeSlider->setValue(brushSize);
    brushSizeSlider->setMaximumWidth(150);
    connect(brushSizeSlider, SIGNAL(valueChanged(int)), this, SLOT(updateBrushSize(int)));
    brushControlLayout->addWidget(brushSizeSlider);
    
    brushControlLayout->addStretch();
    
    mainLayout->addLayout(brushControlLayout);
    
    // Image display area
    imageLabel = new QLabel(this);
    imageLabel->setScaledContents(false);
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setMouseTracking(true);
    mainLayout->addWidget(imageLabel, 1);
    
    // Save button
    QHBoxLayout *saveLayout = new QHBoxLayout();
    saveButton = new QPushButton(tr("另存新檔"), this);
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveImage()));
    saveLayout->addStretch();
    saveLayout->addWidget(saveButton);
    
    mainLayout->addLayout(saveLayout);
    
    setMouseTracking(true);
}

void ZoomWindow::updateZoomedImage()
{
    if (croppedImage.isNull()) return;
    
    int newWidth = (int)(croppedImage.width() * zoomScale);
    int newHeight = (int)(croppedImage.height() * zoomScale);
    
    zoomedImage = croppedImage.scaled(newWidth, newHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    imageLabel->setPixmap(QPixmap::fromImage(zoomedImage));
}

void ZoomWindow::saveImage()
{
    if (zoomedImage.isNull()) {
        QMessageBox::warning(this, tr("錯誤"), tr("沒有可儲存的影像"));
        return;
    }
    
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("儲存影像"),
        "",
        tr("PNG Files (*.png);;JPEG Files (*.jpg);;BMP Files (*.bmp)"));
    
    if (!fileName.isEmpty()) {
        if (zoomedImage.save(fileName)) {
            QMessageBox::information(this, tr("成功"), tr("影像已儲存"));
        } else {
            QMessageBox::warning(this, tr("錯誤"), tr("儲存影像失敗"));
        }
    }
}

void ZoomWindow::updateZoomScale(int value)
{
    // Update both controls to stay in sync
    scaleSlider->blockSignals(true);
    scaleSpinBox->blockSignals(true);
    
    scaleSlider->setValue(value);
    scaleSpinBox->setValue(value);
    
    scaleSlider->blockSignals(false);
    scaleSpinBox->blockSignals(false);
    
    zoomScale = value / 10.0;
    updateZoomedImage();
}

void ZoomWindow::toggleBrushTool()
{
    brushEnabled = brushButton->isChecked();
    if (brushEnabled) {
        imageLabel->setCursor(Qt::CrossCursor);
        brushButton->setText(tr("停用筆刷"));
    } else {
        imageLabel->setCursor(Qt::ArrowCursor);
        brushButton->setText(tr("筆刷工具"));
        isDrawing = false;
    }
}

void ZoomWindow::selectBrushColor()
{
    QColor color = QColorDialog::getColor(brushColor, this, tr("選擇筆刷顏色"));
    if (color.isValid()) {
        brushColor = color;
        colorButton->setStyleSheet(QString("background-color: %1").arg(brushColor.name()));
    }
}

void ZoomWindow::updateBrushSize(int size)
{
    brushSize = size;
}

QPoint ZoomWindow::mapToImageCoordinates(const QPoint &widgetPos)
{
    // Map widget coordinates to image coordinates
    QPoint labelPos = imageLabel->mapFrom(this, widgetPos);
    QPixmap pixmap = imageLabel->pixmap(Qt::ReturnByValue);
    
    if (pixmap.isNull()) return QPoint(-1, -1);
    
    // Calculate the offset due to centering
    int xOffset = (imageLabel->width() - pixmap.width()) / 2;
    int yOffset = (imageLabel->height() - pixmap.height()) / 2;
    
    int imageX = labelPos.x() - xOffset;
    int imageY = labelPos.y() - yOffset;
    
    // Check if the point is within the image bounds
    if (imageX >= 0 && imageX < pixmap.width() && imageY >= 0 && imageY < pixmap.height()) {
        return QPoint(imageX, imageY);
    }
    
    return QPoint(-1, -1);
}

void ZoomWindow::mousePressEvent(QMouseEvent *event)
{
    if (brushEnabled && event->button() == Qt::LeftButton) {
        QPoint imagePos = mapToImageCoordinates(event->pos());
        if (imagePos.x() >= 0 && imagePos.y() >= 0) {
            isDrawing = true;
            lastDrawPoint = imagePos;
        }
    }
}

void ZoomWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (brushEnabled && isDrawing) {
        QPoint imagePos = mapToImageCoordinates(event->pos());
        if (imagePos.x() >= 0 && imagePos.y() >= 0) {
            // Draw on the zoomed image
            QPainter painter(&zoomedImage);
            painter.setPen(QPen(brushColor, brushSize, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            painter.drawLine(lastDrawPoint, imagePos);
            
            // Update the cropped image as well (scale back the drawing)
            QPainter croppedPainter(&croppedImage);
            QPoint scaledLast(lastDrawPoint.x() / zoomScale, lastDrawPoint.y() / zoomScale);
            QPoint scaledCurrent(imagePos.x() / zoomScale, imagePos.y() / zoomScale);
            croppedPainter.setPen(QPen(brushColor, brushSize / zoomScale, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            croppedPainter.drawLine(scaledLast, scaledCurrent);
            
            lastDrawPoint = imagePos;
            imageLabel->setPixmap(QPixmap::fromImage(zoomedImage));
        }
    }
}

void ZoomWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        isDrawing = false;
    }
}

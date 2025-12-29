# Zoom and Region Selection Feature

## Overview
This implementation adds the ability to zoom in on an image in the main window by selecting a region via mouse dragging. The zoom scale is customizable, and the new zoom window includes a "Save As" function and a brush tool for drawing on and modifying the image.

## Features

### 1. Region Selection
- **Mouse Dragging**: Click and drag on the main image window to select a region
- **Visual Feedback**: A blue dashed rectangle shows the selected region during dragging
- **Coordinate Mapping**: Properly maps window coordinates to image coordinates

### 2. Zoom Window
- **Customizable Zoom Scale**: Use the slider or spinbox to adjust zoom level from 1.0x to 10.0x
- **High Quality**: Uses smooth transformation for better quality
- **Independent Window**: Each zoomed region opens in its own window

### 3. Brush Tool
- **Toggle On/Off**: Enable/disable the brush tool with the button
- **Color Selection**: Choose any brush color using the color picker
- **Adjustable Size**: Brush size ranges from 1 to 50 pixels
- **Live Drawing**: Draw directly on the zoomed image
- **Synchronized Updates**: Drawing updates both the zoomed view and the original cropped region

### 4. Save Functionality
- **Multiple Formats**: Save as PNG, JPEG, or BMP
- **Save Dialog**: Standard file dialog for choosing save location
- **Success Feedback**: Message box confirms successful save

## Implementation Details

### Files Added
1. **zoomwindow.h**: Header file for the ZoomWindow class
2. **zoomwindow.cpp**: Implementation of the ZoomWindow class

### Files Modified
1. **imageprocessor.h**: Added region selection support
2. **imageprocessor.cpp**: Implemented mouse event handlers for region selection
3. **ImageProcessor.pro**: Added new source files to the project
4. **imagetransform.cpp**: Fixed include case sensitivity issue (Qpainter → QPainter)

### Key Classes and Methods

#### ZoomWindow Class
- `ZoomWindow(const QImage &sourceImage, const QRect &region, double initialScale, QWidget *parent)`: Constructor
- `updateZoomScale(int value)`: Updates the zoom level
- `toggleBrushTool()`: Enables/disables the brush tool
- `selectBrushColor()`: Opens color picker dialog
- `updateBrushSize(int size)`: Adjusts brush size
- `saveImage()`: Saves the zoomed image to file
- `mapToImageCoordinates(const QPoint &widgetPos)`: Maps widget coordinates to image coordinates

#### ImageProcessor Class (Modified)
- Added member variables for tracking selection state
- `mousePressEvent()`: Starts region selection
- `mouseMoveEvent()`: Updates selection rectangle while dragging
- `mouseReleaseEvent()`: Completes selection and opens zoom window
- `drawSelectionRect()`: Draws the selection overlay
- `mapToImageCoordinates()`: Maps coordinates accounting for image scaling

## Usage

### Selecting and Zooming a Region
1. Open an image in the main window
2. Click and drag on the image to select a region
3. Release the mouse button to open the zoom window
4. The zoomed region will appear in a new window

### Adjusting Zoom Level
1. Use the horizontal slider to adjust zoom
2. Or use the spinbox for precise control
3. The image updates in real-time

### Drawing on the Image
1. Click the "筆刷工具" (Brush Tool) button to enable drawing
2. Click "選擇顏色" (Select Color) to choose a brush color
3. Adjust the brush size with the slider
4. Click and drag on the image to draw
5. Click "停用筆刷" (Disable Brush) to stop drawing

### Saving the Modified Image
1. Click the "另存新檔" (Save As) button
2. Choose a filename and format
3. Click Save

## Technical Notes

### Coordinate Mapping
The implementation properly handles coordinate transformations between:
- Window coordinates (where the mouse click occurs)
- Widget coordinates (imgWin label position)
- Image coordinates (actual pixel positions in the image)

This ensures accurate region selection regardless of window size or image scaling.

### Brush Tool Implementation
The brush tool draws on both:
1. The zoomed image (for immediate visual feedback)
2. The cropped image (scaled appropriately)

This dual-drawing approach ensures that the modifications are preserved when zooming in/out.

### Memory Management
- Zoom windows are set to delete on close (`Qt::WA_DeleteOnClose`)
- This prevents memory leaks when multiple zoom windows are opened

## Build Instructions

```bash
# Generate Makefile
qmake6 ImageProcessor.pro

# Build the project
make

# Run the application
./ImageProcessor
```

## Dependencies
- Qt 6.x (Core, GUI, Widgets modules)
- C++17 or later

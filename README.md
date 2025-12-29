# MouseEvent

Qt-based Image Processor with Zoom and Region Selection

## Features

### Image Processing
- Open and display images (BMP, PNG, JPEG formats)
- Geometric transformations (mirror, rotate)
- Mouse event tracking and coordinate display

### Zoom and Region Selection ✨ NEW
- **Interactive Region Selection**: Click and drag to select any region of an image
- **Customizable Zoom**: Zoom from 1.0x to 10.0x with real-time preview
- **Brush Tool**: Draw and modify zoomed images with:
  - Customizable brush color
  - Adjustable brush size (1-50 pixels)
  - Real-time drawing
- **Save Functionality**: Export zoomed/modified images as PNG, JPEG, or BMP

## Build Requirements

- Qt 6.x (Core, GUI, Widgets modules)
- C++17 or later
- qmake6 or CMake
- A C++ compiler (g++, clang, MSVC)

## Building

### Linux/macOS
```bash
# Install Qt 6 (Ubuntu/Debian example)
sudo apt-get install qt6-base-dev build-essential

# Build the project
qmake6 ImageProcessor.pro
make

# Run
./ImageProcessor
```

### Windows
```bash
# Use Qt Creator or:
qmake ImageProcessor.pro
nmake  # or mingw32-make
ImageProcessor.exe
```

## Usage

### Basic Image Operations
1. **Open Image**: File → Open File (Ctrl+O)
2. **Zoom In/Out**: Use toolbar buttons
3. **Geometric Transform**: Tools → Geometric Transform (Ctrl+G)

### Zoom and Region Selection
1. **Select Region**: 
   - Click and drag on the image to select a region
   - A blue dashed rectangle shows the selected area
   - Release to open the zoom window

2. **Adjust Zoom**:
   - Use the horizontal slider or spinbox
   - Range: 1.0x to 10.0x
   - Image updates in real-time

3. **Drawing on Zoomed Image**:
   - Click "筆刷工具" (Brush Tool) to enable
   - Click "選擇顏色" (Select Color) to choose brush color
   - Adjust brush size with slider
   - Click and drag to draw

4. **Save Modified Image**:
   - Click "另存新檔" (Save As)
   - Choose format (PNG, JPEG, BMP)
   - Select location and save

## Documentation

- [ZOOM_FEATURE.md](ZOOM_FEATURE.md) - Detailed feature documentation
- [TEST_PLAN.md](TEST_PLAN.md) - Comprehensive test plan and results

## Project Structure

```
MouseEvent/
├── imageprocessor.cpp/h    # Main window with region selection
├── imagetransform.cpp/h    # Geometric transformations
├── mouseevent.cpp/h        # Mouse event demonstrations
├── zoomwindow.cpp/h        # Zoom window with brush tool
├── main.cpp                # Application entry point
└── ImageProcessor.pro      # Qt project file
```

## License

[Add your license here]

## Contributors

[Add contributors here]

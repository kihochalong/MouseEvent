# Test Plan for Zoom and Region Selection Feature

## Test Environment
- Qt Version: 6.x
- Platform: Linux
- Build Status: ✅ Successfully compiled

## Manual Testing Guide

### Test 1: Basic Region Selection
**Objective**: Verify that users can select a region using mouse dragging

**Steps**:
1. Run the application: `./ImageProcessor`
2. Open an image (File → Open File)
3. Click and drag on the image to select a region
4. Observe the blue dashed rectangle appears while dragging

**Expected Result**:
- A blue dashed rectangle should be drawn showing the selected area
- The rectangle should update in real-time as the mouse moves
- Mouse position coordinates should be displayed in the status bar

### Test 2: Zoom Window Opens
**Objective**: Verify that a zoom window opens after selection

**Steps**:
1. Select a region (minimum 6x6 pixels) by dragging
2. Release the mouse button

**Expected Result**:
- A new "縮放視窗" (Zoom Window) opens
- The selected region is displayed zoomed at 2.0x scale (default)
- The window contains zoom controls, brush controls, and a save button

### Test 3: Customizable Zoom Scale
**Objective**: Test the zoom scale adjustment feature

**Steps**:
1. Open a zoom window with a selected region
2. Adjust the zoom slider (horizontal slider)
3. Try the spinbox control as well

**Expected Result**:
- Both slider and spinbox controls are synchronized
- Zoom range is 1.0x to 10.0x
- The zoomed image updates smoothly in real-time
- Image quality remains high (using smooth transformation)

### Test 4: Brush Tool - Enable/Disable
**Objective**: Test the brush tool toggle functionality

**Steps**:
1. Open a zoom window
2. Click "筆刷工具" (Brush Tool) button
3. Observe the cursor change and button text change
4. Click again to disable

**Expected Result**:
- When enabled:
  - Cursor changes to crosshair
  - Button text changes to "停用筆刷" (Disable Brush)
- When disabled:
  - Cursor returns to normal arrow
  - Button text changes back to "筆刷工具"

### Test 5: Brush Tool - Color Selection
**Objective**: Test brush color selection

**Steps**:
1. Open a zoom window
2. Click "選擇顏色" (Select Color) button
3. Choose a color from the color picker dialog
4. Click OK

**Expected Result**:
- A standard Qt color picker dialog appears
- After selection, the color button background changes to the selected color
- The selected color is used for drawing

### Test 6: Brush Tool - Size Adjustment
**Objective**: Test brush size adjustment

**Steps**:
1. Open a zoom window
2. Enable the brush tool
3. Adjust the "筆刷大小" (Brush Size) slider
4. Draw lines at different sizes

**Expected Result**:
- Brush size ranges from 1 to 50 pixels
- Drawn lines reflect the selected brush size
- Larger values create thicker lines

### Test 7: Brush Tool - Drawing
**Objective**: Test drawing functionality

**Steps**:
1. Open a zoom window
2. Enable the brush tool
3. Click and drag on the zoomed image to draw

**Expected Result**:
- Lines are drawn on the image in real-time
- Drawing uses the selected color and size
- Lines are smooth and continuous
- Both the zoomed image and the underlying cropped image are updated

### Test 8: Save Functionality
**Objective**: Test the "Save As" feature

**Steps**:
1. Open a zoom window
2. (Optionally) Draw some modifications
3. Click "另存新檔" (Save As) button
4. Choose a filename and format (PNG, JPEG, or BMP)
5. Click Save

**Expected Result**:
- A file save dialog appears
- File format options include PNG, JPEG, and BMP
- After saving, a success message appears
- The saved file contains the zoomed and modified image
- The file can be opened in other image viewers

### Test 9: Multiple Zoom Windows
**Objective**: Test opening multiple zoom windows simultaneously

**Steps**:
1. Open an image
2. Select and open first zoom window
3. Return to main window
4. Select a different region and open second zoom window
5. Verify both windows remain functional

**Expected Result**:
- Multiple zoom windows can be open simultaneously
- Each window is independent
- Modifications in one window don't affect others
- Each window can be closed independently

### Test 10: Edge Cases

#### Test 10a: Small Selections
**Steps**: Try to select a region smaller than 5x5 pixels

**Expected Result**: No zoom window opens (selection too small)

#### Test 10b: Selection Outside Image Bounds
**Steps**: Try to drag selection partially outside the image

**Expected Result**: Selection is clipped to image bounds

#### Test 10c: Zoom at Maximum Scale
**Steps**: Set zoom to 10.0x (maximum)

**Expected Result**: Image scales correctly, may show pixelation (expected)

#### Test 10d: Brush at Different Zoom Levels
**Steps**: 
1. Enable brush and draw at 2.0x zoom
2. Change zoom to 5.0x
3. Draw again

**Expected Result**: 
- Brush size scales appropriately
- Minimum brush size is 1 pixel on cropped image
- Drawing is accurate at all zoom levels

### Test 11: Window Cleanup
**Objective**: Test memory management

**Steps**:
1. Open multiple zoom windows
2. Close each window using the X button
3. Monitor for any crashes or memory leaks

**Expected Result**:
- Windows close cleanly
- No crashes or errors
- Memory is freed (Qt::WA_DeleteOnClose attribute)

## Automated Build Test

### Build Verification
```bash
# Clean previous build
make clean
rm -f Makefile

# Generate new Makefile
qmake6 ImageProcessor.pro

# Build project
make

# Verify executable
ls -lh ImageProcessor
file ImageProcessor
```

**Expected Result**:
- Build completes without errors
- Only deprecation warnings from existing code (not from new changes)
- Executable is created successfully

## Test Results Summary

### Build Status
✅ **PASSED** - Project builds successfully with Qt 6
- No compilation errors
- Deprecation warnings are from existing code only
- Executable created: ImageProcessor (117KB)

### Code Quality
✅ **PASSED** - Code review feedback addressed:
- Uses qRound() for coordinate scaling
- Minimum brush size of 1 pixel enforced
- Magic numbers replaced with named constants
- No CodeQL security issues detected

### Feature Completeness
✅ **IMPLEMENTED** - All required features:
- Region selection via mouse dragging
- Customizable zoom scale (1.0x to 10.0x)
- New window for zoomed image display
- Save As functionality (PNG, JPEG, BMP)
- Brush tool with:
  - Color selection
  - Adjustable size
  - Drawing capability

## Known Limitations
1. The existing code uses deprecated Qt 6 methods (event->x() and event->y())
   - These warnings are from original code, not from new changes
   - Should be addressed separately to use event->position()

2. The application requires a GUI environment to run
   - Cannot be tested in headless mode without modifications

3. Selection rectangle visual feedback uses the same image label
   - More advanced UI might use overlay with QGraphicsScene

## Recommendations for Future Enhancements
1. Add undo/redo functionality for brush strokes
2. Add more drawing tools (rectangle, ellipse, fill)
3. Add eraser tool
4. Add brush transparency/opacity control
5. Save original image with modifications applied
6. Add keyboard shortcuts for common operations
7. Add zoom to cursor position instead of center
8. Add pan functionality for zoomed view

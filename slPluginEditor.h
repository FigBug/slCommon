#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "slProcessor.h"
#include "slComponents.h"
#include "slLookAndFeel.h"

//==============================================================================
class slAudioProcessorEditor : public AudioProcessorEditor,
                               protected Button::Listener,
                               protected ComboBox::Listener
{
public:
    slAudioProcessorEditor (slProcessor&, int cx = 100, int cy = 100) noexcept;
    
protected:
    Rectangle<int> getControlsArea();
    
    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* b) override;
    void comboBoxChanged (ComboBox* c) override;
    
    Rectangle<int> getGridArea (int x, int y, int w = 1, int h = 1);
    void setGridSize (int x, int y);
    
    slProcessor& slProc;
    const int cx;
    const int cy;
    
    int headerHeight = 60;
    int inset = 4;
    
    OwnedArray<ParamComponent> controls;

    ParamComponent* componentForId (const String& uid);
    
    slLookAndFeel lf;
    
    ComboBox programs;
    TextButton addButton {"A"};
    TextButton deleteButton {"D"};
    TextButton helpButton {"H"};
    
    String additionalProgramming;
    
    void refreshPrograms();
};

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "slProcessor.h"
#include "slComponents.h"
#include "slLookAndFeel.h"

//==============================================================================
class slAudioProcessorEditor;
class UpdateChecker : public Timer,
                      public Thread
{
public:
    UpdateChecker (slAudioProcessorEditor& editor_);
    ~UpdateChecker();
    
    void timerCallback() override;
    void run() override;
    
    slAudioProcessorEditor& editor;
};

//==============================================================================
class NewsChecker : public Timer,
                    public Thread
{
public:
    NewsChecker (slAudioProcessorEditor& editor_);
    ~NewsChecker();
    
    void timerCallback() override;
    void run() override;
    
    slAudioProcessorEditor& editor;
};

//==============================================================================
class slAudioProcessorEditorBase : public AudioProcessorEditor
{
public:
    slAudioProcessorEditorBase (slProcessor& p) : AudioProcessorEditor (p), proc (p)
    {
    }
    
    void makeResizable (int minX, int minY, int maxX, int maxY)
    {
        addAndMakeVisible (resizer = new ResizableCornerComponent (this, &resizeLimits));
        resizeLimits.setSizeLimits (minX, minY, maxX, maxY);
        
        ValueTree state (proc.state);
        
        if (state.hasProperty ("width") && state.hasProperty ("height"))
            setSize (state["width"], state["height"]);
        
        resized();
    }
    
    void resized() override
    {
        const Rectangle<int> r (getLocalBounds());
        
        if (resizer != nullptr)
        {
            resizer->setBounds (Rectangle<int> (r).removeFromRight (15).removeFromBottom (15));
            
            proc.state.setProperty ("width", getWidth(), nullptr);
            proc.state.setProperty ("height", getHeight(), nullptr);
        }
    }

    ComponentBoundsConstrainer resizeLimits;
    
private:
    slProcessor& proc;
    ScopedPointer<ResizableCornerComponent> resizer;
};

//==============================================================================
class slAudioProcessorEditor : public slAudioProcessorEditorBase,
                               protected Button::Listener,
                               protected ComboBox::Listener
{
public:
    slAudioProcessorEditor (slProcessor&, int cx = 100, int cy = 100) noexcept;
    ~slAudioProcessorEditor();
    
    void updateReady (String updateUrl);
    void newsReady (String newsUrl);

    slProcessor& slProc;
    
protected:
    virtual Rectangle<int> getControlsArea();
    
    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* b) override;
    void comboBoxChanged (ComboBox* c) override;
    
    virtual Rectangle<int> getGridArea (int x, int y, int w = 1, int h = 1);
    void setGridSize (int x, int y);
    
    const int cx;
    const int cy;
    
    int headerHeight = 60;
    int inset = 4;
    
    ScopedPointer<UpdateChecker> updateChecker;
    ScopedPointer<NewsChecker> newsChecker;
    
    OwnedArray<ParamComponent> controls;

    ParamComponent* componentForId (const String& uid);
    
    slLookAndFeel lf;
    
    ComboBox programs;
    TextButton addButton {"A"};
    TextButton deleteButton {"D"};
    TextButton socaButton {"S"};
    TextButton helpButton {"H"};
    TextButton newsButton {"N"};
    TextButton updateButton {"U"};
    
    SharedResourcePointer<TooltipWindow> tooltipWindow;
    
    String additionalProgramming;
    String updateUrl;
    String newsUrl;
    
    void refreshPrograms();
};

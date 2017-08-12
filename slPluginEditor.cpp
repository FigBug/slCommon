#include "slPluginEditor.h"

slAudioProcessorEditor::slAudioProcessorEditor (slProcessor& p, int cx_, int cy_) noexcept
  : AudioProcessorEditor (p), slProc (p), cx (cx_), cy (cy_)
{
    setLookAndFeel (&lf);
    
    addAndMakeVisible (&programs);
    addAndMakeVisible (&addButton);
    addAndMakeVisible (&deleteButton);
    addAndMakeVisible (&helpButton);

    programs.addListener (this);
    addButton.addListener (this);
    deleteButton.addListener (this);
    helpButton.addListener (this);
    
    refreshPrograms();
}

void slAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (Colours::black);
}

void slAudioProcessorEditor::resized()
{
    const int pw = 100;
    const int ph = 20;
    const int tw = pw + ph + ph + 10;
    
    programs.setBounds (getWidth() / 2 - tw / 2, 30, pw, ph);
    addButton.setBounds (programs.getRight() + 5, programs.getY(), ph, ph);
    deleteButton.setBounds (addButton.getRight() + 5, programs.getY(), ph, ph);
    
    helpButton.setBounds (getWidth() - ph - 5, 5, ph, ph);
}

Rectangle<int> slAudioProcessorEditor::getControlsArea()
{
    return getLocalBounds();
}

Rectangle<int> slAudioProcessorEditor::getGridArea (int x, int y, int w, int h)
{
    return Rectangle<int> (inset + x * cx, headerHeight + y * cy + inset, w * cx, h * cy);
}

ParamComponent* slAudioProcessorEditor::componentForId (const String& uid)
{
    for (auto* c : controls)
    {
        if (c->getUid() == uid)
            return c;
    }
    return nullptr;
}

void slAudioProcessorEditor::setGridSize (int x, int y)
{
    setSize (x * cx + inset * 2, y * cy + inset * 2 + headerHeight);
}

void slAudioProcessorEditor::refreshPrograms()
{
    programs.clear();
    
    for (int i = 0; i < processor.getNumPrograms(); i++)
        programs.addItem (processor.getProgramName (i), i + 1);
    
    programs.setSelectedItemIndex (slProc.getCurrentProgram(), dontSendNotification);
    deleteButton.setEnabled (slProc.getCurrentProgram() != 0);
}

void slAudioProcessorEditor::buttonClicked (Button* b)
{
    if (b == &addButton)
    {
        AlertWindow w ("", "Create preset", AlertWindow::NoIcon, this);
        w.addTextEditor ("name", "", "Name:");
        w.addButton ("OK", 1);
        w.addButton ("Cancel", 0);
        w.setLookAndFeel (&lf);
        
        if (w.runModalLoop())
        {
            String txt = File::createLegalFileName (w.getTextEditor ("name")->getText());
            if (txt.isNotEmpty())
            {
                slProc.saveProgram (txt);
                refreshPrograms();
            }
        }
    }
    else if (b == &deleteButton)
    {
        AlertWindow w ("", "Delete preset '" + processor.getProgramName (programs.getSelectedItemIndex()) + "'?", AlertWindow::NoIcon, this);
        w.addButton ("Yes", 1);
        w.addButton ("No", 0);
        w.setLookAndFeel (&lf);
        
        if (w.runModalLoop())
        {
            slProc.deleteProgram (programs.getSelectedItemIndex());
            refreshPrograms();
        }
    }
    else if (b == &helpButton)
    {
        String msg;
        
        msg += JucePlugin_Name " v" JucePlugin_VersionString "\n\n";
        msg += "Programming:\nRoland Rabien\nDavid Rowland\nROLI JUCE Framework\n";
        if (additionalProgramming.isNotEmpty())
            msg += additionalProgramming;
        msg += "\n\n";
        msg += "Copyright ";
        msg += String (__DATE__ + 7);
        
        AlertWindow w ("---- About ----", msg, AlertWindow::NoIcon, this);
        w.addButton ("OK", 1);
        w.setLookAndFeel (&lf);
        
        w.runModalLoop();
    }
}

void slAudioProcessorEditor::comboBoxChanged (ComboBox* c)
{
    int idx = programs.getSelectedItemIndex();
    deleteButton.setEnabled (idx != 0);
    processor.setCurrentProgram (idx);
}

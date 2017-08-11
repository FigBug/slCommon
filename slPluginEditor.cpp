#include "slPluginEditor.h"

slAudioProcessorEditor::slAudioProcessorEditor (slProcessor& p, int cx_, int cy_) noexcept
  : AudioProcessorEditor (p), slProc (p), cx (cx_), cy (cy_)
{
    setLookAndFeel (&lf);
    
    addAndMakeVisible (&programs);
    addAndMakeVisible (&addButton);
    addAndMakeVisible (&deleteButton);

    programs.addListener (this);
    addButton.addListener (this);
    deleteButton.addListener (this);
    
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
    addButton.setBounds (programs.getRight() + 5, programs.getY(), programs.getHeight(), programs.getHeight());
    deleteButton.setBounds (addButton.getRight() + 5, programs.getY(), programs.getHeight(), programs.getHeight());
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
}

void slAudioProcessorEditor::comboBoxChanged (ComboBox* c)
{
    int idx = programs.getSelectedItemIndex();
    deleteButton.setEnabled (idx != 0);
    processor.setCurrentProgram (idx);
}

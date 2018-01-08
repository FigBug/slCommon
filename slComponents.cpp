/*
  ==============================================================================

    slComponents.cpp
    Created: 3 Nov 2015 9:33:48am
    Author:  Roland Rabien

  ==============================================================================
*/

#include "slComponents.h"

//==============================================================================
Readout::Readout (slParameter* parameter_)
  : parameter (parameter_)
{
    parameter->addListener (this);
    setText (parameter->getUserValueText(), dontSendNotification);
    
    addChildComponent (editor);
    editor.addListener (this);
    editor.setOpaque (false);
    
    setBorderSize (BorderSize<int> (0));
}

Readout::~Readout()
{
    parameter->removeListener (this);
}

void Readout::parameterChanged (slParameter*)
{
    setText (parameter->getUserValueText(), dontSendNotification);
}

void Readout::paint (Graphics& g)
{
    if (! editing)
        Label::paint (g);
}

void Readout::resized()
{
    editor.setBounds (getLocalBounds());
}

void Readout::mouseDown (const MouseEvent&)
{
    editing = true;
    editor.setVisible (true);
    editor.setFont (getFont());
    editor.setText (parameter->getUserValueText(), dontSendNotification);
    editor.setJustificationType (getJustificationType());
    editor.setSelectAllWhenFocused (true);
    editor.toFront (true);
    repaint();
}

void Readout::textEditorReturnKeyPressed (SingleLineTextEditor&)
{
    float v = editor.getText().getFloatValue();
    parameter->setUserValueAsUserAction (v);
    
    editor.setVisible (false);
    editing = false;
    repaint();
}

void Readout::textEditorEscapeKeyPressed (SingleLineTextEditor&)
{
    editor.setVisible (false);
    editing = false;
    repaint();
}

void Readout::textEditorFocusLost (SingleLineTextEditor&)
{
    float v = editor.getText().getFloatValue();
    parameter->setUserValueAsUserAction (v);
    
    editor.setVisible (false);
    editing = false;
    repaint();
}

//==============================================================================
ParamComponent::ParamComponent (slParameter* parameter_)
  : Component (""), parameter (parameter_)
{
    
}
//==============================================================================
Knob::Knob (slParameter* parameter, bool fromCentre)
  : ParamComponent (parameter),
    value (parameter),
    knob (parameter, Slider::RotaryHorizontalVerticalDrag, Slider::NoTextBox)
{
    addAndMakeVisible (name);
    addAndMakeVisible (value);
    addAndMakeVisible (knob);
 
    knob.setSkewFactor (parameter->getSkew());
    if (fromCentre)
        knob.getProperties().set ("fromCentre", true);
    
    name.setText (parameter->getShortName(), dontSendNotification);
    value.setJustificationType (Justification::centredTop);
    name.setJustificationType (Justification::centredBottom);
    
    value.setFont (value.getFont().withHeight (15.0));
}

void Knob::resized()
{
    Rectangle<int> r = getLocalBounds().reduced (4);
    
    name.setBounds (r.removeFromTop (20));
    value.setBounds (r.removeFromBottom (20));
    knob.setBounds (r.reduced (2));
}
//==============================================================================
Switch::Switch (slParameter* parameter)
  : ParamComponent (parameter),
    button (parameter)
{
    addAndMakeVisible (&name);
    addAndMakeVisible (&button);
    
    name.setText (parameter->getShortName(), dontSendNotification);
    name.setJustificationType (Justification::centred);
}

void Switch::resized()
{
    Rectangle<int> r = getLocalBounds().withSizeKeepingCentre (getWidth() - 10, 20);
    
    button.setBounds (r);
    name.setBounds (r.translated (0, -20));
    
    int y = name.getY();
    if (y <= 0)
    {
        button.setTopLeftPosition (button.getX(), button.getY() + -y);
        name.setTopLeftPosition (name.getX(), name.getY() + -y);
    }

}

//==============================================================================
Select::Select (slParameter* parameter)
  : ParamComponent(parameter),
    comboBox (parameter)
{
    addAndMakeVisible (&name);
    addAndMakeVisible (&comboBox);

    name.setText (parameter->getShortName(), dontSendNotification);
    name.setJustificationType (Justification::centred);
}

void Select::resized()
{
    Rectangle<int> r = getLocalBounds().withSizeKeepingCentre (getWidth() - 10, 20);
    
    comboBox.setBounds (r);
    name.setBounds (r.translated (0, -20));
    
    int y = name.getY();
    if (y <= 0)
    {
        comboBox.setTopLeftPosition (comboBox.getX(), comboBox.getY() + -y);
        name.setTopLeftPosition (name.getX(), name.getY() + -y);
    }
}

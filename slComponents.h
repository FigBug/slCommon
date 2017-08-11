#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "slParameter.h"

//==============================================================================
class PluginButton : public TextButton,
                     private slParameter::Listener
{
public:
    PluginButton (slParameter* parameter_)
      : parameter (parameter_)
    {
        setButtonText (parameter->getUserValueText());
        setToggleState (parameter->getUserValue() > 0.0f, dontSendNotification);
        
        parameter->addListener (this);
    }
    
    ~PluginButton()
    {
        parameter->removeListener (this);
    }
    
    void parameterChanged (slParameter*) override
    {
        setToggleState (parameter->getUserValue() > 0.0f, dontSendNotification);
        setButtonText (parameter->getUserValueText());
    }

    void clicked() override
    {
        parameter->beginUserAction();
        parameter->setUserValueNotifingHost (parameter->getUserValue() > 0.0f ? 0.0f : 1.0f);
        parameter->endUserAction();
        setButtonText (parameter->getUserValueText());
    }
    
    slParameter* parameter;
};
//==============================================================================
class PluginComboBox : public ComboBox,
                       private slParameter::Listener,
                       private ComboBox::Listener
{
public:
    PluginComboBox (slParameter* parameter_)
      : parameter (parameter_)
    {
        for (int i = 0; i <= parameter->getUserRangeEnd(); i++)
            addItem (parameter->userValueToText (i), i + 1);
        
        setSelectedItemIndex (parameter->getUserValue(), dontSendNotification);
        
        parameter->addListener (this);
        addListener (this);
    }
    
    ~PluginComboBox()
    {
        parameter->removeListener (this);
    }
    
    void parameterChanged (slParameter*) override
    {
        setSelectedItemIndex (parameter->getUserValue(), dontSendNotification);
    }
    
    void comboBoxChanged (ComboBox*) override
    {
        parameter->beginUserAction();
        parameter->setUserValueNotifingHost (getSelectedItemIndex());
        parameter->endUserAction();
    }
    
    slParameter* parameter;
};
//==============================================================================
class PluginSlider : public Slider,
                     private Slider::Listener,
                     private slParameter::Listener
{
public:
    PluginSlider (slParameter* parameter_, SliderStyle style, TextEntryBoxPosition textBoxPosition)
      : Slider (style, textBoxPosition), parameter (parameter_)
    {
        addListener (this);
        setRange (parameter->getUserRangeStart(), parameter->getUserRangeEnd());
        setValue (parameter->getUserValue(), juce::dontSendNotification);
        
        parameter->addListener (this);
    }
    
    ~PluginSlider()
    {
        parameter->removeListener (this);
    }
    
    void sliderValueChanged (Slider*) override
    {
        parameter->setUserValueNotifingHost (float (getValue()));
        setValue (parameter->getUserValue());
    }
    
    void sliderDragStarted (Slider*) override
    {
        parameter->beginUserAction();
    }
    
    void sliderDragEnded (Slider*) override
    {
        parameter->endUserAction();
    }
    
    void parameterChanged (slParameter*) override
    {
        setValue (parameter->getUserValue(), dontSendNotification);
    }
    
    void mouseDown (const MouseEvent& e) override
    {
        if (e.mods.isAltDown())
            parameter->setUserValueNotifingHost (parameter->getUserDefaultValue());
        else if (e.mods.isLeftButtonDown())
            Slider::mouseDown (e);
    }
    
    void mouseUp (const MouseEvent& e) override
    {
        if (e.mods.isLeftButtonDown())
            Slider::mouseUp (e);
    }
    
    void mouseDrag (const MouseEvent& e) override
    {
        if (e.mods.isLeftButtonDown())
            Slider::mouseDrag (e);
    }
    
private:
    slParameter* parameter;
};

//==============================================================================
class Readout : public Label,
                private slParameter::Listener
{
public:
    Readout (slParameter* parameter);
    ~Readout();
    
private:
    void parameterChanged (slParameter* source) override;
    
    slParameter* parameter;
};

//==============================================================================
class ParamComponent : public Component
{
public:
    ParamComponent (slParameter* parameter);
    
    String getUid() { return parameter->getUid(); }
    
private:
    slParameter* parameter;
};

//==============================================================================
class Knob : public ParamComponent
{
public:
    Knob (slParameter* parameter);
    
private:
    void resized() override;
    
    Label name;
    Readout value;
    PluginSlider knob;
};
//==============================================================================
class Switch : public ParamComponent
{
public:
    Switch (slParameter* parameter);
    
private:
    void resized() override;
    
    Label name;
    PluginButton button;
};
//==============================================================================
class Select : public ParamComponent
{
public:
    Select (slParameter* parameter);
    
private:
    void resized() override;
    
    Label name;
    PluginComboBox comboBox;
};

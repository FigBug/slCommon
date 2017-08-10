#pragma once

#include "slParameter.h"
#include "slProgram.h"
#include "slUtil.h"

#include <map>

//==============================================================================
/**
*/
class slProcessor : public AudioProcessor,
                    public ChangeBroadcaster
{
public:
    //==============================================================================
    slProcessor();
    ~slProcessor();
        
    //==============================================================================
    void addPluginParameter (slParameter* parameter);
    slParameter* getParameter (const String& uid);
    float parameterValue (const String& uid);
    int parameterIntValue (const String& uid);
    Array<slParameter*> getPluginParameters();
    
    File getProgramDirectory();
    File getSettingsFile();
    
    void loadAllPrograms();

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;
    void saveProgram (String name);
    void deleteProgram (int index);

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================

public:
    OwnedArray<LevelTracker> inputLevels;
    OwnedArray<LevelTracker> outputLevels;
    
    ScopedPointer<PropertiesFile> properties;
    
    std::map<String, slParameter*> parameterMap;
    
private:
    void updateParams();
    
    LookAndFeel_V3 lookAndFeel;
    
    int currentProgram {0};
    OwnedArray<slProgram> programs;
    
    ValueTree state;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (slProcessor)
};


#include "includes.h"
#include "TalComponent.h"

TalComponent::TalComponent (TalCore* const ownerFilter)
: AudioProcessorEditor (ownerFilter)
{
}

TalComponent::~TalComponent()
{
    // deleteAllChildren();
    // getProcessor()->removeChangeListener (this);
}

//==============================================================================
void TalComponent::paint (Graphics& g)
{
    // just clear the window
    g.fillAll (Colour((const uint8)44, (const uint8)40, (const uint8)40, (const uint8)255));
    // this->accordeonTabContainer->repaint();
}

void TalComponent::resized()
{
}

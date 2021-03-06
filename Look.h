//--------//--------//--------//--------//--------//--------//--------//--------
/*
  ==============================================================================

   This file follows the same open source licensing as the JUCE library.
   Copyright (c) 2016 John Flynn

   Permission is granted to use this software under the terms of either:
   a) the GPL v2 (or any later version)
   b) the Affero GPL v3

   Details of these licenses can be found at: www.gnu.org/licenses

   Judio is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

  ==============================================================================
*/
//--------//--------//--------//--------//--------//--------//--------//--------

#ifndef LOOK_H_INCLUDED
#define LOOK_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

namespace jdo
{

//==============================================================================
/** Custom LookAndFeel class for uniform styling across plugins                    // Slider stylings set in SliderStep not here!!!

    @see juce::LookAndFeel
*/
class CustomLook  : public LookAndFeel_V3
{
public:
    CustomLook();
    ~CustomLook() {}

    void drawRotarySlider (Graphics& g, int x, int y, int width, int height, float sliderPos0to1,
                           float rotaryStartAngle, float rotaryEndAngle, Slider& slider) override;

    Slider::SliderLayout getSliderLayout (Slider& slider) override;

    Label* createSliderTextBox (Slider&) override;

    //==============================================================================    // very hacky now!
    void drawButtonBackground (Graphics&, Button&, const Colour& backgroundColour,      // make button/toggle
                           bool isMouseOverButton, bool isButtonDown) override;         // more generic

    void drawToggleButton (Graphics&, ToggleButton&, bool isMouseOverButton, bool isButtonDown) override;

    //==============================================================================
    virtual Typeface::Ptr getTypefaceForFont (const Font&) override;

    Font getTextButtonFont (TextButton&, int buttonHeight) override;
    Font getComboBoxFont (ComboBox&) override;
    Font getPopupMenuFont() override;

    //==============================================================================
    static const int buttonWidth       {55};
    static const int buttonHeight      {22};
    static const int buttonLargeHeight {42};

private:
    const float fontSize = 13.0f;

    Image knob       {ImageCache::getFromMemory (BinaryData::layout04knob01onfs8_png,
                                                 BinaryData::layout04knob01onfs8_pngSize)};
    Image knobMarker {ImageCache::getFromMemory (BinaryData::layout04knob01dotonfs8_png,
                                                 BinaryData::layout04knob01dotonfs8_pngSize)};

    class SliderLabelComp;
};

//==============================================================================
} // namespace jdo

#endif  // LOOK_H_INCLUDED

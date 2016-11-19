
#include "../JuceLibraryCode/JuceHeader.h"
#include "Aidio/Aidio.h"

#include "Judio/Helper.h"

int main (int argc, const char* argv[])
{
    juce::UnitTestRunner runner;
    runner.runAllTests();

    return 0;
}

//--------//--------//--------//--------//--------//--------//--------//--------

AIDIO_DECLARE_UNIT_TEST_WITH_STATIC_INSTANCE(Helper)

Helper::Helper() : UnitTest ("Helper") {}

void Helper::runTest()
{
    beginTest ("bufferFillAllOnes()");

    {
        AudioBuffer<float> buffer {2, 32};
        jdo::bufferFillAllOnes (buffer);
        expectEquals (buffer.getSample(1, 31), 1.0f);
    }

    beginTest ("bufferSumElements()");

    {
        AudioBuffer<float> buffer {1, 500};
        jdo::bufferFillAllOnes (buffer);
        const float sum {jdo::bufferSumElements(buffer)};
        expectEquals (sum, 500.0f);
    }

    beginTest ("loadBufferFromWavBinaryData()");

    {
        ado::Buffer buffer {1,1};
        jdo::bufferLoadFromWavBinaryData(BinaryData::Stereo64SamplesAllOnes_wav,
                                         BinaryData::Stereo64SamplesAllOnes_wavSize,
                                         buffer);
        float sum {ado::rawBufferSum (buffer.getReadArray(), buffer.numChannels(), buffer.numSamples())};
        expectWithinAbsoluteError (sum, 128.0f, 0.001f);
        jdo::bufferLoadFromWavBinaryData(BinaryData::Stereo64SamplesAllOnes_wav,
                                         BinaryData::Stereo64SamplesAllOnes_wavSize,
                                         buffer);
        sum = ado::rawBufferSum (buffer.getReadArray(), buffer.numChannels(), buffer.numSamples());
        expectWithinAbsoluteError (sum, 128.0f, 0.001f);
    }

    beginTest ("nextPowerOf2()");

    {
        int x = 55;
        expectEquals(jdo::nextPowerOf2(x), 64);
        x = -234;
        expectEquals(jdo::nextPowerOf2(x), 1);
        x = 16300;
        expectEquals(jdo::nextPowerOf2(x), 16384);
    }
}

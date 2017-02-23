/*
  ==============================================================================

    Buffer.cpp
    Created: 11 Jan 2017 2:48:52pm
    Author:  John Flynn

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "../Aidio.h"

//==============================================================================

#if AIDIO_UNIT_TESTS

AIDIO_DECLARE_UNIT_TEST_WITH_STATIC_INSTANCE(Buffer)

Buffer::Buffer() : UnitTest ("Buffer") {}

void Buffer::runTest()
{
    beginTest ("Bufferview");

    {
        juce::AudioBuffer<float> b1 {2, 8};
        ado::BufferView buffer1 {ado::makeBufferView (b1)};
        buffer1.fillAllOnes();
        buffer1.fillAscending();

        expectEquals<float>(buffer1(0,7), 8);
    }

    beginTest ("Bufferview Test subtle pointer weirdness due to makeBufferView()");

    {
        juce::AudioBuffer<float> b1 {2, 8};
        ado::BufferView buffer1 {ado::makeBufferView (b1)};

        buffer1.fillAllOnes();

        juce::AudioBuffer<float> b2 {1, 8};
        ado::BufferView buffer2 {ado::makeBufferView(b2)};

        buffer2.fillAllOnes();
        buffer2.fillAllOnes();
        buffer1.fillAscending();

        expectEquals<float>(buffer1(0,7), 8);
        expectEquals<float>(buffer2(0,5), 1);
    }

    beginTest ("Bufferview test clear");

    {
        juce::AudioBuffer<float> b1 {6, 32};
        ado::BufferView bv {ado::makeBufferView (b1)};

        bv.clear();

        for (auto chan : bv)
            for (const auto& samp : chan)
                expectEquals<float>(samp, 0.0f);
    }

    beginTest ("Bufferview test writing in range for loops");

    {
        juce::AudioBuffer<float> b1 {6, 32};
        ado::BufferView bv {ado::makeBufferView (b1)};

        for (auto chan : bv)
            for (auto& samp : chan)
                samp = 3.245f;

        for (const auto& chan : bv)
            for (const auto& samp : chan)
                expectEquals<float>(samp, 3.245f);
    }

    beginTest ("Bufferview test write specific value");

    {
        juce::AudioBuffer<float> b1 {6, 32};
        ado::BufferView bv {ado::makeBufferView (b1)};
        
        bv.clear();
        bv(4,14) = 0.999f;

        expectEquals<float>(bv(4,13), 0);
        expectEquals<float>(bv(4,14), 0.999f);
        expectEquals<float>(bv(4,15), 0);
    }

    //==============================================================================

    beginTest ("Constructs");
    expectDoesNotThrow (ado::Buffer (2, 512));

    beginTest ("Must have channels");
    expectThrows (ado::Buffer (0, 512));

    beginTest ("Must have samples");
    expectThrows (ado::Buffer (2, 0));

    beginTest ("Must have +ve samples");
    expectThrows (ado::Buffer (2, -1));

    beginTest ("No ridiculous numChannels");
    expectThrows (ado::Buffer (size_t(2147483647)+12039812, 0));

    beginTest ("clearAndResize()");

    {
        ado::Buffer source7 {2, 12};
        source7.fillAscending();
        expectEquals (source7.getReadArray()[1][10], 11.f);
        source7.clearAndResize(4, 30);
        expectEquals (source7.getReadArray()[3][29], 0.f);
        source7.fillAscending();
        expectEquals (source7.getReadArray()[3][29], 30.f);
    }

    beginTest ("clearAndResize() with sampling rate");

    {
        ado::Buffer source7 {2, 12};
        expectEquals (source7.getSampleRate(), 44100);
        source7.fillAscending();
        expectEquals (source7.getReadArray()[1][10], 11.f);
        source7.clearAndResize(4, 30, 88200);
        expectEquals (source7.getReadArray()[3][29], 0.f);
        expectEquals (source7.getSampleRate(), 88200);
        source7.fillAscending();
        expectEquals (source7.getReadArray()[3][29], 30.f);
    }

    beginTest ("clearAndResize() must clear pointer array first");

    {
        ado::Buffer source7 {2, 12};
        source7.fillAscending();
        expectEquals (source7.getReadArray()[1][11], 12.f);
        source7.clearAndResize(4, 30);
        source7.fillAscending();
        expectEquals (source7.getReadArray()[2][29], 30.f);
        source7.clearAndResize(1, 9);
        expectEquals (source7.getReadArray()[0][7], 0.f);
    }

    beginTest ("numChannels()");

    {
        ado::Buffer b {4, 1024};
        expectEquals (b.getNumChannels(), 4);
    }

    beginTest ("numSamples()");

    {
        ado::Buffer b {4, 1024};
        expectEquals (b.getNumSamples(), 1024);
    }

    beginTest ("getWriteArray()");

    {
        ado::Buffer b {4, 1024};
        float** bb {b.getWriteArray()};
        int chan = 2;
        int samp = 385;
        expectEquals (bb[chan][samp], 0.0f);
        bb[chan][samp] = 0.239f;
        expectEquals (bb[chan][samp], 0.239f);
    }

    beginTest ("fillAllOnes()");

    {
        ado::Buffer b {4, 1024};
        expectDoesNotThrow (b.fillAllOnes());
        float elementSum = 0.0f;
        for (int i = 0; i < b.getNumSamples(); ++i)
            elementSum += b.getWriteArray()[0][i];
        expectEquals (elementSum, static_cast<float> (b.getNumSamples()));
    }

    beginTest ("fillAscending()");

    {
        ado::Buffer b {64, 4};
        expectDoesNotThrow (b.fillAscending());
        float elementSum = 0.0f;
        for (int i = 0; i < b.getNumSamples(); ++i)
            elementSum += b.getWriteArray()[0][i];
        expectEquals (elementSum, 1.f + 2.f + 3.f + 4.f);
    }

    beginTest ("Clear()");

    {
        ado::Buffer b {4, 1024};
        expectDoesNotThrow (b.fillAllOnes());
        expectDoesNotThrow (b.clear());
        float elementSum = 0.0f;
        for (int i = 0; i < b.getNumSamples(); ++i)
            elementSum += b.getWriteArray()[0][i];
        expectEquals (elementSum, 0.0f);
    }

    beginTest ("getSamplingRate()");

    {
        ado::Buffer b {4, 1024, 96000};
        const int rate = b.getSampleRate();
        expectEquals (rate, 96000);
    }

    beginTest ("bufferSumElements()");

    {
        ado::Buffer b {1, 5};
        b.fillAscending();
        const float sum {ado::bufferSumElements(b)};
        expectEquals (sum, 1.f + 2 + 3 + 4 + 5);
    }

    beginTest ("operator *= ()");

    {
        ado::Buffer b {1, 128};
        b.fillAllOnes();
        float sum {ado::bufferSumElements(b)};
        expectEquals (sum, 128.0f);
        b *= 0.5f;
        sum = ado::bufferSumElements(b);
        expectEquals (sum, 64.0f);
    }
}

#endif // AIDIO_UNIT_TESTS

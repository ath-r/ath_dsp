# ath_dsp

This library contains classes and routines for audio synthesis and signal processing, including DSP algorithms and mechanisms for controlling signal flow and application architecture. It is designed to be self-sufficient and independent of external frameworks such as JUCE, enabling use in projects built on alternative frameworks as well as on embedded platforms.

## File map:
* control
  * [Events.h](./control/Events.h)
    * Templated `EventOut` class for horizontal connection of processes.
  * [Midi.h](./control/Midi.h)
    * Classes and enums to represent different types of MIDI data and messages. Strongly typed.
  * [Parameter.h](./control/Parameter.h)
    * Framework-agnostic data container to describe plugin/processor parameters.
  * [VoiceManager.h](./control/VoiceManager.h)
    * Simple voice manager that connects with voice objects using horizontal events.
* dsp
  * [Context.h](./dsp/Context.h)
    * Data struct to communicate changes in sample rate. Provides a quick way to get the current sampling period $T$.
  * [Filter.h](./dsp/Filter.h) 
    * Contains a variety of useful IIR filters, companion methods, and transfer functions:
      * Naive digital filters
      * TPT filters
        * Low-pass
        * High-pass
        * SVF
      * Biquad
        * Biquad class with multiple possible topologies:
          * Direct Form I
          * Direct Form II
          * Transposed Direct Form I
          * Transposed Direct Form II
        * Biquad cascade
  * [FIR.h](./dsp/FIR.h)
    * Contains FIR filter classes and routines to calculate coefficients: 
      * FIR filter class with dynamic coefficient and buffer allocation
      * Windowed sinc lowpass generator
  * [PhaseCounter.h](./dsp/PhaseCounter.h)
    * A per-sample phase accumulator that tracks elapsed time, storing the current phase in seconds.
  * cv
    * [LinearSmoother.h](./dsp/cv/LinearSmoother.h)
      * Constant Rate Linear Smoother (slew limiter)
      * Constant Time Linear Smoother
* math
  * [Complex.h](./math/Complex.h)
    * Simple complex number template that works with both scalars and SIMD vectors.
  * [Math.h](./math/Math.h)
    * Basic math: `sign, abs, trunc, frac, max, min, clamp`
    * Powers of $x$ and their inverted easings, integer exponentiation
    * Linear interpolation, logarithmic interpolation in bases 2 and 10
    * Trigonometric function approximations, sinc, Dirichlet kernel, Chebyshev polynomials
    * Routines to convert between MIDI note numbers, Hz, and semitones; linear amplitude and dBs
  * [Polynomial.h](./math/Polynomial.h)
    * Polynomial class with compile-time order and antiderivative calculation (for ADAA)
  * [Random.h](./math/Random.h)
    * Random generator base class
    * Linear Congruential Generator (default parameters give periodicity $2^{32}$)
    * MT19937 Mersenne Twister ($2^{19937} − 1$ periodicity)
  * [Simd.h](./math/Simd.h)
    * SIMD classes for AVX2 instruction set. Includes `int4, int8, float4, float8` and companion mathematical routines.

declare name "Distributed WFS";
declare description "Basic WFS for a distributed setup consisting of modules that each handle two output channels.";
import("stdfaust.lib");
import("wfsParams.lib");

// Take each source...
process = par(i, N_SOURCES, _ <:
    // ...and distribute it across the speaker array for this module.
    speaker(x(i), y(i), ss1x, ss1y), speaker(x(i), y(i), ss2x, ss2y))
    // Merge onto the output speakers.
    :> par(i, SPEAKERS_PER_MODULE, _)
with{
    // Secondary source co-ordinates.
    ss1x = hslider("ss/0/x", 0, -MAX_X, MAX_X, .001);
    ss1y = hslider("ss/0/y", 0, -MAX_X, MAX_X, .001);
    ss2x = hslider("ss/1/x", 0, -MAX_X, MAX_X, .001);
    ss2y = hslider("ss/1/y", 0, -MAX_X, MAX_X, .001);

    // Virtual/focused source co-ordinates.
    x(p) = hslider("vs/%p/x", 0, -MAX_X, MAX_X, 0.001);
    y(p) = hslider("vs/%p/y", 0, MIN_Y, MAX_Y, 0.001);

    // Output for a given loudspeaker.
    speaker(vsx, vsy, ssx, ssy) = _ <:
        _,prefilter(hyp) : select2(vsy > 0) :
//        _,_ : select2(vsy > 0) :
        (fsDelay,vsDelay : select2(vsy > 0)),_ :
        de.fdelay(maxHyp * samplesPerMeter)
    with{
        maxHyp = MIN_Y^2 + (2 * MAX_X)^2 : sqrt;

        // Number of samples it takes sound to travel one meter.
        samplesPerMeter = ma.SR / CELERITY;

        cx = ssx - vsx;
        y = ssy - vsy;
        hyp = cx^2 + y^2 : sqrt;

        vsDelay = smallDelay
        with {
            smallDelay = (hyp - y) * samplesPerMeter;
        };

        fsDelay = (maxHyp - hyp) * samplesPerMeter;

        // Simulate distance by changing gain and applying a lowpass as a function
        // of distance
        prefilter(distance) = *(dGain) : fi.lowpass(2, fc)
        with{
            // Inverse square law: A(d) = 1 / d, using a minimum distance of 1 m.
            dGain = 1 / max(distance^(.5), 1);

            fc = dGain * 15000 + 5000;
        };
    };
};

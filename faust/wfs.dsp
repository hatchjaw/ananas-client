declare name "Distributed WFS";
declare description "Basic WFS for a distributed setup consisting of modules that each handle two output channels.";
import("stdfaust.lib");
import("wfsParams.lib");

nModules = N_SPEAKERS / SPEAKERS_PER_MODULE;

// Simulate distance by changing gain and applying a lowpass as a function
// of distance
distanceSim(distance) = *(dGain) : fi.lowpass(2, fc)
with{
    // Inverse square law: A(d) = 1 / d, using a minimum distance of 1 m.
    dGain = 1 / max(distance^(.5), 1);

    fc = dGain*15000 + 5000;
};

// Create a speaker array *perspective* for one source
// i.e. give each source a distance simulation and a delay
// relative to each speaker.
speakerArray(x, y, s, id) = _ <:
    par(i, SPEAKERS_PER_MODULE, _,distanceSim(hypotenuse(i)) : select2(y > 0) :
        (focusedSourceDelay(i),virtualSourceDelay(i) : select2(y > 0)),_ : de.fdelay(MAX_HYPOTENUSE * samplesPerMeter))
with{
    MAX_ARRAY_WIDTH = (N_SPEAKERS - 1) * MAX_SPEAKER_DIST;
    MAX_HYPOTENUSE = MIN_Y_DIST^2 + MAX_ARRAY_WIDTH^2 : sqrt;

    arrayWidth = (N_SPEAKERS - 1) * s;
    currentMaxHypotenuse = MIN_Y_DIST^2 + arrayWidth^2 : sqrt;
    // Number of samples it takes sound to travel one meter.
    samplesPerMeter = ma.SR/CELERITY;

    // y (front-to-back) is always just y, the longitudinal
    // distance of the source from the array.
    // Get x between the source and specific speaker in the array, i.e. the
    // cathetus on the x-axis of the right triangle described by y and the
    // speaker position.
    // E.g. for 16 speakers (8 modules), with a spacing, s, of .25 m:
    //      array width, w = (16-1) * .25 = 3.75,
    //        let module m = 2 (third module in array)
    //       let speaker j = 0 (first speaker in module)
    //               let x = 0 (m, relative to left of array)
    //                  cx = x - s * (m * 2 + j)
    //                     = 0 - .25 * (2 * 2 + 0)
    //                     = -.25 * 4
    //                     = -1
    //
    //               let m = 7, j = 1, x = 0
    //                  cx = 0 - .25 * (7 * 2 + 1) = -3.75
    //
    //               let m = 0, j = 0, x = 0
    //                  cx = 0 + .25 * (0 * 2 + 0) = 0
    cathetusX(k) = x - (s * (k + id * 2));
    hypotenuse(j) = cathetusX(j)^2 + y^2 : sqrt;

    virtualSourceDelay(k) = smallDelay(k)
    with {
        smallDelay(j) = (hypotenuse(j) - y) * samplesPerMeter;
    };

    focusedSourceDelay(k) = (currentMaxHypotenuse - hypotenuse(k)) * samplesPerMeter;
};

// Take each source...
sourcesArray(s) = par(i, ba.count(s), ba.take(i + 1, s) :
    // ...and distribute it across the speaker array for this module.
    speakerArray(x(i), y(i), spacing, moduleID))
    // Merge onto the output speakers.
    :> par(i, SPEAKERS_PER_MODULE, _)
with{
    globalGroup(x) = vgroup("Global settings", x);
    // Set which speakers to control. A module ID of -1 indicates that this
    // module's ID hasn't in fact been set at all (or has been superceded by
    // another module taking its place).
    moduleID = globalGroup(hslider("moduleID", -1, -1, nModules - 1, 1));
    // Set speaker spacing (m)
    spacing = globalGroup(hslider("spacing[unit:m]", .2, .05, MAX_SPEAKER_DIST, .01));

    maxX = spacing*(N_SPEAKERS-1);

    // Use normalised input co-ordinate space; scale to dimensions.
    x(p) = hslider("%p/x", 0, -1, 1, 0.001) : +(1) : /(2) : *(maxX);
    y(p) = yPos <: *(MAX_Y_DIST),*(-MIN_Y_DIST) : select2(yPos < 0)
    with {
        yPos = hslider("%p/y", 0, -1, 1, 0.001);
    };
};

// Distribute input channels (i.e. sources) across the sources array.
process = sourcesArray(par(i, N_SOURCES, _));

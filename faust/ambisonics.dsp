declare name        "Sampling Ambisonic Decoder";
declare version     "1.0";
declare author      "Pierre Lecomte";
declare license     "CC-BY-NC-SA-4.0";
declare copyright   "(c) Pierre Lecomte";
declare options "[osc:on]";

//###Sampling Ambisonic Decoder###
// This tool decodes a $L$-th degree Ambisonic sound scene on a $N$-node
// loudspeaker layout using the Sampling Ambisonic Decoder (SAD) technique:
// For each loudspeaker, the driving signal is calculated from a continuous
// distribution of loudspeaker[^lecomte], sampled at the loudspeaker position
// (hence the term "sampling decorder").
//
// For the $n$-th loudspeaker at position $(r_n, \theta_n, \phi_n)$, with
// $n \in \\{1, \cdots, N\\}$, the driving signal $s_n(z)$ is given by:
//
// $$\begin{equation}
// s_n(z) = \sum_{l=0}^L \frac{w_{\text{max-}r_E, l}(L)}{F_l(r_n, z)} \sum_{m=-l}^l Y_{l,m}(\theta_n, \phi_n) b_{l, m}(z)
// \label{eq:spk}
// \end{equation},$$
//
// where $w_{\text{max-}r_E}$ are the
// [max-$r_E$ weights]({% link docs/ylm.md %}#wrelmax-l1-l),
// $\frac{1}{F_l(r_n, z)}$ are the The Near Field Compensation
// [(NFC) filters]({% link docs/radial.md %}#near-field-compensation-filters)),
// $Y_{l,m}(\theta_n, \phi_n)$ are the Spherical Harmonics evaluated at
// loudspeaker direction and $b_{l,m}(z)$ are the Ambisonic components.
//
// The NFC filters can be activated or not at compilation time with parameter
// `nfcon`. If activated (`nfcon=1`), the gain attenuation and propagation delay
// between loudspeakers are as well equalized. If not activated (`nfcon=0`),
// $\frac{1}{F_l(r_n, z)} = 1$ in Eq. \eqref{eq:spk}.
//
//
// ## Compilation parameters
// - `L`: maximal Spherical Harmonics degree (i.e., Ambisonics order), $L > 0$,
// - `N`: number of loudspeakers, $N > 0$,
// - `nfcon`: activate or not NFC: `0` for no NFC, `1` for NFC.
// - `speaker(n) = (x, y, z)` $n$-th loudspeaker Cartesian coordinates in
//   meters. One loudspeaker per line.
//
// ## Inputs / Outputs
// - Inputs: $(L+1)^2$
// - Outputs: $N$
//
// ## User Interface
//
// |--------------------+------------+-----------+-----------+
// | Element            | OSC        | Min value | Max value |
// |--------------------|:----------:|:---------:|:---------:|
// | Outputs level (dB) | `levelout` | -70       | 6         |
// |--------------------+------------+-----------+-----------+
//
//
// [^lecomte]: P. Lecomte, P.-A. Gauthier, C. Langrenne, A. Berry, et A. Garcia,
// « A Fifty-Node Lebedev Grid and Its Applications to Ambisonics »,
// Journal of the Audio Engineering Society, vol. 64, nᵒ 11, p. 868‑881, 2016.
//######

import("stdfaust.lib");
import("grids.lib");
import("radial.lib");
import("ylm.lib");

// COMPILATION PARAMETERS
teensy_id = 1;
L	=	3; // Maximum degree $L$.
teensy_outs = 2;
N   =   16; // Total number of loudspeakers
nfcon = 1; // Include NFC (1=included, 0=not included)

// Loudspeakers Cartesian coordinates $(x, y, z)$ in meters.
speaker(0) = (0.71, 0, 0);
speaker(1) = (0.502, 0.502, 0);
speaker(2) = (0, 0.71, 0);
speaker(3) = (0.502, -0.502, 0);
speaker(4) = (-0.71, 0, 0);
speaker(5) = (-0.502, -0.502, 0);
speaker(6) = (0, -0.71, 0);
speaker(7) = (0.502, -0.502, 0);
speaker(8) = (0.4, 0, 0.62);
speaker(9) = (0.283, 0.283, 0.62);
speaker(10) = (0, 0.4, 0.62);
speaker(11) = (0.283, -0.283, 0.62);
speaker(12) = (0, -0.4, 0.62);
speaker(13) = (-0.283, -0.283, 0.62);
speaker(14) = (0, -0.4, 0.62);
speaker(15) = (0.283, -0.283, 0.62);

// DO NOT EDIT BELOW HERE

// Inputs/Outputs
ins	=	(L+1)^2;

rs = par(i, N, ba.take(1, cart2spher(ba.take(1, speaker(i)), ba.take(2, speaker(i)), ba.take(3, speaker(i)))));
rmax = sup(rs);

// User Interface
volout	=	hslider("Outputs Gain[unit:dB][style:knob][osc:/levelout -70 6]", 0, -70, 6, 0.1) : ba.db2linear;// : si.smoo;

row(i) = case{
                (0) => par(l, L + 1, par(m, 2 * l + 1, _ * ylm(l, m - l, t, p)) :> nfc(l,r) * wre(L, L, l) ) :>_ / rmax : de.delay(rmax/c, (rmax-r)/c); // normalize by rmax
                (1) => par(l, L + 1, par(m, 2 * l + 1, _ * ylm(l, m - l, t, p)) :> _* wre(L, L, l)) :>_;
                }(nfcon==0)
                with {
                    r = ba.take(1, cart2spher(ba.take(1, speaker(i)), ba.take(2, speaker(i)), ba.take(3, speaker(i))));
                    t = ba.take(2, cart2spher(ba.take(1, speaker(i)), ba.take(2, speaker(i)), ba.take(3, speaker(i))));
                    p = ba.take(3, cart2spher(ba.take(1, speaker(i)), ba.take(2, speaker(i)), ba.take(3, speaker(i))));
                    };

matrix	=	si.bus(ins)<:par(i, teensy_outs, row(i+(teensy_id*2)) * (volout));

process = matrix;

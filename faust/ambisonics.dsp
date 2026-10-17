declare name "Distributed HOA";
declare description "Third order HOA for a distributed setup consisting of modules that each handle two output channels.";

N = 16;
coeffsPerRow = 16;

coefficients0 = par(j, coeffsPerRow, hslider("dm/0/%j", 0, -10, 10, 1e-8));
coefficients1 = par(j, coeffsPerRow, hslider("dm/1/%j", 0, -10, 10, 1e-8));

process = par(i, N, _) <:
    (par(i, N, _),coefficients0 : linalg.dot(N)),
    (par(i, N, _),coefficients1 : linalg.dot(N));

routes = environment {
    // Differentiable analog to ro.interleave
    // interleave(2, 2) Maps input signals 1,2,3,4 to 1,3,2,4
    //
    // twotwo = _,_,_,_ <: _,!, _,!, !,_, !,_; // ro.interleave(2,2)
    // twothree = _,_,_,_,_,_ <: _,!, _,!, _,!, !,_, !,_, !,_; // ro.interleave(2,3)
    // threetwo = _,_,_,_,_,_ <: _,!,!, _,!,!, !,_,!, !,_,!, !,!,_, !,!,_; // ro.interleave(3,2)
    // fourtwo = _,_,_,_,_,_,_,_ <: _,!,!,!, _,!,!,!, !,_,!,!, !,_,!,!, !,!,_,!, !,!,_,!, !,!,!,_, !,!,!,_; // 4,2
    //
    // In general, R*C groups of R signals.
    // The rth signal in each of C subgroups passes; the rest are cut.
    interleave(1, C) = par(i, C, _);
    interleave(R, C) = par(i, R * C, _) <: par(r, R, par(c, C, distribute(r, 1, R - r - 1)));
} with {
    // Helper function for distributing groups of signals.
    // p: number of signals to cut at the start of the group.
    // q: number of signals to pass.
    // r: number of signals to cut at the end of the group.
    distribute(0, q, 0) = par(i, q, _);
    distribute(p, q, 0) = par(i, p, !),par(j, q, _);
    distribute(0, q, r) = par(i, q, _),par(j, r, !);
    distribute(p, q, r) = par(i, p, !),par(j, q, _),par(k, r, !);
};

linalg = environment {
    dot(N) = routes.interleave(N, 2) : par(i, N, *) :> _;
};

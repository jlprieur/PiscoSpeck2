/****************************************************************************
* Name: psc1_language.h
*
* JLP
* Version 26/07/2015
****************************************************************************/
#ifndef _psc1_language__
#define _psc1_language__

// agrave: 00E0
// acirc:  00E2
// ccedil: 00E7
// egrave: 00E8
// eacute: 00E9
// ecirc:  00EA
// euml:   00EB
// ntilde: 00F1

#define N_UNICODE 47
static char html_item1[][N_UNICODE] = {
// 18 latin letters with accents
"aacute",
"agrave",
"acirc",
"ccedil",
"egrave",
"eacute",
"Eacute",
"ecirc",
"euml",
"iacute",
"iuml",
"icirc",
"ntilde",
"oacute",
"ouml",
"uacute",
"uuml",
"ucirc",
// ligatures:
"szlig",
"oelig",
// 27 Greek letters:
"alpha",
"beta",
"gamma",
"Gamma",
"delta",
"Delta",
"epsilon",
"zeta",
"eta",
"theta",
"lambda",
"Lambda",
"mu",
"nu",
"xi",
"pi",
"rho",
"sigma",
"Sigma",
"tau",
"phi",
"chi",
"Phi",
"psi",
"Psi",
"omega",
"Omega"
};

static wxString unicode_item1[N_UNICODE] = {
// "aacute",
L"\u00E1",
// "agrave",
L"\u00E0",
// "acirc",
L"\u00E2",
// "ccedil",
L"\u00E7",
// "egrave",
L"\u00E8",
// "eacute",
L"\u00E9",
// "Eacute",
L"\u00C9",
// "ecirc",
L"\u00EA",
// "euml",
L"\u00EB",
// "iacute",
L"\u00ED",
// "iuml",
L"\u00EF",
// "icirc",
L"\u00EE",
// "ntilde",
L"\u00F1",
// "oacute",
L"\u00F3",
// "ouml",
L"\u00F6",
// "uacute",
L"\u00FA",
// "uuml",
L"\u00FC",
// "ucirc",
L"\u00FB",
// ligatures:
// "szlig",
L"\u00DF",
// "oelig",
L"\u0153",
// 26 Greek letters:
// "alpha",
L"\u03B1",
// "beta",
L"\u03B2",
// "gamma",
L"\u03B3",
// "Gamma",
L"\u0393",
// "delta",
L"\u03B4",
// "Delta",
L"\u0394",
// "epsilon",
L"\u03B5",
// "zeta",
L"\u03B6",
// "eta",
L"\u03B7",
// "theta",
L"\u03B8",
// "lambda",
L"\u03BC",
// "Lambda",
L"\u039B",
// "mu",
L"\u03BC",
// "nu",
L"\u03BD",
// "xi",
L"\u03BE",
// "pi",
L"\u03C0",
// "rho",
L"\u03C1",
// "sigma",
L"\u03C3",
// "Sigma",
L"\u03A3",
// "tau",
L"\u03C4",
// "phi",
L"\u03C6",
// "chi",
L"\u03C7",
// "Phi",
L"\u03A6",
// "psi",
L"\u03C8",
// "Psi",
L"\u03A8",
// "omega",
L"\u03C9",
// "Omega"
L"\u03A9"
};

#endif

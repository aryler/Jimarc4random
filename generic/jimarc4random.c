/*
 * Copyright (c) 2014 Stuart Cassoff <stwo@users.sourceforge.net>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */


/*
 * jimarc4random.c
 *
 * Jim interface to arc4random(3).
 *
 */


#ifdef __cplusplus
extern "C" {
#endif


#include <stdlib.h>      /* arc4random(3) */
#include <jim-subcmd.h>


static int Ja4r_Arc4random_Random  (Jim_Interp *interp, int argc, Jim_Obj *const *argv);
static int Ja4r_Arc4random_Bytes   (Jim_Interp *interp, int argc, Jim_Obj *const *argv);
static int Ja4r_Arc4random_Uniform (Jim_Interp *interp, int argc, Jim_Obj *const *argv);

static const jim_subcmd_type Ja4r_Cmds[] = {
	{ "random"  , NULL         , Ja4r_Arc4random_Random  , 0, 0, 0 },
	{ "bytes"   , "nbytes"     , Ja4r_Arc4random_Bytes   , 1, 1, 0 },
	{ "uniform" , "upperbound" , Ja4r_Arc4random_Uniform , 1, 1, 0 },
	{ NULL      , NULL         , NULL                    , 0, 0, 0 }
};


/***/


static int
Ja4r_Arc4random_Random (Jim_Interp *interp, int argc, Jim_Obj *const *argv) {
	Jim_SetResultInt(interp, arc4random());
	return JIM_OK;
}


static int
Ja4r_Arc4random_Bytes (Jim_Interp *interp, int argc, Jim_Obj *const *argv) {
	jim_wide nbytes;
	unsigned char *buf;

	if (Jim_GetWide(interp, argv[0], &nbytes) != JIM_OK) {
		return JIM_ERR;
	}
	if (nbytes < 1) {
		Jim_SetResultFormatted(interp, "bad value \"%#s\" for nbytes: must be > 0", argv[0]);
		return JIM_ERR;
	}

	if ((buf = Jim_Alloc(nbytes)) == NULL) {
		Jim_SetResultFormatted(interp, "couldn't allocate %d bytes", nbytes);
		return JIM_ERR;
	}

	arc4random_buf(buf, nbytes);

	Jim_SetResult(interp, Jim_NewStringObjNoAlloc(interp, buf, nbytes));

	return JIM_OK;
}


static int
Ja4r_Arc4random_Uniform (Jim_Interp *interp, int argc, Jim_Obj *const *argv) {
	jim_wide ubound;

	if (Jim_GetWide(interp, argv[0], &ubound) != JIM_OK) {
		return JIM_ERR;
	}
	if (ubound < 0) {
		Jim_SetResultFormatted(interp, "bad value \"%#s\" for upperbound: must be >= 0", argv[0]);
		return JIM_ERR;
	}

	Jim_SetResultInt(interp, arc4random_uniform(ubound));

	return JIM_OK;
}


/***/


int
Jim_arc4randomInit (Jim_Interp *interp) {
	if (Jim_CreateCommand(interp, "arc4random", Jim_SubCmdProc, (void *) Ja4r_Cmds, NULL) != JIM_OK) { return JIM_ERR; }
	if (Jim_PackageProvide(interp, PACKAGE_NAME, PACKAGE_VERSION, JIM_ERRMSG)             != JIM_OK) { return JIM_ERR; }
	return JIM_OK;
}


#ifdef __cplusplus
}
#endif


/* EOF */

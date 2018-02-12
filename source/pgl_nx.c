#include <switch.h>

#include "pTinyGL/pgl.h"
#include "api/zbuffer.h"
#include "api/zgl.h"

static ZBuffer *pglBuffer;
static u32 *nxBuffer;

int pglInit(int width, int height) {

    gfxInitDefault();
    gfxSetMode(GfxMode_TiledSingle);
    gfxFlushBuffers();
    gfxWaitForVsync();

    nxBuffer = (u32 *) gfxGetFramebuffer(NULL, NULL);
    pglBuffer = ZB_open(width, height, ZB_MODE_RGBA, 0, 0, 0, 0);
    glInit(pglBuffer);

    return 0;
}

static void ZB_copyFrameBufferRGB32(ZBuffer *zb, u32 *buf) {

    unsigned short *q;
    unsigned int v, r, g, b;
    int x, y;

    q = zb->pbuf;

    for (x = 0; x < zb->xsize; x++) {
        for (y = 0; y < zb->ysize; y++) {

            v = q[y * zb->xsize + x];
            r = (v & 0xf800) >> 11;
            g = (v & 0x07e0) >> 5;
            b = v & 0x001f;

            buf[(u32) gfxGetFramebufferDisplayOffset((u32) x, (u32) y)]
                    = RGBA8_MAXALPHA(r << 3, g << 2, b << 3);
        }
    }
}

void pglSwap() {

    ZB_copyFrameBufferRGB32(pglBuffer, nxBuffer);
    //ZB_copyFrameBuffer(pglBuffer, nxBuffer, 1280 * 4);

    gfxFlushBuffers();
    gfxSwapBuffers();
    gfxWaitForVsync();
}

void pglClose() {

    glClose();
    ZB_close(pglBuffer);
    gfxExit();
}

void *pglGetFrameBuffer() {
    return (void *) pglBuffer->pbuf;
}

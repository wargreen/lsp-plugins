/*
 * complex.h
 *
 *  Created on: 22 апр. 2019 г.
 *      Author: sadko
 */

#ifndef DSP_ARCH_AARCH64_ASIMD_PCOMPLEX_H_
#define DSP_ARCH_AARCH64_ASIMD_PCOMPLEX_H_

#ifndef DSP_ARCH_AARCH64_ASIMD_IMPL
    #error "This header should not be included directly"
#endif /* DSP_ARCH_AARCH64_ASIMD_IMPL */

namespace asimd
{
    void pcomplex_mul2(float *dst, const float *src, size_t count)
    {
        ARCH_AARCH64_ASM
        (
            __ASM_EMIT("subs        %[count], %[count], #16")
            __ASM_EMIT("b.lo        2f")

            // x16 blocks
            __ASM_EMIT("1:")
            __ASM_EMIT("ld2         {v16.4s, v17.4s}, [%[src]], #0x20")     // v16  = sr1, v17 = si1
            __ASM_EMIT("ld2         {v24.4s, v25.4s}, [%[dst]], #0x20")     // v24  = dr1, v25 = di1
            __ASM_EMIT("ld2         {v18.4s, v19.4s}, [%[src]], #0x20")     // v18  = sr2, v19 = si2
            __ASM_EMIT("ld2         {v26.4s, v27.4s}, [%[dst]], #0x20")     // v26  = dr2, v27 = di2
            __ASM_EMIT("ld2         {v20.4s, v21.4s}, [%[src]], #0x20")     // v20  = sr3, v21 = si3
            __ASM_EMIT("ld2         {v28.4s, v29.4s}, [%[dst]], #0x20")     // v28  = dr3, v29 = di3
            __ASM_EMIT("ld2         {v22.4s, v23.4s}, [%[src]], #0x20")     // v22  = sr4, v23 = si4
            __ASM_EMIT("ld2         {v30.4s, v31.4s}, [%[dst]], #0x20")     // v30  = dr4, v31 = di4

            __ASM_EMIT("fmul        v0.4s, v16.4s, v24.4s")                 // v0   = sr*dr
            __ASM_EMIT("fmul        v2.4s, v18.4s, v26.4s")
            __ASM_EMIT("fmul        v4.4s, v20.4s, v28.4s")
            __ASM_EMIT("fmul        v6.4s, v22.4s, v30.4s")
            __ASM_EMIT("fmul        v1.4s, v24.4s, v17.4s")                 // v1   = si*dr
            __ASM_EMIT("fmul        v3.4s, v26.4s, v19.4s")
            __ASM_EMIT("fmul        v5.4s, v28.4s, v21.4s")
            __ASM_EMIT("fmul        v7.4s, v30.4s, v23.4s")
            __ASM_EMIT("fmls        v0.4s, v17.4s, v25.4s")                 // v0   = sr*dr - si*di
            __ASM_EMIT("fmls        v2.4s, v19.4s, v27.4s")
            __ASM_EMIT("fmls        v4.4s, v21.4s, v29.4s")
            __ASM_EMIT("fmls        v6.4s, v23.4s, v31.4s")
            __ASM_EMIT("fmla        v1.4s, v16.4s, v25.4s")                 // v1   = si*dr + sr*di
            __ASM_EMIT("fmla        v3.4s, v18.4s, v27.4s")
            __ASM_EMIT("fmla        v5.4s, v20.4s, v29.4s")
            __ASM_EMIT("fmla        v7.4s, v22.4s, v31.4s")

            __ASM_EMIT("sub         %[dst], %[dst], #0x80")                 // dst -= 0x80
            __ASM_EMIT("subs        %[count], %[count], #16")
            __ASM_EMIT("st2         {v0.4s, v1.4s}, [%[dst]], #0x20")
            __ASM_EMIT("st2         {v2.4s, v3.4s}, [%[dst]], #0x20")
            __ASM_EMIT("st2         {v4.4s, v5.4s}, [%[dst]], #0x20")
            __ASM_EMIT("st2         {v6.4s, v7.4s}, [%[dst]], #0x20")
            __ASM_EMIT("b.hs        1b")

            // x8 blocks
            __ASM_EMIT("2:")
            __ASM_EMIT("adds        %[count], %[count], #8")
            __ASM_EMIT("b.lt        4f")
            __ASM_EMIT("ld2         {v16.4s, v17.4s}, [%[src]], #0x20")     // v16  = sr1, v17 = si1
            __ASM_EMIT("ld2         {v24.4s, v25.4s}, [%[dst]], #0x20")     // v24  = dr1, v25 = di1
            __ASM_EMIT("ld2         {v18.4s, v19.4s}, [%[src]], #0x20")     // v18  = sr2, v19 = si2
            __ASM_EMIT("ld2         {v26.4s, v27.4s}, [%[dst]], #0x20")     // v26  = dr2, v27 = di2

            __ASM_EMIT("fmul        v0.4s, v16.4s, v24.4s")                 // v0   = sr*dr
            __ASM_EMIT("fmul        v2.4s, v18.4s, v26.4s")
            __ASM_EMIT("fmul        v1.4s, v24.4s, v17.4s")                 // v1   = si*dr
            __ASM_EMIT("fmul        v3.4s, v26.4s, v19.4s")
            __ASM_EMIT("fmls        v0.4s, v17.4s, v25.4s")                 // v0   = sr*dr - si*di
            __ASM_EMIT("fmls        v2.4s, v19.4s, v27.4s")
            __ASM_EMIT("fmla        v1.4s, v16.4s, v25.4s")                 // v1   = si*dr + sr*di
            __ASM_EMIT("fmla        v3.4s, v18.4s, v27.4s")

            __ASM_EMIT("sub         %[dst], %[dst], #0x40")                 // dst -= 0x40
            __ASM_EMIT("sub         %[count], %[count], #8")
            __ASM_EMIT("st2         {v0.4s, v1.4s}, [%[dst]], #0x20")
            __ASM_EMIT("st2         {v2.4s, v3.4s}, [%[dst]], #0x20")

            // x4 blocks
            __ASM_EMIT("4:")
            __ASM_EMIT("adds        %[count], %[count], #4")
            __ASM_EMIT("b.lt        6f")
            __ASM_EMIT("ld2         {v16.4s, v17.4s}, [%[src]], #0x20")     // v16  = sr1, v17 = si1
            __ASM_EMIT("ld2         {v24.4s, v25.4s}, [%[dst]]")            // v24  = dr1, v25 = di1
            __ASM_EMIT("fmul        v0.4s, v16.4s, v24.4s")                 // v0   = sr*dr
            __ASM_EMIT("fmul        v1.4s, v24.4s, v17.4s")                 // v1   = si*dr
            __ASM_EMIT("fmls        v0.4s, v17.4s, v25.4s")                 // v0   = sr*dr - si*di
            __ASM_EMIT("fmla        v1.4s, v16.4s, v25.4s")                 // v1   = si*dr + sr*di
            __ASM_EMIT("st2         {v0.4s, v1.4s}, [%[dst]], #0x20")

            // x1 blocks
            __ASM_EMIT("6:")
            __ASM_EMIT("adds        %[count], %[count], #3")
            __ASM_EMIT("b.lt        8f")
            __ASM_EMIT("7:")
            __ASM_EMIT("ld2r        {v16.4s, v17.4s}, [%[src]], #0x08")     // v16  = sr1, v17  = si1
            __ASM_EMIT("ld2r        {v24.4s, v25.4s}, [%[dst]]")            // v24  = dr1, v25  = di1
            __ASM_EMIT("fmul        v0.4s, v16.4s, v24.4s")                 // v0   = sr*dr
            __ASM_EMIT("fmul        v1.4s, v24.4s, v17.4s")                 // v1   = si*dr
            __ASM_EMIT("fmls        v0.4s, v17.4s, v25.4s")                 // v0   = sr*dr - si*di
            __ASM_EMIT("fmla        v1.4s, v16.4s, v25.4s")                 // v1   = si*dr + sr*di
            __ASM_EMIT("str         s0, [%[dst]], #0x04")
            __ASM_EMIT("subs        %[count], %[count], #1")
            __ASM_EMIT("str         s1, [%[dst]], #0x04")
            __ASM_EMIT("b.ge        7b")

            __ASM_EMIT("8:")

            : [dst] "+r" (dst), [src] "+r" (src),
              [count] "+r" (count)
            :
            : "cc", "memory",
              "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7",
              // "v8", "v9", "v10", "v11", "v12", "v13", "v14", "v15", // Avoid usage if possible
              "v16", "v17", "v18", "v19", "v20", "v21", "v22", "v23",
              "v24", "v25", "v26", "v27", "v28", "v29", "v30", "v31"
        );
    }
}

#endif /* DSP_ARCH_AARCH64_ASIMD_PCOMPLEX_H_ */

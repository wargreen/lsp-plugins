/*
 * Trigger.cpp
 *
 *  Created on: 29 Jan 2020
 *      Author: crocoduck
 */

#include <core/util/Trigger.h>
#include <core/debug.h>
#include <dsp/dsp.h>

#define MEM_LIM_SIZE 16

namespace lsp
{
    Trigger::Trigger()
    {
        enTriggerType       = TRG_TYPE_NONE;
        enTriggerState      = TRG_STATE_WAITING;

        nPostTrigger        = 0;
        nPostTriggerCounter = 0;

        fThreshold          = 0.0f;

        vMemory             = NULL;
        pData               = NULL;

        bSync               = true;
    }

    Trigger::~Trigger()
    {
    }

    bool Trigger::init()
    {
        // 1X memory buffer
        size_t samples = MEM_LIM_SIZE;

        float *ptr = alloc_aligned<float>(pData, samples);
        if (ptr == NULL)
            return false;

        lsp_guard_assert(float *save = ptr);

        vMemory = ptr;
        ptr += MEM_LIM_SIZE;

        lsp_assert(ptr <= &save[samples]);

        return true;
    }

    void Trigger::destroy()
    {
        free_aligned(pData);
        pData   = NULL;
        vMemory = NULL;
    }

    void Trigger::update_settings()
    {
        prepare_memory();
        nPostTriggerCounter = 0;

        bSync = false;
    }

    void Trigger::prepare_memory()
    {
        dsp::fill_zero(vMemory, MEM_LIM_SIZE);
    }

    void Trigger::shift_memory()
    {
        dsp::move(vMemory, vMemory + 1, MEM_LIM_SIZE - 1);
    }

    void Trigger::single_sample_processor(float value)
    {
        shift_memory();
        vMemory[MEM_LIM_SIZE] = value;
        ++nPostTriggerCounter;

        switch (enTriggerType)
        {
            case TRG_TYPE_SIMPLE_RISING_EDGE:
            {
                float diff = vMemory[MEM_LIM_SIZE] - vMemory[MEM_LIM_SIZE - 1];

                if (diff > 0.0f)
                    enTriggerState = TRG_STATE_ARMED;
                else
                {
                    enTriggerState = TRG_STATE_WAITING;
                    return;
                }

                if ((value >= fThreshold) && (nPostTriggerCounter >= nPostTrigger))
                {
                    enTriggerState = TRG_STATE_FIRED;
                    nPostTriggerCounter = 0;
                }
                else
                    enTriggerState = TRG_STATE_WAITING;

                break;
            }
            case TRG_TYPE_SIMPLE_FALLING_EDGE:
            {
                float diff = vMemory[MEM_LIM_SIZE] - vMemory[MEM_LIM_SIZE - 1];

                if (diff < 0.0f)
                    enTriggerState = TRG_STATE_ARMED;
                else
                {
                    enTriggerState = TRG_STATE_WAITING;
                    return;
                }

                if ((value <= fThreshold) && (nPostTriggerCounter >= nPostTrigger))
                {
                    enTriggerState = TRG_STATE_FIRED;
                    nPostTriggerCounter = 0;
                }
                else
                    enTriggerState = TRG_STATE_WAITING;

                break;
            }
            case TRG_TYPE_NONE:
            case TRG_TYPE_EXTERNAL:
            default:
                return;
        }
    }
}

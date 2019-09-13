/*
 * LSPColor.cpp
 *
 *  Created on: 5 нояб. 2017 г.
 *      Author: sadko
 */

#include <ui/tk/tk.h>

namespace lsp
{
    namespace tk
    {
        LSPColor::LSPColor()
        {
            pWidget     = NULL;
        }

        LSPColor::LSPColor(LSPWidget *widget)
        {
            pWidget     = widget;
        }
        
        LSPColor::~LSPColor()
        {
        }

        void LSPColor::color_changed()
        {
        }

        void LSPColor::trigger_change()
        {
            if (pWidget != NULL)
                pWidget->query_draw();
            color_changed();
        }
    } /* namespace tk */
} /* namespace lsp */

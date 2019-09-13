/*
 * CtlGroup.cpp
 *
 *  Created on: 17 июл. 2017 г.
 *      Author: sadko
 */

#include <ui/ctl/ctl.h>

namespace lsp
{
    namespace ctl
    {
        const ctl_class_t CtlGroup::metadata = { "CtlGroup", &CtlWidget::metadata };

        CtlGroup::CtlGroup(CtlRegistry *src, LSPGroup *widget): CtlWidget(src, widget)
        {
            pClass          = &metadata;
        }

        CtlGroup::~CtlGroup()
        {
        }

        void CtlGroup::init()
        {
            CtlWidget::init();

            if (pWidget == NULL)
                return;
            LSPGroup *grp       = static_cast<LSPGroup *>(pWidget);

            // Initialize color controllers
            sColor.init_hsl(pRegistry, grp, grp->color(), A_COLOR, A_HUE_ID, A_SAT_ID, A_LIGHT_ID);
            sTextColor.init_basic(pRegistry, grp, grp->text_color(), A_TEXT_COLOR);
        }

        void CtlGroup::set(widget_attribute_t att, const char *value)
        {
            LSPGroup *grp       = (pWidget != NULL) ? static_cast<LSPGroup *>(pWidget) : NULL;

            switch (att)
            {
                case A_TEXT:
                    if (grp != NULL)
                        grp->set_text(value);
                    break;
                case A_BORDER:
                    if (grp != NULL)
                        PARSE_INT(value, grp->set_border(__));
                    break;
                case A_RADIUS:
                    if (grp != NULL)
                        PARSE_INT(value, grp->set_radius(__));
                    break;
                case A_EMBED:
                    if (grp != NULL)
                        PARSE_BOOL(value, grp->set_embed(__));
                    break;
                default:
                {
                    sColor.set(att, value);
                    sTextColor.set(att, value);
                    CtlWidget::set(att, value);
                    break;
                }
            }
        }

        status_t CtlGroup::add(CtlWidget *child)
        {
            if (pWidget == NULL)
                return STATUS_BAD_STATE;

            LSPGroup *grp     = static_cast<LSPGroup *>(pWidget);
            return grp->add(child->widget());
        }
    } /* namespace ctl */
} /* namespace lsp */

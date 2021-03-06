//
// Created by tatiana on 27.09.18.
//

#ifndef GUICHAN_MOUSELISTENER_H
#define GUICHAN_MOUSELISTENER_H

#include "guichan/gui/MultipleInheritable.h"

namespace gui {
    class MouseEvent;

    /**
     * Interface for listening for mouse events from widgets.
     *
     * @see Widget::addMouseListener, Widget::removeMouseListener
     * @since 0.1.0
     */
    class MouseListener : public inheritable_enable_shared_from_this<MouseListener> {
    public:

        /**
         * Destructor.
         */
        virtual ~MouseListener();

        /**
         * Called when the mouse has entered into the widget area.
         *
         * @param mouseEvent Describes the event.
         * @since 0.6.0
         */
        virtual void mouseEntered(MouseEvent& mouseEvent);

        /**
         * Called when the mouse has exited the widget area.
         *
         * @param mouseEvent Describes the event.
         * @since 0.6.0
         */
        virtual void mouseExited(MouseEvent& mouseEvent);

        /**
         * Called when a mouse button has been pressed on the widget area.
         *
         * NOTE: A mouse press is NOT equal to a mouse click.
         *       Use mouseClickMessage to check for mouse clicks.
         *
         * @param mouseEvent Describes the event.
         * @since 0.6.0
         */
        virtual void mousePressed(MouseEvent& mouseEvent);

        /**
         * Called when a mouse button has been released on the widget area.
         *
         * @param mouseEvent Describes the event.
         * @since 0.6.0
         */
        virtual void mouseReleased(MouseEvent& mouseEvent);

        /**
         * Called when a mouse button is pressed and released (clicked) on
         * the widget area.
         *
         * @param mouseEvent Describes the event.
         * @since 0.6.0
         */
        virtual void mouseClicked(MouseEvent& mouseEvent);

        /**
         * Called when the mouse wheel has moved up on the widget area.
         *
         * @param mouseEvent Describes the event.
         * @since 0.6.0
         */
        virtual void mouseWheelMovedUp(MouseEvent& mouseEvent);

        /**
         * Called when the mouse wheel has moved down on the widget area.
         *
         * @param mousEvent Describes the event.
         * @since 0.6.0
         */
        virtual void mouseWheelMovedDown(MouseEvent& mouseEvent);

        /**
         * Called when the mouse has moved in the widget area and no mouse button
         * has been pressed (i.e no widget is being dragged).
         *
         * @param mouseEvent Describes the event.
         * @since 0.6.0
         */
        virtual void mouseMoved(MouseEvent& mouseEvent);

        /**
         * Called when the mouse has moved and the mouse has previously been
         * pressed on the widget.
         *
         * @param mouseEvent Describes the event.
         * @since 0.6.0
         */
        virtual void mouseDragged(MouseEvent& mouseEvent);

    protected:
        /**
         * Constructor.
         *
         * You should not be able to make an instance of MouseListener,
         * therefore its constructor is protected.
         */
        MouseListener();
    };
}

#endif //GUICHAN_MOUSELISTENER_H

//
// Created by tatiana on 28.09.18.
//

#ifndef GUICHAN_ZTEXTBOX_H
#define GUICHAN_ZTEXTBOX_H

#include "guichan/gui/ZWidget.h"
#include "guichan/gui/MouseListener.h"
#include "guichan/gui/KeyListener.h"

namespace gui {
    class ZText;

    /**
     * An implementation of a text box where a user can enter text that contains of many lines.
     */
    class ZTextBox : public ZWidget, public MouseListener, public KeyListener{
    public:
        /**
         * Constructor.
         */
        ZTextBox();

        virtual void init();
        virtual void shutdown();

        /**
         * Constructor.
         *
         * @param text The default text of the text box.
         */
        virtual void init(const std::string& text);

        /**
         * Sets the text of the text box.
         *
         * @param text The text of the text box.
         * @see getText
         */
        void setText(const std::string& text);

        /**
         * Gets the text of the text box.
         *
         * @return The text of the text box.
         * @see setText
         */
        std::string getText() const;

        /**
         * Gets a certain row from the text.
         *
         * @param row The number of the row to get from the text.
         * @return A row from the text of the text box.
         * @see setTextRow
         */
        std::string getTextRow(int row) const;

        /**
         * Sets the text of a certain row of the text.
         *
         * @param row The number of the row to set in the text.
         * @param text The text to set in the given row number.
         * @see getTextRow
         */
        void setTextRow(int row, const std::string& text);

        /**
         * Gets the number of rows in the text.
         *
         * @return The number of rows in the text.
         */
        unsigned int getNumberOfRows() const;

        /**
         * Gets the caret position in the text.
         *
         * @return The caret position in the text.
         * @see setCaretPosition
         */
        unsigned int getCaretPosition() const;

        /**
         * Sets the position of the caret in the text.
         *
         * @param position the positon of the caret.
         * @see getCaretPosition
         */
        void setCaretPosition(unsigned int position);

        /**
         * Gets the row number where the caret is currently located.
         *
         * @return The row number where the caret is currently located.
         * @see setCaretRow
         */
        unsigned int getCaretRow() const;

        /**
         * Sets the row where the caret should be currently located.
         *
         * @param The row where the caret should be currently located.
         * @see getCaretRow
         */
        void setCaretRow(int row);

        /**
         * Gets the column where the caret is currently located.
         *
         * @return The column where the caret is currently located.
         * @see setCaretColumn
         */
        unsigned int getCaretColumn() const;

        /**
         * Sets the column where the caret should be currently located.
         *
         * @param The column where the caret should be currently located.
         * @see getCaretColumn
         */
        void setCaretColumn(int column);

        /**
         * Sets the row and the column where the caret should be curretly
         * located.
         *
         * @param row The row where the caret should be currently located.
         * @param column The column where the caret should be currently located.
         * @see getCaretRow, getCaretColumn
         */
        void setCaretRowColumn(int row, int column);

        /**
         * Scrolls the text to the caret if the text box is in a scroll area.
         *
         * @see ScrollArea
         */
        virtual void scrollToCaret();

        /**
         * Checks if the text box is editable.
         *
         * @return True it the text box is editable, false otherwise.
         * @see setEditable
         */
        bool isEditable() const;

        /**
         * Sets the text box to be editable or not.
         *
         * @param editable True if the text box should be editable, false otherwise.
         */
        void setEditable(bool editable);

        /**
         * Adds a row of text to the end of the text.
         *
         * @param row The row to add.
         */
        virtual void addRow(const std::string &row);

        /**
         * Checks if the text box is opaque. An opaque text box will draw
         * it's background and it's text. A non opaque text box only draw it's
         * text making it transparent.
         *
         * @return True if the text box is opaque, false otherwise.
         * @see setOpaque
         */
        bool isOpaque();

        /**
         * Sets the text box to be opaque or not. An opaque text box will draw
         * it's background and it's text. A non opaque text box only draw it's
         * text making it transparent.
         *
         * @param opaque True if the text box should be opaque, false otherwise.
         * @see isOpaque
         */
        void setOpaque(bool opaque);


        // Inherited from Widget

        virtual void draw_item(std::shared_ptr<Graphics> graphics);

        virtual void fontChanged();


        // Inherited from KeyListener

        virtual void keyPressed(KeyEvent& keyEvent);


        // Inherited from MouseListener

        virtual void mousePressed(MouseEvent& mouseEvent);

        virtual void mouseDragged(MouseEvent& mouseEvent);

    protected:
        /**
         * Draws the caret. Overloaded this method if you want to
         * change the style of the caret.
         *
         * @param graphics a Graphics object to draw with.
         * @param x the x position.
         * @param y the y position.
         */
        virtual void drawCaret(std::shared_ptr<Graphics> graphics, int x, int y);

        /**
         * Adjusts the text box's size to fit the text.
         */
        virtual void adjustSize();

        /**
         * Holds the text of the text box.
         */
        std::shared_ptr<ZText> mText;

        /**
         * True if the text box is editable, false otherwise.
         */
        bool mEditable;

        /**
         * True if the text box is editable, false otherwise.
         */
        bool mOpaque;
    };
}

#endif //GUICHAN_ZTEXTBOX_H

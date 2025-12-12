/**
 * ============================================================================
 * UIStyles - Centralized UI Styling Utilities
 * ============================================================================
 *
 * Purpose: Avoid code duplication by centralizing all colors, fonts, and
 *          stylesheet definitions in one place.
 *
 * Usage:
 *   #include "uistyles.h"
 *
 *   // Use colors
 *   setStyleSheet("background-color: " + UIColors::BLUE_PRIMARY);
 *
 *   // Use stylesheet generators
 *   myButton->setStyleSheet(UIStyles::blueButton());
 *
 *   // Use dialog utilities
 *   UIDialogs::showInfo(this, "Success", "Operation completed!");
 */

#ifndef UISTYLES_H
#define UISTYLES_H

#include <QString>
#include <QWidget>
#include <QMessageBox>

// ============================================================================
// COLORS - All color constants used in the application
// ============================================================================
namespace UIColors {
    // Background Colors
    inline const QString BACKGROUND_DARK_BLUE_GRAY = "#34495e";  // ManageCoursesPage background
    inline const QString BACKGROUND_VERY_DARK = "#0D1B2A";       // Timetable background
    inline const QString BACKGROUND_WHITE = "#ffffff";
    inline const QString BACKGROUND_LIGHT_GRAY = "#ecf0f1";
    inline const QString BACKGROUND_SELECTED_ROW = "#E8E8E8";    // Selected table row

    // Blue Theme (Primary buttons: Edit, Search, Add, Table Header)
    inline const QString BLUE_PRIMARY = "#3498db";
    inline const QString BLUE_HOVER = "#2980b9";
    inline const QString BLUE_PRESSED = "#21618c";
    inline const QString BLUE_DEEP = "#2d5a8c";                  // Timetable course cells

    // Red Theme (Delete/Danger buttons)
    inline const QString RED_PRIMARY = "#e74c3c";
    inline const QString RED_HOVER = "#c0392b";
    inline const QString RED_PRESSED = "#a93226";

    // Green Theme (Sort dropdown, Success elements)
    inline const QString GREEN_PRIMARY = "#2ecc71";
    inline const QString GREEN_HOVER = "#27ae60";

    // Gray Theme (Cancel, Clear, Disabled elements)
    inline const QString GRAY_PRIMARY = "#95a5a6";
    inline const QString GRAY_HOVER = "#7f8c8d";
    inline const QString GRAY_LIGHT = "#e0e0e0";                 // Message box buttons
    inline const QString GRAY_GRID = "#d0d0d0";                  // Table grid lines

    // Text Colors
    inline const QString TEXT_BLACK = "#000000";
    inline const QString TEXT_WHITE = "#FFFFFF";

    // Special
    inline const QString TRANSPARENT = "transparent";
}

// ============================================================================
// STYLESHEET GENERATORS - Reusable stylesheet functions
// ============================================================================
namespace UIStyles {

    /**
     * Blue Button - Standard size (for Search, Generate, etc.)
     */
    inline QString blueButton() {
        return QString(
            "QPushButton {"
            "background-color: %1;"
            "color: white;"
            "border: none;"
            "border-radius: 4px;"
            "padding: 5px;"
            "font-size: 11px;"
            "font-weight: 600;"
            "}"
            "QPushButton:hover {"
            "background-color: %2;"
            "}"
        ).arg(UIColors::BLUE_PRIMARY, UIColors::BLUE_HOVER);
    }

    /**
     * Blue Button Compact - For table action buttons (Edit)
     */
    inline QString blueButtonCompact() {
        return QString(
            "QPushButton {"
            "background-color: %1;"
            "color: white;"
            "border: none;"
            "border-radius: 4px;"
            "padding: 4px 8px;"
            "font-size: 11px;"
            "font-weight: 600;"
            "min-width: 55px;"
            "min-height: 26px;"
            "max-height: 26px;"
            "}"
            "QPushButton:disabled {"
            "background-color: %1;"
            "color: white;"
            "opacity: 0.6;"
            "}"
            "QPushButton:hover:enabled {"
            "background-color: %2;"
            "}"
            "QPushButton:pressed {"
            "background-color: %3;"
            "}"
        ).arg(UIColors::BLUE_PRIMARY, UIColors::BLUE_HOVER, UIColors::BLUE_PRESSED);
    }

    /**
     * Red Button - Standard size (for major delete actions)
     */
    inline QString redButton() {
        return QString(
            "QPushButton {"
            "background-color: %1;"
            "color: white;"
            "border: none;"
            "border-radius: 4px;"
            "padding: 5px;"
            "font-size: 11px;"
            "font-weight: 600;"
            "}"
            "QPushButton:hover {"
            "background-color: %2;"
            "}"
        ).arg(UIColors::RED_PRIMARY, UIColors::RED_HOVER);
    }

    /**
     * Red Button Compact - For table action buttons (Delete)
     */
    inline QString redButtonCompact() {
        return QString(
            "QPushButton {"
            "background-color: %1;"
            "color: white;"
            "border: none;"
            "border-radius: 4px;"
            "padding: 4px 8px;"
            "font-size: 11px;"
            "font-weight: 600;"
            "min-width: 55px;"
            "min-height: 26px;"
            "max-height: 26px;"
            "}"
            "QPushButton:disabled {"
            "background-color: %1;"
            "color: white;"
            "opacity: 0.6;"
            "}"
            "QPushButton:hover:enabled {"
            "background-color: %2;"
            "}"
            "QPushButton:pressed {"
            "background-color: %3;"
            "}"
        ).arg(UIColors::RED_PRIMARY, UIColors::RED_HOVER, UIColors::RED_PRESSED);
    }

    /**
     * Gray Button - For Clear Search, Cancel buttons
     */
    inline QString grayButton() {
        return QString(
            "QPushButton {"
            "background-color: %1;"
            "color: white;"
            "border: none;"
            "border-radius: 4px;"
            "padding: 5px;"
            "font-size: 11px;"
            "font-weight: 600;"
            "}"
            "QPushButton:hover {"
            "background-color: %2;"
            "}"
        ).arg(UIColors::GRAY_PRIMARY, UIColors::GRAY_HOVER);
    }

    /**
     * Checkbox - Blue theme with hover effects
     */
    inline QString checkboxBlue() {
        return QString(
            "QCheckBox::indicator {"
            "width: 20px;"
            "height: 20px;"
            "border: 2px solid %1;"
            "border-radius: 4px;"
            "background-color: white;"
            "}"
            "QCheckBox::indicator:hover {"
            "border: 2px solid %2;"
            "background-color: %3;"
            "}"
            "QCheckBox::indicator:checked {"
            "background-color: %1;"
            "border: 2px solid %2;"
            "}"
            "QCheckBox::indicator:checked:hover {"
            "background-color: %2;"
            "}"
        ).arg(UIColors::BLUE_PRIMARY, UIColors::BLUE_HOVER, UIColors::BACKGROUND_LIGHT_GRAY);
    }

    /**
     * Line Edit - White background with blue border
     */
    inline QString lineEditWhite() {
        return QString(
            "QLineEdit {"
            "background-color: %1;"
            "color: %2;"
            "border: 2px solid %3;"
            "border-radius: 4px;"
            "padding: 5px;"
            "font-size: 11px;"
            "}"
        ).arg(UIColors::BACKGROUND_WHITE, UIColors::TEXT_BLACK, UIColors::BLUE_PRIMARY);
    }

    /**
     * ComboBox - White background with green border
     */
    inline QString comboBoxGreen() {
        return QString(
            "QComboBox {"
            "background-color: %1;"
            "color: %2;"
            "border: 2px solid %3;"
            "border-radius: 4px;"
            "padding: 5px;"
            "font-size: 11px;"
            "}"
            "QComboBox:hover {"
            "border: 2px solid %4;"
            "}"
            "QComboBox::drop-down {"
            "border: none;"
            "}"
        ).arg(UIColors::BACKGROUND_WHITE, UIColors::TEXT_BLACK,
              UIColors::GREEN_PRIMARY, UIColors::GREEN_HOVER);
    }

    /**
     * Table Header - Blue background with white text
     */
    inline QString tableHeaderBlue() {
        return QString(
            "QHeaderView::section {"
            "background-color: %1;"
            "color: white;"
            "padding: 8px;"
            "border: none;"
            "font-weight: bold;"
            "font-size: 11px;"
            "}"
        ).arg(UIColors::BLUE_PRIMARY);
    }

    /**
     * Table Widget - White background, no selection highlighting
     */
    inline QString tableWidgetWhite() {
        return QString(
            "QTableWidget {"
            "background-color: white;"
            "gridline-color: %1;"
            "font-size: 11px;"
            "selection-background-color: transparent;"
            "}"
            "QTableWidget::item {"
            "color: black;"
            "padding: 0px;"
            "}"
            "QTableWidget::item:selected {"
            "background-color: transparent;"
            "color: black;"
            "}"
            "QTableWidget::item:hover {"
            "background-color: transparent;"
            "}"
        ).arg(UIColors::GRAY_GRID);
    }

    /**
     * Message Box - Standard styling for all dialogs
     */
    inline QString messageBox() {
        return QString(
            "QMessageBox {"
            "background-color: %1;"
            "}"
            "QLabel {"
            "color: %2;"
            "font-size: 11px;"
            "background-color: transparent;"
            "}"
            "QPushButton {"
            "background-color: %3;"
            "color: %2;"
            "font-size: 11px;"
            "min-width: 60px;"
            "padding: 5px;"
            "}"
        ).arg(UIColors::BACKGROUND_WHITE, UIColors::TEXT_BLACK, UIColors::GRAY_LIGHT);
    }
}

// ============================================================================
// DIALOG UTILITIES - Convenient functions for showing styled message boxes
// ============================================================================
namespace UIDialogs {

    /**
     * Show Information Message Box
     */
    inline void showInfo(QWidget *parent, const QString &title, const QString &message) {
        QMessageBox msgBox(parent);
        msgBox.setWindowTitle(title);
        msgBox.setText(message);
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStyleSheet(UIStyles::messageBox());
        msgBox.exec();
    }

    /**
     * Show Warning Message Box
     */
    inline void showWarning(QWidget *parent, const QString &title, const QString &message) {
        QMessageBox msgBox(parent);
        msgBox.setWindowTitle(title);
        msgBox.setText(message);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStyleSheet(UIStyles::messageBox());
        msgBox.exec();
    }

    /**
     * Show Error Message Box
     */
    inline void showError(QWidget *parent, const QString &title, const QString &message) {
        QMessageBox msgBox(parent);
        msgBox.setWindowTitle(title);
        msgBox.setText(message);
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setStyleSheet(UIStyles::messageBox());
        msgBox.exec();
    }

    /**
     * Show Confirmation Dialog (Yes/No)
     * @return true if user clicked Yes, false otherwise
     */
    inline bool showConfirmation(QWidget *parent, const QString &title, const QString &message) {
        QMessageBox msgBox(parent);
        msgBox.setWindowTitle(title);
        msgBox.setText(message);
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setStyleSheet(UIStyles::messageBox());
        return msgBox.exec() == QMessageBox::Yes;
    }
}

#endif // UISTYLES_H

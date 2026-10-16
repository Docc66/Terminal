#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QSpinBox>
#include <QSlider>
#include <QFontComboBox>
#include <QPushButton>
#include <QLabel>
#include "Settings.h"

class SettingsDialog : public QDialog {
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget* parent = nullptr);

signals:
    void settingsChanged();

private slots:
    void onApply();
    void onOk();
    void onCancel();
    void onBackgroundColorClicked();
    void onTextColorClicked();

private:
    QComboBox* m_themeCombo;
    QFontComboBox* m_fontCombo;
    QSpinBox* m_fontSizeSpinBox;
    QPushButton* m_backgroundColorButton;
    QPushButton* m_textColorButton;
    QComboBox* m_cursorStyleCombo;
    QComboBox* m_defaultShellCombo;
    QSpinBox* m_scrollbackSpinBox;
    QSlider* m_opacitySlider;
    QLabel* m_opacityLabel;

    QColor m_selectedBackgroundColor;
    QColor m_selectedTextColor;

    void setupUI();
    void loadSettings();
    void saveSettings();
    void updateColorButton(QPushButton* button, const QColor& color);
};

#endif

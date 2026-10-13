#include "SettingsDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QGroupBox>
#include <QColorDialog>
#include <QDialogButtonBox>

SettingsDialog::SettingsDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("Settings");
    setMinimumWidth(500);
    setupUI();
    loadSettings();
}

void SettingsDialog::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // Theme group
    QGroupBox* themeGroup = new QGroupBox("Theme", this);
    QFormLayout* themeLayout = new QFormLayout(themeGroup);
    
    m_themeCombo = new QComboBox(this);
    m_themeCombo->addItem("Dark", Settings::Dark);
    m_themeCombo->addItem("Light", Settings::Light);
    themeLayout->addRow("Theme:", m_themeCombo);

    mainLayout->addWidget(themeGroup);

    // Font group
    QGroupBox* fontGroup = new QGroupBox("Font", this);
    QFormLayout* fontLayout = new QFormLayout(fontGroup);
    
    m_fontCombo = new QFontComboBox(this);
    m_fontCombo->setFontFilters(QFontComboBox::MonospacedFonts);
    fontLayout->addRow("Font Family:", m_fontCombo);
    
    m_fontSizeSpinBox = new QSpinBox(this);
    m_fontSizeSpinBox->setRange(8, 24);
    m_fontSizeSpinBox->setValue(10);
    fontLayout->addRow("Font Size:", m_fontSizeSpinBox);

    mainLayout->addWidget(fontGroup);

    // Colors group
    QGroupBox* colorsGroup = new QGroupBox("Colors", this);
    QFormLayout* colorsLayout = new QFormLayout(colorsGroup);
    
    m_backgroundColorButton = new QPushButton("Choose Color", this);
    connect(m_backgroundColorButton, &QPushButton::clicked,
            this, &SettingsDialog::onBackgroundColorClicked);
    colorsLayout->addRow("Background:", m_backgroundColorButton);
    
    m_textColorButton = new QPushButton("Choose Color", this);
    connect(m_textColorButton, &QPushButton::clicked,
            this, &SettingsDialog::onTextColorClicked);
    colorsLayout->addRow("Text:", m_textColorButton);

    mainLayout->addWidget(colorsGroup);

    // Terminal group
    QGroupBox* terminalGroup = new QGroupBox("Terminal", this);
    QFormLayout* terminalLayout = new QFormLayout(terminalGroup);
    
    m_cursorStyleCombo = new QComboBox(this);
    m_cursorStyleCombo->addItem("Block", Settings::Block);
    m_cursorStyleCombo->addItem("Underline", Settings::Underline);
    m_cursorStyleCombo->addItem("Bar", Settings::Bar);
    terminalLayout->addRow("Cursor Style:", m_cursorStyleCombo);
    
    m_defaultShellCombo = new QComboBox(this);
    m_defaultShellCombo->addItem("PowerShell", Settings::PowerShell);
    m_defaultShellCombo->addItem("Command Prompt", Settings::CommandPrompt);
    terminalLayout->addRow("Default Shell:", m_defaultShellCombo);
    
    m_scrollbackSpinBox = new QSpinBox(this);
    m_scrollbackSpinBox->setRange(1000, 100000);
    m_scrollbackSpinBox->setSingleStep(1000);
    m_scrollbackSpinBox->setValue(10000);
    terminalLayout->addRow("Scrollback Lines:", m_scrollbackSpinBox);

    mainLayout->addWidget(terminalGroup);

    // Appearance group
    QGroupBox* appearanceGroup = new QGroupBox("Appearance", this);
    QFormLayout* appearanceLayout = new QFormLayout(appearanceGroup);
    
    QHBoxLayout* opacityLayout = new QHBoxLayout();
    m_opacitySlider = new QSlider(Qt::Horizontal, this);
    m_opacitySlider->setRange(50, 100);
    m_opacitySlider->setValue(100);
    m_opacityLabel = new QLabel("100%", this);
    m_opacityLabel->setMinimumWidth(40);
    connect(m_opacitySlider, &QSlider::valueChanged, [this](int value) {
        m_opacityLabel->setText(QString("%1%").arg(value));
    });
    opacityLayout->addWidget(m_opacitySlider);
    opacityLayout->addWidget(m_opacityLabel);
    appearanceLayout->addRow("Opacity:", opacityLayout);

    mainLayout->addWidget(appearanceGroup);

    // Buttons
    QDialogButtonBox* buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Apply | QDialogButtonBox::Cancel,
        this
    );
    connect(buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked,
            this, &SettingsDialog::onOk);
    connect(buttonBox->button(QDialogButtonBox::Apply), &QPushButton::clicked,
            this, &SettingsDialog::onApply);
    connect(buttonBox->button(QDialogButtonBox::Cancel), &QPushButton::clicked,
            this, &SettingsDialog::onCancel);
    
    mainLayout->addWidget(buttonBox);
}

void SettingsDialog::loadSettings() {
    const Settings& settings = Settings::instance();
    
    m_themeCombo->setCurrentIndex(m_themeCombo->findData(settings.theme()));
    m_fontCombo->setCurrentFont(settings.font());
    m_fontSizeSpinBox->setValue(settings.font().pointSize());
    m_cursorStyleCombo->setCurrentIndex(m_cursorStyleCombo->findData(settings.cursorStyle()));
    m_defaultShellCombo->setCurrentIndex(m_defaultShellCombo->findData(settings.defaultShell()));
    m_scrollbackSpinBox->setValue(settings.scrollbackLines());
    m_opacitySlider->setValue(settings.opacity());
    
    m_selectedBackgroundColor = settings.backgroundColor();
    m_selectedTextColor = settings.textColor();
    
    updateColorButton(m_backgroundColorButton, m_selectedBackgroundColor);
    updateColorButton(m_textColorButton, m_selectedTextColor);
}

void SettingsDialog::saveSettings() {
    Settings& settings = Settings::instance();
    
    settings.setTheme(static_cast<Settings::Theme>(
        m_themeCombo->currentData().toInt()));
    
    QFont font = m_fontCombo->currentFont();
    font.setPointSize(m_fontSizeSpinBox->value());
    settings.setFont(font);
    
    settings.setBackgroundColor(m_selectedBackgroundColor);
    settings.setTextColor(m_selectedTextColor);
    
    settings.setCursorStyle(static_cast<Settings::CursorStyle>(
        m_cursorStyleCombo->currentData().toInt()));
    
    settings.setDefaultShell(static_cast<Settings::DefaultShell>(
        m_defaultShellCombo->currentData().toInt()));
    
    settings.setScrollbackLines(m_scrollbackSpinBox->value());
    settings.setOpacity(m_opacitySlider->value());
    
    settings.save();
    
    emit settingsChanged();
}

void SettingsDialog::onApply() {
    saveSettings();
}

void SettingsDialog::onOk() {
    saveSettings();
    accept();
}

void SettingsDialog::onCancel() {
    reject();
}

void SettingsDialog::onBackgroundColorClicked() {
    QColor color = QColorDialog::getColor(m_selectedBackgroundColor, this,
                                          "Select Background Color");
    if (color.isValid()) {
        m_selectedBackgroundColor = color;
        updateColorButton(m_backgroundColorButton, color);
    }
}

void SettingsDialog::onTextColorClicked() {
    QColor color = QColorDialog::getColor(m_selectedTextColor, this,
                                          "Select Text Color");
    if (color.isValid()) {
        m_selectedTextColor = color;
        updateColorButton(m_textColorButton, color);
    }
}

void SettingsDialog::updateColorButton(QPushButton* button, const QColor& color) {
    QString style = QString(
        "QPushButton {"
        "  background-color: %1;"
        "  border: 1px solid #555555;"
        "  padding: 5px;"
        "  border-radius: 3px;"
        "}"
    ).arg(color.name());
    button->setStyleSheet(style);
}
